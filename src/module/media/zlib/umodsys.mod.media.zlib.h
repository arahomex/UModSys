#ifndef __UMODSYS_MOD_MEDIA_ZLIB_H
#define __UMODSYS_MOD_MEDIA_ZLIB_H 1
/*************************************************************/
// file: umodsys.mod.media.zlib.h
// info: zlib media
/*************************************************************/

#include <umodsys/stdbase.h>
#include <umodsys/lib/media/libmedia.common.h>
#include <umodsys/lib/media/libmedia.utils.h>
#include <umodsys/lib/media/libmedia.archive.h>
#include <umodsys/core/syshlp.h>

#include <umodsys/core/arch_cvt.h>

#include <zlib.h>

#define U_MOD UMODSYS_MODULE_NAME(media, zlib)
UMODSYS_MODULE_BEGIN(media, zlib)

//***************************************
// TYPEDEFS
//***************************************

using namespace core;
using namespace base;
using namespace libmedia;

struct RGenerator;

//***************************************
// INCLUDE COMPONENTS
//***************************************

#include "umodsys.mod.media.zlib.defs.h"
#include "umodsys.mod.media.zlib.types.h"

//***************************************
// RGenerator
//***************************************

struct RGenerator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
    return t_names<RBinArchiveFrameClient_ZIP>(list)
    ;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    int rv = 0;
    t_types<RBinArchiveFrameClient_ZIP>(rv, list, name) 
    ;
    return rv;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    return t_gen_param<RBinArchiveFrameClient_ZIP>(this, obj, name, args)
    ;
  }
  //
  UMODSYS_BASE_GENERATOR_IMPLEMENT(U_MOD::RGenerator, 1, IGenerator)
};

//***************************************
// END
//***************************************

UMODSYS_MODULE_END()

#endif
