#ifndef __UMODSYS_TYPES_STRING_UTILS_COREPREALLOC_H
#define __UMODSYS_TYPES_STRING_UTILS_COREPREALLOC_H 1
/*************************************************************/
// file: umodsys/tl/string/su_coreprealloc.h
// info: string utilities - core pre-allocated data
/*************************************************************/

#include <umodsys/tl/string/su_base.h>

namespace UModSys {
namespace tl {
namespace su {

template<size_t nChars, typename CharT> 
struct TSCorePrealloc {
  UMODSYS_STRING_CLASS_HEADER(CharT)
  enum { N_Chars = nChars };
  struct Buffer {
    Char buffer[nChars];
    //
    inline operator Char*(void) { return buffer; }
    inline operator const Char*(void) const { return buffer; }
    inline void clear(void) { buffer[0] = 0; }
  };
  typedef TSCorePrealloc<nChars, CharT> Self;
  //
  mutable Buffer text;
  mutable size_t length;
  //
  inline TSCorePrealloc(void) : length(0) { clear(); }
  inline explicit TSCorePrealloc(core::Void* p) {}
  inline TSCorePrealloc(const Self& R) : text(R.text), length(R.length) {}
  inline explicit TSCorePrealloc(Str s) { set(s); }
  inline TSCorePrealloc(Str s, size_t L) { set(s, L); }
  inline TSCorePrealloc(Str s, Str s_end) { set(s, s_end-s); }
  //
  inline ConstStr get_s(void) const { return ConstStr(text, length); }
  inline Str get_text(void) const { return text; }
  inline size_t get_length(void) const { return length; }
  inline Str operator()(void) const { return text; }
  inline operator Str(void) const { return text; }
  inline size_t operator~(void) const { return length; }
  inline Str c_str(void) const { return text; }
  inline bool empty(void) const { return length==0; }
  //
  inline BufferStr get_buf(void) const { return BufferStr(text, nChars, length); }
  inline size_t get_bufmax(void) const { return nChars; }
  inline void set_length(size_t L) { length=L; }
  inline OStr get_buftext(void) const { return text; }
  //
  inline void clear(void) { text.clear(); length=0; }
  inline void reset(void) { text.clear(); length=0; }
  inline void set(void) { clear(); }
  inline void set(Str s) {
    size_t n = su::slen(s); 
    if(n>=nChars) n=nChars-1;
    su::smemcpy(text.buffer, s, n);
    text[n] = 0;
    length = n;
  }
  inline void set(Str s, size_t n) {
    if(n>=nChars) n=nChars-1;   
    su::smemcpy(text.buffer, s, n);
    text[n] = 0;
    length = n;
  }
  inline void set(const Self& R) {
    size_t n = R.length; 
    if(n>nChars) n = nChars;
    su::smemcpy(text.buffer, R.text.buffer, n);
    length = R.length;
  }
  //
  inline const Self& operator=(const Self& R) { set(R); return *this; }
  inline const Self& operator=(Str R) { set(R); return *this; }
};


} // namespace su
} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TYPES_STRING_UTILS_COREPREALLOC_H
