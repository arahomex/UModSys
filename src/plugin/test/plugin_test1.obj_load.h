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
  RLines_Filter(DOwner *own, const SParameters& args) : refs(own) {}
  ~RLines_Filter(void) {}
  //
  bool filter_load(IRefObject::P& obj, const SInfo& info) {
    RLines::P rv = new(M()) RLines(refs.owner, *info.params);
    if(!filter_load(rv(), info))
      return false;
    obj = rv;
    return true;
  }
  bool filter_load(IRefObject* obj, const SInfo& info) {
    return false;
  }
  bool filter_save(IRefObject* obj, SInfo& info) {
    return false;
  }
};
