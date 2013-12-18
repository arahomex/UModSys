
//***************************************
// RBinArchiveFrameClient_ZIP::
//***************************************

bool RBinArchiveFrameClient_ZIP::data_load(IBinArchiveFrame* arch, SCMemShared& outmem, NameNode* node, const SFlags& flags)
{
  outmem.clear();
  if(node->length<0)
    return false; // directory
  if(node->length==0)
    return true; // empty file
  //
  IStreamReader *rd = node->_node_flag1 ? arch->filerw : arch->filer;
  if(rd==NULL)
    return false;
  //
  if(!rd->reader_seek( fi(node)->pos ))
    return false;
  //
  fileFilePrefix::Holder FP;
  if(!rd->reader_read(FP.bytes, FP.SIZE))
    return false;
  FP.get();
  if(FP.sig!=FP.SIGNATURE)
    return false;
  if(!rd->reader_seekdelta(FP.fnameLen+FP.xtraLen))
    return false;
  //
  SMemShared mem;
  if(!mem.alloc(node->length, UMODSYS_SOURCEINFO))
    return false;
  //
  int mycrc = 0;
  if (FP.compression == co_Store) {
    // Simply read in raw stored data.
    if(FP.cSize!=node->length || FP.cSize!=FP.ucSize) 
      return false; // not enougth data
    //
    if(!rd->reader_read(mem.get_data(), FP.cSize))
      return false;
    mycrc = ::crc32(mycrc, static_cast<Bytef*>(mem.get_data()), mem.get_size());
    //
    if(mycrc!=FP.crc32) 
      return false; // bad CRC found
    outmem = mem;
    return true;
  } else if (FP.compression != co_Deflate) {
    return false; // bad compression detected 
  }
  // Alloc compressed data buffer and read the whole stream
  SBuffer data(arch->utils);
  if(!data.alloc(FP.cSize+1, UMODSYS_SOURCEINFO))
    return false;
  data.get_tdata<char>()[FP.cSize] = 0;
  //
  if(FP.cSize!=fi(node)->csize || FP.ucSize!=node->length) 
    return false; // not enougth data
  if(!rd->reader_read(data.data, FP.cSize)) {
    return false;
  }
  //
  // Setup the inflate stream.
  z_stream stream;
  int err;
  //
  stream.next_in = static_cast<Bytef*>(data.data);
  stream.avail_in = static_cast<uInt>(FP.cSize)+1;
  stream.next_out = static_cast<Bytef*>(mem.get_data());
  stream.avail_out = FP.ucSize;
  stream.zalloc = 0;
  stream.zfree = 0;
  //
  // Perform inflation. wbits < 0 indicates no zlib header inside the data.
  err = inflateInit2(&stream, -MAX_WBITS);
  //          * windowBits is passed < 0 to tell that there is no zlib header.
  //          * Note that in this case inflate *requires* an extra "dummy" byte
  //          * after the compressed stream in order to complete decompression and
  //          * return Z_STREAM_END.
  if (err == Z_OK) {
    err = inflate(&stream, Z_FINISH);
    inflateEnd(&stream);
    if (err == Z_STREAM_END) err = Z_OK;
    inflateEnd(&stream);
  }
  //
  if (err != Z_OK)
    return false; // error inflate data
  //
  mycrc = ::crc32(mycrc, static_cast<Bytef*>(mem.get_data()), mem.get_size());
  if(mycrc!=FP.crc32) 
    return false; // bad CRC found
  //
  outmem = mem;
  return true; // OK
}

//***************************************

bool RBinArchiveFrameClient_ZIP::data_save(IBinArchiveFrame* arch, const SCMem& mem, const DCString& media_name, const SFlags& flags)
{
  if(media_name.length>=syshlp::MAX_FILENAME_LEN)
    return false; // name too long
  //
  NameNode* node = arch->add_node_w(media_name, sizeof(FileInfo));
  if(node==NULL)
    return false; // bad/unallocated node
  //
  FileInfo *x = fi(node);
  x->pos = arch->filew->writer_pos();
  x->xattr = x->attr = 0;
  x->md = x->mt = 0;
  x->csize = 0;
  x->crc = ::crc32(0, static_cast<const Bytef*>(mem.data), mem.size);
  x->compression = arch->user_flags[3];
  node->length = mem.size;
  //
  fileFilePrefix::Holder FP;
  FP.sig = FP.SIGNATURE;
  FP.version = zv_Archiver;
  FP.flag = 0;
  FP.compression = x->compression;
  FP.modTime = x->mt;
  FP.modDate = x->md;
  FP.crc32 = x->crc;
  FP.ucSize = node->length;
  FP.xtraLen = 0;
  FP.fnameLen = node->name.length;
  FP.version = zv_Archiver;
  //
  if(x->compression==co_Store || mem.size==0) {
    FP.cSize = x->csize = mem.size;
    FP.set();
    if(!arch->filew->writer_write(FP.bytes, FP.SIZE)
      || !arch->filew->writer_write(media_name.text, media_name.length)
      || !arch->filew->writer_write(mem.data, mem.size))
      return false; // error save a data
  } else if(x->compression==co_Deflate) {
    SBuffer data(arch->utils);
    if(!data.alloc(mem.size*2, UMODSYS_SOURCEINFO))
      return false;
    z_stream stream;
    int err;
    //
    stream.next_in = static_cast<Bytef*>(const_cast<void*>(mem.data));
    stream.avail_in = static_cast<uInt>(mem.size);
    stream.next_out = static_cast<Bytef*>(data.data);
    stream.avail_out = static_cast<uInt>(data.size);
    stream.zalloc = 0;
    stream.zfree = 0;
    //
    err = deflateInit2(&stream, arch->user_flags[4], Z_DEFLATED, -MAX_WBITS, 9, Z_DEFAULT_STRATEGY);
    if (err == Z_OK) {
      err = deflate(&stream, Z_FINISH);
      deflateEnd(&stream);
      if (err == Z_STREAM_END) err = Z_OK;
      deflateEnd(&stream);
    }
    //
    if (err != Z_OK)
      return false; // error deflate data
    //
    FP.cSize = x->csize = stream.total_out;
    FP.set();
    if(!arch->filew->writer_write(FP.bytes, FP.SIZE)
      || !arch->filew->writer_write(media_name.text, media_name.length)
      || !arch->filew->writer_write(data.data, x->csize))
      return false; // error save a data
  }
  //
  return true;
}

