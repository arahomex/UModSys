#include "umodsys.mod.ui.SDL_core.h"

#include "version_module.h"

UMODSYS_MODULE_BEGIN(ui, SDL_core)
UMODSYS_MODULE_BODY0()

#include "umodsys.mod.ui.SDL_core.swindow.h"
#include "umodsys.mod.ui.SDL_core.font.h"
#include "umodsys.mod.ui.SDL_core.window.h"
#include "umodsys.mod.ui.SDL_core.mouse.h"
#include "umodsys.mod.ui.SDL_core.keyboard.h"
#include "umodsys.mod.ui.SDL_core.terminal.h"
#include "umodsys.mod.ui.SDL_core.terminalx.h"

struct RModuleReg : public IModuleReg {
  UMODSYS_CORE_MODREG_DEF(RModuleReg)
  //
  RModuleReg(void) 
  : UMODSYS_MODULEREG(
    "ui::SDL_core", 
    UMODSYS_VERSION_MOD_UI_SDLCORE, 
    ""
  ) {}
  ~RModuleReg(void) {}
  bool do_open(void) { RGenerator::s_reg(this); return true; }
  bool do_close(void) { RGenerator::s_unreg(this); return true; }
};

UMODSYS_BASE_MODREG_BODY(RModuleReg)
UMODSYS_CORE_GENERATOR_BODY(RGenerator, RModuleReg)

UMODSYS_MODULE_END()
