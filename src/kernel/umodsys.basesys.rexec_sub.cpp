#include "umodsys.basesys.rsystem.h"
#include "umodsys.basesys.rmodule.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;



//***************************************
// SExecTCL::FailedContext::
//***************************************

SExecTCL::FailedContext::FailedContext(void) 
{
}
SExecTCL::FailedContext::~FailedContext(void) 
{
}

IExecTCL::eStatus SExecTCL::FailedContext::tcl_getvar(IExecTCL& tcl, const String& name, String& value) 
{ 
  return sFalse;
}

IExecTCL::eStatus SExecTCL::FailedContext::tcl_setvar(IExecTCL& tcl, const String& name, const String& value) 
{
  return sFalse;
}

IExecTCL::eStatus SExecTCL::FailedContext::tcl_command(IExecTCL& tcl, size_t argc, const String argv[]) 
{
  String cmd = argv[0];
  String err = tcl.get_thread()->get_error_text();
  dbg_put(
    rsdl_TCL, "SExecTCL::FailedContext::command(ERROR %d:\"%.*s\" +%d) %d \"%.*s\"\n", 
    int(~cmd), int(~cmd), *cmd, int(argc),
    tcl.get_thread()->get_error(), int(~err), *err
  );
  return sError;
}

//***************************************
// SExecTCL::Context::
//***************************************

SExecTCL::Context::Context(void) 
{
}
SExecTCL::Context::~Context(void) 
{
}

IExecTCL::eStatus SExecTCL::Context::tcl_command(IExecTCL& tcl, size_t argc, const String argv[]) 
{
  String cmd = argv[0];
  if(cmd=="vardump") {
    tcl.get_thread()->print_f("Vars: %d {\n", vars.size());
    for(typename SExecTCL::StringMap::const_iterator x=vars.begin(), e=vars.end(); x!=e; ++x) {
      tcl.get_thread()->print_s("  '");
      tcl.get_thread()->print_s((*x).first.get_text());
      tcl.get_thread()->print_s("'='");
      tcl.get_thread()->print_s((*x).second.get_text());
      tcl.get_thread()->print_s("'\n");
    }
    tcl.get_thread()->print_f("} #Vars\n");
    return sTrue;
  }
  if((cmd=="set") || (cmd=="=")) {
    if(argc==3) {
//          dbg_put(rsdl_TCL, "SExecTCL::Context::command(set \"%.*s\" \"%.*s\")\n", int(~argv[1]), *argv[1], int(~argv[2]), *argv[2]);
      tcl.set_result( tcl.var_set(argv[1], argv[2]) );
      return sTrue;
    }
  }
  return sFalse;
}

IExecTCL::eStatus SExecTCL::Context::tcl_getvar(IExecTCL& tcl, const String& name, String& value) 
{
  StringName key(name);
  const StringValue* val = vars(key);
  if(val!=NULL) {
//        dbg_put(rsdl_TCL, "SExecTCL::Context::get_var(\"%.*s\")=>\"%.*s\"\n", int(~name), *name, int(~*val), **val);
    value = *val;
    return sTrue;
  }
  return sFalse;
}

IExecTCL::eStatus SExecTCL::Context::tcl_setvar(IExecTCL& tcl, const String& name, const String& value) 
{
  StringName key(name);
  StringValue& v = vars[key];
  v = StringValue(value);
  return sTrue;
}

//***************************************
// SExecTCL::Range::
//***************************************

SExecTCL::Range::Range(int a, int b, int c) 
: begin(a), end(c), step(b) 
{ 
  start(); 
}

SExecTCL::Range::~Range(void) 
{
}

IExecTCL::eStatus SExecTCL::Range::tcl_command(IExecTCL& tcl, size_t argc, const String argv[]) 
{
  return sFalse;
}

IExecTCL::eStatus SExecTCL::Range::tcl_getvar(IExecTCL& tcl, const String& name, String& value) 
{
  if(name=="value") {
    sprintf(tmp, "%d", cur);
    value = tmp;
    return sTrue;
  }
  return sFalse;
}

IExecTCL::eStatus SExecTCL::Range::tcl_setvar(IExecTCL& tcl, const String& name, const String& value) 
{
  return sFalse;
}

//***************************************
// SExecTCL::ControlTCL::
//***************************************

SExecTCL::ControlTCL::ControlTCL(void) 
{
}

SExecTCL::ControlTCL::~ControlTCL(void) 
{
}

