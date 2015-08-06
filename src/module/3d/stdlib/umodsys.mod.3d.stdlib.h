#ifndef __UMODSYS_MOD_3D_STDLIB_H
#define __UMODSYS_MOD_3D_STDLIB_H 1
/*************************************************************/
// file: umodsys.mod.2d.stdlib.h
// info: default 3d classes:
//       - renderer
//       - scene
//       - camera
//       - visualizer - UI
/*************************************************************/

#include <umodsys/stdbase.h>
#include <umodsys/tl/container/scatter_array.h>
#include <umodsys/tl/container/dynarray.h>
//#include <umodsys/core/syshlp.h>

#include <umodsys/lib/3d/lib3d.common.h>
#include <umodsys/lib/3d/lib3d.renderer.h>
#include <umodsys/lib/3d/lib3d.driver.h>
#include <umodsys/lib/3d/lib3d.visual.h>
#include <umodsys/lib/3d/lib3d.logic.h>
#include <umodsys/lib/3d/lib3d.scene.h>
#include <umodsys/lib/3d/lib3d.objects.h>
#include <umodsys/lib/3d/lib3d.renderstate.h>

#define U_MOD UMODSYS_MODULE_NAME(lib3d, stdlib)
UMODSYS_MODULE_BEGIN(lib3d, stdlib)

//***************************************
// TYPEDEFS
//***************************************

using namespace core;
using namespace base;
using namespace UModSys::lib3d;

struct RGenerator;

//***************************************
// INCLUDE COMPONENTS
//***************************************

#include "umodsys.mod.3d.stdlib.types.h"
#include "umodsys.mod.3d.stdlib.types_obj_primitive.h"
#include "umodsys.mod.3d.stdlib.types_obj_camera.h"

//***************************************
// RGenerator
//***************************************

struct RGenerator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
    return t_names<RRenderer>(list)
         + t_names<RVisualizerScene>(list)
         + t_names<RVisualSceneMapper_Direct>(list)
    //
         + t_names<object_camera::RGeneral>(list)
         + t_names<object_primitive::RGeneral>(list)
    ;
    return 0;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    int rv = 0;
    t_types<RRenderer>(rv, list, name) 
      || t_types<RVisualizerScene>(rv, list, name) 
      || t_types<RVisualSceneMapper_Direct>(rv, list, name) 
      //
      || t_types<object_camera::RGeneral>(rv, list, name) 
      || t_types<object_primitive::RGeneral>(rv, list, name) 
    ;
    return rv;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    return t_gen_param<RRenderer>(this, obj, name, args)
      || t_gen_param<RVisualizerScene>(this, obj, name, args)
      || t_gen_param<RVisualSceneMapper_Direct>(this, obj, name, args)
      //
      || t_gen_param<object_camera::RGeneral>(this, obj, name, args)
      || t_gen_param<object_primitive::RGeneral>(this, obj, name, args)
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
