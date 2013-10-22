
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
  };
  typedef tl::TScatterArray< SCacheElem, DCString > DCache;
  //
  DCache cache;
  DMediaFlags auto_values;
  //
  RLibraryBinCache(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags())
  : refs(own), auto_values(av)/*, cache(local_memory())*/ {
  }
  ~RLibraryBinCache(void) {
  }
  //
  // SFlags::IResolver
  DMediaFlags::eStates get_parent_flag(int shift) const {
    return auto_values.get_s(shift);
  }
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
  bool bin_put(const SCMemShared& mem, const DCString& media_name) {
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
    return obj_std_get(obj, media_name, opts);
  }
  bool obj_load(IRefObject* obj, const DCString& media_name, const SObjOptions& opts) {
    return obj_std_load(obj, media_name, opts);
  }
  bool obj_save(IRefObject* obj, const DCString& media_name, const SObjOptions& opts) {
    return obj_std_save(obj, media_name, opts);
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

//***************************************
// END
//***************************************
