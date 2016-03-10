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
  dbg_put(rsdl_TCL, "RSystem::exec_args(%d)\n", argc);
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
          continue;
        } else if(tl::su::sbegin(opt, "script=", &value) || tl::su::sbegin(opt, "script:", &value)) {
          exec_script(value);
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

bool RSystem::tcl_getvar(IExecTCL& tcl, const IExecTCL::String& name, IExecTCL::String& value)
{
  return false;
}

bool RSystem::tcl_setvar(IExecTCL& tcl, const IExecTCL::String& name, const IExecTCL::String& value)
{
  return false;
}

bool RSystem::tcl_command(IExecTCL& atcl, size_t argc, const IExecTCL::String argv[])
{
  tl::TArray<const IExecTCL::String> args(argv, argc, argc);
  IExecTCL* stcl = atcl.get_other(SExecTCL::get_tinfo());
  if(stcl==NULL) {
    dbg_put(rsdl_TCL, "Invalid TCL\n");
    return false;
  }
  SExecTCL& tcl = static_cast<SExecTCL&>(*stcl);
  const SExecTCL::String &cmd = argv[0];
  if(0) {
    dbg_put(rsdl_TCL, "RSystem::command(%d \"%.*s\" ", int(~args), int(~cmd), cmd.c_str());
    for(size_t i=1; i<~args; i++) {
      const SExecTCL::String &arg = args[i];
      dbg_put(rsdl_TCL, " \"%.*s\"", int(~arg), arg.c_str());
    }
    dbg_put(rsdl_TCL, ")\n");
  }
  //
    if(cmd=="puts") {
      for(int i=1; i<args.size(); i++) {
        if(i>1) tcl.print_f(" ");
        tcl.print_s(args[i]);
      }
      tcl.print_s("\n");
      return true;
    } else if(cmd=="argdump") {
      tcl.print_f("Args: %d {\n", args.size());
      for(int i=0; i<args.size(); i++) {
        tcl.print_f("  %d:'", i);
        tcl.print_s(args[i]);
        tcl.print_s("'\n");
      }
      tcl.print_s("} #Args\n");
      return true;
    } else if(cmd=="?") {
      tcl.set_result(args.size()>1 ? args[1] : SExecTCL::String());
      return true;
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
            for(SExecTCL::Range r(0, 1,
                                  atoi(SExecTCL::StringValue(args[2]).c_str())); r.valid(); r++) {
              tcl.eval(args.Last(), &r);
            }
            return true;
          } else if(args.size()==2+3) {
            for(SExecTCL::Range r(atoi(SExecTCL::StringValue(args[2]).c_str()),
                                  1,
                                  atoi(SExecTCL::StringValue(args[3]).c_str())); r.valid(); r++) {
              tcl.eval(args.Last(), &r);
            }
            return true;
          } else if(args.size()==3+3) {
            for(SExecTCL::Range r(atoi(SExecTCL::StringValue(args[2]).c_str()),
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
        { 
          char buf[32]; 
          sprintf(buf, "%d", tcl.eval_expr(v)+1);
          tcl.set_result(tcl.var_set(args[1], SExecTCL::String(buf)));
        }
        return true;
      }
    } else if(cmd=="module") {
      if(args.size()>=2) {
        if(args[1]=="scan") {
          if(args.size()==2) {
            get_modloader()->moduledb_scan("", false);
            return true;
          } else if(args.size()==3) {
            get_modloader()->moduledb_scan(args[2], true);
            return true;
          } 
        } else if(args[1]=="save") {
          if(args.size()==2) {
            get_modloader()->moduledb_save("moduledb.conf-hdb");
            return true;
          } else if(args.size()==3) {
            get_modloader()->moduledb_save(args[2]);
            return true;
          }
        } else if(args[1]=="shell") {
          if(args.size()==2) {
            IRefObject::TypeId tids[0x100];
            size_t ns = find_shells(tids, 0x100, NULL);
            dbg_put(rsdl_TCL, "shells found: %d\n", ns);
            for(size_t i=0; i<ns; i++) {
              dbg_put(rsdl_TCL, "  shell: %p %s\n", tids[i], tids[i]->name);
            }
            dbg_put(rsdl_TCL, "/shells found: %d\n", ns);
            return true;
          }
        }
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

bool RSystem::exec_script(BStr filename)
{
  size_t len;
  char buf[0x10000];
  if(!syshlp::get_file(filename, len, buf, sizeof(buf), true)) {
    dbg_put(rsdl_TCL, "No script: %s\n", filename);
    return false;
  }
  dbg_put(rsdl_TCL, "Script begin: %s\n", filename);
  //
  DCString spp(buf, len);
  SExecTCL::Parser pp(spp.begin(), spp.end());
  SExecTCL::Thread thread;
  SExecTCL failed(thread, &tcl_ctxfail);
  SExecTCL master(thread, &tcl_ctx, &failed);
  SExecTCL col(thread, this, &master);
  //
  int rv = pp.Parse(col);
  if(rv!=SExecTCL::Parser::tEnd) {
    dbg_put(rsdl_TCL, "TCL end %s with error %d\n", filename, rv);
    return false;
  } else {
    dbg_put(rsdl_TCL, "TCL end %s.\n", filename);
  }
  //
  return true;
}

//***************************************
// ::
//***************************************
