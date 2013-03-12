#ifndef __NANOUTL_TYPES_TUPLE_H
#define __NANOUTL_TYPES_TUPLE_H 1
/*************************************************************/
// nanoutl/types/tuple.h: unnamed list of different values
/*************************************************************/

#include <nanoutl/core/std.h>
#include <nanoutl/types/fast.h>

namespace NanoUTL {
namespace types {

/*************************************************************/

template<
  typename A1=core::Void, 
  typename A2=core::Void, 
  typename A3=core::Void, 
  typename A4=core::Void, 
  typename A5=core::Void, 
  typename A6=core::Void, 
  typename A7=core::Void, 
  typename A8=core::Void, 
  typename A9=core::Void, 
  typename A10=core::Void
> struct TTuple;

/*************************************************************/
/*************************************************************/

template<
  typename A1, typename A2, typename A3, typename A4, typename A5, 
  typename A6, typename A7, typename A8, typename A9, typename A10
> 
struct TTuple {
  A1 v1;
  A2 v2;
  A3 v3;
  A4 v4;
  A5 v5;
  A6 v6;
  A7 v7;
  A8 v8;
  A9 v9;
  A10 v10;
  //
  inline TTuple(void) {}
  inline TTuple(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9, const A10& a10)
   : v1(a1), v2(a2), v3(a3), v4(a4), v5(a5), v6(a6), v7(a7), v8(a8), v9(a9), v10(a10) {}
};

/*************************************************************/

template<
  typename A1, typename A2, typename A3, typename A4, typename A5, 
  typename A6, typename A7, typename A8, typename A9
> 
struct TTuple<A1, A2, A3, A4, A5, A6, A7, A8, A9, core::Void> {
  A1 v1;
  A2 v2;
  A3 v3;
  A4 v4;
  A5 v5;
  A6 v6;
  A7 v7;
  A8 v8;
  A9 v9;
  //
  inline TTuple(void) {}
  inline TTuple(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9)
   : v1(a1), v2(a2), v3(a3), v4(a4), v5(a5), v6(a6), v7(a7), v8(a8), v9(a9) {}
};

/*************************************************************/

template<
  typename A1, typename A2, typename A3, typename A4, typename A5, 
  typename A6, typename A7, typename A8
> 
struct TTuple<A1, A2, A3, A4, A5, A6, A7, A8, core::Void, core::Void> {
  A1 v1;
  A2 v2;
  A3 v3;
  A4 v4;
  A5 v5;
  A6 v6;
  A7 v7;
  A8 v8;
  //
  inline TTuple(void) {}
  inline TTuple(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8)
   : v1(a1), v2(a2), v3(a3), v4(a4), v5(a5), v6(a6), v7(a7), v8(a8) {}
};

/*************************************************************/

template<
  typename A1, typename A2, typename A3, typename A4, typename A5, 
  typename A6, typename A7
> 
struct TTuple<A1, A2, A3, A4, A5, A6, A7, core::Void, core::Void, core::Void> {
  A1 v1;
  A2 v2;
  A3 v3;
  A4 v4;
  A5 v5;
  A6 v6;
  A7 v7;
  //
  inline TTuple(void) {}
  inline TTuple(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7)
   : v1(a1), v2(a2), v3(a3), v4(a4), v5(a5), v6(a6), v7(a7) {}
};

/*************************************************************/

template<
  typename A1, typename A2, typename A3, typename A4, typename A5, 
  typename A6
> 
struct TTuple<A1, A2, A3, A4, A5, A6, core::Void, core::Void, core::Void, core::Void> {
  A1 v1;
  A2 v2;
  A3 v3;
  A4 v4;
  A5 v5;
  A6 v6;
  //
  inline TTuple(void) {}
  inline TTuple(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
   : v1(a1), v2(a2), v3(a3), v4(a4), v5(a5), v6(a6) {}
};

/*************************************************************/

template<
  typename A1, typename A2, typename A3, typename A4, typename A5
> 
struct TTuple<A1, A2, A3, A4, A5,
             core::Void, core::Void, core::Void, core::Void, core::Void> {
  A1 v1;
  A2 v2;
  A3 v3;
  A4 v4;
  A5 v5;
  //
  inline TTuple(void) {}
  inline TTuple(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
   : v1(a1), v2(a2), v3(a3), v4(a4), v5(a5) {}
};

/*************************************************************/

template<
  typename A1, typename A2, typename A3, typename A4
> 
struct TTuple<A1, A2, A3, A4, core::Void, 
             core::Void, core::Void, core::Void, core::Void, core::Void> {
  A1 v1;
  A2 v2;
  A3 v3;
  A4 v4;
  //
  inline TTuple(void) {}
  inline TTuple(const A1& a1, const A2& a2, const A3& a3, const A4& a4)
   : v1(a1), v2(a2), v3(a3), v4(a4) {}
};

template<typename A1, typename A2, typename A3, typename A4> 
inline TTuple<A1, A2, A3, A4> tuple(const A1& a1, const A2& a2, const A3& a3, const A4& a4)
{ 
  return TTuple<A1, A2, A3, A4>(a1, a2, a3, a4); 
}

/*************************************************************/

template<
  typename A1, typename A2, typename A3
> 
struct TTuple<A1, A2, A3, core::Void, core::Void, 
             core::Void, core::Void, core::Void, core::Void, core::Void> {
  A1 v1;
  A2 v2;
  A3 v3;
  //
  inline TTuple(void) {}
  inline TTuple(const A1& a1, const A2& a2, const A3& a3)
   : v1(a1), v2(a2), v3(a3) {}
};

template<typename A1, typename A2, typename A3> 
inline TTuple<A1, A2, A3> tuple(const A1& a1, const A2& a2, const A3& a3)
{ 
  return TTuple<A1, A2, A3>(a1, a2, a3); 
}

/*************************************************************/

template<
  typename A1, typename A2
> 
struct TTuple<A1, A2, core::Void, core::Void, core::Void, 
             core::Void, core::Void, core::Void, core::Void, core::Void> {
  A1 v1;
  A2 v2;
  //
  inline TTuple(void) {}
  inline TTuple(const A1& a1, const A2& a2)
   : v1(a1), v2(a2) {}
};

template<typename A1, typename A2> 
inline TTuple<A1, A2> tuple(const A1& a1, const A2& a2)
{ 
  return TTuple<A1, A2>(a1, a2); 
}

/*************************************************************/

template<
  typename A1
> 
struct TTuple<A1, core::Void, core::Void, core::Void, core::Void,
             core::Void, core::Void, core::Void, core::Void, core::Void> {
  A1 v1;
  //
  inline TTuple(void) {}
  inline TTuple(const A1& a1)
   : v1(a1) {}
};

template<typename A1> 
inline TTuple<A1> tuple(const A1& a1)
{ 
  return TTuple<A1>(a1); 
}

/*************************************************************/

template<> 
struct TTuple<core::Void, core::Void, core::Void, core::Void, core::Void,
             core::Void, core::Void, core::Void, core::Void, core::Void> {
  //
  inline TTuple(void) {}
};

inline TTuple<> tuple(void)
{ 
  return TTuple<>(); 
}

/*************************************************************/
/*************************************************************/


/*************************************************************/
/*************************************************************/

} // namespace types
} // namespace NanoUTL

#endif // __NANOUTL_TYPES_TUPLE_H
