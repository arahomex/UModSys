#ifndef __UMODSYS_COMMON_MEMORY_H
#define __UMODSYS_COMMON_MEMORY_H 1
//*****************************************************
// ums/core/cpu/cpumem.h: memory related functions
//*****************************************************

#include <ums/core/stdtypedefs.h>
#include <ums/core/arch.h>

namespace UModSys {
namespace core {

//***************************************
//***************************************

struct SMem;    // memory
struct SCMem;    // const memory
struct SMemShared;    // shared memory
struct SCMemShared;   // shared const memory

//struct SMemChunk;    // allocated memory

typedef struct SCMemShared UMS_CSMem;

struct UMS_MemAllocator;

template<typename T>
struct UMS_MemAllocatorPtr;

/*************************************************************
 Memory objects
 *************************************************************/

struct SMem {
  void *data;
  size_t size;
  //
  inline void assign(void *d, size_t s) { data=d; size=s; }
  inline void set(void *d, size_t s) { data=d; size=s; }
  inline void set(void *d, size_t s, size_t qs) { data=d; size=s*qs; }
  inline void clear(void) { data=NULL; size=0; }
  //
  inline SMem(void) : data(NULL), size(0) {}
  inline SMem(void *d, size_t s) : data(d), size(s) {}
  inline SMem(void *d, size_t s, size_t qs) : data(d), size(s*qs) {}
  //
  inline BByte* get_data(size_t shift) const { return reinterpret_cast<BByte*>(data)+shift; }
  inline BByte* get_data(void) const { return reinterpret_cast<BByte*>(data); }
  template<typename T> T* get_tdata(void) const { return reinterpret_cast<T*>(data); }
  template<typename T> T* get_tdata(int shift) const { return reinterpret_cast<T*>(reinterpret_cast<BByte*>(data)+shift); }
  template<typename T> int get_tsize(void) const { return size/sizeof(T) : 0; }
  //
  inline void move(int n) { size-=n; data=reinterpret_cast<BByte*>(data)+n; }
  inline void move(size_t n) { size-=n; data=reinterpret_cast<BByte*>(data)+n; }
  inline void framemove(int n) { data=reinterpret_cast<BByte*>(data)+n; }
  inline bool split(SMem& v, size_t n) {
    if(n>size)
      return false;
    v.data = data; v.size = n;
    size-=n; data=reinterpret_cast<BByte*>(data)+n;
    return true;
  }
  //
  template<typename T>
  inline void t_data(T* &value) const {
    value = reinterpret_cast<T*>(data);
  }
  template<typename T>
  inline void t_datasize(T* &value, int &count) const {
    value = reinterpret_cast<T*>(data);
    count = size/sizeof(T);
  }
  template<typename T>
  inline void t_data(T* &value, int shift) const {
    value = reinterpret_cast<T*>( reinterpret_cast<BByte*>(data) + shift );
  }
  //
  template<typename T>
  inline void t_cdata(const T* &value) const {
    value = reinterpret_cast<const T*>(data);
  }
  template<typename T>
  inline void t_cdatasize(const T* &value, int &count) const {
    value = reinterpret_cast<const T*>(data);
    count = size/sizeof(T);
  }
  template<typename T>
  inline void t_cdata(const T* &value, int shift) const {
    value = reinterpret_cast<const T*>( reinterpret_cast<const BByte*>(data) + shift );
  }
  //
  template<typename T>
  inline bool t_data_out(T* &value, size_t count) {
    size_t n = count*sizeof(T);
    if(n>size) return false;
    value = reinterpret_cast<T*>(data);
    size -= n; data = reinterpret_cast<BByte*>(data) + n;
    return true;
  }
  template<typename T>
  inline bool t_data_out(const T* value, size_t count) {
    size_t n = count*sizeof(T);
    if(n>size) return false;
    memcpy(data, value, n);
    size -= n; data = reinterpret_cast<BByte*>(data) + n;
    return true;
  }
  //
  inline bool ucopy(SCMem& rv, const SCMem& R) const;
  inline bool put(const SCMem& R);
  inline bool put(const SMem& R);
};

struct SCMem {
  const void *data;
  size_t size;
  //
  inline void assign(const void *d, size_t s) { data=d; size=s; }
  inline void set(const void *d, size_t s) { data=d; size=s; }
  inline void set(const void *d, size_t s, size_t qs) { data=d; size=s*qs; }
  inline void clear(void) { data=NULL; size=0; }
  //
  inline SCMem(void) : data(NULL), size(0) {}
  inline SCMem(const void *d, size_t s) : data(d), size(s) {}
  inline SCMem(const void *d, size_t s, size_t qs) : data(d), size(s*qs) {}
  inline SCMem(const SMem& m) : data(m.data), size(m.size) {}
  inline SCMem& operator=(const SMem& m) { data = m.data; size = m.size; return *this; }
  //
  inline const BByte* get_data(size_t shift) const { return reinterpret_cast<const BByte*>(data)+shift; }
  inline const BByte* get_data(void) const { return reinterpret_cast<const BByte*>(data); }
  template<typename T> const T* get_tdata(void) const { return reinterpret_cast<const T*>(data); }
  template<typename T> const T* get_tdata(int shift) const { return reinterpret_cast<const T*>(reinterpret_cast<const BByte*>(data)+shift); }
  template<typename T> int get_tsize(void) const { return size/sizeof(T) : 0; }
  //
  inline void move(int n) { size-=n; data=reinterpret_cast<const BByte*>(data)+n; }
  inline void move(size_t n) { size-=n; data=reinterpret_cast<const BByte*>(data)+n; }
  inline void framemove(int n) { data=reinterpret_cast<const BByte*>(data)+n; }
  inline bool split(SCMem& v, size_t n) {
    if(n>size)
      return false;
    v.data = data; v.size = n;
    size-=n; data=reinterpret_cast<const BByte*>(data)+n;
    return true;
  }
  //
  template<typename T>
  inline void t_data(const T* &value) const {
    value = reinterpret_cast<const T*>(data);
  }
  template<typename T>
  inline void t_datasize(const T* &value, int &count) const {
    value = reinterpret_cast<const T*>(data);
    count = size/sizeof(T);
  }
  template<typename T>
  inline void t_data(const T* &value, int shift) const {
    value = reinterpret_cast<const T*>( reinterpret_cast<const BByte*>(data) + shift );
  }
  //
  template<typename T>
  inline bool t_data_in(const T* &value, size_t count) {
    size_t n = count*sizeof(T);
    if(n>size) return false;
    value = reinterpret_cast<const T*>(data);
    size -= n; data = reinterpret_cast<const BByte*>(data) + n;
    return true;
  }
  template<typename T>
  inline bool t_data_in(T* value, size_t count) {
    size_t n = count*sizeof(T);
    if(n>size) return false;
    memcpy(value, data, n);
    size -= n; data = reinterpret_cast<const BByte*>(data) + n;
    return true;
  }
};

inline bool SMem::ucopy(SCMem& rv, const SCMem& R) const 
{
  if(size<R.size)
    return false;
  memcpy(data, R.data, R.size);
  rv.assign(data, R.size);
  return true;
}

inline bool SMem::put(const SCMem& R)  
{
  if(size<R.size)
    return false;
  memcpy(data, R.data, R.size);
  move(R.size);
  return true;
}

inline bool SMem::put(const SMem& R)  
{
  if(size<R.size)
    return false;
  memcpy(data, R.data, R.size);
  move(R.size);
  return true;
}

/*************************************************************
 Fixed buffers
 *************************************************************/

template<size_t BufSize>
struct TMemBuffer : public SMem {
  BByte buffer[BufSize];
  //
  inline TMemBuffer(void) : SMem(buffer, BufSize) {}
  inline void reset(void) { assign(buffer, BufSize); }
  inline SCMem filled(void) const { return SCMem(buffer, get_data()-buffer); }
  inline SMem filled(void) { return SMem(buffer, get_data()-buffer); }
  inline size_t filled_size(void) const { return get_data()-buffer; }
};

template<size_t BufSize>
struct TCMemBuffer : public SCMem {
  BByte buffer[BufSize];
  //
  inline TCMemBuffer(void) : SCMem(buffer, BufSize) {}
  inline void reset(void) { assign(buffer, BufSize); }
  //
  operator SMem(void) { return SMem(buffer, BufSize); }
  SMem getw(void) { return SMem(buffer, BufSize); }
};

/*************************************************************
 Ring buffer
 *************************************************************/

struct SRingBuffer : public SMem {
  size_t cshift, csize;
  //
  inline SRingBuffer(void *d, size_t s) : SMem(d, s), cshift(0), csize(0) {}
  inline SRingBuffer(void) : cshift(0), csize(0) {}
  //
  void operator=(const SMem& mem) { clear(); SMem::operator=(mem); }
  void clear(void) { SMem::clear(); cshift=csize=0; }
  //
  inline size_t get_free(void) const { return size-csize; }
  inline size_t get_used(void) const { return csize; }
  //
  inline bool insert(const SCMem& mem) {
    size_t s2 = mem.size;
    if(s2==0)
      return true;
    if(s2>size-csize)
      return false;
    size_t start = (cshift+csize)%size;
    size_t left = size-start;
    if(left>s2) { // once
      memcpy(get_data(start), mem.data, s2);
      csize += s2;
      return true;
    }
    memcpy(get_data(start), mem.data, left);
    memcpy(data, mem.get_data(left), s2 - left);
    csize += s2;
    return true;
  }
  //
  inline bool remove(const SMem& mem) {
    size_t s2 = mem.size;
    if(s2==0)
      return true;
    if(s2>csize)
      return false;
    size_t left = size-cshift;
    if(size-cshift>s2) { // once
      if(mem.data!=NULL)
        memcpy(mem.data, get_data(cshift), s2);
      cshift += s2; csize -= s2;
    } else {
      if(mem.data!=NULL) {
        memcpy(mem.data, get_data(cshift), left);
        memcpy(mem.get_data(left), data, s2 - left);
      }
      cshift = s2-left; csize -= s2;
    }
    return true;
  }
  //
  inline bool replace(const SCMem& mem, size_t shift=0) {
    size_t s2 = mem.size;
    if(s2==0)
      return true;
    if(s2+shift>csize)
      return false;
    if(shift<size-cshift) { // start in first place
      size_t left = size-cshift-shift;
      if(left>s2) { // once
        memcpy(get_data(cshift+shift), mem.data, s2);
      } else {
        memcpy(get_data(cshift+shift), mem.data, left);
        memcpy(data, mem.get_data(left), s2 - left);
      }
    } else { // start in second place
      shift = (cshift+shift)%size;
      memcpy(get_data(shift), mem.data, s2);
    }
    return true;
  }
  //
  inline bool lookup(const SMem& mem, size_t shift=0) {
    size_t s2 = mem.size;
    if(s2==0)
      return true;
    if(s2+shift>csize)
      return false;
    if(shift<size-cshift) { // start in first place
      size_t left = size-cshift-shift;
      if(left>s2) { // once
        memcpy(mem.data, get_data(cshift+shift), s2);
      } else {
        memcpy(mem.data, get_data(cshift+shift), left);
        memcpy(mem.get_data(left), data, s2 - left);
      }
    } else { // start in second place
      shift = (cshift+shift)%size;
      memcpy(mem.data, get_data(shift), s2);
    }
    return true;
  }
};

/*************************************************************
 Memory fixed alloc chunk
 *************************************************************/

struct SMemChunk : private SMem {
  inline void clear(void) {}
  inline SMemChunk(void) {}
  //
  inline operator SCMem(void) const { return SCMem(data, size); }
  inline operator SMem(void) const { return SMem(data, size); }
  inline SMem get(void) const { return SMem(data, size); }
  inline size_t get_size(void) const { return size; }
  inline void* get_data(void) const { return data; }
  //
  inline SMem& _alter(void) { return *this; }
  //
  template<typename T>
  inline void t_data(T* &value) const {
    value = reinterpret_cast<T*>(data);
  }
  template<typename T>
  inline void t_data(T* &value, int shift) const {
    value = reinterpret_cast<T*>( reinterpret_cast<byte*>(data) + shift );
  }
};

/*************************************************************
 Memory shared chunk
 *************************************************************/

struct SMemorySharedData {
  IMemHeap *heap;
  size_t size;
  int rc_count;
  //
  void link(void);
  void unlink(void);
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

struct UMS_MemAllocator {
  struct SizeMemory {};
  struct SizeMemoryArray {};
  struct SizeSingle {};
  struct SizeSingleExt {};
  struct SizeArray {};
  struct SizeArrayExt {};
  //
  int allocated;
  SMem data;
  //
  inline UMS_MemAllocator(void) : allocated(0) {}
  inline void operator=(const SMem &d) { data = d; }
//  inline void operator=(const SMemChunk &d) { data = d.get(); }
  inline void operator=(const SMemShared &d) { data = d.get(); }
  inline int operator~(void) const { return allocated; }
  inline void clear(void) { allocated=0; data.clear(); }
  //
  template<typename T>
  inline void alloc(UMS_MemAllocatorPtr<T>& p) {
    p.offset = allocated;
    allocated += p.bytesize;
  }
  template<typename T>
  inline UMS_MemAllocator& operator<<(UMS_MemAllocatorPtr<T>& p)
    { alloc(p); return *this; }
  //
  template<typename T>
  inline void fill(UMS_MemAllocatorPtr<T>& p) {
    if(p.offset>=0) {
      data.t_data(p.data, p.offset);
    }
  }
  template<typename T>
  inline UMS_MemAllocator& operator>>(UMS_MemAllocatorPtr<T>& p)
    { fill(p); return *this; }
};

template<typename T>
struct UMS_MemAllocatorPtr {
  T* data;
  int bytesize;
  int offset;
  int quant;
  //
  inline UMS_MemAllocatorPtr(int bs, const UMS_MemAllocator::SizeMemory &)
  : data(NULL), offset(-1),
    bytesize(UMS_CPU_SIZE_CORRECT(bs)),
    quant(UMS_CPU_SIZE_CORRECT(bs)) {}
  inline UMS_MemAllocatorPtr(int count, int bs, const UMS_MemAllocator::SizeMemoryArray &)
  : data(NULL), offset(-1),
    bytesize(UMS_CPU_SIZE_CORRECT(bs)*count),
    quant(UMS_CPU_SIZE_CORRECT(bs)) {}
  inline UMS_MemAllocatorPtr(const UMS_MemAllocator::SizeSingle &)
  : data(NULL), offset(-1),
    bytesize(UMS_CPU_SIZE_CORRECT(sizeof(T))),
    quant(UMS_CPU_SIZE_CORRECT(sizeof(T))) {}
  inline UMS_MemAllocatorPtr(int ext, const UMS_MemAllocator::SizeSingleExt &)
  : data(NULL), offset(-1),
    bytesize(UMS_CPU_SIZE_CORRECT(sizeof(T)+ext)),
    quant(UMS_CPU_SIZE_CORRECT(sizeof(T)+ext)) {}
  inline UMS_MemAllocatorPtr(int count, const UMS_MemAllocator::SizeArray &)
  : data(NULL), offset(-1),
    bytesize(UMS_CPU_SIZE_CORRECT(sizeof(T))*count),
    quant(UMS_CPU_SIZE_CORRECT(sizeof(T))) {}
  inline UMS_MemAllocatorPtr(int count, int ext, const UMS_MemAllocator::SizeArrayExt &)
  : data(NULL), offset(-1),
    bytesize(UMS_CPU_SIZE_CORRECT(sizeof(T)+ext)*count),
    quant(UMS_CPU_SIZE_CORRECT(sizeof(T)+ext)) {}
  //
  inline operator T*(void) const { return data; }
  inline T* operator*(void) const { return data; }
  inline int operator~(void) const { return bytesize; }
  inline T* operator()(int id) const { return reinterpret_cast<T*>(reinterpret_cast<byte*>(data)+id*quant); }
};

//***************************************
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_COMMON_MEMORY_H
