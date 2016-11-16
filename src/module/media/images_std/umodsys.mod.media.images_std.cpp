#include "umodsys.mod.media.images_std.h"

#include "version_module.h"

UMODSYS_MODULE_BEGIN(media, images_std)
UMODSYS_MODULE_BODY0()

struct RModuleReg : public IModuleReg {
  UMODSYS_CORE_MODREG_DEF(RModuleReg)
  //
  RModuleReg(void) 
  : UMODSYS_MODULEREG(
    "media::images_std", 
    UMODSYS_VERSION_MOD_MEDIA_IMAGESSTD, 
    ""
  ) {}
  ~RModuleReg(void) {}
  bool do_open(void) { RGenerator::s_reg(this); return true; }
  bool do_close(void) { RGenerator::s_unreg(this); return true; }
};

UMODSYS_CORE_MODREG_BODY(RModuleReg)
UMODSYS_CORE_GENERATOR_BODY(RGenerator, RModuleReg)

UMODSYS_MODULE_END()
