#ifndef __UMODSYS_CORE_MEM_SHARED_H
#define __UMODSYS_CORE_MEM_SHARED_H 1
/*************************************************************/
// file: umodsys/core/mem/mem_fixed.h
// info: shared chunk memory functions
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>
#include <umodsys/core/mem/mem_fixed.h>
#include <umodsys/core/mem/allocator.h>

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
  void link(void);
  void unlink(void);
  static SMemorySharedData* new_data(size_t num);
  inline void* data(void) { return this+1; }
  inline void* data(int add) { return static_cast<BByte*>(data())+add; }
};

/*************************************************************/

struct SMemShared {
  inline void clear(void) { if(data) { data->unlink(); data=NULL; } }
  inline ~SMemShared(void) { if(data) data->unlink(); }
  inline SMemShared(void) : data(NULL) {}
  inline SMemShared(const SMemShared& M) : data(M.data) { if(data) data->link(); }
  inline SMemShared(SMemorySharedData *adata) : data(adata) { if(data) data->link(); }
  inline SMemShared(size_t ns) : data(SMemorySharedData::new_data(ns)) { if(data) data->link(); }
  inline const SMemShared& operator=(const SMemShared& M)
    { if(M.data!=data) { if(data) data->unlink(); data=M.data; if(data) data->link(); } return *this; }
  inline const SMemShared& operator=(SMemorySharedData *adata)
    { if(data!=adata) { if(data) data->unlink(); data=adata; if(data) data->link(); } return *this; }
  //
  inline operator SCMem(void) const { return data ? SCMem(data->data(), data->size) : SCMem(); }
  inline operator SMem(void) const { return data ? SMem(data->data(), data->size) : SMem(); }
  inline SMem get(void) const { return data ? SMem(data->data(), data->size) : SMem(); }
  inline size_t get_size(void) const { return data ? data->size : 0; }
  inline void* get_data(void) const { return data ? data->data() : NULL; }
  inline void* get_data(int shift) const { return data ? data->data(shift) : NULL; }
  template<typename T> T* get_tdata(void) const { return data ? reinterpret_cast<T*>(data->data()) : NULL; }
  template<typename T> int get_tsize(void) const { return data ? data->size/sizeof(T) : 0; }
  //
  inline SMemorySharedData*& _alter(void) { return data; }
  inline SMemorySharedData* _get(void) const { return data; }
  //
  template<typename T>
  inline void t_data(T* &value) const {
    value = reinterpret_cast<T*>(get_data());
  }
  template<typename T>
  inline void t_data(T* &value, int shift) const {
    value = reinterpret_cast<T*>( get_data(shift) );
  }
protected:
  SMemorySharedData* data;
};

//***************************************

struct SCMemShared {
  inline void clear(void) { if(data) { data->unlink(); data=NULL; } }
  inline ~SCMemShared(void) { if(data) data->unlink(); }
  inline SCMemShared(void) : data(NULL) {}
  inline SCMemShared(const SMemShared& M) : data(M._get()) { if(data) data->link(); }
  inline SCMemShared(const SCMemShared& M) : data(M.data) { if(data) data->link(); }
  inline const SCMemShared& operator=(const SCMemShared& M)
    { if(M.data!=data) { if(data) data->unlink(); data=M.data; if(data) data->link(); } return *this; }
  inline const SCMemShared& operator=(const SMemShared& M)
    { if(M._get()!=data) { if(data) data->unlink(); data=M._get(); if(data) data->link(); } return *this; }
  //
  inline operator SCMem(void) const { return data ? SCMem(data->data(), data->size) : SCMem(); }
  inline SCMem get(void) const { return data ? SCMem(data->data(), data->size) : SCMem(); }
  inline size_t get_size(void) const { return data ? data->size : 0; }
  inline const void* get_data(void) const { return data ? data->data() : NULL; }
  inline const void* get_data(int shift) const { return data ? data->data(shift) : NULL; }
  template<typename T> const T* get_tdata(void) const { return data ? reinterpret_cast<const T*>(data->data()) : NULL; }
  template<typename T> int get_tsize(void) const { return data ? data->size/sizeof(T) : 0; }
  //
  inline SMemorySharedData*& _alter(void) { return data; }
  inline SMemorySharedData* _get(void) const { return data; }
  //
  template<typename T>
  inline void t_data(const T* &value) const {
    value = reinterpret_cast<const T*>(get_data());
  }
  template<typename T>
  inline void t_data(T* &value, int shift) const {
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