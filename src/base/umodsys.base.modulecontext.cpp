#include <umodsys/base/module_impl.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// SModuleContext::
//***************************************

SModuleContext::~SModuleContext(void)
{
}

SModuleContext::SModuleContext(void)
: isys(NULL), icon(NULL), imem(NULL)
{
}

void SModuleContext::Link(ISystem* is, IMemAlloc* privmem)
{
  isys = is;
  icon = isys!=NULL ? isys->get_console() : NULL;
  imem = privmem;
}

//***************************************
// ::
//***************************************

base::SModuleContext UModSys::M;

//***************************************
// ::
//***************************************
