
//***************************************
// RStreamReader_FILE::
//***************************************

inline DFilePosition file_get_size(FILE* file)
{
  if(file==NULL)
    return 0;
  DFilePosition cur = ftell(file);
  fseek(file, 0, SEEK_END);
  DFilePosition endsize = ftell(file);
  fseek(file, cur, SEEK_SET);
  return endsize;
}

struct RStreamReader_FILE : public IStreamReader 
{
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(media,stdio)::RStreamReader_FILE, 1, IStreamReader)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  FILE *file;
  DFilePosition endsize;
  //
  ~RStreamReader_FILE(void) {
    reader_close();
  }
  RStreamReader_FILE(DOwner *own, const SParameters& args)
  : refs(own), file(NULL), endsize(0) {
    open(args);
  }
  RStreamReader_FILE(DOwner *own, BCStr osname)
  : refs(own), file(NULL), endsize(0) {
    open(osname);
  }
  //
  inline void open(const SParameters& args) { 
    BCStr filename;
    if(args.get("filename", filename)) {
      open(filename);
    }
  }
  inline void open(BCStr osname) { file = syshlp::c_fopen(osname, "rb"); get_size(); }
  inline void open(FILE *f) { file = f; get_size(); }
  inline bool validate_construction(void) const { return file!=NULL; }
  inline void get_size(void) { endsize = file_get_size(file); }
  //
  bool reader_seek(DFilePosition pos) {
    return file && !fseek(file, pos, SEEK_SET);
  }
  bool reader_seekend(DFilePosition pos) {
    return file && !fseek(file, -pos, SEEK_END);
  }
  bool reader_seekdelta(DFilePosition pos) {
    return file && !fseek(file, pos, SEEK_CUR);
  }
  DFilePosition reader_pos(void) {
    return file ? ftell(file) : 0;
  }
  DFilePosition reader_size(void) {
    return endsize;
  }
  bool reader_read(void* data, size_t size) {
    if(size==0)
      return true;
    if(!file || !data)
      return false;
    DFilePosition p = ftell(file);
    if(fread(data, 1, size, file)==size)
      return true; // ok
    //
    fseek(file, p, SEEK_SET);
    return false;
  }
  void reader_close(void) {
    if(file) {
      fclose(file);
      file = NULL;
    }
  }
};

//***************************************
// RStreamWriter_FILE::
//***************************************

struct RStreamWriter_FILE : public IStreamWriter
{
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(media,stdio)::RStreamWriter_FILE, 1, IStreamWriter)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  bool changed;
  FILE *file;
  BWStr handle;
  DFilePosition endsize;
  //
  ~RStreamWriter_FILE(void) {
    writer_close();
  }
  RStreamWriter_FILE(DOwner *own, const SParameters& args)
  : refs(own), handle(NULL), file(NULL), endsize(0), changed(true) {
    open(args);
  }
  RStreamWriter_FILE(DOwner *own, BCStr osname, bool safe)
  : refs(own), handle(NULL), file(NULL), endsize(0), changed(true) {
    open(osname, safe);
  }
  //
  inline void open(const SParameters& args) { 
    BCStr filename;
    bool safe = false;
    args.get("safe", safe);
    if(args.get("filename", filename)) {
      open(filename, safe);
    }
  }
  inline void open(BCStr osname, bool safe) { 
    if(safe) {
      file = syshlp::c_fopentemp(handle, osname);
    } else {
      file = syshlp::c_fopen(osname, "wb"); 
    }
    get_size();
  }
  inline void open(FILE *f, BWStr h) { file = f; handle = h; get_size(); }
  inline void get_size(void) { if(changed) { endsize = file_get_size(file); changed=false; } }
  inline bool validate_construction(void) const { return file!=NULL; }
  //
  bool writer_seek(DFilePosition pos) {
    return file && !fseek(file, pos, SEEK_SET);
  }
  bool writer_seekend(DFilePosition pos) {
    return file && !fseek(file, -pos, SEEK_END);
  }
  bool writer_seekdelta(DFilePosition pos) {
    return file && !fseek(file, pos, SEEK_CUR);
  }
  bool writer_setsize(DFilePosition pos) {
    if(!file)
      return false;
    fflush(file);
    int rv = _chsize(_fileno(file), pos);
    fflush(file);
    return rv!=0;
  }
  DFilePosition writer_pos(void) {
    return file ? ftell(file) : 0;
  }
  DFilePosition writer_size(void) {
    get_size();
    return endsize;
  }
  bool writer_write(const void* data, size_t size) {
    if(size==0)
      return true;
    if(!file || !data)
      return false;
    DFilePosition p = ftell(file);
    if(fwrite(data, 1, size, file)==size)
      return true; // ok
    fseek(file, p, SEEK_SET);
    return false;
  }
  bool writer_copy(IStreamReader* from, DFilePosition count) {
    if(from==NULL)
      return false;
    char block[1024];
    while(count>0) {
      int cnt = count>sizeof(block) ? sizeof(block) : count;
      if(!from->reader_read(block, cnt))
        return false;
      if(!writer_write(block, cnt))
        return false;
      count -= cnt;
    }
    return true;
  }
  bool writer_copy(IStreamReader* from, DFilePosition start, DFilePosition count) {
    if(from==NULL)
      return false;
    if(!from->reader_seek(start))
      return false;
    return writer_copy(from, count);
  }
  void writer_close(void) {
    if(handle) {
      syshlp::c_fendtemp(file, handle, true);
    }
    if(file) {
      fclose(file);
      file = NULL;
    }
  }
  void writer_abort(void) {
    if(handle) {
      syshlp::c_fendtemp(file, handle, false);
    }
    if(file) {
      fclose(file);
      file = NULL;
    }
  }
};

//***************************************
// END
//***************************************
