
//***************************************
// RLibraryBinTree::
//***************************************

struct RLibraryBinTree : public ILibraryBinTree
{
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(media,std)::RLibraryBinTree, 1, ILibraryBinTree)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  typedef SId SMountIndex;
  //
  struct SMountInfo : SPoint {
    DStringShared mount_name;
    //
    void operator=(const SPoint& r) { SPoint::operator=(r); }
  };
  typedef tl::TScatterArray< SMountInfo, SMountIndex, SMountIndex > DMounts;
  //
  DMounts mounts;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
  //
  RLibraryBinTree(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags())
  : refs(own), flags(av)/*, cache(local_memory())*/ {
  }
  ~RLibraryBinTree(void) {
  }
  //
  // SFlags::ISetter
  DMediaFlags::eStates get_flag(int shift) const { return flags.get_s(shift); }
  DMediaFlags get_flags_auto(void) const { return auto_flags; }
  DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag) { return flags.getset_s(shift, flag); }
  //
  // general data functions
  IStreamReader::P bin_reader(const DCString& media_name, const SFlags& flags) {
    for(DMounts::CIter x=mounts(); x; ++x) {
      const SMountInfo& mi = x->second;
      const DCString &mnt = mi.mount_name();
      if(~media_name<=~mnt || !tl::su::seq(mnt(), media_name(), ~mnt) 
         || !mi.archive.valid() || !(mi.pm & mp_Read))
        continue;
      IStreamReader::P rv = mi.archive->data_reader(media_name + ~mnt, flags);
      if(rv.valid())
        return rv;
    }
    return NULL;
  }
  IStreamWriter::P bin_writer(const DCString& media_name, const SFlags& flags) {
    for(DMounts::CIter x=mounts(); x; ++x) {
      const SMountInfo& mi = x->second;
      const DCString &mnt = mi.mount_name();
      if(~media_name<=~mnt || !tl::su::seq(mnt(), media_name(), ~mnt) 
         || !mi.archive.valid() || !(mi.pm & mp_Write))
        continue;
      IStreamWriter::P rv = mi.archive->data_writer(media_name + ~mnt, flags);
      if(rv.valid())
        return rv;
    }
    return NULL;
  }
  bool bin_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags) {
    mem.clear();
    for(DMounts::CIter x=mounts(); x; ++x) {
      const SMountInfo& mi = x->second;
      const DCString &mnt = mi.mount_name();
      if(~media_name<=~mnt || !tl::su::seq(mnt(), media_name(), ~mnt) 
         || !mi.archive.valid() || !(mi.pm & mp_Read))
        continue;
      if(mi.archive->data_load(media_name + ~mnt, mem, flags))
        return true;
    }
    return false;
  }
  bool bin_save(const DCString& media_name, const SCMem& mem, const SFlags& flags) {
    for(DMounts::CIter x=mounts(); x; ++x) {
      const SMountInfo& mi = x->second;
      const DCString &mnt = mi.mount_name();
      if(~media_name<=~mnt || !tl::su::seq(mnt(), media_name(), ~mnt) 
         || !mi.archive.valid() || !(mi.pm & mp_Read))
        continue;
      if(mi.archive->data_save(media_name + ~mnt, mem, flags))
        return true;
    }
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
  bool obj_save(const DCString& media_name, const IRefObject* obj, const SObjOptions& opts) {
    return false;
  }
  //
  // sub-archives
  SPoint mount_get(const SId& id) const {
    const SMountInfo* x = mounts(id);
    if(x==NULL)
      return SPoint();
    return *x;
  }
  bool mount_add(const SId& id, const SPoint& mp) {
    SMountInfo* x = mounts(id);
    if(x==NULL) {
      DStringShared mn2 = id.mountpoint;
      x = mounts(SId(mn2(), id.order), void_null());
      if(x==NULL)
        return false;
      x->mount_name = mn2;
    } 
    *x = mp;
    return true;
  }
  bool mount_remove(const SId& id) {
    return mounts.Remove(id);
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
