#ifndef __UMODSYS_MOD_MEDIA_IMAGES_STD_H
#define __UMODSYS_MOD_MEDIA_IMAGES_STD_H 1
/*************************************************************/
// file: umodsys.mod.media.images_std.h
// info: 
/*************************************************************/

#include <umodsys/stdbase.h>
#include <umodsys/lib/ui/libui.common.h>
#include <umodsys/lib/ui/libui.term_con.h>

#include "SDL.h"
extern "C" {
} // extern "C"

#include <umodsys/core/syshlp.h>

UMODSYS_MODULE_BEGIN(ui, SDL_core)

//***************************************
// TYPEDEFS
//***************************************

using namespace core;
using namespace base;
using namespace libui;

struct RGenerator;

//***************************************
// INCLUDE COMPONENTS
//***************************************

#include "umodsys.mod.ui.SDL_core.terminal.h"
#include "umodsys.mod.ui.SDL_core.utils.h"

//***************************************
// RGenerator
//***************************************

struct RGenerator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
    return t_names<RTerminal_SDL_core>(list)
    ;
    return 0;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    int rv = 0;
    t_types<RTerminal_SDL_core>(rv, list, name) 
    ;
    return rv;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    return t_gen_param<RTerminal_SDL_core>(this, obj, name, args)
    ;
    return false;
  }
  //
  UMODSYS_BASE_GENERATOR_IMPLEMENT(UMODSYS_MODULE_NAME(ui,SDL_core)::RGenerator, 1, IGenerator)
};

//***************************************
// END
//***************************************

UMODSYS_MODULE_END()

#endif
