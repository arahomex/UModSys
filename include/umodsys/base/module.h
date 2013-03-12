#ifndef __UMODSYS_BASE_MODULE_H
#define __UMODSYS_BASE_MODULE_H 1
/*************************************************************/
// file: umodsys/base/module.h
// info: base module object
/*************************************************************/

#include <umodsys/core/stdcore.h>

namespace UModSys {
namespace base {

//***************************************
// SModuleInfo
//***************************************

struct SModuleInfo {
  DCString name;
  SVersion verno;
  DCString info;
  //
  inline SModuleInfo(const char* n, int vh, int vl, const char *i)
    : name(n), info(i), verno(vh, vl) {}
  inline SModuleInfo(void) {}
};

//***************************************
// IModule
//***************************************

struct IModule
: public core::IRefObject
{
public:
  ~IModule(void);
public:
  virtual const DCString& get_sys_libname(void) const =0;
  virtual const SModuleInfo& get_info(void) const =0;
  virtual bool is_loaded(void) const =0;
  virtual bool load(void);
  virtual bool unload(void);
public:
  UMODSYS_REFOBJECT_INTIMPLEMENT(IModule, 2, IRefObject);
};

//***************************************
// IModuleLoader
//***************************************

struct IModuleLoader
{
public:
  IModule* load_module(const DCString& name, 
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_CONSOLE_H
