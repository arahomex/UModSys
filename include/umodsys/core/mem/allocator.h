#ifndef __UMODSYS_CORE_MEM_ALLOCATOR_H
#define __UMODSYS_CORE_MEM_ALLOCATOR_H 1
/*************************************************************/
// file: umodsys/core/mem/allocator.h
// info: allocator bases
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>

namespace UModSys {
namespace core {

//***************************************
//***************************************

struct SCountCalcPower2 {
  inline static size_t calculate_count(size_t newcount, size_t oldcount) {
    register size_t o = oldcount;
    if(o==0) o = 1;
    while(o<newcount) o<<=1;
    return o; // power 2 of oldcount greater or equal newcount
  }
};

struct SCountCalcExact {
  inline static size_t calculate_count(size_t newcount, size_t oldcount) {
    return newcount==0 ? 1 : oldcount; // power 2 of oldcount greater or equal newcount
  }
};

//***************************************
//***************************************

struct SMemAlloc_Malloc {
  inline static void* mem_alloc(size_t n, const SSourceContext* sctx=NULL) { return ::malloc(n); }
  inline static void* mem_realloc(void* op, size_t n, const SSourceContext* sctx=NULL) { return ::realloc(op, n); }
  inline static void mem_free(void *op, const SSourceContext* sctx=NULL) { return ::free(op); }
  inline static const SMemAlloc_Malloc& get_default(void) { static SMemAlloc_Malloc rv; return rv; }
};

//***************************************

struct IMemAlloc {
  virtual void* mem_alloc(size_t n, const SSourceContext* sctx) =0;
  virtual void* mem_realloc(void* op, size_t n, const SSourceContext* sctx) =0;
  virtual void mem_free(void *op, const SSourceContext* sctx) =0;
};

struct SIMemAlloc {
public:
  IMemAlloc* imem;
public:
  inline SIMemAlloc(IMemAlloc *im) : imem(im) {}
  SIMemAlloc(void); // use module information
  //
  inline void* mem_alloc(size_t n, const SSourceContext* sctx=NULL) { return imem->mem_alloc(n, sctx); }
  inline void* mem_realloc(void* op, size_t n, const SSourceContext* sctx=NULL) { return imem->mem_realloc(op, n, sctx); }
  inline void mem_free(void *op, const SSourceContext* sctx=NULL) { imem->mem_free(op, sctx); }
};

//***************************************
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_ALLOCATOR_H
