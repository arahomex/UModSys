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
  typedef TSCoreBuffer<CharT> CoreBuffer;
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
  inline bool empty(void) const { return length==0; }
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
struct TSCoreBufferBuffer {
  UMODSYS_STRING_CLASS_HEADER(CharT)
  typedef TSCoreBufferBuffer<CharT> Self;
  //
  OStr text;
  size_t maxlength;
  //
  inline TSCoreBufferBuffer(OStr s, size_t maxL) { bufsetup(s, maxL); }
  inline TSCoreBufferBuffer(OStr s, Str s_end) { bufsetup(s, s_end-s); }
  inline TSCoreBufferBuffer(void) { bufsetup(NULL, size_t(0)); }
  inline explicit TSCoreBufferBuffer(core::Void* p) {}
  //
  inline void bufsetup(OStr s, Str s_end) { bufset(s, s_end-s); }
  inline void bufsetup(const Self& b) { *this = b; }
  inline void bufsetup(OStr s, size_t maxL) { text=s; maxlength=maxL; }
  inline void clear(void) { text=NULL; maxlength=0; }
};

template<typename CharT> 
struct TSCoreBuffer : TSCoreBufferBuffer<CharT> {
  UMODSYS_STRING_CLASS_HEADER(CharT)
  typedef TSCoreBuffer<CharT> Self;
  typedef TSCoreBufferBuffer<CharT> BufData, Buf;
  typedef TSCoreConst<CharT> CoreConst;
  //
  size_t length;
  //
  inline TSCoreBuffer(void) : length(0) {}
  inline explicit TSCoreBuffer(core::Void* p) {}
  inline TSCoreBuffer(const Self& R) : Buf(R), length(R.length) {}
  inline TSCoreBuffer(const Buf& buf, size_t L=0) : Buf(buf), length(L) {}
  inline explicit TSCoreBuffer(OStr s, size_t maxL, size_t L=0) { setup(s, maxL, L); }
  inline explicit TSCoreBuffer(OStr s, Str s_end, size_t L=0) { setup(s, s_end-s, L); }
  inline explicit TSCoreBuffer(OStr s, Str s_end, Str s_eos) { setup(s, s_end-s, s_eos-s); }
  //
  inline ConstStr get_s(void) const { return ConstStr(Buf::text, length); }
//  inline ConstStr operator*(void) const { return ConstStr(Buf::text, length); }
  inline Str get_text(void) const { return Buf::text; }
  inline size_t get_length(void) const { return length; }
//  inline Str operator()(void) const { return Buf::text; }
//  inline operator Str(void) const { return Buf::text; }
  inline size_t operator~(void) const { return length; }
  inline Str c_str(void) const { return Buf::text ? Buf::text : ""; }
  //
  inline const BufferStr& get_buf(void) const { return *this; }
  inline BufferStr& get_buf(void) { return *this; }
  inline size_t get_bufmax(void) const { return Buf::maxlength; }
  inline void set_length(size_t L) { length=L; }
  inline OStr get_buftext(void) const { return Buf::text; }
  //
  inline void clear(void) { Buf::clear(); reset(); }
  inline void reset(void) { length=0; }
  inline void set(void) { reset(); }
  //
  inline void setup(Str s, Str s_end) { set(s, s_end-s); }
  inline void setup(const Self& R) { Buf::bufsetup(R); length=R.length; }
  inline void setup(OStr s, size_t maxL, size_t L) { Buf::bufsetup(s, maxL); length=L; }
  //
  inline bool append(Str s, size_t L) { return cat(s, L); }
  inline bool set(Str s, size_t L) { return safecpy(s, L); }
  //
  inline bool cat(Str s, size_t n) {
    if(n==0)
      return true;
    if(s==NULL)
      return false;
    if(length+n+1>Buf::maxlength)
      return false;
    su::smemcpy(Buf::text+length, s, n);
    length += n;
    Buf::text[length] = 0;
    return true;
  }
  inline bool safecpy(Str s) {
    if(Buf::maxlength==0)
      return false;
    size_t n = su::slen(s); 
    if(n>=Buf::maxlength) 
      n=Buf::maxlength-1;
    su::smemcpy(Buf::text, s, n);
    Buf::text[n] = 0;
    length = n;
    return true;
  }
  inline bool safecpy(Str s, size_t n) {
    if(Buf::maxlength==0)
      return false;
    if(n>=Buf::maxlength) 
      n=Buf::maxlength-1;
    su::smemcpy(Buf::text, s, n);
    Buf::text[n] = 0;
    length = n;
    return true;
  }
  //
  inline const Self& operator=(const Self& R) { set(R); return *this; }
  inline const Self& operator=(Str R) { safecpy(R); return *this; }
  inline const Self& operator<<(const Self& R) { safecpy(R.text, R.length); return *this; }
};


} // namespace su
} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TYPES_STRING_UTILS_COREBUFFER_H
