
//***************************************
// RLibraryObjFilter::
//***************************************

DMediaFlags RLibraryObjFilter::auto_flags;

RLibraryObjFilter::RLibraryObjFilter(DOwner *own, const SParameters& args, DMediaFlags av)
: refs(own), flags(av)/*, cache(local_memory())*/ 
{
}

RLibraryObjFilter::~RLibraryObjFilter(void) 
{
}

//***************************************
//***************************************

//
// SFlags::ISetter

DMediaFlags::eStates RLibraryObjFilter::get_flag(int shift) const 
{ 
  return flags.get_s(shift); 
}

DMediaFlags RLibraryObjFilter::get_flags_auto(void) const 
{ 
  return auto_flags; 
}

DMediaFlags::eStates RLibraryObjFilter::set_flag(int shift, DMediaFlags::eStates flag) 
{ 
  return flags.getset_s(shift, flag); 
}

//***************************************
//***************************************

// general object functions

bool RLibraryObjFilter::obj_fget(const IBinObjFilter::SInfo& info, IRefObject::P& obj) 
{
  return filters_load(info, obj);
}

bool RLibraryObjFilter::obj_fload(const IBinObjFilter::SInfo& info, IRefObject* obj) 
{
  return filters_load(info, obj);
}

bool RLibraryObjFilter::obj_fsave(IBinObjFilter::SInfo& info, const IRefObject* obj) 
{
  return filters_save(info, obj);
}

//***************************************
//***************************************

//
// object filtering

bool RLibraryObjFilter::filters_add(IBinObjFilter::P filter, const SParameters* filter_params, int order) 
{
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

bool RLibraryObjFilter::filters_remove(IBinObjFilter* filter, int order) 
{
  SId id(order, filter);
  return filters.Remove(id);
}

bool RLibraryObjFilter::filters_clear(void) 
{
  filters.Clear();
  return true;
}

bool RLibraryObjFilter::filters_load(const IBinObjFilter::SInfo& info, IRefObject::P& obj) 
{
  for(DFilters::CIter x = filters(); x; ++x) {
    const SFilter& f = x->second;
    if(f.filter.invalid())
      continue;
    if(f.filter->filter_load(info, obj))
      return true;
  }
  return false;
}

bool RLibraryObjFilter::filters_load(const IBinObjFilter::SInfo& info, IRefObject* obj) 
{
  for(DFilters::CIter x = filters(); x; ++x) {
    const SFilter& f = x->second;
    if(f.filter.invalid())
      continue;
    if(f.filter->filter_load(info, obj))
      return true;
  }
  return false;
}

bool RLibraryObjFilter::filters_save(IBinObjFilter::SInfo& info, const IRefObject* obj) 
{
  for(DFilters::CIter x = filters(); x; ++x) {
    const SFilter& f = x->second;
    if(f.filter.invalid())
      continue;
    if(f.filter->filter_save(info, obj))
      return true;
  }
  return false;
}

//***************************************
// END
//***************************************
