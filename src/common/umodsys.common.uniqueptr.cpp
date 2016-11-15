#include <umodsys/common/base/uniq.h>
#include <umodsys/common/stdinlines.h>

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
  size_t n = 0;
  for(const SUniquePointer* x=root.next, *y=x; x!=&root; y=x, x=x->next, n++) {
    if(x->upi!=NULL) {
      res->upi_remove(x->upi);
      x->upi = NULL;
    }
  }
}

//***************************************
// ::
//***************************************
