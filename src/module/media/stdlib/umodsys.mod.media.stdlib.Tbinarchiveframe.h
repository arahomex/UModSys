
//***************************************
// RBinArchiveFrame::
//***************************************

template<typename String> 
RBinArchiveFrame::NameNode* RBinArchiveFrame::new_node(const String& nodename, int extra, SMem* memto) 
{
  NameNode* rv;
  size_t req = nodesize(~nodename, extra);
  if(memto) {
    if(req>memto->size)
      return NULL;
    rv = static_cast<NameNode*>(memto->data);
    memto->move(req);
  } else {
    rv = static_cast<NameNode*>(M.mem().mem_alloc(req, UMODSYS_SOURCEINFO));
  }
  if(!rv)
    return NULL;
  //
  rv->setname(rv+1, extra, nodename);
  rv->extra = rv+1;
  rv->length = 0;   
  rv->_node_flag1 = (memto==NULL);
  //
  return rv;
}

//***************************************
//***************************************

DMediaFlags::eStates RBinArchiveFrame::get_flag(int shift) const 
{ 
  return flags.get_s(shift); 
}

DMediaFlags RBinArchiveFrame::get_flags_auto(void) const 
{ 
  return auto_flags; 
}

DMediaFlags::eStates RBinArchiveFrame::set_flag(int shift, DMediaFlags::eStates flag) 
{ 
  return flags.getset_s(shift, flag); 
}

//***************************************
//***************************************

IStreamReader::P RBinArchiveFrame::data_reader(const DCString& media_name, const SFlags& flags)
{
  if(!client.valid())
    return NULL;
  NameNode *n = find_node(media_name);
  return n ? client->data_reader(this, n, flags)() : NULL;
}

IStreamWriter::P RBinArchiveFrame::data_writer(const DCString& media_name, const SFlags& flags)
{
  if(!client.valid() || !filew.valid())
    return NULL;
  return client->data_writer(this, media_name, flags);
}

bool RBinArchiveFrame::data_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags)
{
  if(!client.valid())
    return false;
  NameNode *n = find_node(media_name);
  return n ? client->data_load(this, mem, n, flags) : false;
}

bool RBinArchiveFrame::data_save(const DCString& media_name, const SCMem& mem, const SFlags& flags)
{
  if(!client.valid() || !filew.valid())
    return false;
  return client->data_save(this, mem, media_name, flags);
}

bool RBinArchiveFrame::data_list(const DCString& mask, size_t namestart, DIFileInfoArray& list, const SFlags& flags)
{
  if(!client)
    return false;
  //
  bool rv = false;
  for(NameNode *n = nodes.min_node(); n; n = nodes.next_node(n)) {
    if(ignore_case) {
      if(!tl::su::wildcmp(mask+namestart, *n->name))
        continue;
    } else {
      if(!tl::su::wildcmp(mask+namestart, *n->name))
        continue;
    }
    if(list.isfull())
      break;
    SFileInfo fi;
    if(!fill_info(fi, n, mask, namestart))
      return false;
    if(!list.push(fi))
      return false;
    rv = true;
  }
  return rv;
}

bool RBinArchiveFrame::data_info(const DCString& mask, size_t namestart, SFileInfo& list, const SFlags& flags)
{
  if(!client)
    return false;
  //
  for(NameNode *n = nodes.min_node(); n; n = nodes.next_node(n)) {
    if(ignore_case) {
      if(!tl::su::utf_cmp_nocase(mask+namestart, ~mask, *n->namenc, ~n->namenc)!=0)
        continue;
    } else {
      if(!tl::su::scmp(mask+namestart, *n->name)!=0)
        continue;
    }
    if(!client->node_info(this, list, n))
      continue;
    if(!fill_info(list, n, mask, namestart))
      return false;
    return true;
  }
  return false;
}

int RBinArchiveFrame::get_permissions(void) 
{
  if(!client)
    return 0;
  return permission;
}

//***************************************
//***************************************

bool RBinArchiveFrame::open(IClient* client, const SParameters* params)
{
  close();
  //
  if(params==NULL || client==NULL && !params->ref_get("client", client) || client==NULL) {
    return false; // invalid args
  }
  this->client = client;
  //  
  ILibrary::P mg;
  if(!params->ref_get("library", mg) || !mg.valid()) {
    close();
    return false; // invalid args
  }
  //
  bool wmode = false;
  BCStr fname = NULL;
  //
  params->get("write", wmode);
  params->get("filename", fname);
  //
  if(!mg.valid() || fname==NULL || *fname==0) {
    close(); // bad library MG
    return false;
  }
  //
  if(!client->arc_init(this, params)) {
    close();
    return false; // error
  }
  //
  if(wmode) {
    bool rmode = openr(mg, fname, params);
    if(rmode) {
      filew = mg->bin_writer(fname, SFlags(mf_safe::yes(), this));
      if(!filer.valid())
        return true; // ok, but no reader
      permission = mp_WL;
    } else {
      filew = mg->bin_writer(fname);
      if(!filer.valid()) {
        close();
        return false; // error, no reader or writer
      }
      filew->t_ref_get_other_interface(filerw);
      if(filerw) 
        filerw->ref_add();
      permission = mp_RWL;
    }
    //
    if(!client->filew_init(this, params)) {
      closew(true);
      close();
      return false; // error, can't init write stream
    }
  } else {
    if(!openr(mg, fname, params)) {
      close();
      return false; // can't open read stream
    }
    permission = mp_RL;
  }
  valid = true;
  return true;
}

