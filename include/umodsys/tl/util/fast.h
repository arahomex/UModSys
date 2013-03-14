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
  inline static void aconstruct(Type* p, int count) { for(int i=0; i<count; i++) new(p++) Type; }
  inline static void aconstructcopy(Type* p, int count, const Type& cp) { for(int i=0; i<count; i++) new(p++) Type(cp); }
  inline static void aconstructcopya(Type* p, int count, const Type* cp) { for(int i=0; i<count; i++) new(p++) Type(*cp++); }
  inline static void adestruct(Type* p, int count) { for(int i=0; i<count; i++) (p++)->~Type(); }
};

/*************************************************************/
/*************************************************************/

// ::UModSys::types::
#define UMODSYS_SCALAR_CONSTRUCTOR(_type) \
  template<> struct TTypeConstructor<_type> { \
    typedef _type Type; \
    typedef _type Const; \
    typedef _type& Variable; \
    inline static void construct(Type* p) {} \
    inline static void constructcopy(Type* p, const Type& cp) { *p=cp; } \
    inline static void copy(Type* p, const Type& cp) { *p=cp; } \
    inline static void aconstruct(Type* p, int count) {} \
    inline static void aconstructcopy(Type* p, int count, const Type& cp) { for(int i=0; i<count; i++) *p++=cp; } \
    inline static void aconstructcopya(Type* p, int count, const Type* cp) { for(int i=0; i<count; i++) *p++=*cp++; } \
    inline static void destruct(Type* p) {} \
    inline static void adestruct(Type* p, int count) {} \
  }


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
