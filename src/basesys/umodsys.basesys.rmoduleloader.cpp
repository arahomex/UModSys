#include "umodsys.basesys.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RModuleLoader::
//***************************************

size_t RModuleLoader::moduledb_findobjname(IRefObject::TypeId intr, IRefObject::TypeId found[], size_t nfound, core::BCStr mask)
{
  size_t n = 0;
  if(found!=NULL) {
    n += mod_this->lib_findobjname(intr, found+n, nfound-n, mask);
  } else {
    n += mod_this->lib_findobjname(intr, NULL, nfound, mask);
  }
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrarySO* ml = mod_list(i);
    if(ml==NULL)
      continue;
    if(found!=NULL) {
      n += ml->lib_findobjname(intr, found+n, nfound-n, mask);
    } else {
      n += ml->lib_findobjname(intr, NULL, nfound, mask);
    }
  }
  return n;
}

bool RModuleLoader::moduledb_generate(IRefObject::P& obj, IRefObject::TypeId name, const SParameters& args)
{
  if(mod_this->lib_generate(obj, name, args))
    return true;
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrarySO* ml = mod_list(i);
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
  return ~mod_list + 1;
}

IModuleLibrary* RModuleLoader::moduledb_lib_get(size_t id) const
{
  if(id==0)
    return mod_this();
  if(id<=~mod_list)
    return mod_list(id-1)();
  return NULL;
}

bool RModuleLoader::moduledb_lib_drop(IModuleLibrary* lib)
{
  return false;
}

//***************************************

size_t RModuleLoader::moduledb_module_count(void)
{
  size_t n = ~mod_this->modules;
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrarySO* ml = mod_list(i);
    if(ml==NULL)
      continue;
    n += ~ml->modules;
  }
  return n;
}

IModule* RModuleLoader::moduledb_module_get(size_t id) const
{
  if(id<~mod_this->modules) {
    return mod_this->modules(id);
  }
  id -= ~mod_this->modules;
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrarySO* ml = mod_list(i);
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
  for(size_t k=0; k<~mod_this->modules; k++) {
    RModule* m = mod_this->modules(k);
    if(m==NULL)
      continue;
    if(m->minfo.eq(name, verno))
      return m;
  }
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrarySO* ml = mod_list(i);
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
  for(int i=0; i<~mod_list; i++) {
    if(mod_list(i).valid()) {
      mod_list(i)->lib_free();
    }
  }
  mod_this->lib_free();
  //
  mod_this.clear();
  mod_list.Free();
  mod_pool.free();
}

size_t RModuleLoader::moduledb_cleanup(void)
{
//  dbg_put(rsdl_System, "RModuleLoader::moduledb_cleanup()\n");
  size_t n, rv = 0, s=0;
  do {
    n = mod_this->cleanup();
    for(int i=0; i<~mod_list; i++) {
      n += mod_list(i)->cleanup();
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
  FILE *f = syshlp::u_fopen(cachepath, "wb");
  if(f==NULL)
    return false;
  fprintf(f, "# LIST OF LIBRARY MODULES:\n\n");
  if(!mod_this->save_db(f)) {
    fclose(f);
    return false;
  }
  for(size_t i=0; i<~mod_list; i++) {
    RModuleLibrarySO* ml = mod_list(i);
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
  mod_this->load0();
  for(size_t i=0; i<~mod_list; i++) {
    mod_list(i)->load0();
  }
  size_t rv = RModuleLibrarySO::pfd_scan(sys, mod_list, mask);
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
