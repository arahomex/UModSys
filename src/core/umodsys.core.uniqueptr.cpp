#include <umodsys/core/uniqueptr.h>
#include <umodsys/core/stdinlines.h>

using namespace UModSys;
using namespace UModSys::core;

//***************************************
// SUniquePointer::
//***************************************

SUniquePointer SUniquePointer::root(0);

SUniquePointer::SUniquePointer(const char *g, const char *n, int vn) 
: info(g, n, vn), upi(NULL) 
{
  if(root.next==NULL)
    s_makeroot(&root);
  s_append(this, &root); 
}

SUniquePointer::SUniquePointer(int v)
: tl::TList2Node<SUniquePointer>(void_null()), info(NULL, NULL, 0), upi(NULL)
{ 
//  s_makeroot(this);  !! DO NOT USE THIS
}

SUniquePointer::~SUniquePointer(void) 
{ 
  if(info.group) delme();
}

void SUniquePointer::s_resolve(IUniquePointerResolver* res) 
{
  if(root.next==NULL)
    s_makeroot(&root);
  for(const SUniquePointer* x=root.next; x!=&root; x=x->next) {
    if(x->upi==NULL) {
      x->upi = res->upi_add(&x->info);
    }
  }
}

void SUniquePointer::s_unresolve(IUniquePointerResolver* res) 
{
  for(const SUniquePointer* x=root.next; x!=&root; x=x->next) {
    if(x->upi!=NULL) {
      res->upi_remove(x->upi);
      x->upi = NULL;
    }
  }
}

//***************************************
// SUniquePtrList::
//***************************************

SUniquePtrList::SUniquePtrList(HUniquePointer *a, size_t n) 
: arr(a), maxlen(n), curlen(0) 
{
}

bool SUniquePtrList::add(HUniquePointer val)
{
  if(val==NULL)
    return false; // never add NULL
  for(size_t i=0; i<curlen; i++) {
    if(arr[i]==val)
      return true; // already added
  }
  if(curlen>=maxlen)
    return false; // overflow
  arr[curlen++] = val;
  return true;
}

size_t SUniquePtrList::find(HUniquePointer val) const
{
  if(val==NULL)
    return array_index_none; // never add NULL
  for(size_t i=0; i<curlen; i++) {
    if(arr[i]==val)
      return i; // already added
  }
  return array_index_none;
}

//***************************************
// ::
//***************************************
