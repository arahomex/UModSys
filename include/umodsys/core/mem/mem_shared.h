#ifndef __UMODSYS_CORE_MEM_SHARED_H
#define __UMODSYS_CORE_MEM_SHARED_H 1
/*************************************************************/
// file: umodsys/core/mem/mem_fixed.h
// info: shared chunk memory functions
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>
#include <umodsys/core/mem/mem_fixed.h>
#include <umodsys/core/mem/mem_alloc.h>

namespace UModSys {
namespace core {

//***************************************
//***************************************

struct SMemorySharedData;  // shared allocated memory data
struct SMemShared;         // shared memory
struct SCMemShared;        // shared const memory


/*************************************************************
 Memory shared chunk
 *************************************************************/

struct SMemorySharedData  {
  IMemAlloc *heap;
  size_t size;
  int rc_count;
  //

  void link(const SSourceContext* sctx);
  void unlink(const SSourceContext* sctx);
  static SMemorySharedData* new_data(IMemAlloc *h, size_t num, const SSourceContext* sctx);
  //
  inline void* data(void) { return this+1; }
  inline void* data(ptrdiff_t add) { return static_cast<BByte*>(data())+add; }
  inline SMemorySharedData* init(size_t sz, IMemAlloc *h) { rc_count=0; size=sz; heap = h; return this; }
  inline static SMemorySharedData* new_data(size_t num, const SSourceContext* sctx) { return new_data(shared_memory(), num, sctx); }
};

/*************************************************************/

struct SMemShared {
  inline ~SMemShared(void) { clear(); }
  inline SMemShared(void) : data(NULL) {}
  inline SMemShared(const SMemShared& M) : data(NULL) { link(M.data, UMODSYS_SOURCEINFO); }
  inline SMemShared(SMemorySharedData *adata) : data(NULL) { link(adata, UMODSYS_SOURCEINFO); }
  inline SMemShared(size_t ns, SSourceContext* ctx) : data(NULL) { link(SMemorySharedData::new_data(ns, ctx), ctx); }
  inline SMemShared(size_t ns) : data(NULL) { link(SMemorySharedData::new_data(ns, UMODSYS_SOURCEINFO), UMODSYS_SOURCEINFO); }
  inline SMemShared(IMemAlloc *h, size_t ns, SSourceContext* ctx) : data(NULL) { link(SMemorySharedData::new_data(h, ns, ctx), ctx); }
  inline SMemShared(IMemAlloc *h, size_t ns) : data(NULL) { link(SMemorySharedData::new_data(h, ns, UMODSYS_SOURCEINFO), UMODSYS_SOURCEINFO); }
  inline SMemShared(const SCMem& M, SSourceContext* ctx) : data(NULL) { alloc(M, ctx); }
  inline SMemShared(const SCMem& M) : data(NULL) { alloc(M, UMODSYS_SOURCEINFO); }
  //
  inline const SMemShared& operator=(const SMemShared& M) { link(M.data, UMODSYS_SOURCEINFO); return *this; }
  inline const SMemShared& operator=(SMemorySharedData *adata) { link(adata, UMODSYS_SOURCEINFO); return *this; }
  inline const SMemShared& operator=(const SCMem& M) { alloc(M, UMODSYS_SOURCEINFO); return *this; }
  //
  inline void clear(void) { link(NULL, UMODSYS_SOURCEINFO); }
  inline bool link(SMemorySharedData *p, SSourceContext* ctx) {
    if(p==data) 
      return true;
    if(data) 
      data->unlink(ctx); 
    data = p;
    if(data) 
      data->link(ctx);
    return true;
  }
  inline bool alloc(size_t sz, SSourceContext* ctx) { 
    SMemorySharedData *p = SMemorySharedData::new_data(sz, ctx); 
    if(p==NULL)
      return false;
    return link(p, ctx);
  }
  inline bool alloc(IMemAlloc *h, size_t sz, SSourceContext* ctx) { 
    SMemorySharedData *p = SMemorySharedData::new_data(h, sz, ctx); 
    if(p==NULL)
      return false;
    return link(p, ctx);
  }
  inline bool alloc(const SCMem& M, SSourceContext* ctx) {
    SMemorySharedData *p = SMemorySharedData::new_data(M.size, ctx);
    if(p==NULL)
      return false;
    memcpy(p->data(), M.data, M.size);
    return link(p, ctx);
  }
  //
  inline bool valid(void) const { return data!=NULL; }
  inline operator SCMem(void) const { return data ? SCMem(data->data(), data->size) : SCMem(); }
  inline operator SMem(void) const { return data ? SMem(data->data(), data->size) : SMem(); }
  inline SMem get(void) const { return data ? SMem(data->data(), data->size) : SMem(); }
  inline size_t get_size(void) const { return data ? data->size : 0; }
  inline void* get_data(void) const { return data ? data->data() : NULL; }
  inline void* get_data(ptrdiff_t shift) const { return data ? data->data(shift) : NULL; }
  template<typename T> T* get_tdata(void) const { return data ? reinterpret_cast<T*>(data->data()) : NULL; }
  template<typename T> T* get_tdata(ptrdiff_t shift) const { return data ? reinterpret_cast<T*>(data->data(shift)) : NULL; }
  template<typename T> size_t get_tsize(void) const { return data ? data->size/sizeof(T) : 0; }
  //
  inline size_t operator~(void) const { return get_size(); }
  inline void* operator()(void) const { return get_data(); }
  inline void* operator()(ptrdiff_t shift) const { return get_data(shift); }
  inline BByte& operator[](ptrdiff_t shift) const { return get_tdata<BByte>()[shift]; }
  //
  inline SMemorySharedData*& _alter(void) { return data; }
  inline SMemorySharedData* _get(void) const { return data; }
  //
  template<typename T>
  inline void t_data(T* &value) const {
    value = reinterpret_cast<T*>(get_data());
  }
  template<typename T>
  inline void t_data(T* &value, ptrdiff_t shift) const {
    value = reinterpret_cast<T*>( get_data(shift) );
  }
protected:
  friend struct SCMemShared;
  SMemorySharedData* data;
};

//***************************************

struct SCMemShared {
  inline ~SCMemShared(void) { clear(); }
  inline SCMemShared(void) : data(NULL) {}
  inline SCMemShared(const SMemShared& M) : data(NULL) { link(M.data, UMODSYS_SOURCEINFO); }
  inline SCMemShared(const SCMemShared& M) : data(NULL) { link(M.data, UMODSYS_SOURCEINFO); }
  //
  inline const SCMemShared& operator=(SMemorySharedData* M) { link(M, UMODSYS_SOURCEINFO); return *this; }
  inline const SCMemShared& operator=(const SCMemShared& M) { link(M.data, UMODSYS_SOURCEINFO); return *this; }
  inline const SCMemShared& operator=(const SMemShared& M) { link(M.data, UMODSYS_SOURCEINFO); return *this; }
  //
  inline void clear(void) { link(NULL, UMODSYS_SOURCEINFO); }
  inline bool link(SMemorySharedData *p, SSourceContext* ctx) {
    if(p==data) 
      return true;
    if(data) 
      data->unlink(ctx); 
    data = p;
    if(data) 
      data->link(ctx);
    return true;
  }
  //
  inline bool valid(void) const { return data!=NULL; }
  inline operator SCMem(void) const { return data ? SCMem(data->data(), data->size) : SCMem(); }
  inline SCMem get(void) const { return data ? SCMem(data->data(), data->size) : SCMem(); }
  inline size_t get_size(void) const { return data ? data->size : 0; }
  inline const void* get_data(void) const { return data ? data->data() : NULL; }
  inline const void* get_data(ptrdiff_t shift) const { return data ? data->data(shift) : NULL; }
  template<typename T> const T* get_tdata(void) const { return data ? reinterpret_cast<const T*>(data->data()) : NULL; }
  template<typename T> const T* get_tdata(ptrdiff_t shift) const { return data ? reinterpret_cast<const T*>(data->data(shift)) : NULL; }
  template<typename T> size_t get_tsize(void) const { return data ? data->size/sizeof(T) : 0; }
  //
  inline size_t operator~(void) const { return get_size(); }
  inline const void* operator()(void) const { return get_data(); }
  inline const void* operator()(ptrdiff_t shift) const { return get_data(shift); }
  inline BByte operator[](ptrdiff_t shift) const { return get_tdata<BByte>()[shift]; }
  //
  inline SMemorySharedData*& _alter(void) { return data; }
  inline SMemorySharedData* _get(void) const { return data; }
  //
  template<typename T>
  inline void t_data(const T* &value) const {
    value = reinterpret_cast<const T*>(get_data());
  }
  template<typename T>
  inline void t_data(T* &value, ptrdiff_t shift) const {
    value = reinterpret_cast<const T*>( get_data(shift) );
  }
protected:
  SMemorySharedData* data;
};

//***************************************
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_MEM_SHARED_H
