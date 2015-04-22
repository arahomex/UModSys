#ifndef __UMODSYS_BASE_REXEC_H
#define __UMODSYS_BASE_REXEC_H 1
/*************************************************************/
// file: src/base/system/umodsys.base.rexec.h
// info:
/*************************************************************/

#include <umodsys/tl/parsers/parser_tcl.h>
#include <umodsys/tl/composite/dynarray.h>

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
  SharedState &ss;
  Strings args;
  StringStream stream;
  String result;
  size_t stack_top;
  //
  SExecTCL(SharedState& ass)
  : ss(ass), stream(ass.top(), ass.left()) {
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
    Self c2(ss);
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
    //
    if(do_cmd(args[0], args)) {
    } else {
      //add("[?"+args[0]+"]");
    }
    //
    execute_end();
  }
  bool do_cmd(const String &cmd, Strings& args);
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
    Self c2(ss);
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
    return String(ss.stack.All()+p, ~src);
  }
  String detach(void) {
    String rv = stream.get_s();
    stream.setup(ss.stack.All(), ss.stack.FreeLen(), 0);
    stream.length = 0;
    stream.maxlength = ss.stack.MaxLen() - ss.stack.Len();
    return rv;
  }
  void ssync(void) {
    // ss.stack.count = stream.end()-ss.stack.begin();
    ss.stack.Resize(~stream);
//printf("{top=%d}", ss.stack.count);
  }
  //
  void set_result(const String& src) {
    result = new_string(src);
  }
  void add(char sym) {
    stream.append(&sym, 1);
    ssync();
  }
  void add(StringP b, StringP e) {
    stream.append(b, e-b);
    ssync();
  }
  void add(const String& ss) {
    stream.append(ss, ~ss);
    ssync();
  }
  void next_arg(void) { args.Push(detach()); }
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
        printf(" %d:'", i);
        print_str(args[i]);
        printf("'");
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
};


//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_REXEC_H