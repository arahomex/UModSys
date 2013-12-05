#include "umodsys.mod.2d.lib2d.h"

#include "version_module.h"

UMODSYS_MODULE_BEGIN(lib2d, lib2d)
UMODSYS_MODULE_BODY0()

struct RModuleReg : public IModuleReg {
  UMODSYS_BASE_MODREG_DEF(RModuleReg)
  //
  RModuleReg(void) 
  : IModuleReg(
    "lib2d::lib2d", 
    UMODSYS_VERSION_MOD_2D_LIB2D_MAJOR, 
    UMODSYS_VERSION_MOD_2D_LIB2D_MINOR, 
    ""
  ) {}
  ~RModuleReg(void) {}
  bool do_open(void) { RGenerator::s_reg(this); return true; }
  bool do_close(void) { RGenerator::s_unreg(this); return true; }
};

UMODSYS_BASE_MODREG_BODY(RModuleReg)
UMODSYS_BASE_GENERATOR_BODY(RGenerator, RModuleReg)

UMODSYS_MODULE_END()
