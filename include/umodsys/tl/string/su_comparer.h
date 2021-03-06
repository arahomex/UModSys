#ifndef __UMODSYS_TYPES_STRING_UTILS_COMPARER_H
#define __UMODSYS_TYPES_STRING_UTILS_COMPARER_H 1
/*************************************************************/
// file: umodsys/tl/string/su_comparer.h
// info: string utilities - comparission
/*************************************************************/

#include <umodsys/tl/string/su_base.h>

namespace UModSys {
namespace tl {
namespace su {

//***************************************
// Comparer binary

template<typename CharT>
struct TComparerBinary : public core::Void {
  //
  TComparerBinary(void) {}
  TComparerBinary(core::Void* r) {}
  //
  inline static void clear(void) {}
  inline static void update(const CharT* a) {}
  inline static void update(const CharT* a, size_t len) {}
  inline static void update(const CharT* a, size_t* len) {}
  //
  inline static bool same(const TComparerBinary& R) { return true; }
  //
  inline static int cmp1(const CharT* mys, size_t myl, const CharT* r) { return su::scmp(mys, myl, r); }
  inline static bool eq1(const CharT* mys, size_t myl, const CharT* r) { return su::seq(mys, myl, r); }
  //
  inline static int cmp2(const CharT* mys, size_t myl, const CharT* r, size_t rl) {
    int rv = core::scalar_compare(myl, rl);
    return rv ? rv : su::scmp(mys, r, myl);
  }
  inline static bool eq2(const CharT* mys, size_t myl, const CharT* r, size_t rl) {
    return myl==rl && su::scmp(mys, r, myl)==0;
  }
  //
  inline static int cmp3(const CharT* mys, size_t myl, const CharT* r, size_t rl, const core::Void& rh) {
    return cmp(mys, myl, r, rl);
  }
  inline static bool eq3(const CharT* mys, size_t myl, const CharT* r, size_t rl, const core::Void& rh) {
    return eq(mys, myl, r, rl);
  }
  //
  inline static bool starts(const CharT* mys, size_t myl, const CharT* r, size_t rl) {
    if(myl<rl) return false;
    return su::scmp(mys, r, rl)==0;
  }
};

//***************************************
// Comparer hash

struct SComparerHash {
  size_t hash;
  //
  inline SComparerHash(void) : hash(0) {}
  inline SComparerHash(core::Void* p) {}
};

//***************************************
// Comparer binary hash

template<typename CharT>
struct TComparerBinaryHash : public SComparerHash {
  //
  TComparerBinaryHash(void) {}
  TComparerBinaryHash(core::Void* r) : SComparerHash(r) {}
  //
  inline void clear(void) { hash=0; }
  inline void update(const CharT* a) { hash = su::shash(a); }
  inline void update(const CharT* a, size_t len) { hash = su::shash(a, len); }
  inline void update(const CharT* a, size_t* len) { hash = su::shashlen(a, *len); }
  //
  inline static bool same(const TComparerBinaryHash& R) { return true; }
  //
  inline static int cmp1(const CharT* mys, size_t myl, const CharT* r) { return su::scmp(mys, myl, r); }
  inline static bool eq1(const CharT* mys, size_t myl, const CharT* r) { return su::seq(mys, myl, r); }
  //
  inline static int cmp2(const CharT* mys, size_t myl, const CharT* r, size_t rl) {
    int rv = core::scalar_compare(myl, rl);
    return rv ? rv : su::scmp(mys, r, myl);
  }
  inline static bool eq2(const CharT* mys, size_t myl, const CharT* r, size_t rl) {
    return myl==rl && su::scmp(mys, r, myl)==0;
  }
  //
  inline int cmp3(const CharT* mys, size_t myl, const CharT* r, size_t rl, size_t rh) const {
    int rv = core::scalar_compare(myl, rl);
    if(rv) return rv;
    rv = core::scalar_compare(hash, rh);
    return rv ? rv : su::scmp(mys, r, myl);
  }
  inline bool eq3(const CharT* mys, size_t myl, const CharT* r, size_t rl, size_t rh) const {
    return myl==rl && hash==rh && su::scmp(mys, r, myl)==0;
  }
  inline int cmp3(const CharT* mys, size_t myl, const CharT* r, size_t rl, const SComparerHash& rh) const {
    int rv = core::scalar_compare(myl, rl);
    if(rv) return rv;
    rv = core::scalar_compare(hash, rh.hash);
    return rv ? rv : su::scmp(mys, r, myl);
  }
  inline bool eq3(const CharT* mys, size_t myl, const CharT* r, size_t rl, const SComparerHash& rh) const {
    return myl==rl && hash==rh.hash && su::scmp(mys, r, myl)==0;
  }
  //
  inline static bool starts(const CharT* mys, size_t myl, const CharT* r, size_t rl) {
    if(myl<rl) return false;
    return su::scmp(mys, r, rl)==0;
  }
};

//***************************************
// Comparer utf nocase hash

template<typename CharT>
struct TComparerUtfNocaseHash : public SComparerHash {
  //
  TComparerUtfNocaseHash(void) {}
  TComparerUtfNocaseHash(core::Void* r) : SComparerHash(r) {}
  //
  inline void clear(void) { hash=0; }
  inline void update(const CharT* a) { hash = su::utf_shash_nocase(a); }
  inline void update(const CharT* a, size_t len) { hash = su::utf_shash_nocase(a, len); }
  inline void update(const CharT* a, size_t* len) { hash = su::utf_shash_nocase(a, len); }
  //
  inline static bool same(const TComparerUtfNocaseHash& R) { return true; }
  //
  inline static int cmp1(const CharT* mys, size_t myl, const CharT* r) { return su::utf_cmp_nocase(mys, myl, r); }
  inline static bool eq1(const CharT* mys, size_t myl, const CharT* r) { return su::utf_cmp_nocase(mys, myl, r)==0; }
  //
  inline static int cmp2(const CharT* mys, size_t myl, const CharT* r, size_t rl) {
    int rv = core::scalar_compare(myl, rl);
    return rv ? rv : su::utf_cmp_nocase(mys, r, myl);
  }
  inline static bool eq2(const CharT* mys, size_t myl, const CharT* r, size_t rl) {
    return myl==rl && su::utf_cmp_nocase(mys, r, myl)==0;
  }
  //
  inline int cmp3(const CharT* mys, size_t myl, const CharT* r, size_t rl, size_t rh) const {
    int rv = core::scalar_compare(myl, rl);
    if(rv) return rv;
    rv = core::scalar_compare(hash, rh);
    return rv ? rv : su::utf_cmp_nocase(mys, r, myl);
  }
  inline bool eq3(const CharT* mys, size_t myl, const CharT* r, size_t rl, size_t rh) const {
    return myl==rl && hash==rh && su::scmp(mys, r, myl)==0;
  }
  inline int cmp3(const CharT* mys, size_t myl, const CharT* r, size_t rl, const SComparerHash& rh) const {
    int rv = core::scalar_compare(myl, rl);
    if(rv) return rv;
    rv = core::scalar_compare(hash, rh.hash);
    return rv ? rv : su::utf_cmp_nocase(mys, r, myl);
  }
  inline bool eq3(const CharT* mys, size_t myl, const CharT* r, size_t rl, const SComparerHash& rh) const {
    return myl==rl && hash==rh.hash && su::utf_cmp_nocase(mys, r, myl)==0;
  }
  //
  inline static bool starts(const CharT* mys, size_t myl, const CharT* r, size_t rl) {
    if(myl<rl) return false;
    return su::utf_cmp_nocase(mys, r, rl)==0;
  }
};

//***************************************
// end

} // namespace su
} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TYPES_STRING_UTILS_COMPARER_H
