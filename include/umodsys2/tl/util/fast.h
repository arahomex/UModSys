#ifndef __NANOUTL_TYPES_FASTTYPE_H
#define __NANOUTL_TYPES_FASTTYPE_H 1
/*************************************************************/
// nanoutl/types/fasttype.h: template to define fastest data transfer or init
/*************************************************************/

#include <nanoutl/core/std.h>

namespace NanoUTL {
namespace types {

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

// ::NanoUTL::types::
#define NANOUTL_SCALAR_CONSTRUCTOR(_type) \
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

NANOUTL_SCALAR_CONSTRUCTOR(signed char);
NANOUTL_SCALAR_CONSTRUCTOR(unsigned char);
NANOUTL_SCALAR_CONSTRUCTOR(char);
NANOUTL_SCALAR_CONSTRUCTOR(signed int);
NANOUTL_SCALAR_CONSTRUCTOR(unsigned int);
NANOUTL_SCALAR_CONSTRUCTOR(signed short);
NANOUTL_SCALAR_CONSTRUCTOR(unsigned short);
NANOUTL_SCALAR_CONSTRUCTOR(signed long);
NANOUTL_SCALAR_CONSTRUCTOR(unsigned long);
NANOUTL_SCALAR_CONSTRUCTOR(float);
NANOUTL_SCALAR_CONSTRUCTOR(double);
NANOUTL_SCALAR_CONSTRUCTOR(bool);

/*************************************************************/
/*************************************************************/

} // namespace types
} // namespace NanoUTL

#endif // __NANOUTL_TYPES_FASTTYPE_H
