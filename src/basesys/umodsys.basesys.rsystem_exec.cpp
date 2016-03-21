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
          continue;
        } else if(tl::su::sbegin(opt, "dflag=", &value) || tl::su::sbegin(opt, "dflag:", &value)) {
#define DF(flag) \
  if(tl::su::seq(value, #flag)) { rsys_dbg.enable(flag); dbg_put(rsdl_TCL, "dflag %s %x\n", #flag, flag); continue; } \
  if(tl::su::seq(value, "+" #flag)) { rsys_dbg.enable(flag); dbg_put(rsdl_TCL, "dflag +%s %x\n", #flag, flag); continue; } \
  if(tl::su::seq(value, "-" #flag)) { rsys_dbg.disable(flag); dbg_put(rsdl_TCL, "dflag -%s %x\n", #flag, flag); continue; }
          DF(rsdl_SysTests);
          DF(rsdl_MemoryError);
          DF(rsdl_Uid);
          DF(rsdl_System);
          DF(rsdl_Module);
          DF(rsdl_ModuleLibrary);
          DF(rsdl_SoLoad);
          DF(rsdl_TCL);
#undef DF
          //continue;
        }
      }
      dbg_put(rsdl_TCL, "RSystem::exec_args(%d) -- Invalid arg %d \"%s\"\n", argc, i, argv[i]);
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

IExecTCL::eStatus RSystem::tcl_getvar(IExecTCL& tcl, const IExecTCL::String& name, IExecTCL::String& value)
{
  return IExecTCL::sFalse;
}

IExecTCL::eStatus RSystem::tcl_setvar(IExecTCL& tcl, const IExecTCL::String& name, const IExecTCL::String& value)
{
  return IExecTCL::sFalse;
}

IExecTCL::eStatus RSystem::tcl_command(IExecTCL& tcl, size_t argc, const IExecTCL::String argv[])
{
//  tl::TArray<const IExecTCL::String> args(argv, argc, argc);
  const SExecTCL::String &cmd = argv[0];
  if(0) {
    dbg_put(rsdl_TCL, "RSystem::command(%d \"%.*s\" ", int(argc), int(~cmd), cmd.c_str());
    for(size_t i=1; i<argc; i++) {
      const SExecTCL::String &arg = argv[i];
      dbg_put(rsdl_TCL, " \"%.*s\"", int(~arg), arg.c_str());
    }
    dbg_put(rsdl_TCL, ")\n");
  }
  //
  if(cmd=="foreach") {
    SExecTCL::ThreadState state(tcl.get_thread());
  } else if(cmd=="module") {
    if(argc>=2) {
      if(argv[1]=="scan") {
        if(argc==2) {
          get_modloader()->moduledb_scan("", false);
          return IExecTCL::sTrue;
        } else if(argc==3) {
          get_modloader()->moduledb_scan(argv[2], true);
          return IExecTCL::sTrue;
        } 
      } else if(argv[1]=="save") {
        if(argc==2) {
          get_modloader()->moduledb_save("moduledb.conf-hdb");
          return IExecTCL::sTrue;
        } else if(argc==3) {
          get_modloader()->moduledb_save(argv[2]);
          return IExecTCL::sTrue;
        }
      } else if(argv[1]=="shell") {
        const int n_shells = 0x1000;
        if(argc==2) {
          IRefObject::TypeId tids[n_shells];
          size_t ns = find_shells(tids, n_shells, NULL);
          dbg_put(rsdl_TCL, "shells found: %d\n", ns);
          for(size_t i=0; i<ns; i++) {
            dbg_put(rsdl_TCL, "  shell: %p %s\n", tids[i], tids[i]->name);
          }
          dbg_put(rsdl_TCL, "/shells found: %d\n", ns);
          return IExecTCL::sTrue;
        } else if(argc==4) {
          IRefObject::TypeId tids[n_shells];
          size_t ns = find_shells(tids, n_shells, NULL);
          IRefObject::TypeId tid = NULL;
          for(size_t i=0; i<ns; i++) {
            if(tl::su::wildcmp(*argv[3], tids[i]->name)) {
              tid = tids[i];
              break;
            }
          }
          if(tid!=NULL) {
            TParametersA<1024> pars;
            pars.add("shell", argv[2]);
            IShell::P sh;
            if(M.t_generate(sh, tid, pars)) {
              set_shell(argv[2], sh);
            }
            dbg_put(rsdl_TCL, "shell["FMT_SS"]: %p %s for '"FMT_SS"'\n", FMT_STR(argv[2]), tid, tid->name, FMT_STR(argv[3]));
          } else {
            dbg_put(rsdl_TCL, "Error shell["FMT_SS"]: for '"FMT_SS"' can't found\n", FMT_STR(argv[2]), FMT_STR(argv[3]));
          }
          return IExecTCL::sTrue;
        }
      }
    }
  }
  //
  IShell::P shell;
  if(get_shell(cmd, shell)) {
    return shell->tcl_command(tcl, argc, argv);
  }
  return IExecTCL::sFalse;
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
  const IShell::P *found = shells.GetT(name);
  if(found==NULL)
    return false;
  shell = *found;
  return true;
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
  SExecTCL::IExecutor* exs[4]={ this, &tcl_ctx, &tcl_control, &tcl_ctxfail };
  SExecTCL col(thread, exs, 4);
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
