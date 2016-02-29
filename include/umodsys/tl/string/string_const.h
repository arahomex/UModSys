#ifndef __UMODSYS_TL_STRING_CONST_H
#define __UMODSYS_TL_STRING_CONST_H 1
/*************************************************************/
// file: umodsys/tl/string/string_const.h
// info: string const
/*************************************************************/

#include <umodsys/tl/string/su_base.h>
#include <umodsys/tl/string/su_char.h>
#include <umodsys/tl/string/su_charw.h>
#include <umodsys/tl/string/su_charl.h>
#include <umodsys/tl/string/su_comparer.h>

#include <umodsys/tl/string/su_coreptr.h>

namespace UModSys {
namespace tl {

//***************************************
// Const string
//***************************************

template<typename CoreT, typename Comparer> 
struct TString : public CoreT, public Comparer {
  typedef typename CoreT::Char CharT;
  UMODSYS_STRING_CLASS_HEADER(CharT)
  typedef typename CoreT::CoreBuffer CoreBuffer;
  typedef TString<CoreT, Comparer> Self, SelfConst;
  typedef TStringBuf<CoreBuffer, Comparer> SelfBuf;
  typedef TString<ConstStr, Comparer> ConstString;
  //
//  template<typename Core2> inline TString(const TString<Core2, Comparer>& R) : CoreT(R), Comparer(R) {}
//  template<typename Core2> inline TString(const TStringBuf<Core2, Comparer>& R) : CoreT(R), Comparer(R) {}
  //
  inline TString(void) {}
  inline explicit TString(core::Void* p) : CoreT(p), Comparer(p) {}
  inline TString(const ConstStr& R) { set(R); }
  inline TString(const Self& R) : CoreT(R), Comparer(R) {}
  inline TString(const SelfBuf& R) : CoreT(R), Comparer(R) {}
  inline TString(const Comparer& cmp, core::Void* p) : CoreT(p), Comparer(cmp) {}
  inline /*explicit*/ TString(Str s) { set(s); }
  inline TString(Str s, size_t L) { set(s, L); }
  inline TString(Str s, Str s_end) { set(s, s_end); }
//  inline explicit TString(const Comparer& cmp) : Comparer(cmp) { set(); }
  inline TString(const Comparer& cmp, Str s) : Comparer(cmp) { set(s); }
  inline TString(const Comparer& cmp, Str s, size_t L) : Comparer(cmp) { set(s, L); }
  inline TString(const Comparer& cmp, Str s, Str s_end) : Comparer(cmp) { set(s, s_end); }
  //
  inline ConstString str(void) const { return ConstString(*this, CoreT::get_text(), CoreT::get_length()); }
  inline SelfConst string(void) const { return SelfConst(*this, CoreT::get_text(), CoreT::get_length()); }
  inline Str begin(void) const { return CoreT::get_text(); }
  inline Str end(void) const { return CoreT::get_text() + CoreT::get_length(); }
  inline Str cbegin(void) const { return CoreT::get_text(); }
  inline Str cend(void) const { return CoreT::get_text() + CoreT::get_length(); }
  //
  inline const Comparer& get_comparer(void) const { return *this; }
  inline Comparer& get_comparer(void) { return *this; }
  //
  inline int cmp(Str s) const 
    { return Comparer::cmp1(CoreT::get_text(), CoreT::get_length(), s); }
  inline bool eq(Str s) const 
    { return Comparer::eq1(CoreT::get_text(), CoreT::get_length(), s); }
  inline bool operator==(Str s) const { return eq(s); }
  inline bool operator!=(Str s) const  { return !eq(s); }
  inline bool operator<(Str s) const { return cmp(s)<0; }
  inline bool operator<=(Str s) const { return cmp(s)<=0; }
  inline bool operator>(Str s) const { return cmp(s)>0; }
  inline bool operator>=(Str s) const { return cmp(s)>=0; }
  //
  template<typename Core2, typename Cmp2> inline int cmp(const TString<Core2, Cmp2>& s) const 
    { return Comparer::cmp2(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length()); }
  template<typename Core2, typename Cmp2> inline bool eq(const TString<Core2, Cmp2>& s) const 
    { return Comparer::eq2(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length()); }
  template<typename Core2, typename Cmp2> inline bool operator==(const TString<Core2, Cmp2>& s) const 
    { return eq(s); }
  template<typename Core2, typename Cmp2> inline bool operator!=(const TString<Core2, Cmp2>& s) const 
    { return !eq(s); }
  template<typename Core2, typename Cmp2> inline bool operator<(const TString<Core2, Cmp2>& s) const 
    { return cmp(s)<0; }
  template<typename Core2, typename Cmp2> inline bool operator<=(const TString<Core2, Cmp2>& s) const 
    { return cmp(s)<=0; }
  template<typename Core2, typename Cmp2> inline bool operator>(const TString<Core2, Cmp2>& s) const 
    { return cmp(s)>0; }
  template<typename Core2, typename Cmp2> inline bool operator>=(const TString<Core2, Cmp2>& s) const 
    { return cmp(s)>=0; }
  //
  template<typename Core2> inline int compare(const TString<Core2, Comparer>& s) const { 
    if(Comparer::same(s.get_comparer()))
      return Comparer::cmp3(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length(), s.get_comparer()); 
    return Comparer::cmp2(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length()); 
  }
  template<typename Core2> inline int cmp(const TString<Core2, Comparer>& s) const { 
    if(Comparer::same(s.get_comparer()))
      return Comparer::cmp3(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length(), s.get_comparer()); 
    return Comparer::cmp2(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length()); 
  }
  template<typename Core2> inline bool eq(const TString<Core2, Comparer>& s) const { 
    if(Comparer::same(s.get_comparer()))
      return Comparer::eq3(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length(), s.get_comparer()); 
    return Comparer::eq2(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length()); 
  }
  //
  template<typename Core2, typename Cmp2> inline int cmp(const TStringBuf<Core2, Cmp2>& s) const 
    { return Comparer::cmp2(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length()); }
  template<typename Core2, typename Cmp2> inline bool eq(const TStringBuf<Core2, Cmp2>& s) const 
    { return Comparer::eq2(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length(), s.get_comparer()); }
  template<typename Core2, typename Cmp2> inline bool operator==(const TStringBuf<Core2, Cmp2>& s) const 
    { return eq(s); }
  template<typename Core2, typename Cmp2> inline bool operator!=(const TStringBuf<Core2, Cmp2>& s) const 
    { return !eq(s); }
  template<typename Core2, typename Cmp2> inline bool operator<(const TStringBuf<Core2, Cmp2>& s) const 
    { return cmp(s)<0; }
  template<typename Core2, typename Cmp2> inline bool operator<=(const TStringBuf<Core2, Cmp2>& s) const 
    { return cmp(s)<=0; }
  template<typename Core2, typename Cmp2> inline bool operator>(const TStringBuf<Core2, Cmp2>& s) const 
    { return cmp(s)>0; }
  template<typename Core2, typename Cmp2> inline bool operator>=(const TStringBuf<Core2, Cmp2>& s) const 
    { return cmp(s)>=0; }
  //
  inline void update(void) { Comparer::update(CoreT::get_text(), CoreT::get_length()); }
  inline void clear(void) { CoreT::clear(); Comparer::clear(); }
  inline void set(void) { clear(); }
  inline void set(Str s) { CoreT::set(s); update(); }
  inline void set(Str s, int L) { CoreT::set(s, L); update(); }
  inline void set(Str s, Str s_end) { CoreT::set(s, s_end); update(); }
  inline void set(const ConstStr& R) { CoreT::set(R); update(); }
  inline void set(const Self& R) { CoreT::set(R); Comparer::operator=(R); }
  inline void set(const SelfBuf& R) { CoreT::set(R); Comparer::operator=(R); }
  inline void set(const Comparer& cmp) { Comparer::operator=(cmp); update(); }
  //
  template<typename Core2, typename Cmp2> inline void set(const TString<Core2, Cmp2>& R) 
    { set(R.get_text(), R.get_length()); }
  template<typename Core2, typename Cmp2> inline void set(const TStringBuf<Core2, Cmp2>& R) 
    { set(R.get_text(), R.get_length()); }
  //
  inline const Self& operator=(const Self& R) { set(R); return *this; }
  inline const Self& operator=(const SelfBuf& R) { set(R); return *this; }
  inline const Self& operator=(Str R) { set(R); return *this; }
  inline const Self& operator=(const Comparer& cmp) { set(cmp); return *this; }
  template<typename Core2, typename Cmp2> inline void operator=(const TString<Core2, Cmp2>& R) 
    { set(R); }
  template<typename Core2, typename Cmp2> inline void operator=(const TStringBuf<Core2, Cmp2>& R) 
    { set(R); }
};

//***************************************
// 
//***************************************

} // namespace tl

namespace core {

  template<typename CoreT, typename Comparer>
  inline int scalar_compare(const tl::TString<CoreT,Comparer>& L, const tl::TString<CoreT,Comparer>& R) 
    { return L.compare(R); }

} // namespace core

} // namespace UModSys

#endif // __UMODSYS_TL_STRING_CONST_H
