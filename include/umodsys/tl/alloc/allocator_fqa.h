#ifndef __NANOUTL_TYPES_ALLOCATOR_FQA_H
#define __NANOUTL_TYPES_ALLOCATOR_FQA_H 1
/*************************************************************/
// nanoutl/types/allocator_malloc.h: simple allocator
/*************************************************************/

#include <nanoutl/core/std.h>
#include <nanoutl/core/source.h>
#include <nanoutl/core/arch.h>

#include <nanoutl/types/list2.h>
#include <nanoutl/types/scatter_array.h>

namespace NanoUTL {
namespace types {

/*************************************************************/

template<typename MemAlloc, size_t QuantElemSize, size_t QuantElemCount=1024>
struct TMemAlloc_QuantAlloc {
protected:
  struct MNode;
  
  struct MNode : public types::TList2Node<MNode> {
    inline void append(MNode *x) { s_append(x, this); }
    inline void root(void) { s_makeroot(this); }
  };
  enum {
    MNodeSize = sizeof(MNode) + QuantElemSize
  };
  struct Data { 
    char Q[QuantElemCount*MNodeSize]; 
  };
  //
  typedef TScatterArray<Data, int, TNodeDeleter<TScatterArrayNode<Data,int>, MemAlloc> > Array;
  //
protected:
  mutable int last_quid;
  mutable Array arr;
  mutable MNode used, unused;
protected:
  static void qlink(MNode* root, Data& d) {
    MNode *p = reinterpret_cast<MNode*>(d.Q);
    for(int i=0; i<QuantElemCount; 
        i++, p = reinterpret_cast<MNode*>(reinterpret_cast<char*>(p)+MNodeSize)) {
      root->append(p);
    }
  }
protected:
  void* qalloc(void) const {
    if(unused.isempty()) {
      Data* d = arr[last_quid++];
      if(d==NULL)
        return NULL;
      qlink(&unused, *d);
      if(unused.isempty())
        return NULL;
    }
    MNode *p = unused.next;
    p->delme();
    used.append(p);
    return p+1;
  }
  void qfree(void* pp) const {
    if(pp==NULL) return;
    MNode* p = reinterpret_cast<MNode*>(pp)-1;
    p->delme();
    unused.append(p);
  }
public:
  TMemAlloc_QuantAlloc(void) : last_quid(0) { used.root(); unused.root(); }
  //
  inline void* mem_alloc(size_t n) const { 
    if(n>QuantElemSize) return NULL;
    return qalloc();
  }
  inline void* mem_realloc(void* op, size_t n) const { 
    if(n>QuantElemSize) return NULL;
    if(n==0) { if(op!=NULL) qfree(op); return NULL; }
    if(op==NULL) return qalloc();
    return op;
  }
  inline void mem_free(void *op) const {
    if(op!=NULL) qfree(op);
  }
  //
  inline static const TMemAlloc_QuantAlloc& get_default(void) { 
    static TMemAlloc_QuantAlloc rv; 
    return rv; 
  }
};


/*************************************************************/
/*************************************************************/

} // namespace types
} // namespace NanoUTL

#endif // __NANOUTL_TYPES_FQA_H
