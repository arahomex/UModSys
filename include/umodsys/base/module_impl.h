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
  SModuleInfo minfo;
  int load_count;
public:
  virtual bool open(ISystem* isys) =0;
  virtual bool close(void) =0;
};

//***************************************
// SModuleContext
//***************************************

struct SModuleContext {
protected:
  IModuleInfo *iinfo;
  ISystem* isys;
  IConsole* icon;
  core::IMemAlloc* imem;
public:
  ~SModuleContext(void);
  SModuleContext(void);
  void Link(ISystem* is, IModuleInfo *imi, core::IMemAlloc* privmem);
  //
  inline operator ISystem*(void) const { return isys; }
  inline operator IConsole*(void) const { return icon; }
  inline operator core::IMemAlloc*(void) const { return imem; }
  inline ISystem* operator->(void) const { return isys; }
  inline core::IMemAlloc& operator()(void) const { return *imem; }
  //
  inline IConsole& con(void) const { return *icon; }
  inline IModuleInfo& inf(void) const { return *iinfo; }
  inline const SModuleInfo& info(void) const { return iinfo->minfo; }
  inline core::IMemAlloc& mem(void) const { return *imem; }
  //
};

//***************************************
// END
//***************************************

} // namespace base

extern base::SModuleContext M;

} // namespace UModSys

#endif // __UMODSYS_BASE_MODULE_IMPLEMENTATION_H
