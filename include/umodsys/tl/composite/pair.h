#ifndef __UMODSYS_TL_COMPOSITE_PAIR_H
#define __UMODSYS_TL_COMPOSITE_PAIR_H 1
/*************************************************************/
// file: umodsys/tl/composite/pair.h
// info: pair
/*************************************************************/

namespace UModSys {
namespace tl {

/*************************************************************/

template<typename A, typename B> struct TPair;
template<typename A, typename B> struct TPairM;

/*************************************************************/

template<typename A, typename B>
struct TPair {
  A first;
  B second;
  //
  inline TPair(const A& a, const B& b) : first(a), second(b) {}
  inline TPair(void);
  //
  operator const TPairM<A, B>& (void);
};

template<typename A, typename B>
struct TPairM {
  A first;
  mutable B second;
  //
  inline TPairM(const A& a, const B& b) : first(a), second(b) {}
  inline TPairM(void);
};

template<typename A, typename B>
TPair<A, B>::TPair(void)
{
}

template<typename A, typename B>
TPair<A, B>::operator const TPairM<A, B>& (void)
{
  return *reinterpret_cast<const TPairM<A, B>*>(this);
}

template<typename A, typename B>
TPairM<A, B>::TPairM(void)
{
}

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_COMPOSITE_PAIR_H
