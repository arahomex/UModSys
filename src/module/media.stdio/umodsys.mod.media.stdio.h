#ifndef __UMODSYS_MOD_MEDIA_STDIO_H
#define __UMODSYS_MOD_MEDIA_STDIO_H 1
/*************************************************************/
// file: umodsys/base/module.h
// info: base module object
/*************************************************************/

#include <io.h>

#include <umodsys/stdbase.h>
#include <umodsys/lib/media/libmedia.common.h>
#include <umodsys/core/syshlp.h>

UMODSYS_MODULE_BEGIN(media, stdio)

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

#include "umodsys.mod.media.stdio.streams.h"
#include "umodsys.mod.media.stdio.archive.h"

//***************************************
// RGenerator
//***************************************

struct RGenerator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
//    M.dbg_put("{{RStreamReader_FILE::_get_interface_type()->name=\"%s\"}}\n", RStreamReader_FILE::_get_interface_type()->name);
    return t_names<RStreamReader_FILE>(list)
         + t_names<RStreamWriter_FILE>(list)
         + t_names<RDataArchiver_OsDir>(list)
    ;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    int rv = 0;
    t_types<RStreamReader_FILE>(rv, list, name) 
      || t_types<RStreamWriter_FILE>(rv, list, name)
      || t_types<RDataArchiver_OsDir>(rv, list, name)
    ;
    return rv;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    return t_gen_param<RStreamReader_FILE>(this, obj, name, args)
        || t_gen_param<RStreamWriter_FILE>(this, obj, name, args)
        || t_gen_param<RDataArchiver_OsDir>(this, obj, name, args)
    ;
  }
  //
  UMODSYS_BASE_GENERATOR_IMPLEMENT(UMODSYS_MODULE_NAME(media,stdio)::RGenerator, 1, IGenerator)
};

//***************************************
// END
//***************************************

UMODSYS_MODULE_END()

#endif
