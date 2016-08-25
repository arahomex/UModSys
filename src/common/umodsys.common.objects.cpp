#include <umodsys/common/objects.h>
#include <umodsys/common/objects_res.h>
#include <umodsys/common/mem/mem_alloc.h>

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
  delete this;
}

//***************************************

void* IRoot::operator new(size_t size, IMemAlloc* m) UMODSYS_NOTHROW()
{
  if(m==NULL)
    return NULL;
  UObjectHeader* mh = static_cast<UObjectHeader*>(m->mem_alloc(size + sizeof(UObjectHeader), UMODSYS_SOURCEINFO));
  if(mh==NULL)
    return NULL;
  mh->mem = m;
  return mh+1;
}

void IRoot::operator delete(void *op, IMemAlloc* m) UMODSYS_NOTHROW()
{
  if(m==NULL || op==NULL)
    return;
  UObjectHeader* mh = static_cast<UObjectHeader*>(op) - 1;
  mh->mem = NULL;
  m->mem_free(mh, UMODSYS_SOURCEINFO);
}

void IRoot::operator delete(void *op) UMODSYS_NOTHROW()
{
  if(op==NULL)
    return;
  UObjectHeader* mh = static_cast<UObjectHeader*>(op) - 1;
  IMemAlloc* m = mh->mem;
  if(m==NULL)
    return; // fatal error
  mh->mem = NULL;
  m->mem_free(mh, UMODSYS_SOURCEINFO);
}

//***************************************
// IRefObject::
//***************************************

IRefObject::~IRefObject(void) 
{
}


//***************************************
// IModObject::
//***************************************

//***************************************
// IGenerator::
//***************************************

//***************************************
// ::
//***************************************

//***************************************
// ::
//***************************************
