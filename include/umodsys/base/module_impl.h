#ifndef __UMODSYS_BASE_MODULE_IMPLEMENTATION_H
#define __UMODSYS_BASE_MODULE_IMPLEMENTATION_H 1
/*************************************************************/
// file: umodsys/base/module_impl.h
// info: base module object implementation
/*************************************************************/

#include <umodsys/core/stdcore.h>
#include <umodsys/base/system.h>

namespace UModSys {
namespace base {

//***************************************
// IModuleInfo
//***************************************

struct IModuleInfo {
public:
  int load_count;
public:
  virtual bool open(ISystem* isys) =0;
  virtual bool close(void) =0;
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_MODULE_IMPLEMENTATION_H
