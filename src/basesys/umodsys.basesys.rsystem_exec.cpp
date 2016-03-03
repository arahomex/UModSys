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
  if(argc==1)
    return false;
  //
  for(int i=1; i<argc; i++) {
    if(argv[i][0]=='-') {
      if(argv[i][1]=='-') { // option
        BCStr opt=argv[i]+2;
        BStr value;
        if(tl::su::sbegin(opt, "test=", &value) || tl::su::sbegin(opt, "test:", &value)) {
          exec_tests(value);
        }
      }
      continue;
    }
  }
//  S.get_modloader()->moduledb_scan("", false);
//  S.get_modloader()->moduledb_scan("*", true);
//  S.get_modloader()->moduledb_save("moduledb.conf-hdb");
  //
  return false;
}

//***************************************
//***************************************

bool RSystem::tcl_getvar(SExecTCL& tcl, const SExecTCL::String& name, SExecTCL::String& value)
{
  return false;
}

bool RSystem::tcl_setvar(SExecTCL& tcl, const SExecTCL::String& name, const SExecTCL::String& value)
{
  return false;
}

bool RSystem::tcl_command(SExecTCL& tcl, const SExecTCL::Strings& args)
{
  const SExecTCL::String &cmd = args[0];
  if(0) {
    dbg_put(rsdl_System, "RSystem::command(%d \"%.*s\" ", int(~args), int(~cmd), cmd.c_str());
    for(size_t i=1; i<~args; i++) {
      const SExecTCL::String &arg = args[i];
      dbg_put(rsdl_System, " \"%.*s\"", int(~arg), arg.c_str());
    }
    dbg_put(rsdl_System, ")\n");
  }
  //
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
        tcl.prints((*x).first.get_text());
        tcl.prints("'='");
        tcl.prints((*x).second.get_text());
        tcl.prints("'\n");
      }
      tcl.printf("} #Vars\n");
      return true;
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
    } else if(cmd=="foreach") {
      SExecTCL::State state(tcl.ss);
      if(args.size()>2+1) {
        if(args[1]=="range") {
          if(args.size()==1+3) {
            for(SExecTCL::Range r(this,
                                  0, 1,
                                  atoi(SExecTCL::StringValue(args[2]).c_str())); r.valid(); r++) {
              tcl.eval(args.Last(), &r);
            }
            return true;
          } else if(args.size()==2+3) {
            for(SExecTCL::Range r(this,
                                  atoi(SExecTCL::StringValue(args[2]).c_str()),
                                  1,
                                  atoi(SExecTCL::StringValue(args[3]).c_str())); r.valid(); r++) {
              tcl.eval(args.Last(), &r);
            }
            return true;
          } else if(args.size()==3+3) {
            for(SExecTCL::Range r(this,
                                  atoi(SExecTCL::StringValue(args[2]).c_str()),
                                  atoi(SExecTCL::StringValue(args[3]).c_str()),
                                  atoi(SExecTCL::StringValue(args[4]).c_str())); r.valid(); r++) {
              tcl.eval(args.Last(), &r);
            }
            return true;
          }
        }
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
    dbg_put(rsdl_System, "RSystem::command(%d:\"%.*s\" +%d)\n", int(~cmd), int(~cmd), *cmd, int(~args));
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
  printf("%.*s", int(~val), val.c_str());
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
