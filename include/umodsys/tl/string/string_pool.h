#ifndef __UMODSYS_TL_STRING_POOL_H
#define __UMODSYS_TL_STRING_POOL_H 1
/*************************************************************/
// file: umodsys/tl/string/string_pool.h
// info: string pool/cache
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
// Static pool
//***************************************

template<typename CharT, typename Comparer, typename MemAllocT>
struct TStaticPool : public MemAllocT {
  UMODSYS_STRING_CLASS_HEADER(CharT)
  typedef TString<su::TSCoreConst<CharT>, Comparer> StringElem;
  typedef TStaticPool<CharT, Comparer, MemAllocT> Self;
  //
  struct CharChunk {
    CharChunk* next;
    size_t len, maxlen;
    CharT buffer[1];
    //
    inline CharChunk(size_t n) { next=NULL; len=0; maxlen=n; }
    //
    inline size_t left(void) const { return maxlen-len; }
    inline CharT* append(Str v, size_t n) {
      CharT* rv = buffer + len;
      su::scpy(rv, v, n);
      rv[n] = 0;
      len += n+1;
      return rv;
    }
  };
  struct StringChunk {
    StringChunk* next;
    size_t len, maxlen;
    StringElem buffer[1];
    //
    inline StringChunk(size_t n) { next=NULL; len=0; maxlen=n; }
    //
    inline size_t left(void) const { return maxlen-len; }
    inline StringElem* append(Str vv) {
      StringElem* rv = buffer + len++;
      new (rv) StringElem(vv);
      return rv;
    }
  };
public:
  inline ~TStaticPool(void) { release(); }
  inline TStaticPool(size_t cm=0x1000, size_t sm=0x100) 
  : cs(NULL), ss(NULL), c_max(cm), s_max(sm), c_len(0), s_len(0) {}
  inline TStaticPool(const MemAllocT& ma, size_t cm=0x1000, size_t sm=0x100) 
  : MemAllocT(ma), cs(NULL), ss(NULL), c_max(cm), s_max(sm), c_len(0), s_len(0) {}
  inline TStaticPool(const Self& R) 
  : MemAllocT(R), cs(NULL), ss(NULL), c_max(R.c_max), s_max(R.s_max), c_len(0), s_len(0) { copyfrom(R); }
  //
  inline void operator=(const Self& R) { copyfrom(R); }
  //
  inline size_t used_chars(void) const { return c_len; }
  //
  inline size_t used_strings(void) const { return s_len; }
  inline size_t operator~(void) const { return s_len; }
  inline size_t len(void) const { return s_len; }
  inline operator size_t(void) const { return s_len; }
  //
  const StringElem* get_s(size_t n) const;
  const StringElem* find_s(const StringElem& ss) const;
  inline const StringElem& operator()(size_t n) const { return *get_s(n); }
  inline const StringElem& operator[](size_t n) const { return *get_s(n); }
  inline const StringElem& get(size_t n) const { return *get_s(n); }
  //
  //
  const StringElem* append(Str v, size_t len);
  inline const StringElem* append(Str v) { return append(v, su::slen(v)); }
  template<typename CoreT, typename Comparer2> 
  inline const StringElem* append(const TString<CoreT, Comparer>& v) 
    { return append(v.get_text(), v.get_length()); }
  template<typename CoreT, typename Comparer2> 
  inline const StringElem* append(const TStringBuf<CoreT, Comparer>& v) 
    { return append(v.get_text(), v.get_length()); }
  //
  void copyfrom(const Self &R);
  void release(void);
public:
  CharChunk *cs;
  StringChunk *ss;
  size_t c_max, c_len, s_max, s_len;
};

template<typename CharT, typename Comparer, typename MemAllocT>
const typename TStaticPool<CharT,Comparer,MemAllocT>::StringElem* 
TStaticPool<CharT,Comparer,MemAllocT>::get_s(size_t n) const
{
  const typename TStaticPool<CharT,Comparer,MemAllocT>::StringChunk *x;
  for(x=ss; x!=NULL; x=x->next) {
    if(n<x->len)
      return &x->buffer[n];
    n -= x->len;
  }
  return NULL;
}

