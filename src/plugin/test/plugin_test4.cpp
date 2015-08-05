#include <umodsys/base/module.h>
#include <umodsys/base/system.h>
#include <umodsys/base/console.h>
#include <umodsys/base/module_impl.h>
#include <umodsys/base/bobjects.h>
#include <umodsys/base/shell.h>

#include "version_plugin_test4.h"

namespace UModSys {
namespace test4 {

using namespace core;
using namespace base;

SDebug s_dbg;

/*
struct RTest4_Shell : public IShell {
  RTest4_Shell(void) {
    M.con().put(0, "RTest4_Shell()\n");
  }
  ~RTest4_Shell(void) {
    M.con().put(0, "~RTest4_Shell()\n");
  }
  IModule* get_module(void) const;
  bool connect(BCStr command) {
    M.con().put(0, "RTest4_Shell::connect(\"%s\")\n", command);
    return true;
  }
  bool disconnect(BCStr command) {
    M.con().put(0, "RTest4_Shell::disconnect(\"%s\")\n", command);
    return true;
  }
  bool process_command(BCStr command, BCStr args) {
    M.con().put(0, "RTest4_Shell::command(\"%s\", \"%s\")\n", command, args);
    return true;
  }
  //
  UMODSYS_SHELL(test4::RTest4_Shell, 1, IShell)
};

static tl::TTypeStaticHolder<RTest4_Shell> shell;
*/
struct RTest4_ModuleReg : public IModuleReg {
  UMODSYS_BASE_MODREG_DEF(RTest4_ModuleReg)
  //
  RTest4_ModuleReg(void) 
  : UMODSYS_MODULEREG(
      "tests::test4", 
      UMODSYS_VERSION_PLUGIN_TEST_TEST4, 
      "Test 2 - module"
    ) {
      s_dbg.menable();
//    M.con().put(0, "RTest4_ModuleReg()\n");
  }
  ~RTest4_ModuleReg(void) {
//    M.con().put(0, "~RTest4_ModuleReg()\n");
  }
  bool do_open(void) {
    s_dbg.put(0, cl_Debug, "RTest4_ModuleReg::open()\n");
//    shell.init(); reg(shell);
    return true;
  }
  bool do_close(void) {
    s_dbg.put(0, cl_Debug, "RTest4_ModuleReg::close()\n");
//    unreg(shell); shell.deinit();
    return true;
  }
};

UMODSYS_BASE_MODREG_BODY(RTest4_ModuleReg)

//IModule* RTest4_Shell::get_module(void) const { return reg.module; }

} // namespace test4
} // namespace UModSys


