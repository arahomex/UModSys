#include <umodsys/lib/3d/lib3d.driver.h>
#include <umodsys/lib/3d/lib3d.material.h>

#include "umodsys.mod.ui.SDL_GL.h"

#include "version_module.h"

UMODSYS_MODULE_BEGIN(ui, SDL_GL)
UMODSYS_MODULE_BODY0()

#include "../SDL_core/umodsys.mod.ui.SDL_core.swindow.h"
#include "../SDL_core/umodsys.mod.ui.SDL_core.keyboard.h"
#include "../SDL_core/umodsys.mod.ui.SDL_core.mouse.h"
#include "../SDL_core/umodsys.mod.ui.SDL_core.terminal.h"
#include "../SDL_core/umodsys.mod.ui.SDL_core.font.h"

#include "umodsys.mod.ui.SDL_GL.u_tex.h"
#include "umodsys.mod.ui.SDL_GL.u_texmax.h"
#include "umodsys.mod.ui.SDL_GL.u_rtex.h"


#include "umodsys.mod.ui.SDL_GL.w_utl.h"

#include "umodsys.mod.ui.SDL_GL.w_2d_info.h"
#include "umodsys.mod.ui.SDL_GL.w_2d_setup.h"
#include "umodsys.mod.ui.SDL_GL.w_2d_draw.h"
#include "umodsys.mod.ui.SDL_GL.w_2d_phase.h"

#include "umodsys.mod.ui.SDL_GL.w_3d_info.h"
#include "umodsys.mod.ui.SDL_GL.w_3d_setup.h"
#include "umodsys.mod.ui.SDL_GL.w_3d_draw.h"
#include "umodsys.mod.ui.SDL_GL.w_3d_phase.h"

#include "umodsys.mod.ui.SDL_GL.vbo.h"
#include "umodsys.mod.ui.SDL_GL.terminalx.h"
#include "umodsys.mod.ui.SDL_GL.glfuncload.h"
#include "umodsys.mod.ui.SDL_GL.glfuncbody.h"

struct RModuleReg : public IModuleReg {
  UMODSYS_CORE_MODREG_DEF(RModuleReg)
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

UMODSYS_CORE_MODREG_BODY(RModuleReg)
UMODSYS_CORE_GENERATOR_BODY(RGenerator, RModuleReg)

UMODSYS_MODULE_END()

