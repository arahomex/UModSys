#ifndef __UMODSYS_LIB3D_RENDERSTATE_H
#define __UMODSYS_LIB3D_RENDERSTATE_H 1
/*************************************************************/
// file: umodsys/lib/lib3d/lib3d.renderstate.h
// info: visualizer
/*************************************************************/

#include <umodsys/lib/3d/lib3d.typedefs.h>
#include <umodsys/lib/3d/lib3d.common.h>
#include <umodsys/lib/3d/lib3d.objects.h>

namespace UModSys {
namespace lib3d {

//***************************************
// TYPES
//***************************************

struct SRenderState {
  IRenderer *renderer;
  IRenderDriver *driver;            // current driver
  DBlendColor color;
  IVisualObject* state;       // current state
  int phase;                        // pass to go
  int outlines;                     // draw extra: outlines
  // light information
  int nlights;                      // count of dynamic lights
  // camera info
  IVisualObject_Camera* camstate;   // camera state
  int extras;
  // camera info extra (requested)
  DPoint ortx;                      // right ort of camera
  DPoint orty;                      // up ort of camera
  DPoint ortz;                      // forward ort of camera
  DPoint origin0;                   // camera point in local coords
  const DMatrix4 *T_local;          // from local to world
  const DMatrix4 *T_camera;         // from camera to world
  DMatrix4 T_world_local;           // from world to local (revert local)
  DMatrix4 T_world_camera;          // from world to camera (revert camera)
  DMatrix4 T_local_camera;          // from local to camera (normal render)
  DMatrix4 T_camera_local;          // from camera to local (raytrace)
  int ncameraplanes;                // count of camera planes
  DPlane* cameraplanes_camera;      // camera planes
  DPlane* cameraplanes_world;       // world camera planes
  DPlane* cameraplanes_local;       // local camera planes
  //
  inline void extras_set(int mask) { extras |= mask; }
  inline void extras_reset(int mask) { extras &= ~mask; }
  inline int is_extras(int mask) const { return extras & mask; }
  inline bool must_extras(int mask) const { return (extras & mask)==mask; }
  //
  inline bool refill(int comps) const {
    if((comps & extras)==comps) return true; // already done
    return renderer->render_extra(*this, comps);
  }
  inline void link_camera(IVisualObject_Camera* cam) {
    camstate = cam;
    T_camera = &cam->T;
    extras_reset(rsc_AllCamera);
    extras_set(rsc_TCamera | rsc_Camera);
  }
  inline void link_camera(void) {
    camstate = NULL;
    T_camera = NULL;
    extras_reset(rsc_AllCamera);
  }
  inline void link(IVisualObject* vs) {
    state = vs;
    T_local = &vs->T;
    extras_reset(rsc_AllLocal);
    extras_set(rsc_TLocal);
  }
  inline void unlink(void) {
    state = NULL;
    T_local = NULL;
    extras_reset(rsc_AllLocal);
  }
  inline bool render(IVisualObject* vs) {
    if(vs==NULL)
      return reset();
    link(vs);
    bool rv = vs->render_state(*this);
    unlink();
    return rv;
  }
  inline bool reset(void) {
    return renderer->render_reset(*this);
  }
  //
  inline SRenderState& operator<<(IVisualObject* vs) { render(vs); return *this; }
  //
  SRenderState(IRenderer* rr, IRenderDriver *drv)
  : driver(drv), renderer(rr), 
    state(NULL), 
    // shape(NULL), render(NULL),
    camstate(NULL),
    //
    T_camera(NULL), T_local(NULL),
    extras(0), phase(0), outlines(0), nlights(0),
    ncameraplanes(0), cameraplanes_world(NULL), cameraplanes_local(NULL)
  {}
};

//***************************************
// END
//***************************************

} // namespace lib3d
} // namespace UModSys

#endif // __UMODSYS_LIB3D_RENDERSTATE_H
