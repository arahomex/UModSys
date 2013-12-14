#include "umodsys.mod.2d.stdlib.h"

#include "version_module.h"

UMODSYS_MODULE_BEGIN(lib2d, stdlib)
UMODSYS_MODULE_BODY0()

struct RModuleReg : public IModuleReg {
  UMODSYS_BASE_MODREG_DEF(RModuleReg)
  //
  RModuleReg(void) 
  : IModuleReg(
    "lib2d::stdlib", 
    UMODSYS_VERSION_MOD_2D_STDLIB_MAJOR, 
    UMODSYS_VERSION_MOD_2D_STDLIB_MINOR, 
    ""
  ) {}
  ~RModuleReg(void) {}
  bool do_open(void) { RGenerator::s_reg(this); return true; }
  bool do_close(void) { RGenerator::s_unreg(this); return true; }
};

UMODSYS_BASE_MODREG_BODY(RModuleReg)
UMODSYS_BASE_GENERATOR_BODY(RGenerator, RModuleReg)

UMODSYS_MODULE_END()