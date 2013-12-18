
//***************************************
// RBinArchiveFrame::
//***************************************

struct RBinArchiveFrame : public IBinArchiveFrame
{
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RBinArchiveFrame, 1, IBinArchiveFrame)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  struct NN_Add;
  friend struct NN_Add;
  //
  struct NN_Add {
    int extra;
    SMem* memto;
    //
    inline NN_Add(int x, SMem* mt) 
      : extra(x), memto(mt) {}
    //
    inline NameNode* operator()(NameNodeCmpUC& cmp) const { return new_node(cmp.name, extra, memto); }
    inline NameNode* operator()(NameNodeCmpC& cmp) const { return new_node(cmp.name, extra, memto); }
  };
  //
  bool valid;
  SMemShared rdir;
  IClient::P client;
  NameNodes nodes;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
  //
  RBinArchiveFrame(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags())
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
  ~RBinArchiveFrame(void) 
  {
    close();
  }
  //
  // SFlags::ISetter
  DMediaFlags::eStates get_flag(int shift) const { return flags.get_s(shift); }
  DMediaFlags get_flags_auto(void) const { return auto_flags; }
  DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag) { return flags.getset_s(shift, flag); }
  //
  // IBinArchive 
  IStreamReader::P data_reader(const DCString& media_name, const SFlags& flags)
  {
    if(!client.valid())
      return NULL;
    NameNode *n = find_node(media_name);
    return n ? client->data_reader(this, n, flags) : NULL;
  }
  IStreamWriter::P data_writer(const DCString& media_name, const SFlags& flags)
  {
    if(!client.valid() || !filew.valid())
      return NULL;
    return client->data_writer(this, media_name, flags);
  }
  bool data_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags)
  {
    if(!client.valid())
      return false;
    NameNode *n = find_node(media_name);
    return n ? client->data_load(this, mem, n, flags) : false;
  }
  bool data_save(const DCString& media_name, const SCMem& mem, const SFlags& flags)
  {
    if(!client.valid() || !filew.valid())
      return false;
    return client->data_save(this, mem, media_name, flags);
  }
  bool data_list(const DCString& mask, DIFileInfoArray& list, const SFlags& flags)
  {
    return false;
  }
  int get_permissions(void) 
  {
    if(!client)
      return 0;
    return permission;
  }
  //
  // IBinArchiveFrame
  bool open(IClient* client, const SParameters* params)
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
    if(mg==NULL || fname==NULL || *fname==0) {
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
        if(filew==NULL)
          return true; // ok, but no reader
        permission = mp_WL;
      } else {
        filew = mg->bin_writer(fname);
        if(filew==NULL) {
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
  bool close(void)
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
  //
  NameNode* add_node_r(const DCString& name, size_t extra, SMem& memto)
  {
    NameNode *onode = NULL, *nnode = NULL;
    if(ignore_case) {
      nodes.gen_node(NameNodeCmpUC(name), NN_Add(extra, &memto), &onode, &nnode);
    } else {
      nodes.gen_node(NameNodeCmpC(name), NN_Add(extra, &memto), &onode, &nnode);
    }
    if(onode)
      return NULL; // this node duplicates previous one
    return nnode;
  }
  NameNode* add_node_w(const DCString& name, size_t extra)
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
  bool remove_node(NameNode* node)
  {
    if(node==NULL)
      return true;
    nodes.remove_node(node);
    kill_node(node);
    return true;
  }
  NameNode* find_node(const DCString& name)
  {
    return ignore_case ? nodes.find_node_u(NameNodeCmpUC(name)) 
                       : nodes.find_node_u(NameNodeCmpC(name));
  }
  size_t node_size(const DCString &name, int extra)
  {
    return nodesize(~name, extra);
  }
  //
  // utility
  bool open(const SParameters& args)
  {
    return open(NULL, &args);
  }
  //
  static bool kill_node(NameNode* node) 
  {
    if(node->_node_flag1==1)
      M.mem().mem_free(node, UMODSYS_SOURCEINFO);
    return true;
  }
  template<typename String>
  inline static NameNode* new_node(const String& nodename, int extra, SMem* memto) 
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
  inline static size_t nodesize(int namelen, int extra) { return UMODSYS_ARCH_SIZE_CORRECT( sizeof(NameNode) + extra + (namelen+1)*sizeof(BChar) ); }
  //
  bool openr(ILibrary* lib, const DCString& fname, const SParameters* params) 
  {
    filer = lib->bin_reader(fname);
    if(filer==NULL)
      return false;
    //
    int rsize = 0;
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
  bool closew(bool abort) {
    if(client==NULL)
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
  //
  //
  inline bool validate_construction(void) const { return valid; }
};

//***************************************
// END
//***************************************
