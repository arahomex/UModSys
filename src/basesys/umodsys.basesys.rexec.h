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

struct SExecTCL {
  typedef core::BStr StringP;
  typedef core::DCString String;
  typedef core::DStringBufSmall StringName, StringValue;
  typedef core::DStringBuffer StringStream;
  //
  typedef tl::TDynarrayStatic<BChar,0x10000> StringStack;
  typedef tl::TDynarrayStatic<String, 64> Strings;
  typedef tl::TScatterArray<StringName, StringValue> StringMap;
  //
  typedef tl::TParser_TCL<SExecTCL> Parser;
  typedef SExecTCL Self;
  //
  struct SharedState {
    StringMap vars;
    StringStack stack;
    //
    char* top(void) { return stack.end(); }
    size_t left(void) const { return stack.MaxLen(); }
  };
  struct State {
    SharedState &ss;
    size_t top;
    //
    State(SharedState& s)
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
  //
  struct IExecutor {
    virtual bool command(SExecTCL& tcl, const String &cmd, const Strings& args) =0;
  };
  //
  //
  IExecutor *executor;
  SharedState &ss;
  Strings args;
  StringStream stream;
  String result;
  size_t stack_top;
  //
  SExecTCL(SharedState& ass, IExecutor *ee)
  : ss(ass), stream(ass.top(), ass.left()), executor(ee) {
    stack_top = ss.stack.Len();
  }
  //
  bool add_esc(StringP psym, int &idx) {
    switch(*psym) {
      case 'a': add(0x07); return false;
      case 'b': add(0x08); return false;
      case 'f': add(0x0c); return false;
      case 'n': add(0x0a); return false;
      case 'r': add(0x0d); return false;
      case 't': add(0x09); return false;
      case 'v': add(0x0b); return false;
      default: add(*psym); return false;
      //
      case 'o':
        return false;
      case 'x':
        return false;
      case 'u':
        return false;
      case 'U':
        return false;
      case '\n':
        return false;
    }
  }
  //
  void add_var(StringP b, StringP e) { add(var_get(string(b,e))); }
  void add_cmt(StringP b, StringP e) {}
  //
  bool exec_command(Parser& ps) {
    Self c2(ss, executor);
    Parser ps2(ps.p, ps.e, true);
    ps2.Parse(c2);
    ps.p = ps2.p; // sync
    c2.finish();
    if(ps2.token==Parser::tError)
      return false;
    add_result(c2);
  //printf("ret{%s}\n", cs(c2.result));
    return true;
  }
  void execute(void) {
    if(args.size()==0)
      return;
    //
    execute_begin();
//    return;
    //
    if(do_cmd(args[0], args)) {
    } else {
      //add("[?"+args[0]+"]");
    }
    //
    execute_end();
  }
  //
  bool do_cmd(const String &cmd, Strings& args) {
    return executor->command(*this, cmd, args);
  }
  //
  int eval_expr(const String& expr) {
    if(~expr==0)
      return 0;
    int rv;
    if(!string_to_int(expr, rv))
      return -1;
    return rv;
  }
  String eval(const String& code) {
    Self c2(ss, executor);
    Parser ps2(code, code + ~code);
    ps2.Parse(c2);
    c2.finish();
    return new_string(c2.result);
  }
  int eval_live_expr(const String& expr) {
    int rv = eval_expr(eval(expr));
    return rv;
  }
  //
  //
  static String string(StringP b, StringP e) { return String(b, e-b); }
  //
  static bool string_to_int(const String& src, int& dest) {
    StringName buf(src);
    return sscanf(buf, "%d", &dest)==1;
  }
  // static void print_str(const String& src) { printf("%.*s", src.count, src.value); }
  //
  String new_string(const String& src) {
    size_t p = ss.stack.Len();
    if(!ss.stack.ResizeRel(~src+1))
        return NULL;
    tl::su::smemcpy(ss.stack.All()+p, src(), ~src);
    ss.stack[p+~src] = 0;
//printf("{new=%d+%d}", end-ss.stack.begin(), ss.stack.end()-end);
    String rv(ss.stack.All()+p, ~src);
//printf("{new=%p@%d=%.*s}", rv.c_str(), int(~rv), int(~rv), rv.c_str());
    return rv;
  }
  String detach(void) {
    String rv = stream.get_s();
    stream_redo();
//printf("{detach=%p@%d=%.*s}", rv.c_str(), int(~rv), int(~rv), rv.c_str());
    return rv;
  }
  void parse_start() {
    stream_redo();
  }
  //
  void stream_redo(void) {
//printf("{stack=%d/%d}", int(~ss.stack), int(ss.stack.FreeLen()));
    stream.setup(ss.stack.FreeStart(), ss.stack.FreeLen(), 0);
    stream.length = 0;
    stream.maxlength = ss.stack.MaxLen() - ss.stack.Len();
  }
  void ssync(void) {
//printf("{size=%d}", int(~stream));
    // ss.stack.count = stream.end()-ss.stack.begin();
    ss.stack.Resize(~stream + (stream - ss.stack.All()));
//printf("{top=%d}", int(~ss.stack));
  }
  //
  void set_result(const String& src) {
    result = new_string(src);
  }
  void add(char sym) {
//printf("{chr=0x%x}", sym);
    stream.append(&sym, 1);
    ssync();
  }
  void add(StringP b, StringP e) {
//printf("{stream:%d:%d}", int(~stream), int(stream.maxlength));
//printf("{str:%d=%.*s}", int(e-b), int(e-b), b);
    stream.append(b, e-b);
    ssync();
  }
  void add(const String& ss) {
//printf("{str:%d=%.*s}", int(~ss), int(~ss), ss.c_str());
    stream.append(ss, ~ss);
    ssync();
  }
  void next_arg(void) {
//printf("{next}");
    args.Push(detach()); 
  }
  size_t stream_size(void) { return ~stream; }
  //
  String var_get(const String& name) {
    return ss.vars[StringName(name)].str();
  }
  String var_set(const String& name, const String& value) {
//printf("{'"); print_str(name); printf("'='"); print_str(value); printf("'}");
    StringValue& v = ss.vars[StringName(name)];
    v = StringValue(value);
    return v.str();
  }
  void execute_begin(void) {
/*
    printf("(");
    for(int i=0; i<args.size(); i++) {
        printf(" %d:'%.*s'", i, int(~args[i]), args[i].c_str());
    }
    printf(" )\n");
*/
  }
  void execute_end(void) {
    args.Clear();
    ss.stack.Resize(stack_top);
    stream.text = ss.stack.end();
    stream.length = 0;
//printf("{exec-end top=%d}", ss.stack.count);
  }
  void finish(void) {
    ss.stack.Resize(stack_top);
//printf("{finish top=%d}", ss.stack.count);
  }
  void add_result(Self &r) {
    add(r.result.begin(), r.result.end());
  }
  //
  void prints(const String& val);
  void prints(StringP val);
  void printf(StringP val, ...);
};


//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_REXEC_H
