#ifndef __NANOUTL_TYPES_WRAP_INDEX_H
#define __NANOUTL_TYPES_WRAP_INDEX_H 1
//*****************************************************
// ums/core/types/wrap_index.h: simple wrappers for defined types
//*****************************************************

#include <nanoutl/core/typedefs.h>

namespace NanoUTL {
namespace types {


template<typename T, typename Determ>
struct TWrapperIndex {
  T value;
  // constructors
  inline TWrapperIndex(T v) : value(v) {}
  inline TWrapperIndex(void) : value(0) {}
  // get operators
//  operator bool(void) const { return value>0; }
  inline T operator*(void) const { return value; }
  // compare operators this
  inline bool operator==(const TWrapperIndex& r) const { return value==r.value; }
  inline bool operator!=(const TWrapperIndex& r) const { return value!=r.value; }
  inline bool operator>=(const TWrapperIndex& r) const { return value>=r.value; }
  inline bool operator<=(const TWrapperIndex& r) const { return value<=r.value; }
  inline bool operator>(const TWrapperIndex& r) const { return value>r.value; }
  inline bool operator<(const TWrapperIndex& r) const { return value<r.value; }
  // compare operators this
  inline bool operator==(T r) const { return value==r; }
  inline bool operator!=(T r) const { return value!=r; }
  inline bool operator>=(T r) const { return value>=r; }
  inline bool operator<=(T r) const { return value<=r; }
  inline bool operator>(T r) const { return value>r; }
  inline bool operator<(T r) const { return value<r; }
  // iterators
  inline void operator++(int) { ++value; }
  inline void operator--(int) { --value; }
  inline void operator++(void) { ++value; }
  inline void operator--(void) { --value; }
};

#define UMS_WRAPPER_INDEX_TYPE(_mytype, _basetype) \
  /*struct _mytype##ID {};*/ typedef UMS::core::types::TWrapperIndex<_basetype, struct _mytype##ID> _mytype;


} // namespace types
} // namespace UMS

#endif // __NANOUTL_TYPES_WRAP_INDEX_H
