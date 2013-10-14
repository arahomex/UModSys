#include "umodsys.basesys.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RModuleLibrary::
//***************************************

bool RModuleLibraryThis::uni_load(void)
{
  ireg = &RModuleLibraryReg_Chain::s_library;
  if(ireg==NULL)
    return false;
  return true;
}

bool RModuleLibraryThis::uni_unload(void)
{
  return true;
}

bool RModuleLibraryThis::uni_open(void)
{
  return true;
}

bool RModuleLibraryThis::uni_close(void)
{
  return true;
}

//***************************************
//***************************************

RModuleLibraryThis::RModuleLibraryThis(ISystem* s)
: refs(M), IModuleLibraryUni(s, NULL)
{
}

RModuleLibraryThis::~RModuleLibraryThis(void)
{
  unlink();
}

//***************************************
// ::
//***************************************
