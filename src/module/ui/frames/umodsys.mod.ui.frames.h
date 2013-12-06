#ifndef __UMODSYS_MOD_UI_FRAMES_H
#define __UMODSYS_MOD_UI_FRAMES_H 1
/*************************************************************/
// file: umodsys.mod.ui.frames.h
// info: 
/*************************************************************/

#include <umodsys/stdbase.h>
#include <umodsys/tl/composite/scatter_array.h>
#include <umodsys/tl/composite/dynarray.h>
//#include <umodsys/core/syshlp.h>

#include <umodsys/lib/ui/libui.common.h>
#include <umodsys/lib/ui/libui.frames.h>
#include <umodsys/lib/ui/libui.theme.h>
#include <umodsys/lib/2d/lib2d.driver.h>

#define U_MOD UMODSYS_MODULE_NAME(ui, frames)
UMODSYS_MODULE_BEGIN(ui, frames)

//***************************************
// TYPEDEFS
//***************************************

using namespace core;
using namespace base;
using namespace libui;
using namespace lib2d;

struct RGenerator;

struct RCollector;
struct RDialog;
struct RFrame_Common;
struct RCollectorTheme;

typedef tl::TDynarrayDynamic< tl::TRefObject<RFrame_Common> > Frames;
typedef lib2d::IMultiImage::P PFont;

//***************************************
// INCLUDE COMPONENTS
//***************************************

#include "umodsys.mod.ui.frames.utils.h"
#include "umodsys.mod.ui.frames.common.h"
#include "umodsys.mod.ui.frames.ctrl.h"
#include "umodsys.mod.ui.frames.dialog.h"
#include "umodsys.mod.ui.frames.static.h"
#include "umodsys.mod.ui.frames.dynamic.h"
#include "umodsys.mod.ui.frames.edit.h"
#include "umodsys.mod.ui.frames.list.h"
#include "umodsys.mod.ui.frames.collector.h"
#include "umodsys.mod.ui.frames.inlines.h"

//***************************************
// RGenerator
//***************************************

struct RGenerator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
    return t_names<RCollector>(list)
//         + t_names<RMultiImage2D_SDL_ttf>(list)
    ;
    return 0;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    int rv = 0;
    t_types<RCollector>(rv, list, name) 
//    || t_types<RMultiImage2D_SDL_ttf>(rv, list, name) 
    ;
    return rv;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    return t_gen_param<RCollector>(this, obj, name, args)
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
