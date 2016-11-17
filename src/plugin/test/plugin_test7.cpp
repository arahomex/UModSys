#include <umodsys/core/module.h>
#include <umodsys/core/system.h>
#include <umodsys/core/console.h>
#include <umodsys/core/module_impl.h>
#include <umodsys/core/bobjects.h>
#include <umodsys/core/shell.h>

#include "version_plugin_test7.h"

namespace UModSys {
namespace test7 {

using namespace core;
using namespace base;

SDebug s_dbg;

/*
struct RTest7_Shell : public IShell {
  RTest7_Shell(void) {
    M.con().put(0, "RTest7_Shell()\n");
  }
  ~RTest7_Shell(void) {
    M.con().put(0, "~RTest7_Shell()\n");
  }
  IModule* get_module(void) const;
  bool connect(BCStr command) {
    M.con().put(0, "RTest7_Shell::connect(\"%s\")\n", command);
    return true;
  }
  bool disconnect(BCStr command) {
    M.con().put(0, "RTest7_Shell::disconnect(\"%s\")\n", command);
    return true;
  }
  bool process_command(BCStr command, BCStr args) {
    M.con().put(0, "RTest7_Shell::command(\"%s\", \"%s\")\n", command, args);
    return true;
  }
  //
  UMODSYS_SHELL(test7::RTest7_Shell, 1, IShell)
};

static tl::TTypeStaticHolder<RTest7_Shell> shell;
*/
struct RTest7_ModuleReg : public IModuleReg {
  UMODSYS_CORE_MODREG_DEF(RTest7_ModuleReg)
  //
  RTest7_ModuleReg(void) 
  : UMODSYS_MODULEREG(
      "tests::test7", 
      UMODSYS_VERSION_PLUGIN_TEST_TEST7, 
      "Test 2 - module"
    ) {
      s_dbg.menable();
//    M.con().put(0, "RTest7_ModuleReg()\n");
  }
  ~RTest7_ModuleReg(void) {
//    M.con().put(0, "~RTest7_ModuleReg()\n");
  }
  bool do_open(void) {
    s_dbg.put(0, cl_Debug, "RTest7_ModuleReg::open()\n");
//    shell.init(); reg(shell);
    return true;
  }
  bool do_close(void) {
    s_dbg.put(0, cl_Debug, "RTest7_ModuleReg::close()\n");
//    unreg(shell); shell.deinit();
    return true;
  }
};

UMODSYS_CORE_MODREG_BODY(RTest7_ModuleReg)

//IModule* RTest7_Shell::get_module(void) const { return reg.module; }

} // namespace test7
} // namespace UModSys


