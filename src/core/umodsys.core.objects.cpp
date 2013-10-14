#include <umodsys/core/objects.h>
#include <umodsys/core/mem/mem_alloc.h>

using namespace UModSys;
using namespace UModSys::core;

//***************************************
// IRoot::
//***************************************

//NANOUTL_ROOT_LUP(NanoUTL::core::IRoot);

IRoot::~IRoot(void) 
{
}

void IRoot::suicide(void)
{
  _delete();
}

IMemAlloc* IRoot::get_heap_allocator(void) const
{
  return local_memory().imem;
}

//***************************************

void* IRoot::operator new(size_t size, IMemAlloc* m) UMODSYS_NOTHROW()
{
  return m ? m->mem_alloc(size, UMODSYS_SOURCEINFO) : NULL;
}

void IRoot::operator delete(void *op, IMemAlloc* m) UMODSYS_NOTHROW()
{
  if(m) {
    m->mem_free(op, UMODSYS_SOURCEINFO);
  }
}

//***************************************
// IRefObject::
//***************************************

//NANOUTL_ROOT_LUP(NanoUTL::core::IRefObject);

IRefObject::~IRefObject(void) 
{
}


//***************************************
// IModObject::
//***************************************

//NANOUTL_ROOT_LUP(NanoUTL::core::IModObject);

//***************************************
// IGenerator::
//***************************************

//NANOUTL_ROOT_LUP(NanoUTL::core::IGenerator);

//***************************************
// ::
//***************************************

//***************************************
// ::
//***************************************
