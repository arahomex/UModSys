#ifndef __UMODSYS_CORE_MEM_ALLOCATOR_H
#define __UMODSYS_CORE_MEM_ALLOCATOR_H 1
/*************************************************************/
// file: umodsys/core/mem/mem_alloc.h
// info: allocator bases
/*************************************************************/

#include <umodsys/core/source.h>

namespace UModSys {
namespace core {

//***************************************
//***************************************

const size_t mem_max_allocation = (~size_t(0)) - 0x100; // max allocation allowed is ~0 less 4K

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
    return newcount==0 ? 1 : newcount; // exact newcount or at least 1
  }
};

//***************************************
//***************************************

struct SMemAlloc_Malloc {
  inline static void* mem_alloc(size_t n, const SSourceContext* sctx) { return sctx ? UMODSYS_MALLOC(n, sctx->file, sctx->line) : UMODSYS_MALLOC(n, __FILE__, __LINE__); }
  inline static void* mem_realloc(void* op, size_t n, const SSourceContext* sctx) { return sctx ? UMODSYS_REALLOC(op, n, sctx->file, sctx->line) : UMODSYS_REALLOC(op, n, __FILE__, __LINE__); }
  inline static void mem_free(void *op, const SSourceContext* sctx) { return sctx ? UMODSYS_FREE(op, sctx->file, sctx->line) : UMODSYS_FREE(op, __FILE__, __LINE__); }
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
  inline void* mem_alloc(size_t n, const SSourceContext* sctx) const { return imem->mem_alloc(n, sctx); }
  inline void* mem_realloc(void* op, size_t n, const SSourceContext* sctx) const { return imem->mem_realloc(op, n, sctx); }
  inline void mem_free(void *op, const SSourceContext* sctx) const { imem->mem_free(op, sctx); }
  //
  inline operator IMemAlloc*(void) const { return imem; }
  inline IMemAlloc* operator ->(void) const { return imem; }
  inline IMemAlloc* operator *(void) const { return imem; }
};

//***************************************
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_ALLOCATOR_H
