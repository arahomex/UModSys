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
    smem_shared.imem = is->get_sharemem();
  } else {
    icon = NULL;
    imodloader = NULL;
    smem_shared.imem = NULL;
  }
  smem.imem = privmem;
}

void SModuleContext::dbg_put(const char *fmt, ...)
{
  if(icon==NULL)
    return;
  va_list va;
  va_start(va, fmt);
  icon->vput(cl_Debug, fmt, va);
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

const core::SIMemAlloc& UModSys::shared_memory(void)
{
  return M.mem_shared();
}

//***************************************
// SParameters::
//***************************************

IParameters* SParameters::get_paramworker(void)
{
  return M->get_params();
}

//***************************************
// SMemorySharedData::
//***************************************

void SMemorySharedData::link(const SSourceContext* sctx)
{
  rc_count++;
}

void SMemorySharedData::unlink(const SSourceContext* sctx)
{
  if(--rc_count<0) {
    heap->mem_free(this, sctx);
  }
}

SMemorySharedData* SMemorySharedData::new_data(IMemAlloc* h, size_t num, const SSourceContext* sctx)
{
  void *rv = h->mem_alloc(num + sizeof(SMemorySharedData), sctx);
  if(rv==NULL)
    return NULL;
  return static_cast<SMemorySharedData*>(rv)->init(num, h);
}


//***************************************
// ::
//***************************************
