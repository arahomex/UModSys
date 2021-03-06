#ifndef __UMODSYS_TL_STRING_BUFFER_H
#define __UMODSYS_TL_STRING_BUFFER_H 1
/*************************************************************/
// file: umodsys/tl/string/string_buffer.h
// info: string buffer
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
// Buffer string
//***************************************

template<typename CoreT, typename Comparer> 
struct TStringBuf : public CoreT, public Comparer {
  typedef typename CoreT::Char CharT;
  UMODSYS_STRING_CLASS_HEADER(CharT)
  typedef typename CoreT::CoreConst CoreConst;
  typedef typename CoreT::Buf Buf;
  typedef TStringBuf<CoreT, Comparer> Self, SelfBuf;
  typedef TString<CoreConst, Comparer> SelfConst;
  typedef TString<ConstStr, Comparer> ConstString;
  //
  inline TStringBuf(void) {}
  inline explicit TStringBuf(core::Void* p) : CoreT(p), Comparer(p) {}
  inline TStringBuf(const ConstStr& R) { set(R); }
  inline TStringBuf(const Self& R) : CoreT(R), Comparer(R) {}
  inline TStringBuf(const Comparer& cmp, core::Void* p) : CoreT(p), Comparer(cmp) {}
  inline /*explicit*/ TStringBuf(Str s) { set(s); }
  inline TStringBuf(Str s, size_t L) { set(s, L); }
  inline TStringBuf(Str s, Str s_end) { set(s, s_end); }
  inline TStringBuf(const Buf& b, Str s) { CoreT::setup(b); set(s); }
  inline TStringBuf(const Buf& b, Str s, size_t L) { CoreT::setup(b); set(s, L); }
  inline TStringBuf(const Buf& b, Str s, Str s_end) { CoreT::setup(b); set(s, s_end); }
//  inline explicit TStringBuf(const Comparer& cmp) : Comparer(cmp) { set(); }
  inline TStringBuf(const Buf& b, const Comparer& cmp, Str s) : Comparer(cmp) { CoreT::setup(b); set(s); }
  inline TStringBuf(const Buf& b, const Comparer& cmp, Str s, size_t L) : Comparer(cmp) { CoreT::setup(b); set(s, L); }
  inline TStringBuf(const Buf& b, const Comparer& cmp, Str s, Str s_end) : Comparer(cmp) { CoreT::setup(b); set(s, s_end); }
  inline TStringBuf(const Comparer& cmp, Str s) : Comparer(cmp) { set(s); }
  inline TStringBuf(const Comparer& cmp, Str s, size_t L) : Comparer(cmp) { set(s, L); }
  inline TStringBuf(const Comparer& cmp, Str s, Str s_end) : Comparer(cmp) { set(s, s_end); }
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
  template<typename Core2> inline int cmp(const TStringBuf<Core2, Comparer>& s) const { 
    if(Comparer::same(s.get_comparer()))
      return Comparer::cmp3(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length(), s.get_comparer()); 
    return Comparer::cmp2(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length()); 
  }
  template<typename Core2> inline int compare(const TStringBuf<Core2, Comparer>& s) const { 
    if(Comparer::same(s.get_comparer()))
      return Comparer::cmp3(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length(), s.get_comparer()); 
    return Comparer::cmp2(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length()); 
  }
  template<typename Core2> inline bool eq(const TStringBuf<Core2, Comparer>& s) const { 
    if(Comparer::same(s.get_comparer()))
      return Comparer::eq3(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length(), s.get_comparer()); 
    return Comparer::eq2(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length()); 
  }
  //
  inline bool starts(const ConstStr& s) const {
    return Comparer::starts(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length(), s.get_comparer()); 
  }
  inline bool starts(const ConstStr& s, ConstStr& left) const {
    if(!Comparer::starts(CoreT::get_text(), CoreT::get_length(), s.get_text(), s.get_length()))
      return false;
    left.set(CoreT::get_text()+s.get_length(), CoreT::get_length()-s.get_length());
    return true;
  }
  //
  inline ConstString str(void) const { return ConstString(*this, CoreT::get_text(), CoreT::get_length()); }
  inline SelfConst string(void) const { return SelfConst(*this, CoreT::get_text(), CoreT::get_length()); }
  inline Str begin(void) const { return CoreT::get_text(); }
  inline Str end(void) const { return CoreT::get_text() + CoreT::get_length(); }
  inline Str cbegin(void) const { return CoreT::get_text(); }
  inline Str cend(void) const { return CoreT::get_text() + CoreT::get_length(); }
  inline OStr begin(void) { return CoreT::get_text(); }
  inline OStr end(void) { return CoreT::get_text() + CoreT::get_length(); }
  //
  inline void set_length(size_t L) { CoreT::set_length(L); update(); }
  inline void update(BufferStr& bs) { set_length(bs.length); }
  inline void update(void) { Comparer::update(CoreT::get_text(), CoreT::get_length()); }
  inline void clear(void) { CoreT::clear(); Comparer::clear(); }
  inline void set(void) { clear(); }
  inline void set(const Comparer& cmp) { Comparer::operator=(cmp); update(); }
  inline bool set(const ConstStr& s) { bool rv = CoreT::set(s); update(); return rv; }
  inline bool set(Str s) { bool rv = CoreT::set(s); update(); return rv; }
  inline bool set(Str s, size_t L) { bool rv = CoreT::set(s, L); update(); return rv; }
  //
  inline bool cpy(const Self& R) { bool rv = CoreT::cpy(R); Comparer::operator=(R); return rv; }
  inline bool safecpy(const Self& R) { bool rv = CoreT::safecpy(R); Comparer::operator=(R); return rv; }
  //
  inline bool append(Str s, size_t L) { bool rv = CoreT::append(s, L); update(); return rv; }
  template<typename Core2, typename Cmp2> inline bool append(const TString<Core2, Cmp2>& s) 
    { bool rv = CoreT::append(s, ~s); update(); return rv; }
  //
  template<typename Core2, typename Cmp2> inline bool set(const TString<Core2, Cmp2>& R) 
    { return set(R.get_text(), R.get_length()); }
  template<typename Core2, typename Cmp2> inline bool set(const TStringBuf<Core2, Cmp2>& R) 
    { return set(R.get_text(), R.get_length()); }
//  template<> inline bool set(const TStringBuf<CoreT, Comparer>& R) 
//    { return set(R.get_text(), R.get_length()); }
  //
  inline const Self& operator=(const Self& R) { set(R); return *this; }
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
  inline int scalar_compare(const tl::TStringBuf<CoreT,Comparer>& L, const tl::TStringBuf<CoreT,Comparer>& R) 
    { return L.compare(R); }

} // namespace core

} // namespace UModSys

#endif // __UMODSYS_TL_STRING_BUFFER_H
