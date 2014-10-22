#ifndef __UMODSYS_TL_COMPOSITE_DYNARRAY_H
#define __UMODSYS_TL_COMPOSITE_DYNARRAY_H 1
/*************************************************************/
// file: umodsys/tl/composite/dynarray.h
// info: dynamic array of elements
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>

#include <umodsys/tl/util/type_constructor.h>
#include <umodsys/tl/alloc/allocator.h>

#include <umodsys/tl/metastl/throws.h>
#include <umodsys/tl/metastl/reverse_iterator.h>

namespace UModSys {
namespace tl {

using core::array_index_none;

/*************************************************************/

//template<typename SNode, typename Allocator=DAllocatorFast > struct TArrayCache;

template<typename SNode, size_t max_len> struct TDynarrayHolderStatic;
template<typename SNode, typename FunAlloc=DAllocatorFast> struct TDynarrayHolderDynamic;

template<typename SHolder> struct TDynarray; // template array basis

template<typename SNode, size_t max_len> struct TDynarrayStatic;
template<typename SNode, typename FunAlloc=DAllocatorFast> struct TDynarrayDynamic;

/*************************************************************/

template<typename SNode, size_t max_len>
struct TDynarrayHolderStatic {
public:
  typedef core::Void MemAlloc, Allocator;
  typedef SNode ItemType;
  typedef ItemType* Iter;
  typedef const ItemType* CIter;
protected:
  SNode buffer[max_len];
public:
  inline TDynarrayHolderStatic(void) UMODSYS_NOTHROW() {}
  inline TDynarrayHolderStatic(const MemAlloc &a) UMODSYS_NOTHROW() {}
  inline ~TDynarrayHolderStatic(void) UMODSYS_NOTHROW() {}
  //
  inline ItemType* get(void) UMODSYS_NOTHROW() { return buffer; }
  inline size_t maxlen(void) const UMODSYS_NOTHROW() { return max_len; }
  inline bool maxlen(size_t sz) UMODSYS_NOTHROW() { return sz<=max_len; }
  inline bool free(void) UMODSYS_NOTHROW() { return true; }
  inline bool compact(void) UMODSYS_NOTHROW() { return true; }
};

/*************************************************************/

template<typename SNode, typename FunAlloc>
struct TDynarrayHolderDynamic : public FunAlloc {
public:
  typedef FunAlloc DAllocator;
  typedef typename FunAlloc::Allocator MemAlloc;
  typedef SNode ItemType;
  typedef ItemType* Iter;
  typedef const ItemType* CIter;
protected:
  SNode* allocated;
  size_t maxsize;
public:
  inline TDynarrayHolderDynamic(void) UMODSYS_NOTHROW() : maxsize(0), allocated(NULL) {}
  inline TDynarrayHolderDynamic(const MemAlloc& a) UMODSYS_NOTHROW() : FunAlloc(a), maxsize(0), allocated(NULL) {}
  inline ~TDynarrayHolderDynamic(void) UMODSYS_NOTHROW() { free(); }
  //
  inline ItemType* get(void) UMODSYS_NOTHROW() { return allocated; }
  inline size_t maxlen(void) const UMODSYS_NOTHROW() { return maxsize; }
  bool maxlen(size_t sz) UMODSYS_NOTHROW() { return DAllocator::t_realloc_array(allocated, maxsize, sz, UMODSYS_SOURCEINFO); }
  bool free(void) UMODSYS_NOTHROW() { return DAllocator::t_free_array(allocated, maxsize, UMODSYS_SOURCEINFO); }
};

/*************************************************************/

template<typename SHolder>
struct TDynarray {
public:
  typedef SHolder Holder;
  typedef typename Holder::ItemType Node;
  typedef typename Holder::Allocator MemAlloc;
  typedef TTypeConstructor<Node> TC;
  //
  // stl typedefs
  typedef Node value_type;
  typedef Node& reference;
  typedef const Node& const_reference;
  typedef Node* pointer;
  typedef const Node* const_pointer;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  //
  typedef typename Holder::Iter iterator;
  typedef ReverseIterator<iterator, value_type, difference_type> reverse_iterator;
  typedef typename Holder::CIter const_iterator;
  typedef ReverseIterator<const_iterator, const value_type, difference_type> const_reverse_iterator;
  //
  static size_t None(void) UMODSYS_NOTHROW() { return array_index_none; }
protected:
  Holder holder;
  Node *items;
  size_t length;
public:
  inline TDynarray(void) UMODSYS_NOTHROW() : items(NULL), length(0) { items = holder.get(); }
  inline TDynarray(const MemAlloc& a) UMODSYS_NOTHROW() : holder(a), items(NULL), length(0) { items = holder.get(); }
  inline ~TDynarray(void) UMODSYS_NOTHROW() { Free(); items = holder.get(); }
  //
  inline const Node* All(void) const UMODSYS_NOTHROW() { return items; }
  inline Node* All(void) UMODSYS_NOTHROW() { return items; }
  //
  inline const Node& CGet(size_t no) const UMODSYS_NOTHROW() { return items[no]; }
  inline const Node& Get(size_t no) const UMODSYS_NOTHROW() { return items[no]; }
  inline Node& Get(size_t no) UMODSYS_NOTHROW() { return items[no]; }
  inline const Node& operator()(size_t no) const UMODSYS_NOTHROW() { return items[no]; }
  inline const Node& operator[](size_t no) const UMODSYS_NOTHROW() { return items[no]; }
  inline Node& operator[](size_t no) UMODSYS_NOTHROW() { return items[no]; }
  //
  inline const Node& Last(void) const UMODSYS_NOTHROW() { return items[length - 1]; }
  inline Node& Last(void) UMODSYS_NOTHROW() { return items[length - 1]; }
  inline const Node& First(void) const UMODSYS_NOTHROW() { return items[0]; }
  inline Node& First(void) UMODSYS_NOTHROW() { return items[0]; }
  //
  inline size_t FirstIndex(void) const UMODSYS_NOTHROW() { return 0; }
  inline size_t LastIndex(void) const UMODSYS_NOTHROW() { return length - 1; }
  //
  inline size_t Len(void) const UMODSYS_NOTHROW() { return length; }
  inline size_t operator~(void) const UMODSYS_NOTHROW() { return length; }
  //
  inline size_t MaxLen(void) const UMODSYS_NOTHROW() { return holder.maxlen(); }
  //
  inline bool Push(void) UMODSYS_NOTHROW() { return ResizeRel(1); }
  inline bool PushN(size_t n=1) UMODSYS_NOTHROW() { return ResizeRel(n); }
  //
  inline bool Pop(Node& def) UMODSYS_NOTHROW() { if(length==0) return false; def = Last(); return ResizeRel(-1); }
  inline bool Pop(void) UMODSYS_NOTHROW() { return ResizeRel(-1); }
  inline bool PopN(size_t n=1) UMODSYS_NOTHROW() { return ResizeRel(-static_cast<ptrdiff_t>(n)); }
  //
  inline bool Resize(size_t newsize) UMODSYS_NOTHROW() { return newsize==array_index_none ? false : ResizeRel(newsize - length); }
  inline bool Clear(void) UMODSYS_NOTHROW() { return Resize(0); }
  inline bool Compact(void) UMODSYS_NOTHROW() { return ReLink(length); }
  inline bool Free(void) UMODSYS_NOTHROW() { return Resize(0) && holder.free() && ReLink(); }
  //
  bool Push(const Node& def) UMODSYS_NOTHROW();
  bool Reserve(size_t newsize) UMODSYS_NOTHROW();
  bool ResizeRel(ptrdiff_t add) UMODSYS_NOTHROW();
  bool RemoveAt(size_t id, size_t n=1) UMODSYS_NOTHROW();
  bool InsertAt(size_t id, size_t n=1) UMODSYS_NOTHROW();
  //
  bool IsEmpty(void) const { return length==0; }
  bool IsFull(void) const { return false; } // never overflow
public:
  // stl typedefs
  inline iterator begin(void) UMODSYS_NOTHROW() { return items; }
  inline iterator end(void) UMODSYS_NOTHROW() { return items + length; }
  inline const_iterator begin(void) const UMODSYS_NOTHROW() { return items; }
  inline const_iterator end(void) const UMODSYS_NOTHROW() { return items + length; }
  inline const_iterator cbegin(void) const UMODSYS_NOTHROW() { return items; }
  inline const_iterator cend(void) const UMODSYS_NOTHROW() { return items + length; }
  //
  inline reverse_iterator rbegin(void) UMODSYS_NOTHROW() { return reverse_iterator(items + length - 1); }
  inline reverse_iterator rend(void) UMODSYS_NOTHROW() { return reverse_iterator(items - 1); }
  inline const_reverse_iterator rbegin(void) const UMODSYS_NOTHROW() { return reverse_iterator(items + length - 1); }
  inline const_reverse_iterator rend(void) const UMODSYS_NOTHROW() { return reverse_iterator(items - 1); }
  inline const_reverse_iterator crbegin(void) const UMODSYS_NOTHROW() { return reverse_iterator(items + length - 1); }
  inline const_reverse_iterator crend(void) const UMODSYS_NOTHROW() { return reverse_iterator(items - 1); }
  //
  inline size_type size(void) const UMODSYS_NOTHROW() { return length; }
  inline bool empty(void) const UMODSYS_NOTHROW() { return length==0; }
  inline const_pointer data(void) const UMODSYS_NOTHROW() { return items; }
  inline pointer data(void) UMODSYS_NOTHROW() { return items; }
  //
  inline const_reference at(size_type n) const UMODSYS_NOTHROW() { return items[n]; }
  inline reference at(size_type n) UMODSYS_NOTHROW() { return items[n]; }
  inline const_reference front(void) const UMODSYS_NOTHROW() { return items[0]; }
  inline reference front(void) UMODSYS_NOTHROW() { return items[0]; }
  inline const_reference back(void) const UMODSYS_NOTHROW() { return items[length-1]; }
  inline reference back(void) UMODSYS_NOTHROW() { return items[length-1]; }
  //
  inline size_type max_size(void) const UMODSYS_NOTHROW() { return holder.maxlen(); }
  inline void reserve(size_type newmaxlen) { if(!Reserve(newmaxlen)) throw_memoryerror(); }
  inline void resize(size_type newlen) { if(!Resize(newlen)) throw_memoryerror(); }
  //
  inline void push_back(const_reference v) { if(!push(v)) throw_memoryerror(); }
  inline void pop_back(void) { if(!Pop()) throw_memoryerror(); }
  //
  inline void insert(iterator pos, const_reference v) { size_t p = pos - items; if(!InsertAt(p)) throw_memoryerror(); items[p] = v; }
  inline void insert(iterator pos, size_type n, const_reference v) { size_t p = pos - items; if(!InsertAt(p, n)) throw_memoryerror(); TC::acopy1(items+p, n, v); }
  template <typename InputIterator> inline void insert(iterator pos, InputIterator first, InputIterator last) { size_t p = pos - items, n = last - first; if(!InsertAt(p, n)) throw_memoryerror(); TC::atcopy(items+p, n, first); }
  //
  inline void erase(iterator pos) { size_t p = pos - items; if(!RemoveAt(p)) throw_memoryerror(); }
  inline void erase(iterator first, iterator last) { size_t p = first - items, n = last - first; if(!RemoveAt(p, n)) throw_memoryerror(); }
protected:
  inline bool ReLink(size_t sz) { if(!holder.maxlen(sz)) return false; items = holder.get(); return true; }
  inline bool ReLink(void) { items = holder.get(); return true; }
  //
  inline void item_construct(size_t no) UMODSYS_NOTHROW() { TC::construct(items+no); }
  inline void item_construct(size_t no, const Node& def) UMODSYS_NOTHROW() { TC::construct(items+no, def); }
  inline void item_destruct(size_t no) UMODSYS_NOTHROW() { TC::destruct(items+no); }
  inline void items_construct(size_t no, size_t count) UMODSYS_NOTHROW() { TC::aconstruct(items+no, count); }
  inline void items_construct(size_t no, size_t count, const Node& def) UMODSYS_NOTHROW() { TC::aconstructcopy(items+no, count, def); }
  inline void items_construct(size_t no, size_t count, const Node* def) UMODSYS_NOTHROW() { TC::aconstructcopya(items+no, count, def); }
  inline void items_destruct(size_t no, size_t count) UMODSYS_NOTHROW() { TC::adestruct(items+no, count); }
//  template<typename Iter> inline void copy(Iter ritems, size_t ecount) { if(ecount<count) ecount = count; TC::copy(items, ritems, ecount); }
};

/*************************************************************/
/*
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
*/


/*************************************************************/

template<typename SNode, typename FunAlloc>
struct TDynarrayDynamic : public TDynarray< TDynarrayHolderDynamic<SNode, FunAlloc> > {
public:
  typedef TDynarray< TDynarrayHolderDynamic<SNode, FunAlloc> > Array;
  //
  inline TDynarrayDynamic(void) UMODSYS_NOTHROW() {}
  inline TDynarrayDynamic(const typename Array::MemAlloc& a) UMODSYS_NOTHROW() : Array(a) {}
  inline ~TDynarrayDynamic(void) UMODSYS_NOTHROW() {}
};

template<typename SNode, size_t max_len>
struct TDynarrayStatic : public TDynarray< TDynarrayHolderStatic<SNode, max_len> > {
public:
  typedef TDynarray< TDynarrayHolderStatic<SNode, max_len> > Array;
  //
  inline TDynarrayStatic(void) UMODSYS_NOTHROW() {}
  inline ~TDynarrayStatic(void) UMODSYS_NOTHROW() {}
};

/*************************************************************/
// INLINES/OUTLINES
/*************************************************************/


/**************************/
// TDynarray::
/**************************/

template<typename SHolder>
inline bool TDynarray<SHolder>::Reserve(size_t newsize) UMODSYS_NOTHROW()
{
  if(newsize==holder.maxlen())
    return false;
  if(newsize<length) {
    items_destruct(newsize, length-newsize);
    length = newsize;
  }
  return ReLink(newsize);
}

template<typename SHolder>
inline bool TDynarray<SHolder>::ResizeRel(ptrdiff_t add) UMODSYS_NOTHROW()
{
  if(add==0)
    return true;
  size_t newsize = length + add;
  if(add>0) {
    if(newsize > holder.maxlen() && !ReLink(newsize))
      return false;
    items_construct(length, add);
    length = newsize;
  } else {
    items_destruct(newsize, -add);
    length = newsize;
  }
  return true;
}

template<typename SHolder>
inline bool TDynarray<SHolder>::InsertAt(size_t id, size_t n) UMODSYS_NOTHROW()
{
  if(id==array_index_none || id>length)
    return false;
  if(length+n>holder.maxlen() && !ReLink(length+n))
    return false;
  if(id!=length) {
    TC::amoveright(items, id+n, id, length-id);
  }
  items_construct(id, n);
  length += n;
  return true;
}

template<typename SHolder>
inline bool TDynarray<SHolder>::RemoveAt(size_t id, size_t n) UMODSYS_NOTHROW()
{
  size_t len = length;
  if(id==array_index_none || id+n>len)
    return false;
  items_destruct(id, n);
  if(id+n!=length) {
    TC::amoveleft(items, id, id+n, length-id);
  }
  length -= n;
  return true;
}

template<typename SHolder>
inline bool TDynarray<SHolder>::Push(const typename TDynarray<SHolder>::Node& def) UMODSYS_NOTHROW()
{
  if(length+1>holder.maxlen() && !ReLink(length+1))
    return false;
  items_construct(length, 1, def);
  length++;
  return true;
}

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_COMPOSITE_ARRAY_H
