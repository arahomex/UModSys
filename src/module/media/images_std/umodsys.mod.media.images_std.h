#ifndef __UMODSYS_MOD_MEDIA_IMAGES_STD_H
#define __UMODSYS_MOD_MEDIA_IMAGES_STD_H 1
/*************************************************************/
// file: umodsys.mod.media.images_std.h
// info: 
/*************************************************************/

#include <umodsys/stdbase.h>
#include <umodsys/lib/media/libmedia.common.h>
#include <umodsys/lib/media/libmedia.utils.h>
#include <umodsys/lib/2d/lib2d.image.h>

extern "C" {
  #include <jpeglib.h>
  #include <png.h>
} // extern "C"

#include <umodsys/core/syshlp.h>

UMODSYS_MODULE_BEGIN(media, images_std)

//***************************************
// TYPEDEFS
//***************************************

using namespace core;
using namespace base;
using namespace libmedia;
using namespace lib2d;

struct RGenerator;

//***************************************
// INCLUDE COMPONENTS
//***************************************

#include "umodsys.mod.media.images_std.filter.h"
#include "umodsys.mod.media.images_std.jpeg.h"
#include "umodsys.mod.media.images_std.png.h"

//***************************************
// RGenerator
//***************************************

struct RGenerator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
    return t_names<RMediaFilter>(list)
    ;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    int rv = 0;
    t_types<RMediaFilter>(rv, list, name) 
    ;
    return rv;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    return t_gen_param<RMediaFilter>(this, obj, name, args)
    ;
  }
  //
  UMODSYS_BASE_GENERATOR_IMPLEMENT(UMODSYS_MODULE_NAME(media,images_std)::RGenerator, 1, IGenerator)
};

//***************************************
// END
//***************************************

UMODSYS_MODULE_END()

#endif
