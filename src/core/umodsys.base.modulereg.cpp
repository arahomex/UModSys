#include <umodsys/core/module_impl.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// IModuleReg::
//***************************************

IModuleReg::IModuleReg(const SModuleInfo &mi, SDebug* dbg)
: minfo(mi), module(NULL), load_count(0), sdbg(dbg)
{
}

IModuleReg::IModuleReg(const char* n, core::Buint16 vh, core::Buint16 vl, const char *i, core::Buint32 bno, core::BTime u, SDebug* dbg)
: minfo(n, vh, vl, i, bno, u), module(NULL), load_count(0), sdbg(dbg)
{
}

IModuleReg::~IModuleReg(void)
{
}

SDebug* IModuleReg::mr_getdebug(void) const
{
  return sdbg;
}

bool IModuleReg::mr_isopen(void) const
{
  return load_count>0;
}


bool IModuleReg::mr_open(void)
{
  if(load_count==0) {
    if(!do_open())
      return false;
  }
  load_count++;
  if(sdbg!=NULL)
    sdbg->console = M.sys().get_console();
  return true;
}

bool IModuleReg::mr_close(void)
{
  load_count--;
  if(load_count==0) {
    if(sdbg!=NULL)
      sdbg->console = NULL;
    return do_close();
  }
  return true;
}

//***************************************
// IModuleLibraryReg::
//***************************************

IModuleLibraryReg::IModuleLibraryReg(void)
{
}

IModuleLibraryReg::~IModuleLibraryReg(void)
{
}

//***************************************
// ::
//***************************************
