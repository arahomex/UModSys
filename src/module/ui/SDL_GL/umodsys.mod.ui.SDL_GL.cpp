#include <umodsys/lib/3d/lib3d.driver.h>
#include <umodsys/lib/3d/lib3d.material.h>

#if 0
#include "umodsys.mod.ui.SDL_GL.h"

#include "version_module.h"

UMODSYS_MODULE_BEGIN(ui, SDL_GL)
UMODSYS_MODULE_BODY0()

struct RModuleReg : public IModuleReg {
  UMODSYS_BASE_MODREG_DEF(RModuleReg)
  //
  RModuleReg(void) 
  : UMODSYS_MODULEREG(
    "ui::SDL_GL", 
    UMODSYS_VERSION_MOD_UI_SDLGL, 
    "SDL2 OpenGL library"
  ) {}
  ~RModuleReg(void) {}
  bool do_open(void) { RGenerator::s_reg(this); return true; }
  bool do_close(void) { RGenerator::s_unreg(this); return true; }
};

UMODSYS_BASE_MODREG_BODY(RModuleReg)
UMODSYS_BASE_GENERATOR_BODY(RGenerator, RModuleReg)

UMODSYS_MODULE_END()

#endif
