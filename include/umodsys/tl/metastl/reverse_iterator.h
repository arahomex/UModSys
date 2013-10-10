#ifndef __UMODSYS_TL_METASTL_REVERSEITERATOR_H
#define __UMODSYS_TL_METASTL_REVERSEITERATOR_H 1
/*************************************************************/
// file: umodsys/tl/metastl/reverse_iterator.h
// info: reverse iterator
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>

namespace UModSys {
namespace tl {

/*************************************************************/

template<typename Iter, typename Obj, typename Diff>
struct ReverseIterator {
public:
  typedef ReverseIterator<Iter, Obj, Diff> Self;
  //
  inline ReverseIterator(Iter x) UMODSYS_NOTHROW() : iter(x) {}
  inline ~ReverseIterator(void) UMODSYS_NOTHROW() {}
  //
  inline Self operator++(void) UMODSYS_NOTHROW() { return Self(iter--); }
  inline Self operator--(void) UMODSYS_NOTHROW() { return Self(iter++); }
  //
  inline const Self& operator++(int x) UMODSYS_NOTHROW() { --iter; return *this; }
  inline const Self& operator--(int x) UMODSYS_NOTHROW() { ++iter; return *this; }
  //
  inline Obj& operator*(void) const UMODSYS_NOTHROW()  { return *iter; }
  inline Obj* operator->(void) const UMODSYS_NOTHROW() { return iter; }
  //
  inline bool operator<(const Self& R) const UMODSYS_NOTHROW() { return iter < R.iter; }
  inline bool operator>(const Self& R) const UMODSYS_NOTHROW() { return iter > R.iter; }
  inline bool operator<=(const Self& R) const UMODSYS_NOTHROW() { return iter <= R.iter; }
  inline bool operator>=(const Self& R) const UMODSYS_NOTHROW() { return iter >= R.iter; }
  inline bool operator==(const Self& R) const UMODSYS_NOTHROW() { return iter == R.iter; }
  inline bool operator!=(const Self& R) const UMODSYS_NOTHROW() { return iter != R.iter; }
  //
  inline Diff operator-(const Self& R) const UMODSYS_NOTHROW() { return R.iter - iter; }
  inline Self operator+(const Diff& R) const UMODSYS_NOTHROW() { return Self(iter - R); }
  inline Self operator-(const Diff& R) const UMODSYS_NOTHROW() { return Self(iter + R); }
protected:
  Iter iter;
};

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_METASTL_REVERSEITERATOR_H
