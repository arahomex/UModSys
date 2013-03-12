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
// Unique Pointer
//***************************************

struct SUniquePointerInfo {
  const char *group;
  const char *name;
  int verno;
  //
  inline SUniquePointerInfo(const char *g, const char *n, int vn) : group(g), name(n), verno(vn) {}
  inline explicit SUniquePointerInfo(Void *p) {}
};

struct IUniquePointerResolver {
  virtual SUniquePointerInfo* upi_add(const SUniquePointerInfo* lupi) =0;
  virtual int upi_remove(SUniquePointerInfo* upi) =0;
};

struct SUniquePointer : public tl::TList2Node<SUniquePointer> {
public:
  SUniquePointerInfo info;
  mutable SUniquePointerInfo* upi;
  //
  static SUniquePointer root;
  //
  SUniquePointer(const char *g, const char *n, int vn);
  explicit SUniquePointer(int v);
  ~SUniquePointer(void);
  //
  static void s_resolve(IUniquePointerResolver* res);
  static void s_unresolve(IUniquePointerResolver* res);
  //
  operator SUniquePointerInfo*(void) const { return upi; }
  SUniquePointerInfo* operator()(void) const { return upi; }
  SUniquePointerInfo* operator*(void) const { return upi; }
  SUniquePointerInfo* get(void) const { return upi; }
};

//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_UNIQUEPTR_H
