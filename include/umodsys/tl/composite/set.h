#ifndef __UMODSYS_TL_COMPOSITE_SET_H
#define __UMODSYS_TL_COMPOSITE_SET_H 1
/*************************************************************/
// file: umodsys/tl/composite/set.h
// info: fixed unique array of elements (set)
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>

#include <umodsys/tl/metastl/throws.h>
#include <umodsys/tl/metastl/reverse_iterator.h>

namespace UModSys {
namespace tl {

/*************************************************************/

template<typename SArray, typename SComparer=core::TScalarCompare<typename SArray::ItemType> > struct TSet;

/*************************************************************/

template<typename SArray, typename SComparer>
struct TSet : public SComparer {
public:
  typedef SArray Base;
  typedef typename Base::ItemType ItemType, Node;
  typedef TTypeConstructor<Node> TC;
  typedef SComparer Comparer;
  //
  // stl typedefs
  typedef typename Base::value_type value_type;
  typedef typename Base::reference reference;
  typedef typename Base::const_reference const_reference;
  typedef typename Base::pointer pointer;
  typedef typename Base::const_pointer const_pointer;
  typedef typename Base::difference_type difference_type;
  typedef typename Base::size_type size_type;
  //
  typedef typename Base::iterator iterator;
  typedef typename Base::reverse_iterator reverse_iterator;
  typedef typename Base::const_iterator const_iterator;
  typedef typename Base::const_reverse_iterator const_reverse_iterator;
  //
  static size_t None(void) UMODSYS_NOTHROW() { return SArray::None(); }
public:
  Base arr;
public:
  inline TSet(void) UMODSYS_NOTHROW() {}
  inline TSet(const Base& a) UMODSYS_NOTHROW() : arr(a) {}
  inline ~TSet(void) UMODSYS_NOTHROW() {}
  //
  inline bool operator()(const Node& val) const UMODSYS_NOTHROW() { return Find(val)!=NULL; }
  inline bool operator[](const Node& val) const UMODSYS_NOTHROW() { return Find(val)!=NULL; }
  inline bool operator[](const Node& val) UMODSYS_NOTHROW() { return Set(val); }
  const Node& Get(size_type n) const UMODSYS_NOTHROW() { return arr.CGet(n); }
  inline size_t Len(void) const UMODSYS_NOTHROW() { return arr.Len(); }
  inline size_t operator~(void) const UMODSYS_NOTHROW() { return arr.Len(); }
  inline void Clear(void) UMODSYS_NOTHROW() { arr.Clear(); }
  //
  size_type FindIndex(const Node& val) const UMODSYS_NOTHROW();
  const Node* Find(const Node& val) const UMODSYS_NOTHROW();
  bool Set(const Node& val) UMODSYS_NOTHROW();
  bool Reset(const Node& val) UMODSYS_NOTHROW();
public:
  // stl typedefs
  inline iterator begin(void) UMODSYS_NOTHROW() { return arr.begin(); }
  inline iterator end(void) UMODSYS_NOTHROW() { return arr.end(); }
  inline const_iterator begin(void) const UMODSYS_NOTHROW() { return arr.begin(); }
  inline const_iterator end(void) const UMODSYS_NOTHROW() { return arr.end(); }
  inline const_iterator cbegin(void) const UMODSYS_NOTHROW() { return arr.cbegin(); }
  inline const_iterator cend(void) const UMODSYS_NOTHROW() { return arr.cend(); }
  //
  inline reverse_iterator rbegin(void) UMODSYS_NOTHROW() { return arr.rbegin(); }
  inline reverse_iterator rend(void) UMODSYS_NOTHROW() { return arr.rend(); }
  inline const_reverse_iterator rbegin(void) const UMODSYS_NOTHROW() { return arr.rbegin(); }
  inline const_reverse_iterator rend(void) const UMODSYS_NOTHROW() { return arr.rend(); }
  inline const_reverse_iterator crbegin(void) const UMODSYS_NOTHROW() { return arr.crbegin(); }
  inline const_reverse_iterator crend(void) const UMODSYS_NOTHROW() { return arr.cend(); }
  //
  inline size_type size(void) const UMODSYS_NOTHROW() { return arr.size(); }
  inline bool empty(void) const UMODSYS_NOTHROW() { return arr.empty(); }
  //
/*
  inline void insert(iterator pos, const_reference v) { size_t p = pos - items; if(!InsertAt(p)) throw_memoryerror(); items[p] = v; }
  inline void insert(iterator pos, size_type n, const_reference v) { size_t p = first - items, n = last - first; if(!InsertAt(p, n)) throw_memoryerror(); TC::acopy1(items+p, n, v); }
  template <typename InputIterator> inline void insert(iterator pos, InputIterator first, InputIterator last) { size_t p = pos - items, n = last - first; if(!InsertAt(p, n)) throw_memoryerror(); TC::atcopy(items+p, n, first); }
  //
  inline void erase(iterator pos) { size_t p = pos - items; if(!RemoveAt(p)) throw_memoryerror(); }
  inline void erase(iterator first, iterator last) { size_t p = first - items, n = last - first; if(!RemoveAt(p, n)) throw_memoryerror(); }
*/
};

/*************************************************************/
// INLINES/OUTLINES
/*************************************************************/


/**************************/
// TSet::
/**************************/

template<typename SArray, typename SComparer>
inline typename SArray::size_type TSet<SArray, SComparer>::FindIndex(const Node& val) const UMODSYS_NOTHROW()
{
  const Node* x = arr.All();
  for(size_t n=~arr; n; n--, x++) {
    if(Comparer::compare(*x, val)==0)
      return x - arr.All();
  }
  return None();
}

template<typename SArray, typename SComparer>
inline const typename SArray::ItemType* TSet<SArray, SComparer>::Find(const Node& val) const UMODSYS_NOTHROW()
{
  const Node* x = arr.All();
  for(size_t n=~arr; n; n--, x++) {
    if(SComparer::compare(*x, val)==0)
      return x;
  }
  return NULL;
}

template<typename SArray, typename SComparer>
inline bool TSet<SArray, SComparer>::Set(const Node& val) UMODSYS_NOTHROW()
{
  if(Find(val))
    return true;
  return arr.Push(val);
}

template<typename SArray, typename SComparer>
inline bool TSet<SArray, SComparer>::Reset(const Node& val) UMODSYS_NOTHROW()
{
  const Node* x = arr.Find();
  if(x==NULL)
    return true;
  return arr.RemoveAt(x-arr.All());
}

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_COMPOSITE_SET_H
