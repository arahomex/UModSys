#include <umodsys/core/module.h>
#include <umodsys/core/system.h>
#include <umodsys/core/console.h>
#include <umodsys/core/module_impl.h>
#include <umodsys/core/bobjects.h>
#include <umodsys/core/shell.h>

#include "version_plugin_test5.h"

namespace UModSys {
namespace test5 {

using namespace core;
using namespace base;

SDebug s_dbg;

/*
struct RTest5_Shell : public IShell {
  RTest5_Shell(void) {
    M.con().put(0, "RTest5_Shell()\n");
  }
  ~RTest5_Shell(void) {
    M.con().put(0, "~RTest5_Shell()\n");
  }
  IModule* get_module(void) const;
  bool connect(BCStr command) {
    M.con().put(0, "RTest5_Shell::connect(\"%s\")\n", command);
    return true;
  }
  bool disconnect(BCStr command) {
    M.con().put(0, "RTest5_Shell::disconnect(\"%s\")\n", command);
    return true;
  }
  bool process_command(BCStr command, BCStr args) {
    M.con().put(0, "RTest5_Shell::command(\"%s\", \"%s\")\n", command, args);
    return true;
  }
  //
  UMODSYS_SHELL(test5::RTest5_Shell, 1, IShell)
};

static tl::TTypeStaticHolder<RTest5_Shell> shell;
*/
struct RTest5_ModuleReg : public IModuleReg {
  UMODSYS_CORE_MODREG_DEF(RTest5_ModuleReg)
  //
  RTest5_ModuleReg(void) 
  : UMODSYS_MODULEREG(
      "tests::test5", 
      UMODSYS_VERSION_PLUGIN_TEST_TEST5, 
      "Test 2 - module"
    ) {
      s_dbg.menable();
//    M.con().put(0, "RTest5_ModuleReg()\n");
  }
  ~RTest5_ModuleReg(void) {
//    M.con().put(0, "~RTest5_ModuleReg()\n");
  }
  bool do_open(void) {
    s_dbg.put(0, cl_Debug, "RTest5_ModuleReg::open()\n");
//    shell.init(); reg(shell);
    return true;
  }
  bool do_close(void) {
    s_dbg.put(0, cl_Debug, "RTest5_ModuleReg::close()\n");
//    unreg(shell); shell.deinit();
    return true;
  }
};

UMODSYS_CORE_MODREG_BODY(RTest5_ModuleReg)

//IModule* RTest5_Shell::get_module(void) const { return reg.module; }

} // namespace test5
} // namespace UModSys


