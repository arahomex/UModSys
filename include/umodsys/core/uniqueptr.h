#ifndef __UMODSYS_CORE_UNIQUEPTR_H
#define __UMODSYS_CORE_UNIQUEPTR_H 1
/*************************************************************/
// file: umodsys/core/uniqueptr.h
// info: unique pointer used for type/module comparission
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>
#include <umodsys/tl/composite/list2.h>

namespace UModSys {
namespace core {

//***************************************
// Unique Pointer Info
//***************************************

struct SUniquePointerInfo {
public:
  inline SUniquePointerInfo(const char *g, const char *n, int vn) : group(g), name(n), verno(vn) {}
  inline explicit SUniquePointerInfo(Void *p) {}
  //
  inline bool operator==(const SUniquePointerInfo& R) const 
    { return group==R.group && name==R.name && verno==R.verno; }
public:
  const char *group;
  const char *name;
  int verno;
};

//***************************************
// Unique Pointer Resolver
//***************************************

struct IUniquePointerResolver {
  virtual HUniquePointer upi_add(const SUniquePointerInfo* lupi) =0;
  virtual int upi_remove(HUniquePointer upi) =0;
};

//***************************************
// Unique Pointer
//***************************************

struct SUniquePointer : public tl::TList2Node<SUniquePointer> {
public:
  SUniquePointer(const char *g, const char *n, int vn);
  explicit SUniquePointer(int v);
  ~SUniquePointer(void);
  //
  static void s_resolve(IUniquePointerResolver* res);
  static void s_unresolve(IUniquePointerResolver* res);
  //
  operator HUniquePointer(void) const { return upi; }
  HUniquePointer operator()(void) const { return upi; }
  HUniquePointer operator*(void) const { return upi; }
  HUniquePointer get(void) const { return upi; }
public:
  SUniquePointerInfo info;
  mutable HUniquePointer upi;
  //
  static SUniquePointer root;
};

//***************************************
// Unique Pointer List
//***************************************

struct SUniquePtrList {
public:
  SUniquePtrList(HUniquePointer *a, size_t n);
  //
  inline size_t len(void) const { return curlen; }
  inline size_t operator~(void) const { return curlen; }
  inline HUniquePointer get(size_t n) const { return arr[n]; }
  inline HUniquePointer operator[](size_t n) const { return arr[n]; }
  inline HUniquePointer operator()(size_t n) const { return arr[n]; }
  //
  void clear(void) { curlen=0; }
  bool add(HUniquePointer val);
  inline SUniquePtrList& operator<<(HUniquePointer val) { add(val); return *this; }
  size_t find(HUniquePointer val) const;
  inline size_t operator()(HUniquePointer vv) const { return find(vv); }
public:
  HUniquePointer *arr;
  size_t curlen, maxlen;
};

//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_UNIQUEPTR_H
