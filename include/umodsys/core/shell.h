#ifndef __UMODSYS_CORE_SHELL_H
#define __UMODSYS_CORE_SHELL_H 1
/*************************************************************/
// file: umodsys/core/shell.h
// info: base shell (TCL-like commands)
/*************************************************************/

#include <umodsys/core/basetypes.h>
#include <umodsys/tl/codec/tcl_parse.h>

namespace UModSys {
namespace base {


//***************************************
// IExecTCL::  - main module functionality
//***************************************

struct IExecTCL {
  typedef core::BChar Char;
  typedef core::BStr StringP;
  typedef core::DCString String;
  typedef core::DStringBuffer StringStream;
  //
  typedef tl::TParser_TCL<IExecTCL> Parser;
  typedef IExecTCL Self;
  //
  enum eStatus {
    sFalse       =0,   // ok, no result
    sTrue        =1,   // ok, got result
    sError       =-1,  // failed, error
  };
  //
  inline static bool sFailed(eStatus s) { return s<0; }
  inline static bool sOk(eStatus s) { return s>=0; }
  inline static bool sResult(eStatus s) { return s>0; }
  inline static bool sHave(eStatus s) { return s!=0; }
  //
  struct IThread {
    virtual size_t stack_top(void) const = 0;
    virtual size_t stack_maxtop(void) const = 0;
    virtual Char* stack_value(void) = 0;
    virtual void stack_movetop(size_t top) = 0;
    //
    inline bool stack_stream(StringStream &stream) { // get all free space
      char* s = stack_value();
      size_t top = stack_top(), max = stack_maxtop();
      stream.setup(s+top, max-top, 0);
      return stream.maxlength>0;
    }
    inline Char* stack_add(size_t sz) {
      size_t top = stack_top(), max = stack_maxtop();
      if(max-top<sz)
        return NULL;
      stack_movetop(top+sz);
      return stack_value()+top;
    }
    inline Char* stack_add(size_t sz, StringP src) {
      size_t top = stack_top(), max = stack_maxtop();
      if(max-top<sz)
        return NULL;
      stack_movetop(top+sz);
      Char* rv = stack_value()+top;
      memmove(rv, src, sz);
      return rv;
    }
    inline Char* stack_addz(size_t sz, StringP src) {
      size_t top = stack_top(), max = stack_maxtop();
      if(max-top<=sz)
        return NULL;
      stack_movetop(top+sz+1);
      Char* rv = stack_value()+top;
      memmove(rv, src, sz);
      rv[sz] = 0;
      return rv;
    }
    //
    virtual int get_error(void) const =0;
    virtual String get_error_text(void) const =0;
    virtual void set_error(int err, const String &text) =0;
    //
    virtual void print_s(const String& val) =0;
    virtual void print_s(StringP val) =0;
    virtual void print_f(StringP val, ...) =0;
  };
  //
  struct ThreadState {
    IThread *ss;
    size_t mark;
    //
    inline ThreadState(IThread& s) : ss(&s), mark(s.stack_top()) {}
    inline ThreadState(IThread* s) : ss(s), mark(s->stack_top()) {}
    inline ~ThreadState(void) { reset(); }
    inline void reset(void) { ss->stack_movetop(mark); }
    inline IThread* operator->(void) const { return ss; }
    inline IThread& operator*(void) const { return *ss; }
    inline operator IThread*(void) const { return ss; }
    inline operator IThread&(void) const { return *ss; }
    inline void re(StringStream &s) { s.setup(ss->stack_value()+mark, ss->stack_maxtop()-mark, 0); }
    inline void next(StringStream &s) { ss->stack_stream(s); }
  };
  //
  struct IExecutor {
    virtual eStatus tcl_command(IExecTCL& tcl, size_t argc, const String argv[]) =0;
    virtual eStatus tcl_getvar(IExecTCL& tcl, const String& name, String& value) =0;
    virtual eStatus tcl_setvar(IExecTCL& tcl, const String& name, const String& value) =0;
  };
  //
//  virtual const IExecTCL* get_other(TypeId type) const =0;
//  virtual IExecTCL* get_other(TypeId type) =0;
  //
  virtual IThread* get_thread(void) const =0;
  virtual IExecTCL* get_up(void) const =0;
  virtual size_t get_executor_count(void) const =0;
  virtual IExecutor* get_executor(size_t id) const =0;
  virtual String var_get(const String& name) =0;
  virtual String var_set(const String& name, const String& value) = 0;
  virtual void set_result(const String& src) =0;
  //
  virtual void next_token(int token) = 0;
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
  virtual int eval_expr(const String& expr) =0;
  virtual String eval(const String& code) =0;
  virtual String eval(const String& code, IExecutor *ex2) =0;
  virtual int eval_live_expr(const String& expr) =0;
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

#endif // __UMODSYS_CORE_SHELL_H
