#ifndef __UMODSYS_BASE_MODULEREF_H
#define __UMODSYS_BASE_MODULEREF_H 1
/*************************************************************/
// file: umodsys/base/module.h
// info: base module object
/*************************************************************/

#include <umodsys/stdcommon.h>

namespace UModSys {
namespace modules {


} // namespace modules
} // namespace UModSys

#define UMODSYS_MODULE_BEGIN0(_modulegroup, _modulename) \
  namespace UModSys { namespace modules { \
  namespace _modulegroup { namespace _modulename { \
  extern char fmodule;

#define UMODSYS_MODULE_BEGIN(_modulegroup, _modulename) \
  UMODSYS_MODULE_BEGIN0(_modulegroup, _modulename) \
  extern UModSys::base::SDebug s_dbg;

#define UMODSYS_MODULE_START(_modulegroup, _modulename) \
  UMODSYS_MODULE_BEGIN(_modulegroup, _modulename) \
  UMODSYS_MODULE_BODY0()

#define UMODSYS_MODULE_END() \
  } } } }

#define UMODSYS_MODULE_DEF(_modulegroup, _modulename) \
  UMODSYS_MODULE_BEGIN0(_modulegroup, _modulename) UMODSYS_MODULE_END()

#define UMODSYS_MODULE_NAME(_modulegroup, _modulename) UModSys::modules::_modulegroup::_modulename
  
#define UMODSYS_MODULE_BODY(_modulegroup, _modulename) \
  char ::UMODSYS_MODULE_NAME(_modulegroup, _modulename)::fmodule = 0;

#define UMODSYS_MODULE_BODY0() \
  char fmodule = 0; \
  UModSys::base::SDebug s_dbg;

#define UMODSYS_MODULE_USE(_modulegroup, _modulename) \
  ::UModSys::core::lost(&::UMODSYS_MODULE_NAME(_modulegroup, _modulename)::fmodule)

#endif // __UMODSYS_BASE_MODULEREF_H
