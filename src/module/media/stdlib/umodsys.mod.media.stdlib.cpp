#include "umodsys.mod.media.stdlib.h"

#include "version_module.h"

UMODSYS_MODULE_BEGIN(media, stdlib)
UMODSYS_MODULE_BODY0()

#include "umodsys.mod.media.stdlib.Tutilities.h"
#include "umodsys.mod.media.stdlib.Tbinarchiveframe.h"
#include "umodsys.mod.media.stdlib.Tlibbintree.h"
#include "umodsys.mod.media.stdlib.Tlibbincache.h"
#include "umodsys.mod.media.stdlib.Tliblayered.h"
#include "umodsys.mod.media.stdlib.Tlibobjcache.h"
#include "umodsys.mod.media.stdlib.Tlibobjfilter.h"

struct RModuleReg : public IModuleReg {
  UMODSYS_CORE_MODREG_DEF(RModuleReg)
  //
  RModuleReg(void)
  : UMODSYS_MODULEREG(
    "media::std", 
    UMODSYS_VERSION_MOD_MEDIA_STDLIB, 
    ""
  ) {}
  ~RModuleReg(void) {}
  bool do_open(void) { RGenerator::s_reg(this); return true; }
  bool do_close(void) { RGenerator::s_unreg(this); return true; }
};

UMODSYS_BASE_MODREG_BODY(RModuleReg)
UMODSYS_CORE_GENERATOR_BODY(RGenerator, RModuleReg)

UMODSYS_MODULE_END()
