
//***************************************
// RLibraryBinCache::
//***************************************

struct RLibraryBinCache : public ILibraryBinCache
{
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(media,std)::RLibraryBinCache, 1, ILibraryBinCache)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  struct SCacheElem {
    DStringShared name;
    SCMemShared bin;
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
  RLibraryBinCache(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags())
  : refs(own), flags(av)/*, cache(local_memory())*/ {
  }
  ~RLibraryBinCache(void) {
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
  bool bin_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags) {
    if(!flags.yes<mf_cache>(this))
      return false;
    bool v;
    if(!bincache_get(media_name, &mem, &v))
      return false;
    if(!v && !flags.yes<mf_failuse>(this))
      return false;
    return true;
  }
  bool bin_save(const DCString& media_name, const SCMem& mem, const SFlags& flags) {
    return false;
  }
  bool bin_info(const DCString& media_name, SFileInfo& info, const SFlags& flags) {
    return false;
  }
  bool bin_info(const DCString& media_mask, DIFileInfoArray& info, const SFlags& flags) {
    return false;
  }
  bool bin_get(const DCString& media_name, SCMemShared& mem, bool isinv) {
    bool v;
    if(!bincache_get(media_name, &mem, &v))
      return false;
    if(!v && !isinv)
      return false;
    return true;
  }
  bool bin_put(const DCString& media_name, const SCMemShared* mem) {
    if(mem==NULL)
      return bincache_add(media_name);
    return bincache_add(media_name, *mem);
  }
  // general object functions
  bool obj_fget(const IBinObjFilter::SInfo& info, IRefObject::P& obj) {
    return false;
  }
  bool obj_fload(const IBinObjFilter::SInfo& info, IRefObject* obj) {
    return false;
  }
  bool obj_fsave(IBinObjFilter::SInfo& info, IRefObject* obj) {
    return false;
  }
  bool obj_cget(const DCString& media_name, IRefObject::P& obj, bool isinv) {
    return false;
  }
  bool obj_cput(const DCString& media_name, IRefObject* obj) {
    return false;
  }
  // universal object functions
  bool obj_get(const DCString& media_name, IRefObject::P& obj, const SObjOptions& opts) {
    return false;
  }
  bool obj_load(const DCString& media_name, IRefObject* obj, const SObjOptions& opts) {
    return false;
  }
  bool obj_save(const DCString& media_name, IRefObject* obj, const SObjOptions& opts) {
    return false;
  }
  //
  // binary cache
  bool bincache_get(const DCString& media_name, SCMemShared* mem, bool *valid) const {
    if(mem!=NULL)
      mem->clear();
    SCacheElem* e = cache(media_name);
    if(e==NULL)
      return false;
    if(mem!=NULL)
      *mem = e->bin;
    if(valid!=NULL)
      *valid = e->valid;
    return false;
  }
  bool bincache_add(const DCString& media_name) {
    return bincache_add(media_name, SCMemShared(), false);
  }
  bool bincache_add(const DCString& media_name, const SCMemShared& mem) {
    return bincache_add(media_name, mem, true);
  }
  bool bincache_remove(const DCString& media_name) {
    return cache.Remove(media_name);
  }
  bool bincache_clear(void) {
    cache.Clear();
    return true;
  }
  //
  inline bool bincache_add(const DCString& media_name, const SCMemShared& mem, bool valid)
  {
    SCacheElem* e = cache(media_name);
    if(e==NULL) {
      DStringShared n2 = media_name;
      e = cache(n2(), void_null());
      if(e==NULL)
        return false;
      e->name = n2;
    }
    e->bin = mem;
    e->valid = valid;
    return true;
  }
  //
  inline bool validate_construction(void) const { return true; }
};

//***************************************
// END
//***************************************
