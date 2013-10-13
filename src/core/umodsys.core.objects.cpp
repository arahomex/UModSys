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
}

//***************************************

void* IRoot::operator new(size_t size, const SMemAlloc_Malloc& m) UMODSYS_NOTHROW()
{
  return m.mem_alloc(size, UMODSYS_SOURCEINFO);
}

void IRoot::operator delete(void *op, const SMemAlloc_Malloc& m) UMODSYS_NOTHROW()
{
  m.mem_free(op, UMODSYS_SOURCEINFO);
}

void* IRoot::operator new(size_t size, const SIMemAlloc& m) UMODSYS_NOTHROW()
{
  return m.mem_alloc(size, UMODSYS_SOURCEINFO);
}

void IRoot::operator delete(void *op, const SIMemAlloc& m) UMODSYS_NOTHROW()
{
  m.mem_free(op, UMODSYS_SOURCEINFO);
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
