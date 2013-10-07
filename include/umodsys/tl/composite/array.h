#ifndef __UMODSYS_TL_COMPOSITE_ARRAY_H
#define __UMODSYS_TL_COMPOSITE_ARRAY_H 1
/*************************************************************/
// file: umodsys/tl/composite/array.h
// info: array of elements
/*************************************************************/

#include <umodsys/core/stdcore.h>

#include <umodsys/tl/util/type_constructor.h>
#include <umodsys/tl/alloc/allocator.h>
#include <umodsys/tl/metastl/throws.h>

namespace UModSys {
namespace tl {

using core::array_index_none;

/*************************************************************/

template<typename SNode> struct TArrayAbstract;

template<typename SNode, typename Allocator=DAllocatorFast > struct TArrayDynamic;
template<typename SNode, typename Allocator=DAllocatorFast > struct TArrayChunked;
template<typename SNode, typename Allocator=DAllocatorFast > struct TArrayCache;

template<typename SNode, size_t nSize> struct TArrayFixed;


/*************************************************************/

template<typename SNode>
struct TArrayAbstract {
protected:
  typedef TTypeConstructor<SNode> TC;
  // stl typedefs
  typedef SNode value_type;
  typedef SNode& reference;
  typedef const SNode& const_reference;
  typedef SNode* pointer;
  typedef const SNode& const_pointer;
  typedef pointer iterator;
  typedef const_pointer const_iterator;
  typedef pointer reverse_iterator;
  typedef const_pointer const_reverse_iterator;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  //
  SNode *items;
  size_t count;
public:
  inline TArrayAbstract(SNode *i, size_t n=0) UMODSYS_NOTHROW() : items(i), count(n) {}
  inline ~TArrayAbstract(void) UMODSYS_NOTHROW() {}
  //
  inline const SNode* get_all(void) const UMODSYS_NOTHROW() { return items; }
  inline SNode* get_all(void) UMODSYS_NOTHROW() { return items; }
  inline const SNode* _all(void) const UMODSYS_NOTHROW() { return items; }
  inline SNode* _all(void) UMODSYS_NOTHROW() { return items; }
  inline const SNode* operator->(void) const UMODSYS_NOTHROW() { return items; }
  inline SNode* operator->(void) UMODSYS_NOTHROW() { return items; }
  //
  inline const SNode& get(size_t no) const UMODSYS_NOTHROW() { return items[no]; }
  inline SNode& get(size_t no) UMODSYS_NOTHROW() { return items[no]; }
  inline const SNode& operator()(size_t no) const UMODSYS_NOTHROW() { return items[no]; }
  inline const SNode& operator[](size_t no) const UMODSYS_NOTHROW() { return items[no]; }
  inline SNode& operator[](size_t no) UMODSYS_NOTHROW() { return items[no]; }
  //
  inline const SNode& last(void) const UMODSYS_NOTHROW() { return items[count-1]; }
  inline SNode& last(void) UMODSYS_NOTHROW() { return items[count-1]; }
  inline const SNode& first(void) const UMODSYS_NOTHROW() { return items[0]; }
  inline SNode& first(void) UMODSYS_NOTHROW() { return items[0]; }
  //
  inline size_t last_index(void) const UMODSYS_NOTHROW() { return count-1; }
  inline size_t first_index(void) const UMODSYS_NOTHROW() { return 0; }
  //
  inline size_t len(void) const UMODSYS_NOTHROW() { return count; }
  inline size_t operator~(void) const UMODSYS_NOTHROW() { return count; }
  //
  inline void copy(const SNode* ritems, size_t ecount) {
    if(ecount<count)
      ecount = count;
    TC::copy(items, ritems, ecount);
  }
public:
  // stl typedefs
  inline iterator begin(void) UMODSYS_NOTHROW() { return items; }
  inline iterator end(void) UMODSYS_NOTHROW() { return items+count; }
  inline const_iterator begin(void) const UMODSYS_NOTHROW() { return items; }
  inline const_iterator end(void) const UMODSYS_NOTHROW() { return items+count; }
  inline const_iterator cbegin(void) const UMODSYS_NOTHROW() { return items; }
  inline const_iterator cend(void) const UMODSYS_NOTHROW() { return items+count; }
  //
  inline reverse_iterator rend(void) UMODSYS_NOTHROW() { return items-1; }
  inline reverse_iterator rbegin(void) UMODSYS_NOTHROW() { return items+count-1; }
  inline const_reverse_iterator rbegin(void) const UMODSYS_NOTHROW() { return items+count-1; }
  inline const_reverse_iterator rend(void) const UMODSYS_NOTHROW() { return items-1; }
  inline const_reverse_iterator crbegin(void) const UMODSYS_NOTHROW() { return items+count-1; }
  inline const_reverse_iterator crend(void) const UMODSYS_NOTHROW() { return items-1; }
  //
  inline size_type size(void) const UMODSYS_NOTHROW() { return count; }
  inline bool empty(void) const UMODSYS_NOTHROW() { return count==0; }
  inline const_pointer data(void) const UMODSYS_NOTHROW() { return items; }
  inline pointer data(void) UMODSYS_NOTHROW() { return items; }
  //
  inline const_reference at(size_type n) const UMODSYS_NOTHROW() { return get(n); }
  inline reference at(size_type n) UMODSYS_NOTHROW() { return get(n); }
  inline const_reference front(void) const UMODSYS_NOTHROW() { return first(); }
  inline reference front(void) UMODSYS_NOTHROW() { return first(); }
  inline const_reference back(void) const UMODSYS_NOTHROW() { return last(); }
  inline reference back(void) UMODSYS_NOTHROW() { return last(); }
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
  inline TArrayFixed(void) UMODSYS_NOTHROW() : Base(fixed, nSize) {}
  inline ~TArrayFixed(void) UMODSYS_NOTHROW() {}
public:
  // stl typedefs
};


/*************************************************************/

template<typename SNode, typename Allocator>
struct TArrayDynamic : public TArrayAbstract<SNode> {
protected:
  typedef TArrayAbstract<SNode> Base;
  //
  size_t allocated;
  Allocator allocator;
public:
  TArrayDynamic(void) UMODSYS_NOTHROW();
  inline TArrayDynamic(const Allocator& a) UMODSYS_NOTHROW() : Base(NULL, 0), allocated(0), allocator(a) {}
  inline ~TArrayDynamic(void) UMODSYS_NOTHROW() { free(); }
  //
  inline size_t maxlen(void) const UMODSYS_NOTHROW() { return allocated; }
  bool set_maxlen(size_t newmaxsize) UMODSYS_NOTHROW();
  //
  bool push(const SNode& def) UMODSYS_NOTHROW();
  inline bool push(void) UMODSYS_NOTHROW() { return resize(Base::count+1); }
  inline bool pushn(size_t n=1) UMODSYS_NOTHROW() { return resize(Base::count+n); }
  //
  inline bool pop(SNode& def) UMODSYS_NOTHROW() { if(Base::count==0) return false; def = Base::last(); return resize(Base::count-1); }
  inline bool pop(void) UMODSYS_NOTHROW() { return resize(Base::count-1); }
  inline bool popn(size_t n=1) UMODSYS_NOTHROW() { return resize(Base::count-n); }
  //
  bool resize(size_t newsize) UMODSYS_NOTHROW();
  bool free(void) UMODSYS_NOTHROW();
  bool clear(void) UMODSYS_NOTHROW() { return free(); }
  bool remove_at(size_t id) UMODSYS_NOTHROW();
  bool insert_at(size_t id) UMODSYS_NOTHROW();
public:
  // stl typedefs
  inline size_type max_size(void) const UMODSYS_NOTHROW() { return allocated; }
  void reserve(size_t newmaxsize) { if(!set_maxlen(newmaxlen)) throw_memoryerror(); }
  //
  inline void push_back(Base::const_reference def) { if(!push(def)) throw_memoryerror(); }
  inline void pop_back(void) { if(!pop(def)) throw_memoryerror(); }
  //
  void insert(Base::iterator pos, Base::const_reference v);
  void insert(Base::iterator pos, Base::size_type n, Base::const_reference v);
  template <class InputIterator> void insert(Base::iterator pos, InputIterator first, InputIterator last);
  //
  void erase(Base::iterator pos);
  void erase(Base::iterator first, Base::iterator last);
};

template<typename SNode, typename Allocator>
inline TArrayDynamic<SNode,Allocator>::TArrayDynamic(void) 
: Base(NULL, 0), allocated(0), allocator() {}

/*************************************************************/

template<typename SNode, typename Allocator> 
struct TArrayCache {
  typedef SNode _Type, Type;
  //
  TArrayDynamic<SNode, Allocator> values;
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
bool TArrayDynamic<SNode, Allocator>::insert_at(size_t id) 
{
    if(id==array_index_none || id>Base::count)
      return false;
    return true;
}

template<typename SNode, typename Allocator>
bool TArrayDynamic<SNode, Allocator>::remove_at(size_t id) 
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
bool TArrayDynamic<SNode, Allocator>::free(void) 
{
  if(!resize(0))
    return false;
  if(!allocator.t_free_array(Base::items, allocated, 0, UMODSYS_SOURCEINFO))
    return false;
  return true;
}

template<typename SNode, typename Allocator>
bool TArrayDynamic<SNode, Allocator>::push(const SNode& def) 
{
    if(Base::count+1>allocated)
      if(!allocator.t_realloc_array(Base::items, allocated, Base::count+1, UMODSYS_SOURCEINFO))
        return false;
    items_construct(Base::count, 1, def);
    Base::count++;
    return true;
}

template<typename SNode, typename Allocator>
bool TArrayDynamic<SNode, Allocator>::set_maxlen(size_t newsize)
{
  if(newsize>Base::allocated) {
    if(!allocator.t_realloc_array(Base::items, allocated, newsize, UMODSYS_SOURCEINFO))
      return false;
  } else if(newsize<Base::allocated) {
    if(newsize<Base::count) {
      Base::items_destruct(newsize, Base::count-newsize);
      Base::count = newsize;
    }
    if(!allocator.t_realloc_array(Base::items, allocated, newsize, UMODSYS_SOURCEINFO))
      return false;
  }
  return true;
}

template<typename SNode, typename Allocator>
bool TArrayDynamic<SNode, Allocator>::resize(size_t newsize) 
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

template<typename SNode, typename Allocator>
inline void TArrayDynamic<SNode, Allocator>::insert(typename Base::iterator pos, typename Base::const_reference v) 
{
  size_t p = pos-Base::begin();
  if(!insert_at(p)) throw_memoryerror();
  Base::at(p) = v;
}

template<typename SNode, typename Allocator>
inline void TArrayDynamic<SNode, Allocator>::erase(typename Base::iterator pos) 
{
  size_t p = pos-Base::begin();
  if(!remove_at(p)) throw_memoryerror();
}

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_COMPOSITE_ARRAY_H
