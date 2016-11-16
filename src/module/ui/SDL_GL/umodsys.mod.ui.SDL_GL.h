#ifndef __UMODSYS_MOD_UI_SDLGL_H
#define __UMODSYS_MOD_UI_SDLGL_H 1
/*************************************************************/
// file: umodsys.mod.ui.SDL_GL.h
// info: 
/*************************************************************/

#include <umodsys/stdcore.h>
#include <umodsys/tl/container/scatter_array.h>
//#include <umodsys/common/stdsyshlp.h>

#include <umodsys/lib/ui/libui.common.h>
#include <umodsys/lib/2d/lib2d.driver.h>
#include <umodsys/lib/2d/lib2d.utils.h>
#include <umodsys/lib/3d/lib3d.driver.h>
#include <umodsys/lib/media/libmedia.common.h>

#include "SDL.h"
#include "SDL_ttf.h"

#ifndef SDL_USE_OPENGL_ES
  #include "SDL_opengl.h"
#else
  #include "SDL_opengles.h"
#endif

#define U_MOD UMODSYS_MODULE_NAME(ui, SDL_GL)
UMODSYS_MODULE_BEGIN(ui, SDL_GL)

//***************************************
// TYPEDEFS
//***************************************

using namespace core;
using namespace base;
using namespace libui;
using namespace lib3d;

struct RGenerator;

//***************************************
// INCLUDE COMPONENTS
//***************************************

typedef struct RRenderDriver3D RRenderDriver;
typedef struct STextureGL SMultiImage2D_SDL_ttf_tex;

#include "umodsys.mod.ui.SDL_GL.glfunc.h"

#include "../SDL_core/umodsys.mod.ui.SDL_core.utils.h"
#include "../SDL_core/umodsys.mod.ui.SDL_core.types.h"
#include "../SDL_core/umodsys.mod.ui.SDL_core.type_font.h"

#include "umodsys.mod.ui.SDL_GL.typesx.h"

//***************************************
// RGenerator
//***************************************

struct RGenerator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
    return t_names<RTerminalX>(list)
    ;
    return 0;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    int rv = 0;
    t_types<RTerminalX>(rv, list, name) 
    ;
    return rv;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    return t_gen_param<RTerminalX>(this, obj, name, args)
    ;
    return false;
  }
  //
  UMODSYS_CORE_GENERATOR_IMPLEMENT(U_MOD::RGenerator, 1, IGenerator)
};

//***************************************
// END
//***************************************

UMODSYS_MODULE_END()

#endif
