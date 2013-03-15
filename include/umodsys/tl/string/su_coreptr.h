#ifndef __UMODSYS_TYPES_STRING_UTILS_COREPTR_H
#define __UMODSYS_TYPES_STRING_UTILS_COREPTR_H 1
/*************************************************************/
// file: umodsys/tl/string/su_coreptr.h
// info: string utilities - core pointer and core buffer data
/*************************************************************/

#include <umodsys/tl/string/su_base.h>

namespace UModSys {
namespace tl {
namespace su {

template<typename CharT> 
struct TSCoreConst {
  UMODSYS_STRING_CLASS_HEADER(CharT)
  typedef TSCoreConst<CharT> Self;
  //
  Str text;
  size_t length;
  //
  inline TSCoreConst(void) : text(NULL), length(0) {}
  inline explicit TSCoreConst(core::Void* p) {}
  inline TSCoreConst(const Self& R) : text(R.text), length(R.length) {}
  inline explicit TSCoreConst(Str s) { set(s); }
  inline TSCoreConst(Str s, size_t L) { set(s, L); }
  inline TSCoreConst(Str s, Str s_end) { set(s, s_end-s); }
  //
  inline const ConstStr& get_s(void) const { return *this; }
  inline Str get_text(void) const { return text; }
  inline size_t get_length(void) const { return length; }
  inline Str operator()(void) const { return text; }
  inline operator Str(void) const { return text; }
  inline size_t operator~(void) const { return length; }
  inline Str c_str(void) const { return text ? text : ""; }
  //
  inline void clear(void) { text=NULL; length=0; }
  inline void set(void) { clear(); }
  inline void set(Str s) { text=s; length=su::slen(s); }
  inline void set(Str s, size_t L) { text=s; length=L; }
  inline void set(Str s, Str s_end) { set(s, s_end-s); }
  inline void set(const Self& R) { text=R.text; length=R.length; }
  //
  inline const Self& operator=(const Self& R) { set(R); return *this; }
  inline const Self& operator=(Str R) { set(R); return *this; }
  //
  inline void set_ofs(void* data, int ofs, int len, void *source) {
    OStr dest = reinterpret_cast<OStr>(reinterpret_cast<core::BByte*>(data)+ofs);
    su::smemcpy(dest, reinterpret_cast<Str>(source), len);
    dest[len] = 0;
    set(dest, len);
  }
  //
  inline void set_ofs(void* data, int ofs, const Self& source) {
    OStr dest = reinterpret_cast<OStr>(reinterpret_cast<core::BByte*>(data)+ofs);
    su::smemcpy(dest, source.text, source.length);
    dest[source.length] = 0;
    set(dest, source.length);
  }
};

template<typename CharT> 
struct TSCoreBuffer {
  UMODSYS_STRING_CLASS_HEADER(CharT)
  typedef TSCoreBuffer<CharT> Self;
  //
  OStr text;
  size_t length;
  size_t maxlength;
  //
  inline TSCoreBuffer(void) : text(NULL), length(0), maxlength(0) {}
  inline explicit TSCoreBuffer(core::Void* p) {}
  inline TSCoreBuffer(const Self& R) : text(R.text), length(R.length), maxlength(R.maxlength) {}
  inline TSCoreBuffer(OStr s, size_t maxL, size_t L=0) { set(s, maxL, L); }
  inline TSCoreBuffer(OStr s, Str s_end, size_t L=0) { set(s, s_end-s, L); }
  inline TSCoreBuffer(OStr s, Str s_end, Str s_eos) { set(s, s_end-s, s_eos-s); }
  //
  inline ConstStr get_s(void) const { return ConstStr(text, length); }
  inline Str get_text(void) const { return text; }
  inline size_t get_length(void) const { return length; }
  inline Str operator()(void) const { return text; }
  inline operator Str(void) const { return text; }
  inline size_t operator~(void) const { return length; }
  inline Str c_str(void) const { return text ? text : ""; }
  //
  inline const BufferStr& get_buf(void) const { return *this; }
  inline BufferStr& get_buf(void) { return *this; }
  inline size_t get_bufmax(void) const { return maxlength; }
  inline void set_length(size_t L) { length=L; }
  inline OStr get_buftext(void) const { return text; }
  //
  inline void clear(void) { text=NULL; length=maxlength=0; }
  inline void reset(void) { length=0; }
  inline void set(void) { reset(); }
  inline void set(Str s) {
    if(maxlength==0)
      return;
    size_t n = su::slen(s); 
    if(n>=maxlength) n=maxlength-1;
    su::smemcpy(text, s, n);
    text[n] = 0;
    length = n;
  }
  inline void set(Str s, size_t n) {
    if(maxlength==0)
      return;
    if(n>=maxlength) n=maxlength-1;
    su::smemcpy(text, s, n);
    text[n] = 0;
    length = n;
  }
  inline void set(Str s, Str s_end) { set(s, s_end-s); }
  //
  inline void set(const Self& R) { text=R.text; length=R.length; maxlength=R.maxlength; }
  inline void set(OStr s, size_t maxL, size_t L) { text=s; length=L; maxlength=maxL; }
  //
  inline const Self& operator=(const Self& R) { set(R); return *this; }
  inline const Self& operator=(Str R) { set(R); return *this; }
  inline const Self& operator<<(const Self& R) { set(R.text, R.length); return *this; }
};


} // namespace su
} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TYPES_STRING_UTILS_COREBUFFER_H
