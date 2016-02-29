#ifndef __UMODSYS_TYPES_STRING_UTILS_CORESHARED_H
#define __UMODSYS_TYPES_STRING_UTILS_CORESHARED_H 1
/*************************************************************/
// file: umodsys/tl/string/su_coreshared.h
// info: string utilities - core shared allocated data
/*************************************************************/

#include <umodsys/tl/string/su_base.h>

namespace UModSys {
namespace tl {
namespace su {

template<typename CharT, typename MemAllocT>
struct TSharedStringInfo : public MemAllocT {
  int ref_count;
  size_t maxlength, length;
  CharT text[1]; // NUL
  //
  inline TSharedStringInfo(int rc) 
  : ref_count(rc), maxlength(0), length(0) { 
    text[0] = 0; 
  }
  inline TSharedStringInfo(size_t ml, size_t l, const MemAllocT& ma) 
  : ref_count(1), maxlength(ml), length(l), MemAllocT(ma) { 
  }
};

//***************************************
// Shared string info

template<typename MemAllocT, typename CharT>
struct TSCoreShared : public MemAllocT {
  UMODSYS_STRING_CLASS_HEADER(CharT)
  typedef TSCoreShared<MemAllocT, CharT> Self;
  typedef TSharedStringInfo<CharT, MemAllocT> Info;
  typedef TSCoreShared<MemAllocT, CharT> CoreBuffer;
  typedef TSCoreShared<MemAllocT, CharT> CoreConst;
  typedef MemAllocT Buf;
  //
  Info* info;
  //
  inline ~TSCoreShared(void) { clear(); }
  inline TSCoreShared(void) : info(get_null()) {}
  inline explicit TSCoreShared(core::Void* p) {}
  inline TSCoreShared(const Self& R) : MemAllocT(R), info(get_null()) { set(R); }
  inline explicit TSCoreShared(Str s) : info(get_null()) { set(s); }
  inline TSCoreShared(Str s, size_t L) : info(get_null()) { set(s, L); }
  inline TSCoreShared(Str s, Str s_end) : info(get_null()) { set(s, s_end-s); }
  //
  inline void set_alloc(const MemAllocT& A) { MemAllocT::operator=(A); }
  inline bool allocate(const MemAllocT& A, size_t L) { MemAllocT::operator=(A); return info_alloc(L); }
  //
  inline ConstStr get_s(void) const { return ConstStr(info->text, info->length); }
  inline operator ConstStr(void) const { return ConstStr(info->text, info->length); }
//  inline ConstStr operator*(void) const { return ConstStr(info->text, info->length); }
  inline Str get_text(void) const { return info->text; }
  inline size_t get_length(void) const { return info->length; }
  inline Str operator*(void) const { return info->text; }
//  inline operator Str(void) const { return info->text; }
  inline size_t operator~(void) const { return info->length; }
  inline Str c_str(void) const { return info ? info->text : ""; }
  inline bool is_null(void) const { return info==NULL || info==get_null(); }
  inline bool empty(void) const { return is_null(); }
  //
  inline void clear(void) { info_release(); }
  inline void set(void) { clear(); }
  inline void set(const Self& R) { info_dup(R.info); }
  //
  inline bool set(Str s) { return set(s, s ? su::slen(s) : 0); }
  inline bool set(Str s, Str s_end) { return set(s, s_end-s); }
  inline bool set(Str s, size_t L) { return info_new(s, L); }
  inline void set(const ConstStr& s) { set(*s, ~s); }
  //
  inline BufferStr get_buf(void) const { return BufferStr(info->text, info->maxlength, info->length); }
  inline size_t get_bufmax(void) const { return info->maxlength; }
  inline void set_length(size_t L) { if(info!=get_null()) info->length=L; }
  inline OStr get_buftext(void) const { return info->text; }
  //
  inline const Self& operator=(const Self& R) { set(R); return *this; }
  inline const Self& operator=(Str R) { set(R); return *this; }
  //
  void info_release(void);
  void info_dup(Info* info);
  bool info_new(Str s, size_t L);
  bool info_alloc(size_t L);
  //
  static Info* get_null(void);
};

template<typename MemAllocT, typename CharT>
inline typename TSCoreShared<MemAllocT,CharT>::Info* TSCoreShared<MemAllocT,CharT>::get_null(void)
{
  static Info s_null(-1);
  return &s_null;
}

template<typename MemAllocT, typename CharT>
inline void TSCoreShared<MemAllocT,CharT>::info_release(void) 
{
  if(info!=NULL && info->ref_count>=0 && --info->ref_count<=0) {
    if(info!=get_null()) {
      info->mem_free(info, UMODSYS_SOURCEINFO);
    }
  }
  info = get_null();
}

template<typename MemAllocT, typename CharT>
inline void TSCoreShared<MemAllocT,CharT>::info_dup(typename TSCoreShared<MemAllocT,CharT>::Info* i2)
{
  if(info==i2) 
    return;
  info_release();
  info = i2;
  info->ref_count++;
}

template<typename MemAllocT, typename CharT>
inline bool TSCoreShared<MemAllocT,CharT>::info_alloc(size_t L)
{
  if(info==NULL && info==get_null()) {
    return info_new(NULL, L);
  }
  if(L<info->maxlength)
    return true;
  return info_new(info->text, L);
}

template<typename MemAllocT, typename CharT>
inline bool TSCoreShared<MemAllocT,CharT>::info_new(Str s, size_t L)
{
  info_release();
  info = static_cast<Info*>( MemAllocT::mem_alloc(sizeof(Info)+L, UMODSYS_SOURCEINFO) );
  if(info==NULL) {
    info = get_null();
    return false;
  }
  new(info) Info(L+1, L, *this);
  if(s!=NULL) {
    su::smemcpy(info->text, s, L);
  }
  info->text[L] = 0;
  return true;
}

} // namespace su
} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TYPES_STRING_UTILS_CORESHARED_H
