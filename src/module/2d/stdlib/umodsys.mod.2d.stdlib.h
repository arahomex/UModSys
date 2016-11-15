#ifndef __UMODSYS_MOD_2D_STDLIB_H
#define __UMODSYS_MOD_2D_STDLIB_H 1
/*************************************************************/
// file: umodsys.mod.2d.stdlib.h
// info: default 2d classes:
//       - memory image2d
//       - memory font
//       - image2d convertor
/*************************************************************/

#include <umodsys/stdbase.h>
#include <umodsys/tl/container/scatter_array.h>
#include <umodsys/tl/container/dynarray.h>
//#include <umodsys/common/stdsyshlp.h>

#include <umodsys/lib/2d/lib2d.image.h>

#define U_MOD UMODSYS_MODULE_NAME(lib2d, stdlib)
UMODSYS_MODULE_BEGIN(lib2d, stdlib)

//***************************************
// TYPEDEFS
//***************************************

using namespace core;
using namespace base;
using namespace UModSys::lib2d;

struct RGenerator;

struct RImage_Memory;

//***************************************
// INCLUDE COMPONENTS
//***************************************

#include "umodsys.mod.2d.stdlib.convert.h"
#include "umodsys.mod.2d.stdlib.types.h"
#include "umodsys.mod.2d.stdlib.convertfast.h"
#include "umodsys.mod.2d.stdlib.convertslow.h"

//***************************************
// RGenerator
//***************************************

struct RGenerator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
    return t_names<RImage_Memory>(list)
         + t_names<RImageFactory_Memory>(list)
         + t_names<RMultiImage>(list)
    ;
    return 0;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    int rv = 0;
    t_types<RImage_Memory>(rv, list, name) 
    || t_types<RImageFactory_Memory>(rv, list, name) 
    || t_types<RMultiImage>(rv, list, name) 
    ;
    return rv;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    return t_gen_param<RImage_Memory>(this, obj, name, args)
      || t_gen_param<RImageFactory_Memory>(this, obj, name, args)
      || t_gen_param<RMultiImage>(this, obj, name, args)
    ;
    return false;
  }
  //
  UMODSYS_BASE_GENERATOR_IMPLEMENT(U_MOD::RGenerator, 1, IGenerator)
};

//***************************************
// END
//***************************************

UMODSYS_MODULE_END()

#endif
