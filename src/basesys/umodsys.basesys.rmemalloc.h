#ifndef __UMODSYS_BASE_RMEMALLOC_H
#define __UMODSYS_BASE_RMEMALLOC_H 1
/*************************************************************/
// file: src/base/system/umodsys.base.rmemalloc.h
// info: implementation of memory allocator
/*************************************************************/

#include "umodsys.basesys.rcommon.h"

#include <umodsys/core/common_exceptions.h>
#include <umodsys/tl/composite/list2.h>

namespace UModSys {
namespace base {
namespace rsystem {

using namespace core;

template<typename E1, typename E2=Void, typename E3=Void, typename E4=Void, typename E5=Void> struct TMemAllocHeader;
template<typename Header, typename SubAllocator> struct TRMemAlloc_CC;

//***************************************
// SMemAllocHeader_*
//***************************************

namespace mem_headers {

struct SMagic {
  enum { eMagic = 0x4D454B49 };
  //
  ptrdiff_t magic;
  //
  inline void clear(void) { magic = 0; }
  inline void init(size_t sz) { magic = eMagic; }
  inline void init(const SSourceContext* d) {}
  //
  template<typename Pool> inline void pinit(Pool* p) {}
  template<typename Pool> inline bool valid(const Pool* p) const { return magic==eMagic; }
};

struct SDebug {
  const SSourceContext* dbg;
  //
  inline void clear(void) {}
  inline void init(size_t sz) {}
  inline void init(const SSourceContext* d) { dbg = d; }
  //
  template<typename Pool> inline void pinit(Pool* p) {}
  template<typename Pool> inline bool valid(const Pool* p) const { return true; }
};

template<typename Pool>
struct TPool {
  Pool* pool;
  //
  inline void clear(void) {}
  inline void init(size_t sz) {}
  inline void init(const SSourceContext* d) {}
  //
  inline void pinit(Pool* p) { pool = p; }
  inline bool valid(const Pool* p) const { return pool==p; }
};

template<typename Header>
struct TMain : public tl::TList2Node<Header> {
  size_t allocated;
  //
  inline void clear(void) { allocated = 0; delme(); }
  inline void init(size_t sz) { allocated = sz; }
  inline void init(const SSourceContext* d) {}
  //
  template<typename Pool> inline void pinit(Pool* p) {}
  template<typename Pool> inline bool valid(const Pool* p) const { return !isempty(); }
  //
  inline size_t getsize(void) const { return allocated; }
};

template<int k>
struct TEmpty {
  inline void clear(void) {}
  inline void init(size_t sz) {}
  inline void init(const SSourceContext* d) {}
  //
  template<typename Pool> inline void pinit(Pool* p) {}
  template<typename Pool> inline bool valid(const Pool* p) const { return true; }
};

} // mem_headers

//***************************************
// TMemAllocHeader
//***************************************

template<typename TT, int k> struct TMemAllocHeaderMap { typedef TT T; };
template<int k> struct TMemAllocHeaderMap<Void,k> { typedef mem_headers::TEmpty<k> T; };

template<typename E1, typename E2, typename E3, typename E4, typename E5>
struct TMemAllocHeader 
: public TMemAllocHeaderMap<E1, 1>::T,
  public TMemAllocHeaderMap<E2, 2>::T,
  public TMemAllocHeaderMap<E3, 3>::T,
  public TMemAllocHeaderMap<E4, 4>::T,
  public TMemAllocHeaderMap<E5, 5>::T,
  public mem_headers::TMain< TMemAllocHeader<E1, E2, E3, E4, E5> > 
{
  typedef typename TMemAllocHeader<E1, E2, E3, E4, E5> Self;
  typedef typename mem_headers::TMain< Self > TM;
  typedef typename TMemAllocHeaderMap<E1, 1>::T T1;
  typedef typename TMemAllocHeaderMap<E2, 2>::T T2;
  typedef typename TMemAllocHeaderMap<E3, 3>::T T3;
  typedef typename TMemAllocHeaderMap<E4, 4>::T T4;
  typedef typename TMemAllocHeaderMap<E5, 5>::T T5;
  //
  inline void clear(void) { TM::clear(); T1::clear(); T2::clear(); T3::clear(); T4::clear(); T5::clear(); }
  inline void init(size_t sz) { TM::init(sz); T1::init(sz); T2::init(sz); T3::init(sz); T4::init(sz); T5::init(sz); }
  inline void init(const SSourceContext* d) { TM::init(d); T1::init(d); T2::init(d); T3::init(d); T4::init(d); T5::init(d); }
  //
  template<typename Pool> inline void pinit(Pool* p) { insert_in(&p->root); TM::pinit(p); T1::pinit(p); T2::pinit(p); T3::pinit(p); T4::pinit(p); T5::pinit(p); }
  template<typename Pool> inline bool valid(const Pool* p) const { return TM::valid(p) && T1::valid(p) && T2::valid(p) && T3::valid(p) && T4::valid(p) && T5::valid(p); }
  //
  inline void insert_in(Self* r) { s_append(this, r); }
};

//***************************************
// TRMemAlloc
//***************************************

template<typename Header, typename SubAllocator>
struct TRMemAlloc_CC : public IMemAlloc, public SubAllocator
{
public:
  typedef Header DHeader;
public:
  TRMemAlloc_CC(IExceptionHandler* eh);
  ~TRMemAlloc_CC(void);
public:
  void* mem_alloc(size_t n, const SSourceContext* sctx);
  void* mem_realloc(void* op, size_t n, const SSourceContext* sctx);
  void mem_free(void *op, const SSourceContext* sctx);
public:
  Header root;
  IExceptionHandler* err_handler;
public:
  size_t check_leaks(bool isdump, IExceptionHandler* eh=NULL);
};


template<typename Header, typename SubAllocator>
TRMemAlloc_CC<Header, SubAllocator>::TRMemAlloc_CC(IExceptionHandler* eh)
: err_handler(eh) {
  DHeader::s_makeroot(&root);
}

template<typename Header, typename SubAllocator>
TRMemAlloc_CC<Header, SubAllocator>::~TRMemAlloc_CC(void)
{
}

template<typename Header, typename SubAllocator>
void* TRMemAlloc_CC<Header, SubAllocator>::mem_alloc(size_t n, const SSourceContext* sctx)
{
   Header *h = static_cast<Header*>(SubAllocator::mem_alloc(n + sizeof(Header), sctx));
   if(h==NULL)
     return NULL;
   h->init(n); 
   h->init(sctx); 
   h->pinit(this);
   return h+1;
}

template<typename Header, typename SubAllocator>
void* TRMemAlloc_CC<Header, SubAllocator>::mem_realloc(void* op, size_t n, const SSourceContext* sctx)
{
  if(op==NULL) {
    if(n==0)
      return NULL;
    return mem_alloc(n, sctx);
  }
  Header *h = static_cast<Header*>(op) - 1;
  if(!h->valid(this)) {
    // report about error
    err_handler->process(EFatal(sctx, cec_MemoryDamaged, ""));
    return NULL; 
  }
  if(n==0) {
    mem_free(op, sctx);
    return NULL;
  }
  //
  size_t olds = h->getsize();
  if(olds==0) {
    err_handler->process(EWarning(sctx, cec_Unsupported, ""));
    return NULL; // realloc unsupported
  }
  if(olds>n) {
    h->init(sctx);
    return op; // don't reduce size
  }
  void *rv = mem_alloc(n, sctx);
  if(rv==NULL)
    return NULL; // no memory
  memcpy(rv, op, scalar_min(n, olds));
  h->clear(); 
  h->init(sctx);
  SubAllocator::mem_free(h, sctx);
  return rv;
}


template<typename Header, typename SubAllocator>
void TRMemAlloc_CC<Header, SubAllocator>::mem_free(void *op, const SSourceContext* sctx)
{
  if(op==NULL)
    return;
  Header *h = static_cast<Header*>(op) - 1;
  if(!h->valid(this)) {
    // report about error
    err_handler->process(EFatal(sctx, cec_MemoryDamaged, ""));
    return; 
  }
  h->clear(); 
  h->init(sctx);
  SubAllocator::mem_free(h, sctx);
  return;
}

template<typename Header, typename SubAllocator>
size_t TRMemAlloc_CC<Header, SubAllocator>::check_leaks(bool isdump, IExceptionHandler* eh)
{
  size_t rv = 0;
  for(Header *x = root.next, *pv=&root; x!=&root; pv=x, x=x->next) {
    if(x==NULL) {
      if(isdump) {
        dbg_put(rsdl_MemoryError, "NULL detected after %p\n", pv);
      }
      break;
    } else {
      if(isdump) {
        if(x->dbg) {
          dbg_put(rsdl_MemoryError, "leak detected %p size %u at %s(%d) %s\n", x, x->allocated, x->dbg->file, x->dbg->line, x->dbg->function);
        } else {
          dbg_put(rsdl_MemoryError, "leak detected %p size %u\n", x, x->allocated);
        }
        rv += x->allocated;
      }
    }
  }
  return rv;
}

//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RMEMALLOC_H
