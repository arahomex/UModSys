
//***************************************
// RStreamReader_FILE::
//***************************************

#define OSDIR_START(rfail, name, req) \
  if((pm & req)==0 || ~name==0) \
    return rfail; \
  SVFileName xname(prefix(), name, false); \
  if(!xname.valid) \
    return rfail; \

struct RDataArchiver_OsDir : public IDataArchive 
{
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(media,stdio)::RDataArchiver_OsDir, 1, IDataArchive)
  UMODSYS_REFOBJECT_REFOTHER(RGenerator);
  //
  int pm;
  DStringShared prefix;
  DMediaFlags auto_values;
  //
  RDataArchiver_OsDir(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags())
  : refs(M, own), auto_values(av) {
    open(args);
  }
  ~RDataArchiver_OsDir(void) {
  }
  //
  static bool scan_entries(const DCString &prefix, const DCString &mask, DIFileInfoArray& list) {
#if 0
    SMediaFileInfo tmp, *x = list.list ? &tmp : NULL;
    if(platform::_fs_scan(prefix, "", mask, false, x, &list, use_scan))
      return true;
#endif
    return false;
  }
  static bool use_scan(void *context, BCStr path, BCStr name, SFileInfo* inf) {
#if 0
    DIFileInfoArray* s = (DIFileInfoArray*)context;
    if(s->list==NULL) {
      s->count++;
      return true;
    }
    if(inf==NULL)
      return false; // bad INF
    //
    if(s->count>=s->max_count)
      return false;
    SMediaFileInfo& x = s->list[s->count++];
    x = *inf;
    x.name.alloc(name);
#endif
    return true;
  }
  //
  inline void open(const SParameters& args) { 
    BCStr filename = "."; pm = mp_All;
    if(args.get("path", filename) || args.get("pathname", filename)) {
//      open(filename);
    }
    args.get("permissions", pm);
    SVFileName xname(filename, true);
    prefix = xname+1;
  }
  inline bool validate_construction(void) const { return true; }
  //
  bool load_data(const DCString& media_name, SCMemShared& mem, const SFlags& flags) {
    OSDIR_START(false, media_name, mp_Read);
    IStreamReader::P rv;
    mem.clear();
    if(!ValidateConstruction(rv, new(M()) RStreamReader_FILE(refs.owner, xname)))
      return false;
    DFilePosition sz = rv->reader_size();
    if(sz>mem_max_allocation) {
      rv->reader_close();
      return false; // max size not allowed
    }
    size_t msize = size_t(sz);
    if(msize==0)
      return true; // ok to load zero size file
    SMemShared m2(msize);
    if(!m2.valid() || m2.get_size()!=msize) {
      rv->reader_close();
      return false; // max size not allowed
    }
    if(!rv->reader_read(m2.get_data(), m2.get_size())) {
      rv->reader_close();
      return false; // max size not allowed
    }
    mem = m2;
    return true;
  }
  IStreamReader::P load_reader(const DCString& media_name, const SFlags& flags) {
    OSDIR_START(NULL, media_name, mp_Read);
    IStreamReader::P rv;
    if(!ValidateConstruction(rv, new(M()) RStreamReader_FILE(refs.owner, xname)))
      return NULL;
    return rv;
  }
  bool save_data(const DCString& media_name, const SCMem& mem, const SFlags& flags) {
    OSDIR_START(false, media_name, mp_Write);
    IStreamWriter::P rv;
    if(!ValidateConstruction(rv, new(M()) RStreamWriter_FILE(refs.owner, xname, flags.yes<mf_safe>(auto_values) )))
      return false;
    if(!rv->writer_write(mem.data, mem.size)) {
      rv->writer_abort();
      return false;
    }
    rv->writer_close();
    return true;
  }
  IStreamWriter::P save_writer(const DCString& media_name, const SFlags& flags) {
    OSDIR_START(NULL, media_name, mp_Write);
    IStreamWriter::P rv;
    if(!ValidateConstruction(rv, new(M()) RStreamWriter_FILE(refs.owner, xname, flags.yes<mf_safe>(auto_values) )))
      return NULL;
    return rv;
  }
  bool get_entrylist(const DCString &mask, tl::TIStackSocket<SFileInfo>& list) {
    OSDIR_START(0, mask, mp_List);
    return false;
  }
  int get_permissions(void) {
    return pm;
  }
};

#undef OSDIR_START

//***************************************
// END
//***************************************
