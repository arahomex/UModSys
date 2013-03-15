#ifndef __UMODSYS_TL_ALLOCATE_ALLOCATOR_H
#define __UMODSYS_TL_ALLOCATE_ALLOCATOR_H 1
/*************************************************************/
// file: umodsys/tl/alloc/allocator.h
// info: simple allocator
/*************************************************************/

#include <umodsys/core/stdcore.h>
#include <umodsys/core/source.h>
#include <umodsys/core/allocator.h>

namespace UModSys {
namespace tl {

/*************************************************************/

using core::SSourceContext;

template<typename MemAlloc=core::SMemAlloc_Malloc, typename CountCalc=core::SCountCalcPower2>
struct TAllocator;

/*************************************************************/
/*************************************************************/

template<typename MemAlloc, typename CountCalc>
struct TAllocator : public MemAlloc {
  template<typename Elem>
  inline Elem* t_alloc_one(Elem* &elem, SSourceContext *ctx) {
    elem = static_cast<Elem*>( MemAlloc::mem_alloc(sizeof(Elem)) );
    return elem;
  };
  template<typename Elem>
  inline Elem* t_alloc_one_extra(Elem* &elem, size_t ex, SSourceContext *ctx) {
    elem = static_cast<Elem*>( MemAlloc::mem_alloc(sizeof(Elem)+ex) );
    return elem;
  };
  //
  template<typename Elem>
  inline void t_free_one(Elem* &elem) {
    if(elem) { MemAlloc::mem_free(elem); elem = NULL; }
  };
  template<typename Elem>
  inline void t_free_one_extra(Elem* &elem, size_t extra) {
    if(elem) { MemAlloc::mem_free(elem); elem = NULL; }
  };
  //
  template<typename Elem>
  inline bool t_realloc_array(Elem* &items, size_t &oldcount, size_t newcount, SSourceContext *ctx) {
    newcount = CountCalc::calculate_count(newcount, oldcount);
    void *p = MemAlloc::mem_realloc(items, sizeof(Elem)*newcount);
    if(p==NULL)
      return false;
    items = static_cast<Elem*>(p);
    oldcount = newcount;
    return true;
  }
  //
  template<typename Elem>
  inline bool t_free_array(Elem* &items, size_t &oldcount, size_t newcount, SSourceContext *ctx) {
    MemAlloc::mem_free(items);
    items = NULL;
    oldcount = 0;
    return true;
  }
};

/*************************************************************/
/*************************************************************/

} // namespace types
} // namespace NanoUTL

#endif // __UMODSYS_TL_ALLOCATE_ALLOCATOR_H