#include <umodsys/base/module.h>
#include <umodsys/base/system.h>
#include <umodsys/base/console.h>
#include <umodsys/base/module_impl.h>
#include <umodsys/base/bobjects.h>
#include <umodsys/base/shell.h>

#include "version_plugin_test3.h"

namespace UModSys {
namespace test3 {

using namespace core;
using namespace base;

SDebug s_dbg;

/*
struct RTest3_Shell : public IShell {
  RTest3_Shell(void) {
    M.con().put(0, "RTest3_Shell()\n");
  }
  ~RTest3_Shell(void) {
    M.con().put(0, "~RTest3_Shell()\n");
  }
  IModule* get_module(void) const;
  bool connect(BCStr command) {
    M.con().put(0, "RTest3_Shell::connect(\"%s\")\n", command);
    return true;
  }
  bool disconnect(BCStr command) {
    M.con().put(0, "RTest3_Shell::disconnect(\"%s\")\n", command);
    return true;
  }
  bool process_command(BCStr command, BCStr args) {
    M.con().put(0, "RTest3_Shell::command(\"%s\", \"%s\")\n", command, args);
    return true;
  }
  //
  UMODSYS_SHELL(test3::RTest3_Shell, 1, IShell)
};

static tl::TTypeStaticHolder<RTest3_Shell> shell;
*/
struct RTest3_ModuleReg : public IModuleReg {
  UMODSYS_CORE_MODREG_DEF(RTest3_ModuleReg)
  //
  RTest3_ModuleReg(void) 
  : UMODSYS_MODULEREG(
      "tests::test3", 
      UMODSYS_VERSION_PLUGIN_TEST_TEST3, 
      "Test 2 - module"
    ) {
      s_dbg.menable();
//    M.con().put(0, "RTest3_ModuleReg()\n");
  }
  ~RTest3_ModuleReg(void) {
//    M.con().put(0, "~RTest3_ModuleReg()\n");
  }
  bool do_open(void) {
    s_dbg.put(0, cl_Debug, "RTest3_ModuleReg::open()\n");
//    shell.init(); reg(shell);
    return true;
  }
  bool do_close(void) {
    s_dbg.put(0, cl_Debug, "RTest3_ModuleReg::close()\n");
//    unreg(shell); shell.deinit();
    return true;
  }
};

UMODSYS_CORE_MODREG_BODY(RTest3_ModuleReg)

//IModule* RTest3_Shell::get_module(void) const { return reg.module; }

} // namespace test3
} // namespace UModSys