//***************************************

IStreamReader::P RBinArchiveFrameClient_ZIP::data_reader(IBinArchiveFrame* arch, NameNode* node, const SFlags& flags)
{
  if(!arch->utils.valid())
    return NULL;
  if(node->length==0) {
    return arch->utils->mem_reader(SCMem(0,0));
  }
  //
  IStreamReader *rd = node->_node_flag1 ? arch->filerw : arch->filer;
  if(rd==NULL)
    return NULL;
  //
  if(fi(node)->compression==co_Store) { // try to open store one
    if(!rd->reader_seek(fi(node)->pos))
      return NULL;
    //
    fileFilePrefix::Holder FP;
    if(!rd->reader_read(FP.bytes, FP.SIZE))
      return NULL;
    FP.get();
    if(FP.sig!=FP.SIGNATURE)
      return NULL;
    //
    if(FP.compression==co_Store) {
      if(FP.ucSize==FP.cSize && FP.cSize==fi(node)->csize) { // use sub-stream
        return arch->utils->sub_reader(rd, fi(node)->pos+FP.SIZE+FP.fnameLen+FP.xtraLen, FP.cSize);
      }
      return NULL; // error in header
    } else if(FP.compression!=co_Deflate) {
      return NULL; // bad compression
    }
    // bad entry in dir, use ordinal memory entry
  }
  // use memory stream data
  SCMemShared mem;
  if(!data_load(arch, mem, node, flags))
    return NULL; // no file
  return arch->utils->mem_reader(mem);
}

//***************************************

IStreamWriter::P RBinArchiveFrameClient_ZIP::data_writer(IBinArchiveFrame* arch, const DCString& media_name, const SFlags& flags)
{
  return NULL;
}

//***************************************

bool RBinArchiveFrameClient_ZIP::node_info(IBinArchiveFrame *arc, SFileInfo& info, NameNode* node)
{
  info.entry_flags = 0;
  FileInfo *x = fi(node);
  info.handle = x->pos;
  return true;
}

//***************************************

bool RBinArchiveFrameClient_ZIP::arc_init(IBinArchiveFrame* arch, const SParameters* params) 
{
  arch->ignore_case = true;
  arch->user_flags[3] = co_Deflate;
  if(params->get("compression", arch->user_flags[4])) {
    if(arch->user_flags[4]<=0) {
      arch->user_flags[3] = co_Store;
    } else if(arch->user_flags[4]>9) {
      arch->user_flags[4] = 9;
    }
  } else {
    arch->user_flags[4] = 9;
  }
  return true;
}

bool RBinArchiveFrameClient_ZIP::arc_deinit(IBinArchiveFrame* arch) 
{
  return false;
}

//***************************************

bool RBinArchiveFrameClient_ZIP::filer_pre_scan(IBinArchiveFrame* arch, size_t &rspace, const SParameters* params)
{
  fileDirHeader::Holder H;
  //
  if(!arch->filer->reader_seekend(H.SIZE))
    return false;
  if(!arch->filer->reader_read(H.bytes, H.SIZE))
    return false;
  //
  H.get(); // fill cached variabled
  //
  if(H.sig!=H.SIGNATURE)
    return false; // not a Zip
  //
  if(!arch->filer->reader_seek(H.dirOffset))
    return false;
  //
  arch->user_flags[0] = H.dirOffset;
  arch->user_flags[1] = H.nDirEntries;
  //
  for(int i=0; i<H.nDirEntries; i++) {
    fileDirEntry::Holder E;
    BChar tmpbuf[syshlp::MAX_FILENAME_LEN];
    if(!arch->filer->reader_read(E.bytes, E.SIZE))
      return false;
    E.get();
    if(E.sig!=E.SIGNATURE || E.fnameLen>=syshlp::MAX_FILENAME_LEN)
      return false; // bad signature or filename to long
    if(!arch->filer->reader_seekdelta(E.fnameLen+E.cmntLen+E.xtraLen))
      return false;
    DCString name(tmpbuf, E.fnameLen);
    rspace += arch->node_size(name, sizeof(FileInfo));
  }
  return true;
}

