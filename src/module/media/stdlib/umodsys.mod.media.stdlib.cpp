#include "umodsys.mod.media.stdlib.h"

#include "version_module.h"

UMODSYS_MODULE_BEGIN(media, stdlib)
UMODSYS_MODULE_BODY0()

struct RModuleReg : public IModuleReg {
  UMODSYS_BASE_MODREG_DEF(RModuleReg)
  //
  RModuleReg(void)
  : IModuleReg(
    "media::std", 
    UMODSYS_VERSION_MOD_MEDIA_STDLIB_MAJOR, 
    UMODSYS_VERSION_MOD_MEDIA_STDLIB_MINOR, 
    ""
  ) {}
  ~RModuleReg(void) {}
  bool do_open(void) { RGenerator::s_reg(this); return true; }
  bool do_close(void) { RGenerator::s_unreg(this); return true; }
};

UMODSYS_BASE_MODREG_BODY(RModuleReg)
UMODSYS_BASE_GENERATOR_BODY(RGenerator, RModuleReg)

UMODSYS_MODULE_END()
