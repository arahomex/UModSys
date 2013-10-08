#ifndef __UMODSYS_TYPES_FASTTYPE_H
#define __UMODSYS_TYPES_FASTTYPE_H 1
/*************************************************************/
// nanoutl/types/fasttype.h: template to define fastest data transfer or init
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>

namespace UModSys {
namespace tl {

/*************************************************************/
/*************************************************************/

template<typename T>
struct TTypeConstructor {
  typedef T Type;
  typedef const T& Const;
  typedef T& Variable;
  //
  inline static void construct(Type* p) { new(p) Type; }
  inline static void destruct(Type* p) { p->~Type(); }
  inline static void constructcopy(Type* p, const Type& cp) { new(p) Type(cp); }
  inline static void copy(Type* p, const Type& cp) { *p = cp; }
  //
  inline static void aconstruct(Type* p, size_t count) { for(size_t i=0; i<count; i++) new(p++) Type; }
  inline static void aconstructcopy(Type* p, size_t count, const Type& cp) { for(size_t i=0; i<count; i++) new(p++) Type(cp); }
  inline static void aconstructcopya(Type* p, size_t count, const Type* cp) { for(size_t i=0; i<count; i++) new(p++) Type(*cp++); }
  inline static void adestruct(Type* p, size_t count) { for(size_t i=0; i<count; i++) (p++)->~Type(); }
  inline static void acopy(Type* p, size_t count, const Type* p2) { for(size_t i=0; i<count; i++) *p++ = *p2++; }
  inline static void acopy1(Type* p, size_t count, const Type& x) { for(size_t i=0; i<count; i++) *p++ = x; }
  //
  template <typename P> inline static void atcopy(Type* p, size_t count, P p2) { for(size_t i=0; i<count; i++) *p++ = *p2++; }
};

template<typename T>
struct TTypeConstructorBinaryFast {
  typedef T Type;
  typedef T Const;
  typedef T& Variable;
  //
  inline static void construct(Type* p) {}
  inline static void destruct(Type* p) {}
  inline static void constructcopy(Type* p, const Type& cp) { *p=cp; }
  inline static void copy(Type* p, const Type& cp) { *p=cp; }
  //
  inline static void aconstruct(Type* p, size_t count) {}
  inline static void aconstructcopy(Type* p, size_t count, const Type& cp) { for(size_t i=0; i<count; i++) *p++=cp; }
  inline static void aconstructcopya(Type* p, size_t count, const Type* cp) { memcpy(p, cp, count*sizeof(Type)); }
  inline static void adestruct(Type* p, size_t count) {}
  inline static void acopy(Type* p, size_t count, const Type* cp) { memcpy(p, cp, count*sizeof(Type)); }
  inline static void acopy1(Type* p, size_t count, const Type& x) { for(size_t i=0; i<count; i++) *p++ = x; }
  //
  template <typename P> inline static void atcopy(Type* p, size_t count, P p2) { for(size_t i=0; i<count; i++) *p++ = *p2++; }
};

/*************************************************************/
/*************************************************************/


// ::UModSys::types::
#define UMODSYS_SCALAR_CONSTRUCTOR(_type) template<> struct TTypeConstructor<_type> : public TTypeConstructorBinaryFast<_type> {};

/*************************************************************/

UMODSYS_SCALAR_CONSTRUCTOR(signed char);
UMODSYS_SCALAR_CONSTRUCTOR(unsigned char);
UMODSYS_SCALAR_CONSTRUCTOR(char);
UMODSYS_SCALAR_CONSTRUCTOR(signed int);
UMODSYS_SCALAR_CONSTRUCTOR(unsigned int);
UMODSYS_SCALAR_CONSTRUCTOR(signed short);
UMODSYS_SCALAR_CONSTRUCTOR(unsigned short);
UMODSYS_SCALAR_CONSTRUCTOR(signed long);
UMODSYS_SCALAR_CONSTRUCTOR(unsigned long);
UMODSYS_SCALAR_CONSTRUCTOR(float);
UMODSYS_SCALAR_CONSTRUCTOR(double);
UMODSYS_SCALAR_CONSTRUCTOR(bool);

/*************************************************************/
/*************************************************************/

} // namespace types
} // namespace UModSys

#endif // __UMODSYS_TYPES_FASTTYPE_H
