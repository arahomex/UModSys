#include <umodsys/base/module_impl.h>

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

IModuleReg* SModuleRegChain::s_get(size_t id)
{
  M.dbg_put("SModuleRegChain::s_get(%d)\n", (int)id);
  for(const SModuleRegChain* x=root.next; x!=&root; x=x->next) {
    if(id==0)
      return x->imr;
    id--;
  }
  return NULL;
}

//***************************************
// ::
//***************************************
