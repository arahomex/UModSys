
//***************************************
// RLibraryObjCache::
//***************************************

struct RLibraryObjCache : public ILibraryObjCache
{
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RLibraryObjCache, 1, ILibraryObjCache)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  struct SCacheElem {
    DStringShared name;
    IRefObject::P obj;
    bool valid;
    //
    SCacheElem(void) : valid(false) {}
  };
  typedef tl::TScatterArray< SCacheElem, DCString, TObjectCompare<DCString> > DCache;
  //
  DCache cache;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
  //
  RLibraryObjCache(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags())
  : refs(own), flags(av)/*, cache(local_memory())*/ {
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
  bool bin_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags) {
    return false;
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
    return false;
  }
  bool bin_put(const DCString& media_name, const SCMemShared* mem) {
    return false;
  }
  // general object functions
  bool obj_fget(const IBinObjFilter::SInfo& info, IRefObject::P& obj) {
    return false;
  }
  bool obj_fload(const IBinObjFilter::SInfo& info, IRefObject* obj) {
    return false;
  }
  bool obj_fsave(IBinObjFilter::SInfo& info, const IRefObject* obj) {
    return false;
  }
  bool obj_cget(const DCString& media_name, IRefObject::P& obj, bool isinv) {
    bool v;
    if(!objcache_get(media_name, &obj, &v))
      return false;
    if(!v && !isinv)
      return false;
    return true;
  }
  bool obj_cput(const DCString& media_name, IRefObject* obj) {
    return objcache_add(media_name, obj);
  }
  // universal object functions
  bool obj_get(const DCString& media_name, IRefObject::P& obj, const SObjOptions& opts) {
    SFlagsChain f2(opts, this);
    if(f2.yes<mf_objects>(this) && obj_cget(media_name, obj, f2.yes<mf_nulluse>(this)))
      return true;
    return false;
  }
  bool obj_load(const DCString& media_name, IRefObject* obj, const SObjOptions& opts) {
    return false;
  }
  bool obj_save(const DCString& media_name, const IRefObject* obj, const SObjOptions& opts) {
    return false;
  }
  //
  // object cache 
  bool objcache_get(const DCString& media_name, IRefObject::P* obj, bool *valid) const {
    if(obj!=NULL)
      obj->clear();
    SCacheElem* e = cache(media_name);
    if(e==NULL)
      return false;
    if(obj!=NULL)
      *obj = e->obj;
    if(valid!=NULL)
      *valid = e->valid;
    return true;
  }
  bool objcache_add(const DCString& media_name, IRefObject* obj) {
    SCacheElem* e = cache(media_name);
    if(e==NULL) {
      DStringShared n2 = media_name;
      e = cache(n2(), void_null());
      if(e==NULL)
        return false;
      e->name = n2;
    }
    e->obj = obj;
    e->valid = obj!=NULL;
    return true;
  }
  bool objcache_remove(const DCString& media_name) {
    return cache.Remove(media_name);
  }
  bool objcache_clear(void) {
    cache.Clear();
    return true;
  }
  //
  // utility
  inline bool validate_construction(void) const { return true; }
};

//***************************************
// END
//***************************************
