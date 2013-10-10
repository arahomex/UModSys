#ifndef __UMODSYS_TL_ALLOCATE_ALLOCATOR_H
#define __UMODSYS_TL_ALLOCATE_ALLOCATOR_H 1
/*************************************************************/
// file: umodsys/tl/alloc/allocator.h
// info: simple allocator
/*************************************************************/

#include <umodsys/stdcore.h>
#include <umodsys/core/source.h>
#include <umodsys/core/mem/mem_alloc.h>

namespace UModSys {
namespace tl {

/*************************************************************/

using core::SSourceContext;

template<typename MemAlloc, typename CountCalc=core::SCountCalcPower2> struct TAllocator;

typedef TAllocator<core::SMemAlloc_Malloc, core::SCountCalcPower2>    DAllocatorMallocFast;
typedef TAllocator<core::SMemAlloc_Malloc, core::SCountCalcExact>     DAllocatorMallocTight;

typedef TAllocator<core::SIMemAlloc, core::SCountCalcPower2>          DAllocatorFast;
typedef TAllocator<core::SIMemAlloc, core::SCountCalcExact>           DAllocatorTight;

/*************************************************************/
/*************************************************************/

template<typename MemAlloc, typename CountCalc>
struct TAllocator : public MemAlloc {
  typedef MemAlloc Allocator;
  //
  template<typename Elem>
  inline Elem* t_alloc_one(Elem* &elem, SSourceContext *ctx) {
    elem = static_cast<Elem*>( MemAlloc::mem_alloc(sizeof(Elem), ctx) );
    return elem;
  };
  template<typename Elem>
  inline Elem* t_alloc_one_extra(Elem* &elem, size_t ex, SSourceContext *ctx) {
    elem = static_cast<Elem*>( MemAlloc::mem_alloc(sizeof(Elem)+ex, ctx) );
    return elem;
  };
  //
  template<typename Elem>
  inline void t_free_one(Elem* &elem, SSourceContext *ctx) {
    if(elem) { MemAlloc::mem_free(elem, ctx); elem = NULL; }
  };
  template<typename Elem>
  inline void t_free_one_extra(Elem* &elem, size_t extra, SSourceContext *ctx) {
    if(elem) { MemAlloc::mem_free(elem, ctx); elem = NULL; }
  };
  //
  template<typename Elem>
  inline bool t_realloc_array(Elem* &items, size_t &oldcount, size_t newcount, SSourceContext *ctx) {
    newcount = CountCalc::calculate_count(newcount, oldcount);
    void *p = MemAlloc::mem_realloc(items, sizeof(Elem)*newcount, ctx);
    if(p==NULL)
      return false;
    items = static_cast<Elem*>(p);
    oldcount = newcount;
    return true;
  }
  //
  template<typename Elem>
  inline bool t_free_array(Elem* &items, size_t &oldcount, SSourceContext *ctx) {
    MemAlloc::mem_free(items, ctx);
    items = NULL;
    oldcount = 0;
    return true;
  }
};

/*************************************************************/
/*************************************************************/

} // namespace tl
} // namespace NanoUTL

#endif // __UMODSYS_TL_ALLOCATE_ALLOCATOR_H
