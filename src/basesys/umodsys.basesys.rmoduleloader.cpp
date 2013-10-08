#include "umodsys.basesys.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RModuleLoader::
//***************************************

size_t RModuleLoader::moduledb_findobjname(IRefObject::TypeId intr, IRefObject::TypeId found[], size_t nfound)
{
  size_t n = 0;
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrary* ml = mod_list(i);
    if(ml==NULL)
      continue;
    if(found!=NULL) {
      n += ml->lib_findobjname(intr, found+n, nfound-n);
    } else {
      n += ml->lib_findobjname(intr, NULL, nfound);
    }
  }
  return n;
}

bool RModuleLoader::moduledb_generate(IRefObject::P& obj, IRefObject::TypeId name, const SParameters& args)
{
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrary* ml = mod_list(i);
    if(ml==NULL)
      continue;
    if(ml->lib_generate(obj, name, args))
      return true;
  }
  return false;
}

//***************************************

size_t RModuleLoader::moduledb_lib_count(void)
{
  return ~mod_list;
}

IModuleLibrary* RModuleLoader::moduledb_lib_get(size_t id) const
{
  return id<~mod_list ? mod_list(id)() : NULL;
}

bool RModuleLoader::moduledb_lib_drop(IModuleLibrary* lib)
{
  return false;
}

//***************************************

size_t RModuleLoader::moduledb_module_count(void)
{
  size_t n = 0;
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrary* ml = mod_list(i);
    if(ml==NULL)
      continue;
    n += ~ml->modules;
  }
  return n;
}

IModule* RModuleLoader::moduledb_module_get(size_t id) const
{
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrary* ml = mod_list(i);
    if(ml==NULL)
      continue;
    if(id<~ml->modules) {
      return ml->modules(id);
    }
    id -= ~ml->modules;
  }
  return NULL;
}

IModule* RModuleLoader::moduledb_find(const core::DCString& name, const core::SVersion& verno) const
{
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrary* ml = mod_list(i);
    if(ml==NULL)
      continue;
    for(size_t k=0; k<~ml->modules; k++) {
      RModule* m = ml->modules(k);
      if(m==NULL)
        continue;
      if(m->minfo.eq(name, verno))
        return m;
    }
  }
  return NULL;
}

//***************************************

void RModuleLoader::moduledb_clear(void)
{
  moduledb_cleanup();
  mod_list.Clear();
}

size_t RModuleLoader::moduledb_cleanup(void)
{
//  dbg_put(rsdl_System, "RModuleLoader::moduledb_cleanup()\n");
  size_t n, rv = 0, s=0;
  do {
    n=0;
    for(int i=0; i<~mod_list; i++) {
      size_t nn = mod_list(i)->cleanup();
      if(nn==0)
        continue;
      n += nn;
    }
    rv += n;
//    dbg_put(rsdl_System, "RModuleLoader::moduledb_cleanup() { n=%d }\n", n);
  } while(n);
  for(int i=0; i<~mod_list; i++) {
    if(!mod_list(i).valid()) {
      mod_list.RemoveAt(i);
      i--; s++;
    }
  }
  dbg_put(rsdl_System, "RModuleLoader::moduledb_cleanup() {removed %d} => %d\n", s, rv);
  return rv;
}

bool RModuleLoader::moduledb_load(const core::DCString& cachepath)
{
  return false;
}

bool RModuleLoader::moduledb_save(const core::DCString& cachepath)
{
  FILE *f = syshlp::c_fopen(cachepath, "wb");
  if(f==NULL)
    return false;
  fprintf(f, "# LIST OF LIBRARY MODULES:\n\n");
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrary* ml = mod_list(i);
    if(ml==NULL)
      continue;
    if(!ml->save_db(f)) {
      fclose(f);
      return false;
    }
  }
  fclose(f);
  return true;
}

size_t RModuleLoader::moduledb_scan(const core::DCString& mask, bool docleanup)
{
  for(size_t i=0; i<~mod_list; i++) {
    mod_list(i)->load0();
  }
  size_t rv = RModuleLibrary::pfd_scan(sys, mod_list, mask);
  if(docleanup) {
    moduledb_cleanup();
  }
  return rv;
}


const DCString& RModuleLoader::moduledb_get_string(const DCString &s)
{
  return get_string(s);
}

//***************************************
//***************************************

const DCString& RModuleLoader::get_string(const DCString& v)
{
  return *mod_pool.append(v);
}

void RModuleLoader::cleanup(void)
{
  dbg_put(rsdl_System, " { mod_list=%d:[] }\n", ~mod_list);
  moduledb_cleanup();
  moduledb_clear();
  mod_list.Clear();
}

//***************************************
//***************************************

RModuleLoader::RModuleLoader(ISystem* s)
: sys(s) 
{
}

RModuleLoader::~RModuleLoader(void)
{
}

//***************************************
// ::
//***************************************
