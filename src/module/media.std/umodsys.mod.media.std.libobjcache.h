
//***************************************
// RLibraryObjCache::
//***************************************

#if 0
struct RLibraryObjCache : public ILibraryObjCache
{
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(media,std)::RLibraryObjCache, 1, ILibraryObjCache)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  struct SCacheElem {
    DStringShared name;
    IRefObject::P obj;
    bool valid;
    //
    SCacheElem(void) : valid(false) {}
  };
  typedef tl::TScatterArray< SCacheElem, DCString > DCache;
  //
  DCache cache;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
  //
  RLibraryObjCache(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags())
  : refs(own), auto_values(av)/*, cache(local_memory())*/ {
  }
  ~RLibraryObjCache(void) {
  }
  //
  // SFlags::ISetter
  DMediaFlags::eStates get_flag(int shift) const { return flags.get_s(shift); }
  DMediaFlags get_flags_auto(void) const { return auto_flags; }
  DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag) { return flags.getset_s(shift, flag); }
  //
  // general data functions
  IStreamReader::P bin_reader(const DCString& media_name, const SFlags& flags) {
    return NULL;
  }
  IStreamWriter::P bin_writer(const DCString& media_name, const SFlags& flags) {
    return NULL;
  }
  bool bin_load(SCMemShared& mem, const DCString& media_name, const SFlags& flags) {
    if(!flags.yes<mf_cache>(auto_values))
      return false;
    return bincache_get(mem, media_name);
  }
  bool bin_save(const SCMem& mem, const DCString& media_name, const SFlags& flags) {
    return false;
  }
//  virtual bool bin_info(SMediaFileInfo& info, const DCString& media_name, const SFlags& flags=SFlags()) =0; // determine attributes
//  virtual bool bin_info(SMediaFileInfoArray& info, const DHString& media_mask, const SFlags& flags=SFlags()) =0; // determine attributes
  bool bin_get(SCMemShared& mem, const DCString& media_name) {
    return bincache_get(mem, media_name);
  }
  bool bin_put(const SCMemShared* mem, const DCString& media_name) {
    return bincache_add(mem, media_name);
  }
  // general object functions
  bool obj_fget(IRefObject::P& obj, const IBinObjFilter::SInfo& info) {
    return false;
  }
  bool obj_fload(IRefObject* obj, const IBinObjFilter::SInfo& info) {
    return false;
  }
  bool obj_fsave(IRefObject* obj, IBinObjFilter::SInfo& info) {
    return false;
  }
  bool obj_cget(IRefObject::P& obj, const DCString& media_name) {
    return false;
  }
  bool obj_cput(IRefObject* obj, const DCString& media_name) {
    return false;
  }
  // universal object functions
  bool obj_get(IRefObject::P& obj, const DCString& media_name, const SObjOptions& opts) {
    return false;
  }
  bool obj_load(IRefObject* obj, const DCString& media_name, const SObjOptions& opts) {
    return false;
  }
  bool obj_save(IRefObject* obj, const DCString& media_name, const SObjOptions& opts) {
    return false;
  }
  //
  // binary cache
  bool bincache_get(SCMemShared& mem, const DCString& media_name) const {
    mem.clear();
    SCacheElem* e = cache(media_name);
    if(e==NULL)
      return false;
    mem = e->bin;
    return false;
  }
  bool bincache_add(const SCMemShared& mem, const DCString& media_name) {
    SCacheElem* e = cache(media_name);
    if(e==NULL) {
      DStringShared n2 = media_name;
      e = cache(n2(), void_null());
      if(e==NULL)
        return false;
      e->name = n2;
    }
    e->bin = mem;
    return true;
  }
  bool bincache_remove(const DCString& media_name) {
    return cache.Remove(media_name);
  }
  bool bincache_clear(void) {
    cache.Clear();
    return true;
  }
  //
  //
  inline bool validate_construction(void) const { return true; }
};
#endif

//***************************************
// END
//***************************************
