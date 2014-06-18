#ifndef __UMODSYS_TL_UTIL_TYPECONSTRUCTOR_H
#define __UMODSYS_TL_UTIL_TYPECONSTRUCTOR_H 1
/*************************************************************/
// file: umodsys/tl/util/type_constructor.h
// info: contruct types
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
  inline static void aconstruct(Type* p, size_t count) { 
    while(count--)
      new(p++) Type; 
  }
  inline static void aconstructcopy(Type* p, size_t count, const Type& cp) { 
    while(count--)
      new(p++) Type(cp); 
  }
  inline static void aconstructcopya(Type* p, size_t count, const Type* cp) { 
    while(count--)
      Type(*cp++); 
  }
  inline static void adestruct(Type* p, size_t count) { 
    while(count--)
      (p++)->~Type(); 
  }
  inline static void acopy(Type* p, size_t count, const Type* p2) { 
    while(count--)
      *p++ = *p2++; 
  }
  inline static void acopy1(Type* p, size_t count, const Type& x) { 
    while(count--)
      *p++ = x; 
  }
  //
  inline static void acopyright(Type* p, size_t dest, size_t src, size_t count) { 
    Type *p1 = p + dest + count, *p2 = p + src + count;
    while(count--)
      *--p1 = *--p2;
  }
  inline static void acopyleft(Type* p, size_t dest, size_t src, size_t count) { 
    Type *p1 = p + dest, *p2 = p + src;
    while(count--)
      *p1++ = *p2++;
  }
  inline static void amoveright(Type* p, size_t dest, size_t src, size_t count) { 
    Type *p1 = p + dest + count, *p2 = p + src + count;
    while(count--) {
      new(p1) Type(*p2);
      p2->~Type();
      --p1; --p2;
    }
  }
  inline static void amoveleft(Type* p, size_t dest, size_t src, size_t count) { 
    Type *p1 = p + dest, *p2 = p + src;
    while(count--) {
      new(p1) Type(*p2);
      p2->~Type();
      ++p1; ++p2;
    }
  }
  //
  template <typename P> inline static void atcopy(Type* p, size_t count, P p2) { 
    while(count--)
      *p++ = *p2++; 
  }
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
  inline static void aconstructcopy(Type* p, size_t count, const Type& cp) { 
    while(count--)
      *p++ = cp; 
  }
  inline static void aconstructcopya(Type* p, size_t count, const Type* cp) { 
    memcpy(p, cp, count*sizeof(Type)); 
  }
  inline static void adestruct(Type* p, size_t count) {}
  inline static void acopy(Type* p, size_t count, const Type* cp) { 
    memcpy(p, cp, count*sizeof(Type)); 
  }
  inline static void acopy1(Type* p, size_t count, const Type& x) { 
    while(count--)
      *p++ = x; 
  }
  //
  inline static void acopyright(Type* p, size_t dest, size_t src, size_t count) { 
    memmove(p+dest, p+src, count*sizeof(T)); 
  }
  inline static void acopyleft(Type* p, size_t dest, size_t src, size_t count) { 
    memmove(p+dest, p+src, count*sizeof(T)); 
  }
  inline static void amoveright(Type* p, size_t dest, size_t src, size_t count) { 
    memmove(p+dest, p+src, count*sizeof(T)); 
  }
  inline static void amoveleft(Type* p, size_t dest, size_t src, size_t count) { 
    memmove(p+dest, p+src, count*sizeof(T)); 
  }
  //
  template <typename P> inline static void atcopy(Type* p, size_t count, P p2) { 
    while(count--)
      *p++ = *p2++; 
  }
};

/*************************************************************/
/*************************************************************/


#define UMODSYS_SCALAR_CONSTRUCTOR(_type) \
  namespace UModSys { namespace tl { template<> struct TTypeConstructor<_type> : public UModSys::tl::TTypeConstructorBinaryFast<_type> {}; } }

#define UMODSYS_SCALAR_CONSTRUCTOR_T1(_t1, _type) \
  namespace UModSys { namespace tl { template< typename _t1 > struct TTypeConstructor< _type > : public UModSys::tl::TTypeConstructorBinaryFast< _type > {}; } }

/*************************************************************/

template<typename BaseType> struct TTypeConstructor<BaseType*> : public TTypeConstructorBinaryFast<BaseType*> {};
template<typename BaseType> struct TTypeConstructor<const BaseType*> : public TTypeConstructorBinaryFast<const BaseType*> {};


/*************************************************************/
/*************************************************************/

} // namespace tl
} // namespace UModSys

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

#endif // __UMODSYS_TL_UTIL_TYPECONSTRUCTOR_H