bool RBinArchiveFrame::close(void)
{
  closew(false);
  //
  nodes.remove_seq(kill_node);
  rdir.clear();
  //
  if(client) {
    client->arc_deinit(this);
    client.clear();
  }
  valid = false;
  permission = 0;
  return true;
}

RBinArchiveFrame::NameNode* RBinArchiveFrame::add_node_r(const DCString& name, size_t extra, SMem& memto)
{
  NameNode *onode = NULL, *nnode = NULL;
  if(ignore_case) {
    nodes.gen_node(NameNodeCmpUC(name), NN_Add(extra, &memto), &onode, &nnode);
  } else {
    nodes.gen_node(NameNodeCmpC(name), NN_Add(extra, &memto), &onode, &nnode);
  }
  if(onode)
    return NULL; // this node duplicates previous one
//M.dbg_put("{'%s'/%u '%s'/%u}\n", nnode->name(), (unsigned)nnode->name.hash, nnode->namenc(), (unsigned)nnode->namenc.hash);
  return nnode;
}

RBinArchiveFrame::NameNode* RBinArchiveFrame::add_node_w(const DCString& name, size_t extra)
{
  NameNode *onode = NULL, *nnode = NULL;
  if(ignore_case) {
    nodes.gen_node(NameNodeCmpUC(name), NN_Add(extra, NULL), &onode, &nnode);
  } else {
    nodes.gen_node(NameNodeCmpC(name), NN_Add(extra, NULL), &onode, &nnode);
  }
  if(onode) { // remove old node
    remove_node(onode);
    return add_node_w(name, extra);
  }
  return nnode;
}

bool RBinArchiveFrame::remove_node(NameNode* node)
{
  if(node==NULL)
    return true;
  nodes.remove_node(node);
  kill_node(node);
  return true;
}

RBinArchiveFrame::NameNode* RBinArchiveFrame::find_node(const DCString& name)
{
  return ignore_case ? nodes.find_node_u(NameNodeCmpUC(name)) 
                     : nodes.find_node_u(NameNodeCmpC(name));
}

size_t RBinArchiveFrame::node_size(const DCString &name, int extra)
{
  return nodesize(~name, extra);
}

//***************************************
//***************************************

bool RBinArchiveFrame::open(const SParameters& args)
{
  return open(NULL, &args);
}

bool RBinArchiveFrame::kill_node(NameNode* node) 
{
  if(node->_node_flag1==1)
    M.mem().mem_free(node, UMODSYS_SOURCEINFO);
  return true;
}

bool RBinArchiveFrame::fill_info(SFileInfo& fi, NameNode *node, const DCString& path, size_t namestart)
{
  if(!client->node_info(this, fi, node))
    return false;
  if(namestart && !node->name.empty()) {
    DStringBufShared tmp;
    if(!tmp.allocate(M.mem_shared(), namestart + ~node->name))
      return false;
    DStringBufShared::BufferStr buf = tmp.get_buf();
    buf.reset();
    buf.cat(*path, namestart);
    buf.cat(*node->name, ~node->name);
    tmp.update(buf);
    fi.name = tmp;
  } else {
    fi.name.set_alloc(M.mem_shared());
    fi.name = node->name;
  }
  if(!node->name.empty() && fi.name.is_null())
    return false;
  fi.size = node->length;
  return true;
}

bool RBinArchiveFrame::openr(ILibrary* lib, const DCString& fname, const SParameters* params) 
{
  filer = lib->bin_reader(fname);
  if(!filer.valid())
    return false;
  //
  size_t rsize = 0;
  if(!client->filer_pre_scan(this, rsize, params))
    return false; // not pre-scanned
  if(!rdir.alloc(rsize, UMODSYS_SOURCEINFO))
    return false; // no memory for read dir
  //
  SMem rd = rdir;
  if(!client->filer_scan(this, rd, params))
    return false; // not pre-scanned
  return true; // OK
}

bool RBinArchiveFrame::closew(bool abort) 
{
  if(!client.valid())
    abort = true;
  //
  if(filew.valid()) {
    if(abort) {
      filew->writer_abort();
    } else {
      NameNode *n, *nm;
      n = nodes.min_node();
      while(n) {
        nm = nodes.next_node(n);
        if(n->_node_flag1==0) { // need to make copy of existing data
          client->filew_copy_data(this, n);
        }
        n = nm;
      }
      //
      client->filew_writedir_start(this);
      n = nodes.min_node();
      while(n) {
        nm = nodes.next_node(n);
        client->filew_writedir_entry(this, n);
        n = nm;
      }
      client->filew_writedir_end(this);
    }
  }
  //
  filer.clear();
  filew.clear();
  filerw.clear();
  return true;
}

//***************************************
//***************************************

DMediaFlags RBinArchiveFrame::auto_flags;

RBinArchiveFrame::RBinArchiveFrame(DOwner *own, const SParameters& args, DMediaFlags av)
: refs(own), flags(av)/*, cache(local_memory())*/ 
{
  valid = false;
  filer = NULL;
  filew = NULL;
  filerw = NULL;
  ignore_case = false;
  permission = 0;
  //
  open(args);
}

RBinArchiveFrame::~RBinArchiveFrame(void) 
{
  close();
}

//***************************************
// END
//***************************************
