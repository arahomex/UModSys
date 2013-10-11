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
: isys(NULL), icon(NULL), smem(NULL)
{
}

void SModuleContext::Link(ISystem* is, IMemAlloc* privmem)
{
  isys = is;
  if(isys!=NULL) {
    icon = isys->get_console();
    imodloader = isys->get_modloader();
  } else {
    icon = NULL;
    imodloader = NULL;
  }
  smem.imem = privmem;
}

void SModuleContext::dbg_put(const char *fmt, ...)
{
  if(icon==NULL)
    return;
  va_list va;
  va_start(va, fmt);
  icon->vput(0, fmt, va);
  va_end(va);
}

//***************************************
// ::
//***************************************

base::SModuleContext UModSys::M;

const core::SIMemAlloc& UModSys::local_memory(void)
{
  return M();
}

//***************************************
// SParameters::
//***************************************

IParameters* SParameters::get_paramworker(void)
{
  return M->get_params();
}

//***************************************
// ::
//***************************************
