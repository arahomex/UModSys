#ifndef __UMODSYS_BASE_REXEC_H
#define __UMODSYS_BASE_REXEC_H 1
/*************************************************************/
// file: src/base/system/umodsys.base.rexec.h
// info:
/*************************************************************/

#include <umodsys/tl/parsers/parser_tcl.h>
#include <umodsys/tl/container/dynarray.h>

#include "umodsys.basesys.rcommon.h"


namespace UModSys {
namespace base {
namespace rsystem {

using namespace core;

//***************************************
// SExecTCL
//***************************************

struct SExecTCL : IExecTCL {
  typedef core::DStringBufSmall StringName, StringValue;
  typedef core::DStringBuffer StringStream;
  //
  typedef tl::TDynarrayStatic<BChar, 0x10000> StringStack;
  typedef tl::TDynarrayStatic<String, 256> Strings;
  typedef tl::TScatterArray<StringName, StringValue> StringMap;
  //
  typedef SExecTCL Self;
  //
  struct Thread;
  struct State;
  //
  struct FailedContext;
  struct Context;
  struct Range;
  struct ControlTCL;
  //
  //
  IExecutor **executors;
  size_t executors_len;
  IExecTCL *up;
  Thread &ss;
  Strings args;
  StringStream stream;
  String result;
  size_t stack_top;
  int error_code;
  StringValue error_text;
  //
  SExecTCL(Thread& ass, IExecutor* ee[], size_t nee, IExecTCL *u=NULL);
  SExecTCL(Thread& ass, IExecTCL *u);
  ~SExecTCL(void);
  //
  inline static TypeId get_tinfo(void) { return tl::TObjectUniqueID<SExecTCL>::get_id(); }
  inline static const char* _root_get_interface_infoname(void) { return "SExecTCL"; }
  inline static int _root_get_interface_infover(void) { return 1; }
  inline static String string(StringP b, StringP e) { return String(b, e-b); }
  //
  static bool string_to_int(const String& src, int& dest);
  //
  int get_error(void) const;
  String get_error_text(void) const;
  void set_error(int err, const String &text);
  IExecTCL* get_up(void) const;
  IExecutor* get_executor(size_t id) const;
  size_t get_executor_count(void) const;
  //
  const IExecTCL* get_other(TypeId type) const;
  IExecTCL* get_other(TypeId type);
  //
  bool add_esc(StringP psym, int &idx);
  void add_var(StringP b, StringP e);
  void add_cmt(StringP b, StringP e);
  //
  bool exec_command(Parser& ps);
  bool execute(void);
  //
  int eval_expr(const String& expr);
  String eval(const String& code);
  String eval(const String& code, IExecutor *ex2);
  int eval_live_expr(const String& expr);
  //
  String new_string(const String& src);
  String detach(void);
  void parse_start();
  void stream_redo(void);
  void ssync(void);
  //
  void set_result(const String& src);
  void add(char sym);
  void add(StringP b, StringP e);
  void add(const String& ss);
  void next_arg(void);
  size_t stream_size(void);
  //
  bool tcl_cmd(Strings& args);
  String var_get(const String& name);
  String var_set(const String& name, const String& value);
  //
  void eval_check(int ec, const String& txt);
  void execute_begin(void);
  void execute_end(void);
  void finish(void);
  void add_result(Self &r);
  //
  void print_s(const String& val);
  void print_s(StringP val);
  void print_f(StringP val, ...);
};

//***************************************
// SExecTCL::Thread
//***************************************

struct SExecTCL::Thread {
  StringStack stack;
  //
  char* top(void) { return stack.end(); }
  size_t left(void) const { return stack.MaxLen(); }
};

//***************************************
// SExecTCL::State
//***************************************

struct SExecTCL::State {
  Thread &ss;
  size_t top;
  //
  State(Thread& s)
  : ss(s) {
    top = ~ss.stack;
  }
  ~State(void) {
    reset();
  }
  void reset(void) {
    ss.stack.resize(top);
  }
};

//***************************************
// SExecTCL::FailedContext
//***************************************

struct SExecTCL::FailedContext : IExecutor {
  //
  bool tcl_getvar(IExecTCL& tcl, const String& name, String& value);
  bool tcl_setvar(IExecTCL& tcl, const String& name, const String& value);
  bool tcl_command(IExecTCL& tcl, size_t argc, const String argv[]);
  //
  //
  FailedContext(void);
  ~FailedContext(void);
};

//***************************************
// SExecTCL::Context
//***************************************

struct SExecTCL::Context : IExecutor {
  StringMap vars;
  //
  bool tcl_getvar(IExecTCL& tcl, const String& name, String& value);
  bool tcl_setvar(IExecTCL& tcl, const String& name, const String& value);
  bool tcl_command(IExecTCL& tcl, size_t argc, const String argv[]);
  //
  //
  Context(void);
  ~Context(void);
};

//***************************************
// SExecTCL::Range
//***************************************

struct SExecTCL::Range : IExecutor {
  int begin, end, step, cur;
  char tmp[64];
  //
  bool tcl_getvar(IExecTCL& tcl, const String& name, String& value);
  bool tcl_setvar(IExecTCL& tcl, const String& name, const String& value);
  bool tcl_command(IExecTCL& tcl, size_t argc, const String argv[]);
  //
  //
  Range(int a, int b, int c);
  ~Range(void);
  //
  void start(void) { cur = begin; }
  void operator++(void) { cur += step; }
  void operator++(int) { cur += step; }
  bool valid(void) { return cur<end; }
};

//***************************************
// SExecTCL::ControlTCL
//***************************************

struct SExecTCL::ControlTCL : IExecutor {
  //
  bool tcl_getvar(IExecTCL& tcl, const String& name, String& value);
  bool tcl_setvar(IExecTCL& tcl, const String& name, const String& value);
  bool tcl_command(IExecTCL& tcl, size_t argc, const String argv[]);
  //
  ControlTCL(void);
  ~ControlTCL(void);
};

//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_REXEC_H
