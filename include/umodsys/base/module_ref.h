#ifndef __UMODSYS_BASE_MODULEREF_H
#define __UMODSYS_BASE_MODULEREF_H 1
/*************************************************************/
// file: umodsys/base/module.h
// info: base module object
/*************************************************************/

#include <umodsys/stdcore.h>

namespace UModSys {
namespace modules {


} // namespace modules
} // namespace UModSys

#define UMODSYS_DEFINE_MODULE(modulename) \
  namespace UModSys { namespace modules { extern char module_ ## modulename; } }
  
#define UMODSYS_BODY_MODULE(modulename) \
  char ::UModSys::modules::module_ ## modulename = 0;

#define UMODSYS_USE_MODULE(modulename) \
  ::UModSys::core::lost(&::UModSys::modules::module_ ## modulename)

#endif // __UMODSYS_BASE_MODULEREF_H
