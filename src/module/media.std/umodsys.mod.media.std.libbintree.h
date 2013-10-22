
//***************************************
// RLibraryBinCache::
//***************************************

struct RLibraryBinTree : public ILibraryBinTree
{
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(media,std)::RLibraryBinTree, 1, ILibraryBinTree)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  struct SMountIndex {
    int order;
    DCString mnt;
    //
    inline SMountIndex(int o, const DCString &m) : mnt(m), order(o) {}
    inline SMountIndex(const DCString &m, int o) : mnt(m), order(o) {}
    //
    static int compare(const SMountIndex& L, const SMountIndex& R) { 
      int rv = -scalar_compare(L.order, R.order); // revert order, a>b mean a before b
      if(rv) return rv;
      return scalar_compare(L.mnt, R.mnt);
    }
  };
  struct SMountInfo : SMountPoint {
    DStringShared mount_name;
    //
    void operator=(const SMountPoint& r) { SMountPoint::operator=(r); }
  };
  typedef tl::TScatterArray< SMountInfo, SMountIndex, SMountIndex > DMounts;
  //
  DMounts mounts;
  DMediaFlags auto_values;
  //
  RLibraryBinTree(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags())
  : refs(own), auto_values(av)/*, cache(local_memory())*/ {
  }
  ~RLibraryBinTree(void) {
  }
  //
  // SFlags::IResolver
  DMediaFlags::eStates get_parent_flag(int shift) const {
    return auto_values.get_s(shift);
  }
  //
  // general data functions
  IStreamReader::P bin_reader(const DCString& media_name, const SFlags& flags) {
    for(DMounts::const_iterator x=mounts.begin(), e=mounts.end(); x!=e; x++) {
      const DCString &mnt = x->mount_name();
      if(~media_name<=~mnt || !tl::su::seq(mnt(), media_name(), ~mnt) 
         || !x->archive.valid() || !(x->pm & mp_Read))
        continue;
      IStreamReader::P rv = x->archive->data_reader(media_name + ~mnt, flags);
      if(rv.valid())
        return rv;
    }
    return NULL;
  }
  IStreamWriter::P bin_writer(const DCString& media_name, const SFlags& flags) {
    for(DMounts::const_iterator x=mounts.begin(), e=mounts.end(); x!=e; x++) {
      const DCString &mnt = x->mount_name();
      if(~media_name<=~mnt || !tl::su::seq(mnt(), media_name(), ~mnt) 
         || !x->archive.valid() || !(x->pm & mp_Write))
        continue;
      IStreamWriter::P rv = x->archive->data_writer(media_name + ~mnt, flags);
      if(rv.valid())
        return rv;
    }
    return NULL;
  }
  bool bin_load(SCMemShared& mem, const DCString& media_name, const SFlags& flags) {
    mem.clear();
    for(DMounts::const_iterator x=mounts.begin(), e=mounts.end(); x!=e; x++) {
      const DCString &mnt = x->mount_name();
      if(~media_name<=~mnt || !tl::su::seq(mnt(), media_name(), ~mnt) 
         || !x->archive.valid() || !(x->pm & mp_Read))
        continue;
      if(x->archive->data_load(media_name + ~mnt, mem, flags))
        return true;
    }
    return false;
  }
  bool bin_save(const SCMem& mem, const DCString& media_name, const SFlags& flags) {
    for(DMounts::const_iterator x=mounts.begin(), e=mounts.end(); x!=e; x++) {
      const DCString &mnt = x->mount_name();
      if(~media_name<=~mnt || !tl::su::seq(mnt(), media_name(), ~mnt) 
         || !x->archive.valid() || !(x->pm & mp_Read))
        continue;
      if(x->archive->data_save(media_name + ~mnt, mem, flags))
        return true;
    }
    return false;
  }
//  virtual bool bin_info(SMediaFileInfo& info, const DCString& media_name, const SFlags& flags=SFlags()) =0; // determine attributes
//  virtual bool bin_info(SMediaFileInfoArray& info, const DHString& media_mask, const SFlags& flags=SFlags()) =0; // determine attributes
  bool bin_get(SCMemShared& mem, const DCString& media_name) {
    return false;
  }
  bool bin_put(const SCMemShared& mem, const DCString& media_name) {
    return false;
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
  // sub-archives
  SMountPoint mount_get(const DCString& mountpoint, int order) const {
    const SMountInfo* x = mounts(SMountIndex(mountpoint, order));
    if(x==NULL)
      return SMountPoint();
    return *x;
  }
  bool mount_add(const SMountPoint& mp, const DCString& mountpoint, int order) {
    SMountInfo* x = mounts(SMountIndex(mountpoint, order));
    if(x==NULL) {
      DStringShared mn2 = mountpoint;
      x = mounts(SMountIndex(mn2(), order), void_null());
      if(x==NULL)
        return false;
      x->mount_name = mn2;
    } 
    *x = mp;
    return true;
  }
  bool mount_remove(const DCString& mountpoint, int order) {
    return mounts.Remove(SMountIndex(mountpoint, order));
  }
  bool mount_clear(void) {
    mounts.Clear();
    return true;
  }
  //
  //
  inline bool validate_construction(void) const { return true; }
};

//***************************************
// END
//***************************************
