#ifndef __UMODSYS_MOD_UI_SDLCORE_H
#define __UMODSYS_MOD_UI_SDLCORE_H 1
/*************************************************************/
// file: umodsys.mod.ui.SDL_core.h
// info: 
/*************************************************************/

#include <umodsys/stdbase.h>
#include <umodsys/tl/container/scatter_array.h>
//#include <umodsys/core/syshlp.h>

#include <umodsys/lib/ui/libui.common.h>
#include <umodsys/lib/2d/lib2d.driver.h>
#include <umodsys/lib/3d/lib3d.driver.h>

#include "SDL.h"
#include "SDL_ttf.h"

#define U_MOD UMODSYS_MODULE_NAME(ui, SDL_core)
UMODSYS_MODULE_BEGIN(ui, SDL_core)

//***************************************
// TYPEDEFS
//***************************************

using namespace core;
using namespace base;
using namespace libui;
using namespace lib2d;

struct RGenerator;

//***************************************
// INCLUDE COMPONENTS
//***************************************

typedef struct RRenderDriver2D RRenderDriver;
typedef SDL_Texture* SMultiImage2D_SDL_ttf_tex;

#include "umodsys.mod.ui.SDL_core.utils.h"
#include "umodsys.mod.ui.SDL_core.types.h"
#include "umodsys.mod.ui.SDL_core.type_font.h"
#include "umodsys.mod.ui.SDL_core.typesx.h"
#include "umodsys.mod.ui.SDL_core.inlines.h"

//***************************************
// RGenerator
//***************************************

struct RGenerator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
    return t_names<RTerminalX>(list)
//         + t_names<RMultiImage2D_SDL_ttf>(list)
    ;
    return 0;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    int rv = 0;
    t_types<RTerminalX>(rv, list, name) 
//    || t_types<RMultiImage2D_SDL_ttf>(rv, list, name) 
    ;
    return rv;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    return t_gen_param<RTerminalX>(this, obj, name, args)
//      || t_gen_param<RMultiImage2D_SDL_ttf>(this, obj, name, args)
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
