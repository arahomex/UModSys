
//***************************************
// RStreamReader_FILE::
//***************************************

#define OSDIR_START(rfail, name, req) \
  if((pm & req)==0 || ~name==0) \
    return rfail; \
  SVFileName xname(prefix.get_text(), name, false); \
  if(!xname.valid) \
    return rfail; \

struct RDataArchiver_OsDir : public IBinArchive 
{
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RDataArchiver_OsDir, 1, IBinArchive)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  int pm;
  DStringShared prefix;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
  //
  RDataArchiver_OsDir(DOwner *own, const SParameters& args, DMediaFlags ff=DMediaFlags())
  : refs(own), flags(ff) {
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
  // SFlags::ISetter
  DMediaFlags::eStates get_flag(int shift) const { return flags.get_s(shift); }
  DMediaFlags get_flags_auto(void) const { return auto_flags; }
  DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag) { return flags.getset_s(shift, flag); }
  //
  bool data_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags) {
    s_dbg.put(0, cl_Debug, "{{os load %s}}", media_name());
    return archive_load_data(*this, media_name, mem, flags);
  }
  bool data_save(const DCString& media_name, const SCMem& mem, const SFlags& flags) {
    return archive_save_data(*this, media_name, mem, flags);
  }
  IStreamReader::P data_reader(const DCString& media_name, const SFlags& flags) {
    OSDIR_START(NULL, media_name, mp_Read);
    IStreamReader::P rv;
    if(!ValidateConstruction(rv, new(M()) RStreamReader_FILE(refs.owner, xname)))
      return NULL;
    return rv;
  }
  IStreamWriter::P data_writer(const DCString& media_name, const SFlags& flags) {
    OSDIR_START(NULL, media_name, mp_Write);
    IStreamWriter::P rv;
    if(!ValidateConstruction(rv, new(M()) RStreamWriter_FILE(refs.owner, xname, flags.yes<mf_safe>(this) )))
      return NULL;
    return rv;
  }
  bool data_list(const DCString& mask, size_t namestart, DIFileInfoArray& list, const SFlags& flags) {
    OSDIR_START(0, mask, mp_List);
    return false;
  }
  bool data_info(const DCString& mask, size_t namestart, SFileInfo& list, const SFlags& flags) {
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
