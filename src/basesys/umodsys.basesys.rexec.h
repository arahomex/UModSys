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
  typedef tl::TDynarrayStatic<String, 64> Strings;
  typedef tl::TScatterArray<StringName, StringValue> StringMap;
  //
//  typedef tl::TParser_TCL<SExecTCL> Parser;
  typedef SExecTCL Self;
  //
  struct Thread {
    StringStack stack;
    //
    char* top(void) { return stack.end(); }
    size_t left(void) const { return stack.MaxLen(); }
  };
  struct State {
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
  //
  struct FailedContext : IExecutor {
    //
    bool tcl_getvar(IExecTCL& tcl, const String& name, String& value) { 
      return false;
    }
    bool tcl_setvar(IExecTCL& tcl, const String& name, const String& value) {
      return false;
    }
    bool tcl_command(IExecTCL& tcl, size_t argc, const String argv[]) {
      String cmd = argv[0];
      dbg_put(rsdl_TCL, "SExecTCL::FailedContext::command(ERROR %d:\"%.*s\" +%d)\n", int(~cmd), int(~cmd), *cmd, int(argc));
      return false;
    }
  };
  //
  struct Context : IExecutor {
    StringMap vars;
    //
    bool tcl_command(IExecTCL& tcl, size_t argc, const String argv[]) {
      String cmd = argv[0];
      if(cmd=="vardump") {
        tcl.print_f("Vars: %d {\n", vars.size());
        for(typename SExecTCL::StringMap::const_iterator x=vars.begin(), e=vars.end(); x!=e; ++x) {
          tcl.print_s("  '");
          tcl.print_s((*x).first.get_text());
          tcl.print_s("'='");
          tcl.print_s((*x).second.get_text());
          tcl.print_s("'\n");
        }
        tcl.print_f("} #Vars\n");
        return true;
      }
      if((cmd=="set") || (cmd=="=")) {
        if(argc==3) {
//          dbg_put(rsdl_TCL, "SExecTCL::Context::command(set \"%.*s\" \"%.*s\")\n", int(~argv[1]), *argv[1], int(~argv[2]), *argv[2]);
          tcl.set_result( tcl.var_set(argv[1], argv[2]) );
          return true;
        }
      }
      return false;
    }
    bool tcl_getvar(IExecTCL& tcl, const String& name, String& value) {
      StringName key(name);
      const StringValue* val = vars(key);
      if(val!=NULL) {
//        dbg_put(rsdl_TCL, "SExecTCL::Context::get_var(\"%.*s\")=>\"%.*s\"\n", int(~name), *name, int(~*val), **val);
        value = *val;
        return true;
      }
      return false;
    }
    bool tcl_setvar(IExecTCL& tcl, const String& name, const String& value) {
      StringName key(name);
      StringValue& v = vars[key];
      v = StringValue(value);
      return true;
    }
    //
    Context(void) {}
    ~Context(void) {}
  };
  //
  struct Range : IExecutor {
    int begin, end, step, cur;
    char tmp[64];
    //
    bool tcl_command(IExecTCL& tcl, size_t argc, const String argv[]) {
      return false;
    }
    bool tcl_getvar(IExecTCL& tcl, const String& name, String& value) {
      if(name=="value") {
        sprintf(tmp, "%d", cur);
        value = tmp;
        return true;
      }
      return false;
    }
    bool tcl_setvar(IExecTCL& tcl, const String& name, const String& value) {
      return false;
    }
    //
    Range(int a, int b, int c) : begin(a), end(c), step(b) { start(); }
    ~Range(void) {}
    //
    void start(void) { cur = begin; }
    void operator++(void) { cur += step; }
    void operator++(int) { cur += step; }
    bool valid(void) { return cur<end; }
    //
  };
  //
  IExecutor *executor;
  IExecTCL *up;
  Thread &ss;
  Strings args;
  StringStream stream;
  String result;
  size_t stack_top;
  //
  SExecTCL(Thread& ass, IExecutor *ee, IExecTCL *u=NULL)
  : ss(ass), stream(ass.top(), ass.left()), executor(ee), up(u) {
    stack_top = ss.stack.Len();
  }
  ~SExecTCL(void) {
  }
  //
  inline static TypeId get_tinfo(void) { return tl::TObjectUniqueID<SExecTCL>::get_id(); }
  inline static const char* _root_get_interface_infoname(void) { return "SExecTCL"; }
  inline static int _root_get_interface_infover(void) { return 1; }
  //
  IExecTCL* get_up(void) const { return up; }
  IExecutor* get_executor(void) const { return executor; }
  //
  const IExecTCL* get_other(TypeId type) const {
    if(type==get_tinfo())
      return this;
    return NULL;
  }
  IExecTCL* get_other(TypeId type) {
    if(type==get_tinfo())
      return this;
    return NULL;
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
    Self c2(ss, executor, this);
    Parser ps2(ps.p, ps.e, true);
    ps2.Parse(c2);
    ps.p = ps2.p; // sync
    c2.finish();
    if(ps2.token>=Parser::tError)
      return false;
    add_result(c2);
    return true;
  }
  bool execute(void) {
    if(args.size()==0)
      return true;
    execute_begin();
    bool rv = tcl_cmd(args);
    execute_end();
    return rv;
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
    Self c2(ss, executor, this);
    Parser ps2(*code, code + ~code);
    ps2.Parse(c2);
    c2.finish();
    return new_string(c2.result);
  }
  String eval(const String& code, IExecutor *ex2) {
    Self c2(ss, ex2, this);
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
  //
  String new_string(const String& src) {
    size_t p = ss.stack.Len();
    if(!ss.stack.ResizeRel(~src+1))
        return String();
    tl::su::smemcpy(ss.stack.All()+p, *src, ~src);
    ss.stack[p+~src] = 0;
    String rv(ss.stack.All()+p, ~src);
    return rv;
  }
  String detach(void) {
    String rv = stream.get_s();
    stream_redo();
    return rv;
  }
  void parse_start() {
    stream_redo();
  }
  //
  void stream_redo(void) {
    stream.setup(ss.stack.FreeStart(), ss.stack.FreeLen(), 0);
    stream.length = 0;
    stream.maxlength = ss.stack.MaxLen() - ss.stack.Len();
  }
  void ssync(void) {
    ss.stack.Resize(~stream + (stream.get_text() - ss.stack.All()));
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
    stream.append(*ss, ~ss);
    ssync();
  }
  void next_arg(void) {
    args.Push(detach()); 
  }
  size_t stream_size(void) { return ~stream; }
  //
  bool tcl_cmd(Strings& args) {
    IExecTCL *tcl = this;
    while(tcl!=NULL) {
      if(tcl->get_executor()->tcl_command(*this, ~args, args.All()))
        return true;
      tcl = tcl->get_up();
    }
    return false;
  }
  //
  String var_get(const String& name) {
    String rvx;
    IExecTCL *tcl = this;
    // always get executor vars
    while(tcl!=NULL) {
      if(tcl->get_executor()->tcl_getvar(*this, name, rvx))
        return rvx;
      tcl = tcl->get_up();
    }
    return String();
  }
  String var_set(const String& name, const String& value) {
    // always set executor vars
    IExecTCL *tcl = this;
    while(tcl!=NULL) {
      if(tcl->get_executor()->tcl_setvar(*this, name, value))
        return value;
      tcl = tcl->get_up();
    }
    return String();
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
  void print_s(const String& val);
  void print_s(StringP val);
  void print_f(StringP val, ...);
};


//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_REXEC_H
