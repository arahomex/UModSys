// ILines::

struct ILines : public IRefObject {
  tl::TDynarrayDynamic<DStringShared> lines;
  //
  UMODSYS_REFOBJECT_INTIMPLEMENT(UMODSYS_MODULE_NAME(test,test1)::ILines, 2, IRefObject);
};

// RLines::

struct RLines : public ILines {
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(test,test1)::RLines, 1, ILines)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  RLines(DOwner *own, const SParameters& args) : refs(own) {}
  ~RLines(void) {}
};

struct RLines_Filter : public libmedia::IBinObjFilter {
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(test,test1)::RLines_Filter, 1, libmedia::IBinObjFilter)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  libmedia::DMediaFlags flags;
  static libmedia::DMediaFlags auto_flags;
  //
  RLines_Filter(DOwner *own, const SParameters& args, libmedia::DMediaFlags ff=libmedia::DMediaFlags()) : refs(own), flags(ff) {}
  ~RLines_Filter(void) {}
  //
  libmedia::DMediaFlags::eStates get_flag(int shift) const { return flags.get_s(shift); }
  libmedia::DMediaFlags get_flags_auto(void) const { return auto_flags; }
  libmedia::DMediaFlags::eStates set_flag(int shift, libmedia::DMediaFlags::eStates flag) { return flags.getset_s(shift, flag); }
  //
  bool filter_load(const SInfo& info, IRefObject::P& obj) {
    RLines::P rv = new(M()) RLines(refs.owner, *info.params);
    if(!filter_load( info, rv() ))
      return false;
    obj = rv;
    return true;
  }
  bool filter_load(const SInfo& info, IRefObject* obj) {
    return false;
  }
  bool filter_save(SInfo& info, IRefObject* obj) {
    return false;
  }
};

libmedia::DMediaFlags RLines_Filter::auto_flags( libmedia::mf_safe::Yes );

