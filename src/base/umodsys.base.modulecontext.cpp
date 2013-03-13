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
: iinfo(NULL), isys(NULL), icon(NULL)
{
}

void SModuleContext::Link(ISystem* is, IModuleInfo *imi, IMemAlloc* privmem)
{
  isys = is;
  iinfo = imi;
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
