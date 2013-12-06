#ifndef __UMODSYS_MOD_MEDIA_STD_H
#define __UMODSYS_MOD_MEDIA_STD_H 1
/*************************************************************/
// file: umodsys.mod.media.std.h
// info: media std libraries
/*************************************************************/

#include <umodsys/stdbase.h>
#include <umodsys/lib/media/libmedia.common.h>
#include <umodsys/lib/media/libmedia.library.h>
#include <umodsys/lib/media/libmedia.utils.h>
#include <umodsys/core/syshlp.h>

#include <umodsys/tl/composite/dynarray.h>
#include <umodsys/tl/composite/scatter_array.h>
#include <umodsys/tl/composite/set.h>

#define U_MOD UMODSYS_MODULE_NAME(media, std)
UMODSYS_MODULE_BEGIN(media, std)

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

#include "umodsys.mod.media.std.libbincache.h"
#include "umodsys.mod.media.std.libbintree.h"
#include "umodsys.mod.media.std.liblayered.h"
#include "umodsys.mod.media.std.libobjcache.h"
#include "umodsys.mod.media.std.libobjfilter.h"

DMediaFlags RLibraryBinCache::auto_flags;
DMediaFlags RLibraryBinTree::auto_flags;
DMediaFlags RLibraryObjCache::auto_flags;
DMediaFlags RLibraryObjFilter::auto_flags;
DMediaFlags RLibraryLayered::auto_flags( mf_safe::Yes );

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
    ;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    int rv = 0;
    t_types<RLibraryBinCache>(rv, list, name) 
      || t_types<RLibraryBinTree>(rv, list, name)
      || t_types<RLibraryLayered>(rv, list, name)
      || t_types<RLibraryObjFilter>(rv, list, name)
      || t_types<RLibraryObjCache>(rv, list, name)
    ;
    return rv;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    return t_gen_param<RLibraryBinCache>(this, obj, name, args)
        || t_gen_param<RLibraryBinTree>(this, obj, name, args)
        || t_gen_param<RLibraryLayered>(this, obj, name, args)
        || t_gen_param<RLibraryObjFilter>(this, obj, name, args)
        || t_gen_param<RLibraryObjCache>(this, obj, name, args)
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
