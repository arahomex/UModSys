#include <umodsys/core/module_impl.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;

//***************************************
// SModuleRegChain::
//***************************************

SModuleRegChain SModuleRegChain::root(1);

SModuleRegChain::SModuleRegChain(IModuleReg* r)
: imr(r) 
{
  if(root.next==NULL)
    s_makeroot(&root);
  s_append(this, &root); 
}

SModuleRegChain::SModuleRegChain(IModuleReg& r)
: imr(&r) 
{
  if(root.next==NULL)
    s_makeroot(&root);
  s_append(this, &root); 
}

SModuleRegChain::SModuleRegChain(int v)
: tl::TList2Node<SModuleRegChain>(void_null()), imr(NULL) 
{
//  s_makeroot(this);  !! DO NOT USE THIS
}

SModuleRegChain::~SModuleRegChain(void)
{
  if(imr) delme();
}

//***************************************
//***************************************

/*
size_t SModuleRegChain::s_gather(IModuleReg* rlist[], size_t nlist)
{
  size_t rv = 0;
  for(const SModuleRegChain* x=root.next; x!=&root; x=x->next) {
    if(rlist!=NULL) {
      if(rv>=nlist)
        return rv;
      rlist[rv++] = x->imr;
    } else {
      rv++;
    }
  }
  return rv;
}
*/

IModuleReg* SModuleRegChain::s_get(size_t id)
{
//  M.dbg_put("SModuleRegChain::s_get(%d)\n", (int)id);
  for(const SModuleRegChain* x=root.next; x!=&root; x=x->next) {
    if(id==0)
      return x->imr;
    id--;
  }
  return NULL;
}


//***************************************
// RModuleLibraryReg_Chain::
//***************************************

size_t RModuleLibraryReg_Chain::mlr_count(void) const
{
  if(SModuleRegChain::root.next==NULL)
    SModuleRegChain::s_makeroot(&SModuleRegChain::root);
  size_t rv = 0;
  while(SModuleRegChain::s_get(rv)!=0) {
    rv++;
  }
  return rv;
}

IModuleReg* RModuleLibraryReg_Chain::mlr_get(size_t id) const
{
  return SModuleRegChain::s_get(id);
}

bool RModuleLibraryReg_Chain::mlr_open(IKernel* isys, core::IMemAlloc* privmem)
{
  M.Link(isys, privmem);
  SUniquePointer::s_resolve(M->get_upr()); // initalize all upis
  return true;
}

bool RModuleLibraryReg_Chain::mlr_close(void)
{
  SUniquePointer::s_unresolve(M->get_upr()); // deinitalize all upis
  M.Link(NULL, NULL);
  return true;
}

bool RModuleLibraryReg_Chain::mlr_isopen(void) const 
{
  return *M!=NULL;
}

//***************************************
//***************************************

RModuleLibraryReg_Chain::RModuleLibraryReg_Chain(void)
{
}

RModuleLibraryReg_Chain::~RModuleLibraryReg_Chain(void)
{
}

RModuleLibraryReg_Chain RModuleLibraryReg_Chain::s_library;

//***************************************
// ::
//***************************************
