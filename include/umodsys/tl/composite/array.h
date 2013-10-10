#ifndef __UMODSYS_TL_COMPOSITE_ARRAY_H
#define __UMODSYS_TL_COMPOSITE_ARRAY_H 1
/*************************************************************/
// file: umodsys/tl/composite/array.h
// info: fixed array of elements
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>

#include <umodsys/tl/util/type_constructor.h>

#include <umodsys/tl/metastl/throws.h>
#include <umodsys/tl/metastl/reverse_iterator.h>

namespace UModSys {
namespace tl {

using core::array_index_none;

/*************************************************************/

template<typename SNode> struct TArray;
template<typename SNode, size_t max_len> struct TArrayFixed;

/*************************************************************/

template<typename SNode>
struct TArray {
public:
  typedef SNode ItemType;
  typedef ItemType* Iter;
  typedef const ItemType* CIter;
  typedef ItemType Node;
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
  typedef Iter iterator;
  typedef ReverseIterator<iterator, value_type, difference_type> reverse_iterator;
  typedef CIter const_iterator;
  typedef ReverseIterator<const_iterator, const value_type, difference_type> const_reverse_iterator;
  //
  static size_t None(void) UMODSYS_NOTHROW() { return array_index_none; }
protected:
  Node *items;
  size_t length, maxlength;
public:
  inline TArray(Node *i, size_t ml, size_t l=0) UMODSYS_NOTHROW() : items(i), length(l), maxlength(ml) {}
  inline ~TArray(void) UMODSYS_NOTHROW() {}
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
  inline size_t MaxLen(void) const UMODSYS_NOTHROW() { return maxlength; }
  //
  inline bool Push(void) UMODSYS_NOTHROW() { return ResizeRel(1); }
  inline bool PushN(size_t n=1) UMODSYS_NOTHROW() { return ResizeRel(n); }
  inline bool Push(const Node& def) UMODSYS_NOTHROW() { if(length+1>maxlength) return false; items[length++] = def; return true; }
  //
  inline bool Pop(Node& def) UMODSYS_NOTHROW() { if(length==0) return false; def = Base::last(); return ResizeRel(-1); }
  inline bool Pop(void) UMODSYS_NOTHROW() { return ResizeRel(-1); }
  inline bool PopN(size_t n=1) UMODSYS_NOTHROW() { return ResizeRel(-static_cast<ptr_diff_t>(n)); }
  //
  inline bool Reserve(size_t newsize) UMODSYS_NOTHROW() { return newsize<=maxlength; }
  inline bool Resize(size_t newsize) UMODSYS_NOTHROW() { return newsize==array_index_none ? false : ResizeRel(newsize - length); }
  inline bool Clear(void) UMODSYS_NOTHROW() { return Resize(0); }
  inline bool Compact(void) UMODSYS_NOTHROW() { return true; }
  inline bool Free(void) UMODSYS_NOTHROW() { return Resize(0); }
  //
  bool ResizeRel(ptrdiff_t add) UMODSYS_NOTHROW();
  bool RemoveAt(size_t id, size_t n=1) UMODSYS_NOTHROW();
  bool InsertAt(size_t id, size_t n=1) UMODSYS_NOTHROW();
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
  inline void resize(size_type newlen) { if(!Resize(newmaxlen)) throw_memoryerror(); }
  //
  inline void push_back(const_reference v) { if(!push(v)) throw_memoryerror(); }
  inline void pop_back(void) { if(!pop()) throw_memoryerror(); }
  //
  inline void insert(iterator pos, const_reference v) { size_t p = pos - items; if(!InsertAt(p)) throw_memoryerror(); items[p] = v; }
  inline void insert(iterator pos, size_type n, const_reference v) { size_t p = first - items, n = last - first; if(!InsertAt(p, n)) throw_memoryerror(); TC::acopy1(items+p, n, v); }
  template <typename InputIterator> inline void insert(iterator pos, InputIterator first, InputIterator last) { size_t p = pos - items, n = last - first; if(!InsertAt(p, n)) throw_memoryerror(); TC::atcopy(items+p, n, first); }
  //
  inline void erase(iterator pos) { size_t p = pos - items; if(!RemoveAt(p)) throw_memoryerror(); }
  inline void erase(iterator first, iterator last) { size_t p = first - items, n = last - first; if(!RemoveAt(p, n)) throw_memoryerror(); }
};

/*************************************************************/

template<typename SNode, size_t max_len>
struct TArrayFixed : public TArray< SNode > {
public:
  SNode buffer[max_len];
public:
  typedef TArray< SNode > Array;
  //
  inline TArrayFixed(size_t n=0) UMODSYS_NOTHROW() : Array(buffer, max_length, n) {}
  inline ~TArrayFixed(void) UMODSYS_NOTHROW() {}
};


/*************************************************************/
// INLINES/OUTLINES
/*************************************************************/


/**************************/
// TArray::
/**************************/

template<typename SNode>
inline bool TArray<SNode>::ResizeRel(ptrdiff_t add) UMODSYS_NOTHROW()
{
  if(add==0)
    return true;
  size_t newsize = length + add;
  if(add>0) {
    if(newsize > maxlength)
      return false;
    length = newsize;
  } else {
    length = newsize;
  }
  return true;
}

template<typename SNode>
inline bool TArray<SNode>::InsertAt(size_t id, size_t n) UMODSYS_NOTHROW()
{
  if(id==array_index_none || id>length)
    return false;
  if(id!=length-1) {
    TC::acopyright(items, id+n, id, length-id-1);
  }
  length += n;
  return true;
}

template<typename SNode>
inline bool TArray<SNode>::RemoveAt(size_t id, size_t n) UMODSYS_NOTHROW()
{
  size_t len = length;
  if(id==array_index_none || id+n>len)
    return false;
  if(id+n!=length) {
    TC::acopyleft(items, id, id+n, length-id-n);
  }
  length -= n;
  return true;
}

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_COMPOSITE_ARRAY_H
