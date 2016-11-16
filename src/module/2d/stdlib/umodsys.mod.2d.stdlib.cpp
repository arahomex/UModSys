#include "umodsys.mod.2d.stdlib.h"

#include "version_module.h"

UMODSYS_MODULE_BEGIN(lib2d, stdlib)
UMODSYS_MODULE_BODY0()

#include "umodsys.mod.2d.stdlib.Timage.h"
#include "umodsys.mod.2d.stdlib.Timagefactory.h"
#include "umodsys.mod.2d.stdlib.Tmultiimage.h"

struct RModuleReg : public IModuleReg {
  UMODSYS_CORE_MODREG_DEF(RModuleReg)
  //
  RModuleReg(void) 
  : UMODSYS_MODULEREG(
    "lib2d::stdlib", 
    UMODSYS_VERSION_MOD_2D_STDLIB, 
    ""
  ) {}
  ~RModuleReg(void) {}
  bool do_open(void) { RGenerator::s_reg(this); return true; }
  bool do_close(void) { RGenerator::s_unreg(this); return true; }
};

UMODSYS_CORE_MODREG_BODY(RModuleReg)
UMODSYS_CORE_GENERATOR_BODY(RGenerator, RModuleReg)

UMODSYS_MODULE_END()
