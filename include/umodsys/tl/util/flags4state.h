#ifndef __UMODSYS_TL_UTIL_FLAGS4STATE_H
#define __UMODSYS_TL_UTIL_FLAGS4STATE_H 1
/*************************************************************/
// file: umodsys/tl/util/flags4state.h.h
// info: flags with 4 states
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>

namespace UModSys {
namespace tl {

/*************************************************************/

template<typename Uid, typename Base> struct TFlags4State;
template<typename Uid, typename Base, int nshift> struct TFlags4StateShift;

template<typename Uid, typename Base>
struct TFlags4State 
{
  typedef TFlags4State<Uid, Base> F4S;
  //
  enum eStates {
    Default  = 0x0,
    Yes      = 0x1,
    No       = 0x2,
    Auto     = 0x3,
    MMask    = 0x3
  };
  //
  Base value;
  //
  inline TFlags4State(void) : value(0) {}
  template<int ns> inline TFlags4State(typename TFlags4StateShift<Uid,Base,ns>::eState s) : value(s) {}
  template<int ns> inline TFlags4State(TFlags4StateShift<Uid,Base,ns> s) : value(s.value) {}
  inline explicit TFlags4State(Base val) : value(val) {}
  //
  inline F4S operator +(const F4S& v) const { return F4S( value | v.value); }
  template<int ns> inline F4S operator +(typename TFlags4StateShift<Uid,Base,ns>::eState s) const { return F4S(value | s); }
  template<int ns> inline F4S operator +(const TFlags4StateShift<Uid,Base,ns>& s) const { return F4S(value | s.value); }
  //
  template<typename T> inline eStates get(void) const { return T::get(*this); }
//  template<int nshift> F4S operator |(typename TFlags4StateShift<Uid,Base,nshift>::eState s) const { return F4S( value | s); }
  //
  inline eStates get_s(int shift) const { return eStates( (value >> (shift<<1)) & MMask ); }
  inline void set_s(int shift, eStates nv) { value &= ~(MMask<<(shift<<1)); value |= nv<<(shift<<1); }
  inline eStates getset_s(int shift, eStates nv) { eStates rv = get_s(shift); set_s(shift, nv); return rv; }
};

template<typename Uid, typename Base, int nshift> 
struct TFlags4StateShift 
{
  typedef TFlags4State<Uid, Base> F4S;
  typedef TFlags4StateShift<Uid, Base, nshift> F4SS;
  typedef typename F4S::eStates eStates;
  //
  enum eState {
      base_shift = nshift,
      shift = nshift<<1,
      Yes = F4S::Yes<<shift,
      No = F4S::No<<shift,
      Default = F4S::Default<<shift,
      Auto = F4S::Auto<<shift,
  };
  //
  inline static F4SS yes(void) { return Yes; }
  inline static F4SS no(void) { return Yes; }
  inline static F4SS def(void) { return Default; }
  inline static F4SS aut(void) { return Auto; }
  //
  inline static eStates vget(Base v) { return eStates( (v >> shift) & F4S::MMask); }
  inline static eStates get(const F4S &v) { return eStates( (v.value >> shift) & F4S::MMask); }
  //
  inline static bool yes(const F4S &v) { return get(v)==F4S::Yes; }
  inline static bool no(const F4S &v) { return get(v)==F4S::No; }
  inline static bool def(const F4S &v) { return get(v)==F4S::Default; }
  inline static bool aut(const F4S &v) { return get(v)==F4S::Auto; }
  //
  Base value;
  //
  inline TFlags4StateShift(eState s) : value(s) {}
  inline TFlags4StateShift(eStates s) : value(s<<shift) {}
  //
  template<int ns> inline F4S operator +(const TFlags4StateShift<Uid,Base,ns>& s2) const { return F4S(value | s2.value); }
  template<int ns> inline friend F4S operator +(eState v, const TFlags4StateShift<Uid,Base,ns>& s2) { return F4S(v | s2.value); }
};

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_UTIL_FLAGS4STATE_H
