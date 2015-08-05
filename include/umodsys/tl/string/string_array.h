#ifndef __UMODSYS_TL_STRING_ARRAY_H
#define __UMODSYS_TL_STRING_ARRAY_H 1
/*************************************************************/
// file: umodsys/tl/string/string_array.h
// info: string arrays
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
// Array str
//***************************************

template<size_t N, typename CharT>
struct TArrayStr {
  UMODSYS_STRING_CLASS_HEADER(CharT)
public:
  inline TArrayStr(void) : used(0) {}
  inline ~TArrayStr(void) {}
  //
  inline size_t operator~(void) const { return used; }
  inline size_t len(void) const { return used; }
  inline operator size_t(void) const { return used; }
  inline operator int(void) const { return (int)used; }
  //
  inline Str operator()(size_t n) const { return arr[n]; }
  inline Str operator[](size_t n) const { return arr[n]; }
  inline Str& operator[](size_t n) { return arr[n]; }
  //
  inline Str get(size_t n) const { return arr[n]; }
  inline void set(size_t n, Str v) const { arr[n] = v; }
  inline bool add(Str v) { if(used>=N) return false; arr[used++]=v; return true; }
  inline void clear(void) { used=0; }
  inline bool push(Str v) { if(used>=N) return false; arr[used++]=v; return true; }
  inline Str pop(void) { if(used==0) return NULL; return arr[--used]; }
  inline bool fpop(void) { if(used==0) return false; --used; return true; }
public:
  Str arr[N];
  size_t used;
};

//***************************************
// Array string
//***************************************

/*
template<size_t N, typename TString=TString<> >
struct TArrayString {
};
*/

//***************************************
// 
//***************************************

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_STRING_ARRAY_H
