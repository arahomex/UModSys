#include <umodsys/core/module.h>
#include <umodsys/core/system.h>
#include <umodsys/core/console.h>
#include <umodsys/core/module_impl.h>
#include <umodsys/core/bobjects.h>
#include <umodsys/core/shell.h>

#include "version_plugin_test6.h"

namespace UModSys {
namespace test6 {

using namespace core;
using namespace base;

SDebug s_dbg;

/*
struct RTest6_Shell : public IShell {
  RTest6_Shell(void) {
    M.con().put(0, "RTest6_Shell()\n");
  }
  ~RTest6_Shell(void) {
    M.con().put(0, "~RTest6_Shell()\n");
  }
  IModule* get_module(void) const;
  bool connect(BCStr command) {
    M.con().put(0, "RTest6_Shell::connect(\"%s\")\n", command);
    return true;
  }
  bool disconnect(BCStr command) {
    M.con().put(0, "RTest6_Shell::disconnect(\"%s\")\n", command);
    return true;
  }
  bool process_command(BCStr command, BCStr args) {
    M.con().put(0, "RTest6_Shell::command(\"%s\", \"%s\")\n", command, args);
    return true;
  }
  //
  UMODSYS_SHELL(test6::RTest6_Shell, 1, IShell)
};

static tl::TTypeStaticHolder<RTest6_Shell> shell;
*/
struct RTest6_ModuleReg : public IModuleReg {
  UMODSYS_CORE_MODREG_DEF(RTest6_ModuleReg)
  //
  RTest6_ModuleReg(void) 
  : UMODSYS_MODULEREG(
      "tests::test6", 
      UMODSYS_VERSION_PLUGIN_TEST_TEST6, 
      "Test 2 - module"
    ) {
      s_dbg.menable();
//    M.con().put(0, "RTest6_ModuleReg()\n");
  }
  ~RTest6_ModuleReg(void) {
//    M.con().put(0, "~RTest6_ModuleReg()\n");
  }
  bool do_open(void) {
    s_dbg.put(0, cl_Debug, "RTest6_ModuleReg::open()\n");
//    shell.init(); reg(shell);
    return true;
  }
  bool do_close(void) {
    s_dbg.put(0, cl_Debug, "RTest6_ModuleReg::close()\n");
//    unreg(shell); shell.deinit();
    return true;
  }
};

UMODSYS_CORE_MODREG_BODY(RTest6_ModuleReg)

//IModule* RTest6_Shell::get_module(void) const { return reg.module; }

} // namespace test6
} // namespace UModSys


