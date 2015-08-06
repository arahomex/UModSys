#ifndef __UMODSYS_MOD_MEDIA_STD_H
#define __UMODSYS_MOD_MEDIA_STD_H 1
/*************************************************************/
// file: umodsys.mod.media.std.h
// info: media std libraries
/*************************************************************/

#include <umodsys/stdbase.h>
#include <umodsys/lib/media/libmedia.common.h>
#include <umodsys/lib/media/libmedia.library.h>
#include <umodsys/lib/media/libmedia.archive.h>
#include <umodsys/lib/media/libmedia.utils.h>
#include <umodsys/core/syshlp.h>

#include <umodsys/tl/container/dynarray.h>
#include <umodsys/tl/container/scatter_array.h>
#include <umodsys/tl/container/set.h>

#define U_MOD UMODSYS_MODULE_NAME(media, stdlib)
UMODSYS_MODULE_BEGIN(media, stdlib)

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

#include "umodsys.mod.media.stdlib.types.h"

//***************************************
// RGenerator
//***************************************

struct RGenerator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
//    M.dbg_put("{{RStreamReader_FILE::_root_get_interface_type()->name=\"%s\"}}\n", RStreamReader_FILE::_root_get_interface_type()->name);
    
    return t_names<RLibraryBinCache>(list)
         + t_names<RLibraryBinTree>(list)
         + t_names<RLibraryLayered>(list)
         + t_names<RLibraryObjFilter>(list)
         + t_names<RLibraryObjCache>(list)
         + t_names<RBinArchiveFrame>(list)
         + t_names<RUtilities>(list)
    ;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    int rv = 0;
    t_types<RLibraryBinCache>(rv, list, name) 
      || t_types<RLibraryBinTree>(rv, list, name)
      || t_types<RLibraryLayered>(rv, list, name)
      || t_types<RLibraryObjFilter>(rv, list, name)
      || t_types<RLibraryObjCache>(rv, list, name)
      || t_types<RBinArchiveFrame>(rv, list, name)
      || t_types<RUtilities>(rv, list, name)
    ;
    return rv;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    return t_gen_param<RLibraryBinCache>(this, obj, name, args)
        || t_gen_param<RLibraryBinTree>(this, obj, name, args)
        || t_gen_param<RLibraryLayered>(this, obj, name, args)
        || t_gen_param<RLibraryObjFilter>(this, obj, name, args)
        || t_gen_param<RLibraryObjCache>(this, obj, name, args)
        || t_gen_param<RBinArchiveFrame>(this, obj, name, args)
        || t_gen_param<RUtilities>(this, obj, name, args)
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
