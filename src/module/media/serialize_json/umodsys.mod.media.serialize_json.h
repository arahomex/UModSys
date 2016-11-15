#ifndef __UMODSYS_MOD_MEDIA_IMAGES_STD_H
#define __UMODSYS_MOD_MEDIA_IMAGES_STD_H 1
/*************************************************************/
// file: umodsys.mod.media.images_std.h
// info: 
/*************************************************************/

#include <umodsys/stdcore.h>
#include <umodsys/tl/container/dynarray.h>
#include <umodsys/lib/media/libmedia.common.h>
#include <umodsys/lib/media/libmedia.serialize.h>

#include <umodsys/common/stdsyshlp.h>

#define U_MOD UMODSYS_MODULE_NAME(media, serialize_json)
UMODSYS_MODULE_BEGIN(media, serialize_json)

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

BChar s_escapes[0x100][8];

inline void init_escapes(void) 
{
  if(s_escapes[0][0]!='\0')
    return;
  tl::su::scpy(s_escapes['\"'],   "\\\"");
  tl::su::scpy(s_escapes['\\'],   "\\\\");
  tl::su::scpy(s_escapes['/'],    "\\/");
  tl::su::scpy(s_escapes['\b'],   "\\b");
  tl::su::scpy(s_escapes['\f'],   "\\f");
  tl::su::scpy(s_escapes['\n'],   "\\n");
  tl::su::scpy(s_escapes['\r'],   "\\r");
  tl::su::scpy(s_escapes['\t'],   "\\t");
  for(int i=0; i<0x20; i++) {
    if(s_escapes[i][0]=='\0') {
      snprintf(s_escapes[i], sizeof(s_escapes[i]), "\\u%04x", i);
    }
  }
}

#include "umodsys.mod.media.serialize_json.reader.h"
#include "umodsys.mod.media.serialize_json.writer.h"

//***************************************
// RGenerator
//***************************************

struct RGenerator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
    return t_names<RSerializeWriter>(list)
    + t_names<RSerializeReader>(list)
    ;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    int rv = 0;
    t_types<RSerializeReader>(rv, list, name) 
    || t_types<RSerializeWriter>(rv, list, name) 
    ;
    return rv;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    init_escapes();
    return t_gen_param<RSerializeReader>(this, obj, name, args)
      || t_gen_param<RSerializeWriter>(this, obj, name, args)
    ;
  }
  //
  UMODSYS_CORE_GENERATOR_IMPLEMENT(U_MOD::RGenerator, 1, IGenerator)
};

//***************************************
// END
//***************************************

UMODSYS_MODULE_END()

#endif
