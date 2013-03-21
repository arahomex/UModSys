#include <umodsys/base/system.h>
#include <umodsys/base/console.h>
#include <umodsys/base/module_impl.h>
#include <umodsys/base/bobjects.h>

namespace UModSys {
namespace test2 {

using namespace core;
using namespace base;

struct RTest2_Shell : public IShell {
  RTest2_Shell(void) {
    M.con().put(0, "RTest2_Shell()\n");
  }
  ~RTest2_Shell(void) {
    M.con().put(0, "~RTest2_Shell()\n");
  }
  IModule* get_module(void) const;
  bool connect(BCStr command) {
    M.con().put(0, "RTest2_Shell::connect(\"%s\")\n", command);
    return true;
  }
  bool disconnect(BCStr command) {
    M.con().put(0, "RTest2_Shell::disconnect(\"%s\")\n", command);
    return true;
  }
  bool process_command(BCStr command, BCStr args) {
    M.con().put(0, "RTest2_Shell::command(\"%s\", \"%s\")\n", command, args);
    return true;
  }
  //
  UMODSYS_SHELL(test2::RTest2_Shell, 1, IShell)
};

static tl::TTypeStaticHolder<RTest2_Shell> shell;

struct RTest2_ModuleReg : public IModuleReg {
  RTest2_ModuleReg(void) {
    minfo.set("Test2", 0, 1, "Test 2 - module");
//    M.con().put(0, "RTest2_ModuleReg()\n");
  }
  ~RTest2_ModuleReg(void) {
//    M.con().put(0, "~RTest2_ModuleReg()\n");
  }
  bool do_open(void) {
    M.con().put(0, "RTest2_ModuleReg::open()\n");
    shell.init();
    reg(shell);
    return true;
  }
  bool do_close(void) {
    M.con().put(0, "RTest2_ModuleReg::close()\n");
    unreg(shell);
    shell.deinit();
    return true;
  }
};

static RTest2_ModuleReg reg;
static SModuleRegChain reg_chain(reg);

IModule* RTest2_Shell::get_module(void) const { return reg.module; }

} // namespace UModSys
} // namespace test2


