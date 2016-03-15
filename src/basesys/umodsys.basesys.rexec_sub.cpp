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

bool SExecTCL::FailedContext::tcl_getvar(IExecTCL& tcl, const String& name, String& value) 
{ 
  return false;
}

bool SExecTCL::FailedContext::tcl_setvar(IExecTCL& tcl, const String& name, const String& value) 
{
  return false;
}

bool SExecTCL::FailedContext::tcl_command(IExecTCL& tcl, size_t argc, const String argv[]) 
{
  String cmd = argv[0];
  String err = tcl.get_thread()->get_error_text();
  dbg_put(
    rsdl_TCL, "SExecTCL::FailedContext::command(ERROR %d:\"%.*s\" +%d) %d \"%.*s\"\n", 
    int(~cmd), int(~cmd), *cmd, int(argc),
    tcl.get_thread()->get_error(), int(~err), *err
  );
  return false;
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

bool SExecTCL::Context::tcl_command(IExecTCL& tcl, size_t argc, const String argv[]) 
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

bool SExecTCL::Context::tcl_getvar(IExecTCL& tcl, const String& name, String& value) 
{
  StringName key(name);
  const StringValue* val = vars(key);
  if(val!=NULL) {
//        dbg_put(rsdl_TCL, "SExecTCL::Context::get_var(\"%.*s\")=>\"%.*s\"\n", int(~name), *name, int(~*val), **val);
    value = *val;
    return true;
  }
  return false;
}

bool SExecTCL::Context::tcl_setvar(IExecTCL& tcl, const String& name, const String& value) 
{
  StringName key(name);
  StringValue& v = vars[key];
  v = StringValue(value);
  return true;
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

bool SExecTCL::Range::tcl_command(IExecTCL& tcl, size_t argc, const String argv[]) 
{
  return false;
}

bool SExecTCL::Range::tcl_getvar(IExecTCL& tcl, const String& name, String& value) 
{
  if(name=="value") {
    sprintf(tmp, "%d", cur);
    value = tmp;
    return true;
  }
  return false;
}

bool SExecTCL::Range::tcl_setvar(IExecTCL& tcl, const String& name, const String& value) 
{
  return false;
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

bool SExecTCL::ControlTCL::tcl_command(IExecTCL& tcl, size_t argc, const String argv[]) 
{
/*
  IExecTCL* stcl = atcl.get_other(SExecTCL::get_tinfo());
  if(stcl==NULL) {
    dbg_put(rsdl_TCL, "Invalid TCL\n");
    return false;
  }
  SExecTCL& tcl = static_cast<SExecTCL&>(*stcl);
*/
  String cmd = argv[0];
  // generic 
  if(cmd=="if") {
    if(argc==3) {
      if(tcl.eval_live_expr(argv[1])) {
        tcl.eval(argv[2]);
      }
      return true;
    }
  } else if(cmd=="while") {
    if(argc==3) {
      SExecTCL::State state(tcl.get_thread());
      while(tcl.eval_live_expr(argv[1])) {
        tcl.eval(argv[2]);
        state.reset();
      }
      return true;
    }
  } else if(cmd=="for") {
    if(argc==5) {
      SExecTCL::State state(tcl.get_thread());
      tcl.eval_live_expr(argv[1]);
      if(tcl.get_thread()->get_error())
        return false;
//dbg_put(rsdl_TCL, "for while \"%.*s\"\n", int(~argv[2]), *argv[2]);
      while(tcl.eval_live_expr(argv[2])) {
        if(tcl.get_thread()->get_error())
          return false;
//dbg_put(rsdl_TCL, "for \"%.*s\"\n", int(~argv[4]), *argv[4]);
        tcl.eval(argv[4]);
        if(tcl.get_thread()->get_error())
          return false;
//dbg_put(rsdl_TCL, "for \"%.*s\"\n", int(~argv[3]), *argv[3]);
        tcl.eval(argv[3]);
        if(tcl.get_thread()->get_error())
          return false;
        state.reset();
      }
      return true;
    }
  // print
  } else if(cmd=="puts") {
    for(int i=1; i<argc; i++) {
      if(i>1) tcl.get_thread()->print_f(" ");
      tcl.get_thread()->print_s(argv[i]);
    }
    tcl.get_thread()->print_s("\n");
    return true;
  } else if(cmd=="puts,") {
    for(int i=1; i<argc; i++) {
      if(i>1) tcl.get_thread()->print_f(" ");
      tcl.get_thread()->print_s(argv[i]);
    }
    return true;
  } else if(cmd=="argdump") {
    tcl.get_thread()->print_f("Args: %d {\n", argc);
    for(int i=0; i<argc; i++) {
      tcl.get_thread()->print_f("  %d:'", i);
      tcl.get_thread()->print_s(argv[i]);
      tcl.get_thread()->print_s("'\n");
    }
    tcl.get_thread()->print_s("} #Args\n");
    return true;
  } else if(cmd=="?") {
    tcl.set_result(argc>1 ? argv[1] : SExecTCL::String());
    return true;
  // math
#define OP(comd, sign) \
  } else if(cmd==#comd) { \
    if(argc==3) { \
      /*dbg_put(rsdl_TCL, "%s [%.*s] [%.*s]\n", #comd, int(~argv[1]), *argv[1], int(~argv[2]), *argv[2])*/; \
      tcl.set_result( SExecTCL::String(tcl.eval_expr(argv[1]) sign tcl.eval_expr(argv[2]) ? "1" : "0") ); \
      return true; \
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
      return true; \
    }
  OP(++, 1+)
  OP(--, -1+)
  OP(inc, 1+)
  OP(dec, -1+)
#undef OP
  } else {
  }
  return false;
}

bool SExecTCL::ControlTCL::tcl_getvar(IExecTCL& tcl, const String& name, String& value) 
{
  return false;
}

bool SExecTCL::ControlTCL::tcl_setvar(IExecTCL& tcl, const String& name, const String& value) 
{
  return false;
}

//***************************************
// ::
//***************************************
