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
// RSystem::
//***************************************

bool RSystem::exec_args(int argc, char** argv)
{
  dbg_put(rsdl_System, "RSystem::exec_args(%d)\n", argc);
  //
//  S.get_modloader()->moduledb_scan("", false);
//  S.get_modloader()->moduledb_scan("*", true);
//  S.get_modloader()->moduledb_save("moduledb.conf-hdb");
  //
  return false;
}

//***************************************
//***************************************

bool RSystem::command(SExecTCL& tcl, const SExecTCL::String &cmd, const SExecTCL::Strings& args)
{
    if(cmd=="puts") {
      for(int i=1; i<args.size(); i++) {
        if(i>1) printf(" ");
        tcl.prints(args[i]);
      }
      tcl.prints("\n");
      return true;
    } else if(cmd=="vardump") {
      tcl.printf("Vars: %d {\n", tcl.ss.vars.size());
      for(typename SExecTCL::StringMap::const_iterator x=tcl.ss.vars.begin(), e=tcl.ss.vars.end(); x!=e; ++x) {
        tcl.prints("  '");
        tcl.prints((*x).first);
        tcl.prints("'='");
        tcl.prints((*x).second);
        tcl.prints("'\n");
      }
      tcl.printf("} #Vars\n");
    } else if(cmd=="argdump") {
      tcl.printf("Args: %d {\n", args.size());
      for(int i=0; i<args.size(); i++) {
        tcl.printf("  %d:'", i);
        tcl.prints(args[i]);
        tcl.prints("'\n");
      }
      tcl.prints("} #Args\n");
      return true;
    } else if(cmd=="?") {
      tcl.set_result(args.size()>1 ? args[1] : SExecTCL::String());
      return true;
    } else if((cmd=="set") || (cmd=="=")) {
      if(args.size()==3) {
        tcl.set_result( tcl.var_set(args[1], args[2]) );
        return true;
      }
    } else if(cmd=="if") {
      if(args.size()==3) {
        if(tcl.eval_live_expr(args[1])) {
          tcl.eval(args[2]);
        }
        return true;
      }
    } else if(cmd=="while") {
      if(args.size()==3) {
        SExecTCL::State state(tcl.ss);
        while(tcl.eval_live_expr(args[1])) {
          tcl.eval(args[2]);
          state.reset();
        }
        return true;
      }
    } else if(cmd=="for") {
      if(args.size()==5) {
        SExecTCL::State state(tcl.ss);
        tcl.eval_live_expr(args[1]);
        while(!tcl.eval_live_expr(args[2])) {
          tcl.eval(args[3]);
          tcl.eval(args[2]);
          state.reset();
        }
        return true;
      }
    } else if(cmd=="<") {
      if(args.size()==3) {
        tcl.set_result( SExecTCL::String(tcl.eval_expr(args[1]) < tcl.eval_expr(args[2]) ? "1" : "0") );
        return true;
      }
    } else if(cmd=="++") {
      if(args.size()==2) {
        const SExecTCL::String& v = tcl.var_get(args[1]);
//String oldv = v;
        { 
          char buf[32]; 
          sprintf(buf, "%d", tcl.eval_expr(v)+1);
          tcl.set_result(tcl.var_set(args[1], SExecTCL::String(buf)));
        }
//printf("{++ '%s'=>'%s'+1/%s}", cs(args[1]), cs(oldv), cs(v));
        return true;
      }
    }
    return false;
}

//***************************************
//***************************************

bool RSystem::execute_command(const DCString args[], size_t len)
{
  return false;
}

bool RSystem::execute_text(const core::DCString& text)
{
  return false;
}

bool RSystem::get_var(const core::DCString& name, core::DCString& value) const
{
  return false;
}

bool RSystem::get_shell(const core::DCString& name, IShell::P& shell) const
{
  return false;
}

bool RSystem::set_var(const core::DCString& name, const core::DCString& value, bool can_new)
{
  return false;
}

bool RSystem::set_shell(const core::DCString& name, IShell* shell)
{
  shells[name] = shell;
  return true;
}


//***************************************
// SExecTCL::
//***************************************

void SExecTCL::prints(const String& val)
{
  printf("%s", val.c_str());
}

void SExecTCL::prints(StringP val)
{
  printf("%s", val);
}

void SExecTCL::printf(StringP val, ...)
{
  va_list va;
  va_start(va, val);
  vprintf(val, va);
  va_end(va);
}

//***************************************
// ::
//***************************************
