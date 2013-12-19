
//***************************************
// RLibraryObjCache::
//***************************************

DMediaFlags RLibraryObjCache::auto_flags;

RLibraryObjCache::RLibraryObjCache(DOwner *own, const SParameters& args, DMediaFlags av)
: refs(own), flags(av)/*, cache(local_memory())*/ 
{
}

RLibraryObjCache::~RLibraryObjCache(void) 
{
}

//***************************************
//***************************************

//
// SFlags::ISetter

DMediaFlags::eStates RLibraryObjCache::get_flag(int shift) const 
{ 
  return flags.get_s(shift); 
}

DMediaFlags RLibraryObjCache::get_flags_auto(void) const 
{ 
  return auto_flags; 
}

DMediaFlags::eStates RLibraryObjCache::set_flag(int shift, DMediaFlags::eStates flag) 
{ 
  return flags.getset_s(shift, flag); 
}

//***************************************
//***************************************

bool RLibraryObjCache::obj_cget(const DCString& media_name, IRefObject::P& obj, bool isinv) 
{
  bool v;
  if(!objcache_get(media_name, &obj, &v))
    return false;
  if(!v && !isinv)
    return false;
  return true;
}

bool RLibraryObjCache::obj_cput(const DCString& media_name, IRefObject* obj) 
{
  return objcache_add(media_name, obj);
}

// universal object functions
bool RLibraryObjCache::obj_get(const DCString& media_name, IRefObject::P& obj, const SObjOptions& opts) 
{
  SFlagsChain f2(opts, this);
  if(f2.yes<mf_objects>(this) && obj_cget(media_name, obj, f2.yes<mf_nulluse>(this)))
    return true;
  return false;
}

//***************************************
//***************************************

//
// object cache 

bool RLibraryObjCache::objcache_get(const DCString& media_name, IRefObject::P* obj, bool *valid) const 
{
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

bool RLibraryObjCache::objcache_add(const DCString& media_name, IRefObject* obj) 
{
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

bool RLibraryObjCache::objcache_remove(const DCString& media_name) 
{
  return cache.Remove(media_name);
}

bool RLibraryObjCache::objcache_clear(void) 
{
  cache.Clear();
  return true;
}

//***************************************
// END
//***************************************
