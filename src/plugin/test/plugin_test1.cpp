#include <umodsys/base/system.h>
#include <umodsys/base/console.h>
#include <umodsys/base/module_impl.h>
#include <umodsys/base/bobjects.h>

namespace UModSys {
namespace test1 {

using namespace core;
using namespace base;

struct RTest1_Shell : public IShell {
  RTest1_Shell(void) {
    M.con().put(0, "RTest1_Shell()\n");
  }
  ~RTest1_Shell(void) {
    M.con().put(0, "~RTest1_Shell()\n");
  }
  IModule* get_module(void) const;
  bool connect(BCStr command) {
    M.con().put(0, "RTest1_Shell::connect(\"%s\")\n", command);
    return true;
  }
  bool disconnect(BCStr command) {
    M.con().put(0, "RTest1_Shell::disconnect(\"%s\")\n", command);
    return true;
  }
  bool process_command(BCStr command, BCStr args) {
    M.con().put(0, "RTest1_Shell::command(\"%s\", \"%s\")\n", command, args);
    return true;
  }
  //
  UMODSYS_SHELL(test1::RTest1_Shell, 1, IShell)
};

static tl::TTypeStaticHolder<RTest1_Shell> shell;

struct RTest1_ModuleReg : public IModuleReg {
  RTest1_ModuleReg(void) {
    minfo.set("Test1", 0, 1, "Test 1 module");
//    M.con().put(0, "RTest1_ModuleReg()\n");
  }
  ~RTest1_ModuleReg(void) {
//    M.con().put(0, "~RTest1_ModuleReg()\n");
  }
  bool open(ISystem* isys) {
    M.con().put(0, "RTest1_ModuleReg::open(%p)\n", isys);
    shell.init();
    return true;
  }
  bool close(void) {
    M.con().put(0, "RTest1_ModuleReg::close()\n");
    shell.deinit();
    return true;
  }
};

static RTest1_ModuleReg reg;
static SModuleRegChain reg_chain(reg);

IModule* RTest1_Shell::get_module(void) const { return reg.module; }

} // namespace UModSys
} // namespace test1


