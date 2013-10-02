#ifndef __UMODSYS_BASE_SYSTEM_H
#define __UMODSYS_BASE_SYSTEM_H 1
/*************************************************************/
// file: umodsys/base/system.h
// info: base system object
/*************************************************************/

#include <umodsys/core/stdcore.h>

#include <umodsys/base/console.h>
#include <umodsys/base/module.h>

namespace UModSys {
namespace base {

//***************************************
// ISystem
//***************************************

struct ISystem : public core::IRoot
{
public:
  ~ISystem(void);
public:
  virtual core::IUniquePointerResolver* get_upr(void) =0;
  virtual core::ISourceContextAdapter* get_sca(void) =0;
  //
  virtual IModuleLoader* get_modloader(void) =0;
  virtual IModuleLibrary* get_syslib(void) = 0;
  //
  virtual core::IMemAlloc* get_sysmem(void) =0;
  virtual core::IMemAlloc* get_sharemem(void) =0;
  virtual IConsole* get_console(void) =0;
  virtual core::IParameters* get_params(void) =0;
public:
  UMODSYS_ROOT_IMPLEMENT1(UModSys::base::ISystem, 3, IRoot);
};

//***************************************
// SModuleContext
//***************************************

struct SModuleContext {
protected:
  ISystem* isys;
  IConsole* icon;
  core::IMemAlloc* imem;
public:
  ~SModuleContext(void);
  SModuleContext(void);
  void Link(ISystem* is, core::IMemAlloc* privmem);
  //
  inline operator ISystem*(void) const { return isys; }
  inline operator IConsole*(void) const { return icon; }
  inline operator core::IMemAlloc*(void) const { return imem; }
  inline ISystem* operator->(void) const { return isys; }
  inline core::IMemAlloc& operator()(void) const { return *imem; }
  inline ISystem* operator*(void) const { return isys; }
  //
  inline ISystem& sys(void) const { return *isys; }
  inline IConsole& con(void) const { return *icon; }
  inline core::IMemAlloc& mem(void) const { return *imem; }
  //
public:
  void dbg_put(const char *fmt, ...);
};

//***************************************
// END
//***************************************

} // namespace base

//***************************************
// 
//***************************************

extern base::SModuleContext M;

//***************************************
// core::SIMemAlloc::
//***************************************

inline core::SIMemAlloc::SIMemAlloc(void) 
: imem(M) {
}

//***************************************
// END
//***************************************

} // namespace UModSys

#endif // __UMODSYS_BASE_SYSTEM_H