bool RBinArchiveFrameClient_ZIP::filer_scan(IBinArchiveFrame* arch, SMem& memto, const SParameters* params)
{
  if(!arch->filer->reader_seek(arch->user_flags[0]))
    return false;
  uint32 count = arch->user_flags[1];
  //
  for(uint32 i=0; i<count; i++) {
    fileDirEntry::Holder E;
    BChar tmpbuf[syshlp::MAX_FILENAME_LEN];
    if(!arch->filer->reader_read(E.bytes, E.SIZE))
      return false;
    E.get();
    if(E.sig!=E.SIGNATURE || E.fnameLen>=syshlp::MAX_FILENAME_LEN)
      return false; // bad signature or filename to long
    if(!arch->filer->reader_read(tmpbuf, E.fnameLen))
      return false;
    tmpbuf[E.fnameLen] = 0;
    DCString name(tmpbuf);
    NameNode *n = arch->add_node_r(name, sizeof(FileInfo), memto);
    if(n==NULL)
      return false; // duplicate name or bad memory space
    if(!arch->filer->reader_seekdelta(E.cmntLen+E.xtraLen))
      return false;
    //
    n->length = E.ucSize;
    FileInfo *x = fi(n);
    x->pos = E.hdrOffset;
    x->csize = E.cSize;
    x->compression = E.compression;
    x->attr = E.intAttr;
    x->xattr = E.extAttr;
    x->mt = E.modTime;
    x->md = E.modDate;
  }
  return true;
}

//***************************************

bool RBinArchiveFrameClient_ZIP::filew_init(IBinArchiveFrame* arch, const SParameters* params)
{
  if(!arch->filew->writer_seek(0))
    return false;
  return true;
}

bool RBinArchiveFrameClient_ZIP::filew_copy_data(IBinArchiveFrame* arch, NameNode* node)
{
  FileInfo *x = fi(node);
  uint32 pos1 = x->pos;
  uint32 pos2 = arch->filew->writer_pos();
  fileFilePrefix::Holder FP;
  if(!arch->filer->reader_read(FP.bytes, FP.SIZE))
    return false;
  FP.get();
  if(FP.sig!=FP.SIGNATURE || FP.ucSize!=node->length || FP.cSize!=x->csize)
    return false; // invalid FP
  if(arch->filew->writer_write(FP.bytes, FP.SIZE) 
     && arch->filew->writer_copy(arch->filer, pos1, node->length+FP.fnameLen+FP.xtraLen)) {
    x->pos = pos2;
    node->_node_flag1 = 2; // written
    return true;
  } else {
    arch->filew->writer_seek(pos2);
    return false;
  }
  return true;
}
bool RBinArchiveFrameClient_ZIP::filew_writedir_start(IBinArchiveFrame* arch)
{
  arch->user_flags[0] = arch->filew->writer_pos();
  arch->user_flags[1] = 0;
  arch->user_flags[2] = 0;
  return true;
}

bool RBinArchiveFrameClient_ZIP::filew_writedir_entry(IBinArchiveFrame* arch, NameNode* node)
{
  if(node->_node_flag1==0)
    return true; // skipped R file
  FileInfo *x = fi(node);
  fileDirEntry::Holder E;
  E.sig = E.SIGNATURE;
  E.verMade = zv_Archiver;
  E.verNeed = zv_Minimal;
  E.flag = 0;
  E.compression = x->compression;
  E.modTime = x->mt;
  E.modDate = x->md;
  E.crc32 = x->crc;
  E.cSize = x->csize;
  E.ucSize = node->length;
  E.xtraLen = 0;
  E.cmntLen = 0;
  E.diskStart = 0;
  E.intAttr = x->attr;
  E.extAttr = x->xattr;
  E.fnameLen = node->name.length;
  E.hdrOffset = x->pos;
  E.set();
  if(!arch->filew->writer_write(E.bytes, E.SIZE)
    || !arch->filew->writer_write(node->name.text, node->name.length))
    return false;
  arch->user_flags[2]+= E.SIZE + node->name.length;
  arch->user_flags[1]++;
  return true;
}

bool RBinArchiveFrameClient_ZIP::filew_writedir_end(IBinArchiveFrame* arch)
{
  fileDirHeader::Holder H;
  H.sig = H.SIGNATURE;
  H.nDisk = H.nStartDisk = 0;
  H.totalDirEntries = H.nDirEntries = arch->user_flags[1];
  H.dirSize = arch->user_flags[2];
  H.dirOffset = arch->user_flags[0];
  H.cmntLen = 0;
  H.set();
  if(!arch->filew->writer_seekend(0) || !arch->filew->writer_write(H.bytes, H.SIZE))
    return false; // can't write header
  return true;
}

//***************************************
// END
//***************************************
