#ifndef __UMODSYS_TL_COMPOSITE_ARRAY_H
#define __UMODSYS_TL_COMPOSITE_ARRAY_H 1
/*************************************************************/
// file: umodsys/tl/composite/array.h
// info: array of elements
/*************************************************************/

#include <umodsys/core/stdcore.h>

#include <umodsys/tl/util/type_constructor.h>
#include <umodsys/tl/alloc/allocator.h>

namespace UModSys {
namespace tl {

using core::array_index_none;

/*************************************************************/

template<typename SNode>
struct TArrayAbstract;

template<typename SNode, typename Allocator=DAllocatorFast >
struct TArray;

template<typename SNode, typename Allocator=DAllocatorFast >
struct TArrayChunked;

template<typename SNode, size_t nSize>
struct TArrayFixed;

template<typename SNode, size_t nSize>
struct TArrayPtrFixed;

template<typename SNode, typename Allocator=DAllocatorFast >
struct TArrayCache;

/*************************************************************/

template<typename SNode>
struct TArrayAbstract {
protected:
  typedef TTypeConstructor<SNode> TC;
  //
  SNode *items;
  size_t count;
public:
  inline TArrayAbstract(SNode *i, size_t n=0) : items(i), count(n) {}
  inline ~TArrayAbstract(void) {}
  //
  inline const SNode* get_all(void) const { return items; }
  inline SNode* get_all(void) { return items; }
  inline const SNode* _all(void) const { return items; }
  inline SNode* _all(void) { return items; }
  inline const SNode* operator->(void) const { return items; }
  inline SNode* operator->(void) { return items; }
  //
  inline const SNode& get(size_t no) const { return items[no]; }
  inline SNode& get(size_t no) { return items[no]; }
  inline const SNode& operator()(size_t no) const { return items[no]; }
  inline const SNode& operator[](size_t no) const { return items[no]; }
  inline SNode& operator[](size_t no) { return items[no]; }
  //
  inline const SNode& last(void) const { return items[count-1]; }
  inline SNode& last(void) { return items[count-1]; }
  inline const SNode& first(void) const { return items[0]; }
  inline SNode& first(void) { return items[0]; }
  //
  inline size_t last_index(void) const { return count-1; }
  inline size_t first_index(void) const { return 0; }
  //
  inline size_t len(void) const { return count; }
  inline size_t operator~(void) const { return count; }
  //
  inline void copy(const SNode* ritems, size_t ecount) {
    if(ecount<count)
      ecount = count;
    TC::copy(items, ritems, ecount);
  }
protected:
  inline void item_construct(size_t no) { TC::construct(items+no); }
  inline void item_construct(size_t no, const SNode& def) { TC::construct(items+no, def); }
  inline void item_destruct(size_t no) { TC::destruct(items+no); }
  inline void items_construct(size_t no, size_t count) { TC::aconstruct(items+no, count); }
  inline void items_construct(size_t no, size_t count, const SNode& def) { TC::aconstructcopy(items+no, count, def); }
  inline void items_construct(size_t no, size_t count, const SNode* def) { TC::aconstructcopya(items+no, count, def); }
  inline void items_destruct(size_t no, size_t count) { TC::adestruct(items+no, count); }
};

/*************************************************************/

template<typename SNode, size_t nSize>
struct TArrayFixed : public TArrayAbstract<SNode> {
protected:
  typedef TArrayAbstract<SNode> Base;
  SNode fixed[nSize];
public:
  inline TArrayFixed(void) : Base(fixed, nSize) {}
  inline ~TArrayFixed(void) {}
};

//***************************************

template<typename SNode, size_t nSize>
struct TArrayPtrFixed : public TArrayAbstract<SNode> {
protected:
  typedef TArrayAbstract<SNode> Base;
  SNode fixed[nSize];
public:
  inline TArrayPtrFixed(void) : Base(fixed, nSize) { items_construct(0, nSize, SNode(NULL)); }
  inline ~TArrayPtrFixed(void) {}
};

/*************************************************************/

template<typename SNode, typename Allocator>
struct TArray : public TArrayAbstract<SNode> {
protected:
  typedef TArrayAbstract<SNode> Base;
  //
  size_t allocated;
  Allocator allocator;
public:
  TArray(void);
  inline TArray(const Allocator& a) : Base(NULL, 0), allocated(0), allocator(a) {}
  inline ~TArray(void) { free(); }
  //
  inline size_t maxlen(void) const { return allocated; }
  //
  bool push(const SNode& def);
  inline bool push(void) { return resize(Base::count+1); }
  inline bool pushn(size_t n=1) { return resize(Base::count+n); }
  //
  inline bool pop(SNode& def) { if(Base::count==0) return false; def = Base::last(); return resize(Base::count-1); }
  inline bool pop(void) { return resize(Base::count-1); }
  inline bool popn(size_t n=1) { return resize(Base::count-n); }
  //
  bool resize(size_t newsize);
  bool free(void);
  bool clear(void) { return free(); }
  bool remove_at(size_t id);
  bool insert_at(size_t id);
};

template<typename SNode, typename Allocator>
inline TArray<SNode,Allocator>::TArray(void) 
: Base(NULL, 0), allocated(0), allocator() {}

/*************************************************************/

template<typename SNode, typename Allocator> 
struct TArrayCache {
  typedef SNode _Type, Type;
  //
  TArray<SNode, Allocator> values;
  const _Type *cvalues;
  size_t count;
  bool dirty;
  //
  inline TArrayCache() 
    : cvalues(NULL), count(0), dirty(true) {
  }
  inline ~TArrayCache(void) {
  }
  //
  inline void alloc(const _Type* t, size_t n) { 
    dirty = true; // must be re-processed by any way
    cvalues = t;
    count = n;
  }
  inline _Type* alloc(size_t n) { 
    dirty = true; // must be re-processed by any way
    cvalues = NULL;
    count = 0;
    if(values.resize(n)) {
      cvalues = values.get_all();
      count = n;
      return values.get_all();
    }
    return NULL;
  }
  //
  void clear(void) { dirty=false; } // not deallocate now
  //
  inline _Type& operator[](size_t id) const { return values[id]; }
  inline operator _Type*(void) const { return values.get_all(); }
  inline operator const _Type*(void) const { return cvalues; }
  inline const _Type* operator()(void) const { return cvalues; }
  inline size_t operator~(void) const { return count; }
};

/*************************************************************/
// INLINES/OUTLINES
/*************************************************************/

template<typename SNode, typename Allocator>
bool TArray<SNode, Allocator>::insert_at(size_t id) 
{
    if(id==array_index_none || id>Base::count)
      return false;
    return true;
}

template<typename SNode, typename Allocator>
bool TArray<SNode, Allocator>::remove_at(size_t id) 
{
    if(id==array_index_none || id>=Base::count)
      return false;
    Base::item_destruct(id);
    Base::count--;
    if(id<Base::count)
      memmove(Base::items+id, Base::items+id+1, (Base::count-id)*sizeof(SNode));
    return true;
}

template<typename SNode, typename Allocator>
bool TArray<SNode, Allocator>::free(void) 
{
  if(!resize(0))
    return false;
  if(!allocator.t_free_array(Base::items, allocated, 0, UMODSYS_SOURCEINFO))
    return false;
  return true;
}

template<typename SNode, typename Allocator>
bool TArray<SNode, Allocator>::push(const SNode& def) 
{
    if(Base::count+1>allocated)
      if(!allocator.t_realloc_array(Base::items, allocated, Base::count+1, UMODSYS_SOURCEINFO))
        return false;
    items_construct(Base::count, 1, def);
    Base::count++;
    return true;
}

template<typename SNode, typename Allocator>
bool TArray<SNode, Allocator>::resize(size_t newsize) 
{
  if(newsize==array_index_none)
    return false;
  if(newsize==Base::count)
    return true;
  if(newsize>Base::count) {
    if(newsize>allocated)
      if(!allocator.t_realloc_array(Base::items, allocated, newsize, UMODSYS_SOURCEINFO))
        return false;
    Base::items_construct(Base::count, newsize-Base::count);
    Base::count = newsize;
  } else {
    Base::items_destruct(newsize, Base::count-newsize);
    Base::count = newsize;
  }
  return true;
}

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_COMPOSITE_ARRAY_H
