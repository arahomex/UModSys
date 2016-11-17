#include <umodsys/core/module.h>
#include <umodsys/core/system.h>
#include <umodsys/core/console.h>
#include <umodsys/core/module_impl.h>
#include <umodsys/core/bobjects.h>
#include <umodsys/core/shell.h>

#include "version_plugin_test0.h"

namespace UModSys {
namespace test0 {

using namespace core;
using namespace base;

SDebug s_dbg;

/*
struct RTest0_Shell : public IShell {
  RTest0_Shell(void) {
    M.con().put(0, "RTest0_Shell()\n");
  }
  ~RTest0_Shell(void) {
    M.con().put(0, "~RTest0_Shell()\n");
  }
  IModule* get_module(void) const;
  bool connect(BCStr command) {
    M.con().put(0, "RTest0_Shell::connect(\"%s\")\n", command);
    return true;
  }
  bool disconnect(BCStr command) {
    M.con().put(0, "RTest0_Shell::disconnect(\"%s\")\n", command);
    return true;
  }
  bool process_command(BCStr command, BCStr args) {
    M.con().put(0, "RTest0_Shell::command(\"%s\", \"%s\")\n", command, args);
    return true;
  }
  //
  UMODSYS_SHELL(test0::RTest0_Shell, 1, IShell)
};

static tl::TTypeStaticHolder<RTest0_Shell> shell;
*/
struct RTest0_ModuleReg : public IModuleReg {
  UMODSYS_CORE_MODREG_DEF(RTest0_ModuleReg)
  //
  RTest0_ModuleReg(void) 
  : UMODSYS_MODULEREG(
      "tests::test0", 
      UMODSYS_VERSION_PLUGIN_TEST_TEST0, 
      "Test 2 - module"
    ) {
      s_dbg.menable();
//    M.con().put(0, "RTest0_ModuleReg()\n");
  }
  ~RTest0_ModuleReg(void) {
//    M.con().put(0, "~RTest0_ModuleReg()\n");
  }
  bool do_open(void) {
    s_dbg.put(0, cl_Debug, "RTest0_ModuleReg::open()\n");
//    shell.init(); reg(shell);
    return true;
  }
  bool do_close(void) {
    s_dbg.put(0, cl_Debug, "RTest0_ModuleReg::close()\n");
//    unreg(shell); shell.deinit();
    return true;
  }
};

UMODSYS_CORE_MODREG_BODY(RTest0_ModuleReg)

//IModule* RTest0_Shell::get_module(void) const { return reg.module; }

} // namespace test0
} // namespace UModSys


