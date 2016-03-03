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
  typedef tl::TDynarrayStatic<BChar, 0x10000> StringStack;
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
    virtual bool tcl_command(SExecTCL& tcl, const Strings& args) =0;
    virtual bool tcl_getvar(SExecTCL& tcl, const String& name, String& value) =0;
    virtual bool tcl_setvar(SExecTCL& tcl, const String& name, const String& value) =0;
  };
  //
  //
  struct Range : IExecutor {
    IExecutor *chain;
    int begin, end, step, cur;
    char tmp[64];
    //
    bool tcl_command(SExecTCL& tcl, const Strings& args) {
      return chain ? chain->tcl_command(tcl, args) : false;
    }
    bool tcl_getvar(SExecTCL& tcl, const String& name, String& value) {
      if(name=="value") {
        sprintf(tmp, "%d", cur);
        value = tmp;
        return true;
      }
      return chain ? chain->tcl_getvar(tcl, name, value) : false;
    }
    bool tcl_setvar(SExecTCL& tcl, const String& name, const String& value) {
      return chain ? chain->tcl_setvar(tcl, name, value) : false;
    }
    //
    Range(IExecutor* n, int a, int b, int c) : chain(n), begin(a), end(c), step(b) { start(); }
    //
    void start(void) { cur = begin; }
    void operator++(void) { cur += step; }
    void operator++(int) { cur += step; }
    bool valid(void) { return cur<end; }
    //
  };
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
    if(ps2.token>=Parser::tError)
      return false;
    add_result(c2);
//printf("ret{%.*s}\n", int(~c2.result), *c2.result);
    return true;
  }
  bool execute(void) {
    if(args.size()==0)
      return true;
    execute_begin();
    bool rv = do_cmd(args);
    execute_end();
    return rv;
  }
  //
  bool do_cmd(Strings& args) {
    return executor->tcl_command(*this, args);
  }
  //
  int eval_expr(const String& expr) {
//printf("?expr{%.*s}", int(~expr), *expr);
    if(~expr==0)
      return 0;
    int rv;
    if(!string_to_int(expr, rv))
      return -1;
//printf("={%d}", rv);
    return rv;
  }
  String eval(const String& code) {
    Self c2(ss, executor);
    Parser ps2(*code, code + ~code);
    ps2.Parse(c2);
    c2.finish();
    return new_string(c2.result);
  }
  String eval(const String& code, IExecutor *ex2) {
    Self c2(ss, ex2);
    Parser ps2(*code, code + ~code);
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
    return sscanf(*buf, "%d", &dest)==1;
  }
  // static void print_str(const String& src) { printf("%.*s", src.count, src.value); }
  //
  String new_string(const String& src) {
    size_t p = ss.stack.Len();
    if(!ss.stack.ResizeRel(~src+1))
        return String();
    tl::su::smemcpy(ss.stack.All()+p, *src, ~src);
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
    ss.stack.Resize(~stream + (stream.get_text() - ss.stack.All()));
//printf("{top=%d}", int(~ss.stack));
  }
  //
  void set_result(const String& src) {
    result = new_string(src);
//printf("set_result{%.*s}\n", int(~result), *result);
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
    stream.append(*ss, ~ss);
    ssync();
  }
  void next_arg(void) {
//printf("{next}");
    args.Push(detach()); 
  }
  size_t stream_size(void) { return ~stream; }
  //
  String var_get(const String& name) {
    String rvx;
    if(executor->tcl_getvar(*this, name, rvx))
      return rvx;
    StringName key(name);
    const StringValue* value = ss.vars(key);
    if(value!=NULL) {
      String rv(*value);
//printf("{found '"); prints(name); printf("' = '"); prints(value->get_text()); printf("' %p/%p}", value->get_text(), rv.get_text());
      return rv;
    }
//printf("{Not found '"); prints(name); printf("'}");
    return String();
  }
  String var_set(const String& name, const String& value) {
    if(executor->tcl_setvar(*this, name, value))
      return value;
    StringName key(name);
    StringValue& v = ss.vars[key];
    v = StringValue(value);
//printf("{'"); prints(key); printf("'='"); prints(v); printf("'}");
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
//printf("add_result{%.*s}\n", int(~r.result), *r.result);
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
