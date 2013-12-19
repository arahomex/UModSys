
//***************************************
// RLibraryBinCache::
//***************************************

DMediaFlags RLibraryBinCache::auto_flags;

RLibraryBinCache::RLibraryBinCache(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags())
: refs(own), flags(av)/*, cache(local_memory())*/ {
}

RLibraryBinCache::~RLibraryBinCache(void) {
}

//***************************************
//***************************************

//
// SFlags::ISetter

DMediaFlags::eStates RLibraryBinCache::get_flag(int shift) const 
{ 
  return flags.get_s(shift); 
}

DMediaFlags RLibraryBinCache::get_flags_auto(void) const 
{ 
  return auto_flags; 
}

DMediaFlags::eStates RLibraryBinCache::set_flag(int shift, DMediaFlags::eStates flag) 
{ 
  return flags.getset_s(shift, flag); 
}

//
// general data functions

IStreamReader::P RLibraryBinCache::bin_reader(const DCString& media_name, const SFlags& flags) 
{
  return NULL;
}

IStreamWriter::P RLibraryBinCache::bin_writer(const DCString& media_name, const SFlags& flags) 
{
  return NULL;
}

bool RLibraryBinCache::bin_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags) 
{
  if(!flags.yes<mf_cache>(this))
    return false;
  bool v;
  if(!bincache_get(media_name, &mem, &v))
    return false;
  if(!v && !flags.yes<mf_failuse>(this))
    return false;
  return true;
}

bool RLibraryBinCache::bin_get(const DCString& media_name, SCMemShared& mem, bool isinv) 
{
  bool v;
  if(!bincache_get(media_name, &mem, &v))
    return false;
  if(!v && !isinv)
    return false;
  return true;
}

bool RLibraryBinCache::bin_put(const DCString& media_name, const SCMemShared* mem) 
{
  if(mem==NULL)
    return bincache_add(media_name);
  return bincache_add(media_name, *mem);
}

//***************************************
//***************************************

//
// binary cache

bool RLibraryBinCache::bincache_get(const DCString& media_name, SCMemShared* mem, bool *valid) const 
{
  if(mem!=NULL)
    mem->clear();
  SCacheElem* e = cache(media_name);
  if(e==NULL)
    return false;
  if(mem!=NULL)
    *mem = e->bin;
  if(valid!=NULL)
    *valid = e->valid;
  return true;
}

bool RLibraryBinCache::bincache_add(const DCString& media_name) 
{
  return bincache_add(media_name, SCMemShared(), false);
}

bool RLibraryBinCache::bincache_add(const DCString& media_name, const SCMemShared& mem) 
{
  return bincache_add(media_name, mem, true);
}

bool RLibraryBinCache::bincache_remove(const DCString& media_name) 
{
  return cache.Remove(media_name);
}

bool RLibraryBinCache::bincache_clear(void) 
{
  cache.Clear();
  return true;
}

//***************************************
//***************************************

bool RLibraryBinCache::bincache_add(const DCString& media_name, const SCMemShared& mem, bool valid)
{
  SCacheElem* e = cache(media_name);
  if(e==NULL) {
    DStringShared n2 = media_name;
    e = cache(n2(), void_null());
    if(e==NULL)
      return false;
    e->name = n2;
  }
  e->bin = mem;
  e->valid = valid;
  return true;
}

//***************************************
// END
//***************************************
