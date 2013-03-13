#include <umodsys/base/system.h>
#include <umodsys/base/console.h>
#include <umodsys/base/module_impl.h>

namespace UModSys {
namespace test1 {

using namespace core;
using namespace base;

struct RTest1_ModuleReg : public IModuleReg {
  RTest1_ModuleReg(void) {
    so_enable();
  }
  ~RTest1_ModuleReg(void) {
  }
  bool open(ISystem* isys) {
    M.con().put(0, "RTest1_ModuleReg::open(%p)\n", isys);
    return true;
  }
  bool close(void) {
    M.con().put(0, "RTest1_ModuleReg::close()\n");
    return true;
  }
};

static RTest1_ModuleReg reg;

} // namespace UModSys
} // namespace test1


