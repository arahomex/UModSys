#include "umodsys.mod.media.zlib.h"

#include "version_module.h"

UMODSYS_MODULE_BEGIN(media, zlib)
UMODSYS_MODULE_BODY0()

#include "umodsys.mod.media.zlib.Tarcframe.h"

struct RModuleReg : public IModuleReg {
  UMODSYS_BASE_MODREG_DEF(RModuleReg)
  //
  RModuleReg(void) 
  : UMODSYS_MODULEREG(
    "media::zlib", 
    UMODSYS_VERSION_MOD_MEDIA_ZLIB,
    ""
  ) {}
  ~RModuleReg(void) {}
  bool do_open(void) { RGenerator::s_reg(this); return true; }
  bool do_close(void) { RGenerator::s_unreg(this); return true; }
};

UMODSYS_BASE_MODREG_BODY(RModuleReg)
UMODSYS_BASE_GENERATOR_BODY(RGenerator, RModuleReg)

UMODSYS_MODULE_END()
