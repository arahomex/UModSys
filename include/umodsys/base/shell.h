#ifndef __UMODSYS_BASE_SHELL_H
#define __UMODSYS_BASE_SHELL_H 1
/*************************************************************/
// file: umodsys/base/shell.h
// info: base shell (TCL-like commands)
/*************************************************************/

#include <umodsys/base/basetypes.h>

namespace UModSys {
namespace base {


//***************************************
// IShell::  - main module functionality
//***************************************

struct IShell : public IRefObject {
  virtual bool connect(void) =0;
  virtual bool disconnect(void) =0;
  virtual bool process_tick(const core::STimeMsec& delta) =0;
  virtual bool process_command(int argc, const core::DCString argv[]) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IShell, 2, IRefObject);
};

//***************************************
// IExecutor::  - TCL-like shell executor
//***************************************

struct IExecutorPure {
  virtual bool get_var(const core::DCString& name, core::DCString& value) const =0;
  virtual bool get_shell(const core::DCString& name, IShell::P& shell) const =0;
  //
  virtual bool execute(const core::DCString& text) =0;
  virtual bool set_var(const core::DCString& name, const core::DCString& value, bool can_new=true) =0;
  virtual bool set_shell(const core::DCString& name, IShell* shell) =0;
};

struct IExecutor : public IRefObject, public IExecutorPure {
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IExecutor, 2, IRefObject);
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_SHELL_H