IExecTCL::eStatus SExecTCL::ControlTCL::tcl_command(IExecTCL& tcl, size_t argc, const String argv[]) 
{
/*
  IExecTCL* stcl = atcl.get_other(SExecTCL::get_tinfo());
  if(stcl==NULL) {
    dbg_put(rsdl_TCL, "Invalid TCL\n");
    return sFalse;
  }
  SExecTCL& tcl = static_cast<SExecTCL&>(*stcl);
*/
#define ERR() if(tcl.get_thread()->get_error()) return sError

  String cmd = argv[0];
  // generic 
  if(cmd=="true" || cmd=="1") {
    tcl.set_result("1");
    return sTrue;
  } else if(cmd=="sFalse" || cmd=="0") {
    tcl.set_result("0");
    return sTrue;
  } else if(cmd=="?") {
    tcl.set_result(argc>1 ? argv[1] : SExecTCL::String());
    return sTrue;
//  } else if(cmd=="if") {
  } else if(cmd=="nop") {
    return sTrue; // do nothing
  } else if(cmd=="eval") {
    for(int i=1; i<argc; i++) {
      tcl.eval(argv[i]);
      ERR();
    }
    return sTrue;
  } else if(cmd=="if") {
    if(argc==3) {
      if(tcl.eval_live_expr(argv[1])) {
        ERR();
        tcl.eval(argv[2]);
        ERR();
      }
      return sTrue;
    }
    if(argc==4) {
      if(tcl.eval_live_expr(argv[1])) {
        ERR();
        tcl.eval(argv[2]);
        ERR();
      } else {
        ERR();
        tcl.eval(argv[3]);
        ERR();
      }
      return sTrue;
    }
  } else if(cmd=="while") {
    if(argc==3) {
      SExecTCL::ThreadState state(tcl.get_thread());
      while(tcl.eval_live_expr(argv[1])) {
        ERR();
        tcl.eval(argv[2]);
        ERR();
        state.reset();
      }
      return sTrue;
    }
  } else if(cmd=="for") {
    if(argc==5) {
      SExecTCL::ThreadState state(tcl.get_thread());
      tcl.eval_live_expr(argv[1]);
      ERR();
      while(tcl.eval_live_expr(argv[2])) {
        ERR();
        tcl.eval(argv[4]);
        ERR();
        tcl.eval(argv[3]);
        ERR();
        state.reset();
      }
      return sTrue;
    }
  } else if(cmd=="foreach") {
    SExecTCL::ThreadState state(tcl.get_thread());
    if(argc>2+1) {
      if(argv[1]=="range") {
        if(argc==1+3) {
          for(SExecTCL::Range r(0, 1,
                                atoi(SExecTCL::StringValue(argv[2]).c_str())); r.valid(); r++) {
            tcl.eval(argv[argc-1], &r);
            ERR();
          }
          return sTrue;
        } else if(argc==2+3) {
          for(SExecTCL::Range r(atoi(SExecTCL::StringValue(argv[2]).c_str()),
                                1,
                                atoi(SExecTCL::StringValue(argv[3]).c_str())); r.valid(); r++) {
            tcl.eval(argv[argc-1], &r);
            ERR();
          }
          return sTrue;
        } else if(argc==3+3) {
          for(SExecTCL::Range r(atoi(SExecTCL::StringValue(argv[2]).c_str()),
                                atoi(SExecTCL::StringValue(argv[3]).c_str()),
                                atoi(SExecTCL::StringValue(argv[4]).c_str())); r.valid(); r++) {
            tcl.eval(argv[argc-1], &r);
            ERR();
          }
          return sTrue;
        }
      }
    }
  // print
  } else if(cmd=="puts") {
    for(int i=1; i<argc; i++) {
      if(i>1) tcl.get_thread()->print_f(" ");
      tcl.get_thread()->print_s(argv[i]);
    }
    tcl.get_thread()->print_s("\n");
    return sTrue;
  } else if(cmd=="puts,") {
    for(int i=1; i<argc; i++) {
      if(i>1) tcl.get_thread()->print_f(" ");
      tcl.get_thread()->print_s(argv[i]);
    }
    return sTrue;
  } else if(cmd=="argdump") {
    tcl.get_thread()->print_f("Args: %d {\n", argc);
    for(int i=0; i<argc; i++) {
      tcl.get_thread()->print_f("  %d:'", i);
      tcl.get_thread()->print_s(argv[i]);
      tcl.get_thread()->print_s("'\n");
    }
    tcl.get_thread()->print_s("} #Args\n");
    return sTrue;
  } else if(cmd=="?") {
    tcl.set_result(argc>1 ? argv[1] : SExecTCL::String());
    return sTrue;
  // math
#define OP(comd, sign) \
  } else if(cmd==#comd) { \
    if(argc==3) { \
      /*dbg_put(rsdl_TCL, "%s [%.*s] [%.*s]\n", #comd, int(~argv[1]), *argv[1], int(~argv[2]), *argv[2])*/; \
      tcl.set_result( SExecTCL::String(tcl.eval_expr(argv[1]) sign tcl.eval_expr(argv[2]) ? "1" : "0") ); \
      return sTrue; \
    }
  OP(<, <) OP(>, >) OP(<=, <=) OP(>=, >=) OP(==, ==) OP(!=, !=)
  OP(lt, <) OP(gt, >) OP(le, <=) OP(ge, >=) OP(eq, ==) OP(ne, !=)
  OP(+, +) OP(-, -) OP(*, *) OP(/, /) OP(%, %)
  OP(add, +) OP(sub, -) OP(mul, *) OP(div, /) OP(mod, %)
#undef OP
  // inc/dec
#define OP(comd, modif) \
  } else if(cmd==#comd) { \
    if(argc==2) { \
      const SExecTCL::String& v = tcl.var_get(argv[1]); \
      { \
        char buf[32]; \
        sprintf(buf, "%d", modif tcl.eval_expr(v)); \
        tcl.set_result(tcl.var_set(argv[1], SExecTCL::String(buf))); \
      } \
      return sTrue; \
    }
  OP(++, 1+)
  OP(--, -1+)
  OP(inc, 1+)
  OP(dec, -1+)
#undef OP
  } else {
  }
  return sFalse;
#undef ERR
}

IExecTCL::eStatus SExecTCL::ControlTCL::tcl_getvar(IExecTCL& tcl, const String& name, String& value) 
{
  return sFalse;
}

IExecTCL::eStatus SExecTCL::ControlTCL::tcl_setvar(IExecTCL& tcl, const String& name, const String& value) 
{
  return sFalse;
}

//***************************************
// ::
//***************************************
