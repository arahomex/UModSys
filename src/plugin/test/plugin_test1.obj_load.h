// ILines::

struct ILines : public IRefObject {
  tl::TDynarrayDynamic<DStringShared> lines;
  //
  UMODSYS_REFOBJECT_INTIMPLEMENT(U_MOD::ILines, 2, IRefObject);
};

// RLines::

struct RLines : public ILines {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RLines, 1, ILines)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  RLines(DOwner *own, const SParameters& args) : refs(own) {}
  ~RLines(void) {}
};

struct RLines_Filter : public libmedia::IBinObjFilter {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RLines_Filter, 1, libmedia::IBinObjFilter)
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
    if(info.reqtype==ILines::_root_get_interface_type() || info.common_type=="test1::ILines") {
      RLines::P rv = new(M()) RLines(refs.owner, *info.params);
      if(!filter_load( info, rv() ))
        return false;
      obj = rv;
      return true;
    }
    return false;
  }
  bool filter_load(const SInfo& info, IRefObject* obj) {
    if(obj==NULL)
      return false;
    ILines *lines;
    if(obj->t_root_get_other_interface(lines)) {
      const char *text = info.binary.get_tdata<char>();
      size_t text_len = info.binary.get_size();
      size_t start=0, end=0;
      for(; end<text_len; end++) {
        if(text[end]!='\n' && text[end]!='\r') 
          continue;
        if(end>start) {
          lines->lines.Push(DStringShared(text+start, end-start));
        }
        start = end+1;
      }
      if(end>start) {
        lines->lines.Push(DStringShared(text+start, end-start));
      }
      return true;
    }
    return false;
  }
  bool filter_save(SInfo& info, const IRefObject* obj) {
    if(obj==NULL)
      return false;
    const ILines *lines;
    if(obj->t_root_get_other_interface(lines)) {
      size_t req = 0;
      for(size_t i=0; i<~lines->lines; i++) {
        req += 1 + ~lines->lines(i);
      }
      SMemShared mem(req);
      if(mem.get_size()==req) {
        req = 0;
        for(size_t i=0; i<~lines->lines; i++) {
          const DStringShared& ss = lines->lines(i);
          memcpy(mem.get_data(req), ss(), ~ss);
          req += ~ss;
          *mem.get_tdata<char>(req) = '\n';
          req++;
        }
        info.binary = mem;
      }
      return true;
    }
    return false;
  }
};

libmedia::DMediaFlags RLines_Filter::auto_flags( libmedia::mf_safe::Yes );

