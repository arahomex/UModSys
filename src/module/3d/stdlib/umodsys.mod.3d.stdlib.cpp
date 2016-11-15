#include "umodsys.mod.3d.stdlib.h"

#include "version_module.h"

UMODSYS_MODULE_BEGIN(lib3d, stdlib)
UMODSYS_MODULE_BODY0()

#include "umodsys.mod.3d.stdlib.Trenderer.h"
#include "umodsys.mod.3d.stdlib.Tvisualizerscene.h"
#include "umodsys.mod.3d.stdlib.Tvisualscenemapper_direct.h"
#include "umodsys.mod.3d.stdlib.Tobj_camera.h"
#include "umodsys.mod.3d.stdlib.Tobj_primitive.h"

struct RModuleReg : public IModuleReg {
  UMODSYS_CORE_MODREG_DEF(RModuleReg)
  //
  RModuleReg(void) 
  : UMODSYS_MODULEREG(
    "lib3d::stdlib", 
    UMODSYS_VERSION_MOD_3D_STDLIB, 
    ""
  ) {}
  ~RModuleReg(void) {}
  bool do_open(void) { RGenerator::s_reg(this); return true; }
  bool do_close(void) { RGenerator::s_unreg(this); return true; }
};

UMODSYS_BASE_MODREG_BODY(RModuleReg)
UMODSYS_CORE_GENERATOR_BODY(RGenerator, RModuleReg)

UMODSYS_MODULE_END()
