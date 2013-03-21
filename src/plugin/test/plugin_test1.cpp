#include <umodsys/base/system.h>
#include <umodsys/base/console.h>
#include <umodsys/base/module_impl.h>
#include <umodsys/base/bobjects.h>

namespace UModSys {
namespace test1 {

using namespace core;
using namespace base;

struct RTest1_Generator;

struct RTest1_Shell : public IShell {
  UMODSYS_BASE_SHELL_DEFINE(test1::RTest1_Shell, 1, IShell)
  //
  RTest1_Shell(DOwner *own) {
    rc_init(own);
    M.con().put(0, "RTest1_Shell()\n");
  }
  ~RTest1_Shell(void) {
    M.con().put(0, "~RTest1_Shell()\n");
  }
  //
  bool connect(void) {
    M.con().put(0, "RTest1_Shell::connect()\n");
    return true;
  }
  bool disconnect(void) {
    M.con().put(0, "RTest1_Shell::disconnect()\n");
    return true;
  }
  bool process_tick(const STimeMsec& delta) {
    M.con().put(0, "RTest1_Shell::tick(%g)\n", delta.fsec());
    return true;
  }
  bool process_command(int argc, const core::DCString argv[]) {
    M.con().put(0, "RTest1_Shell::command(#%d)\n", argc);
    return true;
  }
};

struct RTest1_Generator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
    list<<RTest1_Shell::_get_interface_type();
    return 1;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    if(name==RTest1_Shell::_get_interface_type())
      return RTest1_Shell::_get_interface_types(list);
    return 0;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    if(name==RTest1_Shell::_get_interface_type()) {
      obj = new RTest1_Shell(this);
      return obj.valid();
    }
    return false;
  }
  //
  UMODSYS_BASE_GENERATOR_DEF(test1::RTest1_Generator, 1, IGenerator)
};

struct RTest1_ModuleReg : public IModuleReg {
  UMODSYS_BASE_MODREG_DEF(RTest1_ModuleReg)
  //
  RTest1_ModuleReg(void) {
    minfo.set("Test1", 0, 1, "Test 1 - module");
//    M.con().put(0, "RTest1_ModuleReg()\n");
  }
  ~RTest1_ModuleReg(void) {
//    M.con().put(0, "~RTest1_ModuleReg()\n");
  }
  bool do_open(void) {
    M.con().put(0, "RTest1_ModuleReg::open()\n");
    RTest1_Generator::s_reg(this);
    return true;
  }
  bool do_close(void) {
    M.con().put(0, "RTest1_ModuleReg::close()\n");
    RTest1_Generator::s_unreg(this);
    return true;
  }
};

UMODSYS_BASE_MODREG_BODY(RTest1_ModuleReg)
UMODSYS_BASE_GENERATOR_FUNC(RTest1_Generator, RTest1_ModuleReg)

} // namespace test1
} // namespace UModSys


