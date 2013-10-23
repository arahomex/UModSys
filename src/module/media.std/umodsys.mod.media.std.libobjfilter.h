
//***************************************
// RLibraryObjFilter::
//***************************************

struct RLibraryObjFilter : public ILibraryObjFilter
{
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(media,std)::RLibraryObjFilter, 1, ILibraryObjFilter)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  struct SId {
    int order;
    IBinObjFilter* filter;
    //
    SId(int o, IBinObjFilter* f) : order(o), filter(f) {}
    //
    static int compare(const SId& L, const SId& R) { 
      int rv = -scalar_compare(L.order, R.order); // revert order, a>b mean a before b
      if(rv) return rv;
      return scalar_compare(L.filter, R.filter);
    }
  };
  struct SFilter {
//    SParameters params;
    IBinObjFilter::P filter;
  };
  typedef tl::TScatterArray< SFilter, SId, SId > DFilters;
  //
  DFilters filters;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
  //
  RLibraryObjFilter(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags())
  : refs(own), flags(av)/*, cache(local_memory())*/ {
  }
  ~RLibraryObjFilter(void) {
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
    return filters_load(info, obj);
  }
  bool obj_fload(const IBinObjFilter::SInfo& info, IRefObject* obj) {
    return filters_load(info, obj);
  }
  bool obj_fsave(IBinObjFilter::SInfo& info, const IRefObject* obj) {
    return filters_save(info, obj);
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
  // object filtering
  bool filters_add(IBinObjFilter::P filter, const SParameters* filter_params, int order) {
    SId id(order, filter);
    SFilter* x = filters(id);
    if(x==NULL) {
      x = filters(id, void_null());
      if(x==NULL)
        return false;
      x->filter = filter;
//      x->params = filter_params;
    } 
    return true;
  }
  bool filters_remove(IBinObjFilter* filter, int order) {
    SId id(order, filter);
    return filters.Remove(id);
  }
  bool filters_clear(void) {
    filters.Clear();
    return true;
  }
  bool filters_load(const IBinObjFilter::SInfo& info, IRefObject::P& obj) {
    for(DFilters::CIter x = filters(); x; ++x) {
      const SFilter& f = x->second;
      if(f.filter.invalid())
        continue;
      if(f.filter->filter_load(info, obj))
        return true;
    }
    return false;
  }
  bool filters_load(const IBinObjFilter::SInfo& info, IRefObject* obj) {
    for(DFilters::CIter x = filters(); x; ++x) {
      const SFilter& f = x->second;
      if(f.filter.invalid())
        continue;
      if(f.filter->filter_load(info, obj))
        return true;
    }
    return false;
  }
  bool filters_save(IBinObjFilter::SInfo& info, const IRefObject* obj) {
    for(DFilters::CIter x = filters(); x; ++x) {
      const SFilter& f = x->second;
      if(f.filter.invalid())
        continue;
      if(f.filter->filter_save(info, obj))
        return true;
    }
    return false;
  }
  //
  //
  inline bool validate_construction(void) const { return true; }
};

//***************************************
// END
//***************************************
