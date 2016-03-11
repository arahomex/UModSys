#ifndef __UMODSYS_BASE_SHELL_H
#define __UMODSYS_BASE_SHELL_H 1
/*************************************************************/
// file: umodsys/base/shell.h
// info: base shell (TCL-like commands)
/*************************************************************/

#include <umodsys/base/basetypes.h>
#include <umodsys/tl/parsers/parser_tcl.h>

namespace UModSys {
namespace base {

    

//***************************************
// IExecTCL::  - main module functionality
//***************************************
  
struct IExecTCL {
  typedef core::BStr StringP;
  typedef core::DCString String;
  typedef tl::TParser_TCL<IExecTCL> Parser;
  typedef IExecTCL Self;
  typedef core::HUniquePointer TypeId;
  //
  struct IExecutor {
    virtual bool tcl_command(IExecTCL& tcl, size_t argc, const String argv[]) =0;
    virtual bool tcl_getvar(IExecTCL& tcl, const String& name, String& value) =0;
    virtual bool tcl_setvar(IExecTCL& tcl, const String& name, const String& value) =0;
  };
  //
  virtual const IExecTCL* get_other(TypeId type) const =0;
  virtual IExecTCL* get_other(TypeId type) =0;
  //
  virtual void parse_start(void) = 0;
  virtual void add(char sym) =0;
  virtual void add(StringP b, StringP e) =0;
  virtual void add(const String& ss) =0;
  virtual bool add_esc(StringP psym, int &idx) =0;
  virtual void add_var(StringP b, StringP e) =0;
  virtual void add_cmt(StringP b, StringP e) =0;
  virtual void next_arg(void) =0;
  virtual size_t stream_size(void) =0;
  virtual bool execute(void) =0;
  virtual bool exec_command(Parser& ps) =0;
  //
  virtual int get_error(void) const =0;
  virtual String get_error_text(void) const =0;
  virtual void set_error(int err, const String &text) =0;
  virtual IExecTCL* get_up(void) const =0;
  virtual size_t get_executor_count(void) const =0;
  virtual IExecutor* get_executor(size_t id) const =0;
  virtual String var_get(const String& name) =0;
  virtual String var_set(const String& name, const String& value) = 0;
  virtual void set_result(const String& src) =0;
  virtual void print_s(const String& val) =0;
  virtual void print_s(StringP val) =0;
  virtual void print_f(StringP val, ...) =0;
};

//***************************************
// IShell::  - main module functionality
//***************************************

struct IShell : public IRefObject, public IExecTCL::IExecutor {
public:
  virtual bool connect(void) =0;
  virtual bool disconnect(void) =0;
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
  virtual bool execute_text(const core::DCString& text) =0;
  virtual bool execute_command(const DCString args[], size_t len) =0;
  virtual bool set_var(const core::DCString& name, const core::DCString& value, bool can_new=true) =0;
  virtual bool set_shell(const core::DCString& name, IShell* shell) =0;
  //
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