template<typename CharT, typename Comparer, typename MemAllocT>
const typename TStaticPool<CharT,Comparer,MemAllocT>::StringElem* 
TStaticPool<CharT,Comparer,MemAllocT>::append(typename TStaticPool<CharT,Comparer,MemAllocT>::Str v, size_t len)
{
  typedef typename TStaticPool<CharT,Comparer,MemAllocT>::StringChunk SC;
  typedef typename TStaticPool<CharT,Comparer,MemAllocT>::CharChunk CC;
  SC **px, *x;
  CC **pz, *z;
  typename TStaticPool<CharT,Comparer,MemAllocT>::Str vv;
  void *vp;
  //
  const typename TStaticPool<CharT,Comparer,MemAllocT>::StringElem* rv;
  rv = find_s(TStaticPool<CharT,Comparer,MemAllocT>::StringElem(v, len));
  if(rv!=NULL)
    return rv;
  //
  // -- find space in allocated char chunks
  for(pz=&cs, z=cs; z!=NULL; pz=&z->next, z=z->next) {
    if(len<=z->left()) {
      c_len += len+1;
      vv = z->append(v, len);
      break;
    }
  }
  if(z==NULL) { // allocate new chunk
    size_t clen = len>c_max ? len : c_max;
    vp = MemAllocT::mem_alloc(sizeof(*z)+clen, UMODSYS_SOURCEINFO);
    if(vp==NULL)
      return NULL; // error
    z = new(vp) CC(clen);
    c_len += len+1;
    vv = z->append(v, len);
    *pz = z;
  }
  // -- find space in allocated string chunks
  for(px=&ss, x=ss; x!=NULL; px=&x->next, x=x->next) {
    if(x->left()) {
      s_len++;
      return x->append(vv);
    }
  }
  // allocate new chunk
  vp = MemAllocT::mem_alloc(sizeof(*x)+s_max, UMODSYS_SOURCEINFO);
  if(vp==NULL)
    return NULL; // error
  x = new(vp) SC(s_max);
  *px = x;
  s_len++;
  return x->append(vv);
}

template<typename CharT, typename Comparer, typename MemAllocT>
const typename TStaticPool<CharT,Comparer,MemAllocT>::StringElem* 
TStaticPool<CharT,Comparer,MemAllocT>::find_s(const typename TStaticPool<CharT,Comparer,MemAllocT>::StringElem& vv) const
{
  const typename TStaticPool<CharT,Comparer,MemAllocT>::StringChunk *x;
  //
  for(x=ss; x!=NULL; x=x->next) {
    for(int i=0; i<x->len; i++) {
      if(x->buffer[i]==vv)
        return x->buffer+i;
    }
  }
  return NULL;
}

template<typename CharT, typename Comparer, typename MemAllocT>
void TStaticPool<CharT,Comparer,MemAllocT>::copyfrom(const typename TStaticPool<CharT,Comparer,MemAllocT>::Self& R)
{
}

template<typename CharT, typename Comparer, typename MemAllocT>
void TStaticPool<CharT,Comparer,MemAllocT>::release(void)
{
  typename TStaticPool<CharT,Comparer,MemAllocT>::StringChunk *x, *xx;
  typename TStaticPool<CharT,Comparer,MemAllocT>::CharChunk *z, *zz;
  //
  for(x=ss, ss=NULL; x!=NULL; x=xx) {
    xx = x->next;
    MemAllocT::mem_free(x, UMODSYS_SOURCEINFO);
  }
  for(z=cs, cs=NULL; z!=NULL; z=zz) {
    zz = z->next;
    MemAllocT::mem_free(z, UMODSYS_SOURCEINFO);
  }
}

//***************************************
// 
//***************************************

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_STRING_POOL_H
