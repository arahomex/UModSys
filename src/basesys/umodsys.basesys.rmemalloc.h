#ifndef __UMODSYS_BASE_RMEMALLOC_H
#define __UMODSYS_BASE_RMEMALLOC_H 1
/*************************************************************/
// file: src/base/system/umodsys.base.rmemalloc.h
// info: implementation of memory allocator
/*************************************************************/

#include "umodsys.basesys.rcommon.h"

#include <umodsys/core/common_exceptions.h>

namespace UModSys {
namespace base {
namespace rsystem {

using namespace core;


template<typename E1, typename E2=Void, typename E3=Void, typename E4=Void> struct TMemAllocHeader;

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
  inline void init(void* p) {}
  inline void init(const SSourceContext* d) {}
  inline bool valid(const void* p) const { return magic==eMagic; }
};

template<typename Pool>
struct TPool {
  Pool* pool;
  //
  inline void clear(void) {}
  inline void init(size_t sz) {}
  inline void init(void* p) { pool = static_cast<Pool*>(p); }
  inline void init(const SSourceContext* d) {}
  inline bool valid(const void* p) const { return pool==p; }
};

struct SDebug {
  const SSourceContext* dbg;
  //
  inline void clear(void) {}
  inline void init(size_t sz) {}
  inline void init(void* p) {}
  inline void init(const SSourceContext* d) { dbg = d; }
  inline bool valid(const void* p) const { return true; }
};

} // mem_headers

//***************************************
// TMemAllocHeader
//***************************************

template<typename E1, typename E2, typename E3, typename E4>
struct TMemAllocHeader : public E1, public E2, public E3, public E4 {
  size_t allocated;
  //
  inline void clear(void) { allocated = 0; E1::clear(); E2::clear(); E3::clear(); E4::clear(); }
  inline void init(size_t sz) { allocated = sz; E1::init(sz); E2::init(sz); E3::init(sz); E4::init(sz); }
  inline void init(void* p) { E1::init(p); E2::init(p); E3::init(p); E4::init(p); }
  inline void init(const SSourceContext* d) { E1::init(d); E2::init(d); E3::init(d); E4::init(d); }
  inline bool valid(const void* p) const { return E1::valid(p) && E2::valid(p) && E3::valid(p) && E4::valid(p); }
  inline size_t getsize(void) const { return allocated; }
};

template<typename E1>
struct TMemAllocHeader<E1, Void,Void,Void> : public E1 {
  size_t allocated;
  //
  inline void init(size_t sz) { allocated = 0; allocated = sz; E1::init(sz); }
  inline size_t getsize(void) const { return allocated; }
};

template<typename E1, typename E2>
struct TMemAllocHeader<E1, E2, Void,Void> : public E1, public E2 {
  size_t allocated;
  //
  inline void clear(void) { allocated = 0; E1::clear(); E2::clear(); }
  inline void init(size_t sz) { allocated = sz; E1::init(sz); E2::init(sz);  }
  inline void init(void* p) { E1::init(p); E2::init(p);  }
  inline void init(const SSourceContext* d) { E1::init(d); }
  inline bool valid(const void* p) const { return E1::valid(p) && E2::valid(p); }
  inline size_t getsize(void) const { return allocated; }
};

template<typename E1, typename E2, typename E3>
struct TMemAllocHeader<E1, E2, E3, Void> : public E1, public E2, public E3 {
  size_t allocated;
  //
  inline void clear(void) { allocated = 0; E1::clear(); E2::clear(); E3::clear(); }
  inline void init(size_t sz) { allocated = sz; E1::init(sz); E2::init(sz); E3::init(sz); }
  inline void init(void* p) { E1::init(p); E2::init(p); E3::init(p); }
  inline void init(const SSourceContext* d) { E1::init(d); E2::init(d); E3::init(d); }
  inline bool valid(const void* p) const { return E1::valid(p) && E2::valid(p) && E3::valid(p); }
  inline size_t getsize(void) const { return allocated; }
};

//***************************************
// TRMemAlloc
//***************************************

template<typename Header, typename SubAllocator>
struct TRMemAlloc_CC : public IMemAlloc, public SubAllocator
{
public:
public:
  TRMemAlloc_CC(IExceptionHandler* eh);
  ~TRMemAlloc_CC(void);
public:
  void* mem_alloc(size_t n, const SSourceContext* sctx);
  void* mem_realloc(void* op, size_t n, const SSourceContext* sctx);
  void mem_free(void *op, const SSourceContext* sctx);
public:
//  Header root;
  IExceptionHandler* err_handler;
public:
};


template<typename Header, typename SubAllocator>
TRMemAlloc_CC<Header, SubAllocator>::TRMemAlloc_CC(IExceptionHandler* eh)
: err_handler(eh) {
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
   h->init(n); h->init(sctx); h->init(this);
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
  h->clear(); h->init(sctx);
  SubAllocator::mem_free(h);
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
  h->clear(); h->init(sctx);
  SubAllocator::mem_free(h);
  return;
}

//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RMEMALLOC_H
