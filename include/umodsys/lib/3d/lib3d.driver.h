#ifndef __UMODSYS_LIB3D_DRIVER_H
#define __UMODSYS_LIB3D_DRIVER_H 1
/*************************************************************/
// file: umodsys/lib/lib3d/lib3d.driver.h
// info: driver 3d
/*************************************************************/

#include <umodsys/lib/3d/lib3d.renderer.h>
#include <umodsys/lib/3d/lib3d.material.h>
#include <umodsys/lib/3d/lib3d.geom.h>

#include <umodsys/lib/2d/lib2d.driver.h>

namespace UModSys {
namespace lib3d {

//***************************************
// TYPES
//***************************************

struct SRenderState {
  IRenderer *renderer;
  IScene* scene;
  IRenderDriver *driver;            // current driver
  DBlendColor color;
  const INodeObject* state;         // current state
  const IRenderObject* render;      // current state
  HSceneNode node;                  // current scene node
  int phase;                        // pass to go
  int outlines;                     // draw extra: outlines
  // light information
  int nlights;                      // count of dynamic lights
  // camera info
  INodeObject_Camera* camstate;     // camera state
  HSceneNode camera;                // camera scene node
  int extras;
  // camera info extra (requested)
  DPoint ortx;                      // right ort of camera
  DPoint orty;                      // up ort of camera
  DPoint ortz;                      // forward ort of camera
  DPoint origin0;                   // camera point in local coords
  DMatrix4 T_local_camera;          // local to camera/world
  const DMatrix4 *T_local_world;    // from local to world
  DMatrix4 T_camera_local;          // from camera to local
  const DMatrix4 *T_camera_world;   // from camera to world
  DMatrix4 T_world_local;           // from world to local
  DMatrix4 T_world_camera;          // from world to camera
  int ncameraplanes;                // count of camera planes
  DPlane* cameraplanes_camera;      // camera planes
  DPlane* cameraplanes_world;       // world camera planes
  DPlane* cameraplanes_local;       // local camera planes
  //
  bool refill(int comps) const {
    if((comps & extras)==comps) return true; // already done
    return renderer->render_extra(*this, comps);
  }
  //
  SRenderState(IRenderer* rr, IScene* sc, IRenderDriver *drv, HSceneNode n);
};

//***************************************
// INTERFACES
//***************************************

//***************************************
// IRenderDriver::

struct IRenderDriver : public lib2d::IRenderDriver {
  // -- camera setup
  virtual bool camera_ortho(const DPoint &center, const DPoint &size) =0;
  virtual bool camera_frustum(const DPoint &center, const DPoint &size) =0;
  virtual bool camera_frustum(const DPoint &center, const DTexPoint &fov, DScalar min, DScalar max) =0;
  // -- allocate vector-cache
  // all data will be reset after end of frame
/*
  virtual DPoint* alloc_points(int count) =0;
  virtual void alloc_points(const DPoint* x, int count) =0;
  virtual DColor* alloc_colors(int count) =0;
  virtual void alloc_colors(const DColor* x, int count) =0;
  virtual DColorAlpha* alloc_coloralphas(int count) =0;
  virtual void alloc_coloralphas(const DColorAlpha* x, int count) =0;
  virtual DTexPoint* alloc_texpoints(int count, int maplevel=0) =0;
  virtual void alloc_texpoints(const DTexPoint* x, int count, int maplevel=0) =0;
  virtual DNormal* alloc_normals(int count) =0;
  virtual void alloc_normals(const DNormal* x, int count) =0;
  virtual int* alloc_index(int count, int level=0) =0;
*/
  //
  virtual bool driver_is_reset(void) =0;
  //
  virtual void phase_start(int phaseid, int alpha_mode) =0; // begin desired phase
  virtual void phase_2d(bool nearmode=true) =0; // begin 2d phase (after or before all other)
  virtual void render_start(void) =0; // caches is filled, begin render elements
  virtual void render_stop(void) =0; // caches is not required anymore
  // -- register
  // return ptr, try to adapt data to required ptr
//  virtual IConsoleHFCSDisplay::P new_console_HFCSD(const SIntPoint& pos, const SIntPoint& size, const SParameters* params) =0;
  virtual ITexture::P register_tex(libmedia::ILibrary* mg, const DCString& texname, const SRenderMapFlags& deff) =0;
  virtual ITextureCells::P register_texcells(libmedia::ILibrary * mg, const DCString& texname, const SRenderMapFlags& deff) =0;
  // this one create a new box used for supply textures
  virtual ITexture::P register_tex(const DPoint2i& size, const SRenderMapFlags& deff, lib2d::eImageType type) =0;
  virtual IVertexArray::P create_array(const SVertexElemInfo layers[], int count) =0;
  //
  // -- setup dynamic lights, light indices: <=0 =error, 0x10000+ = HW, 0x20000+ = SW, 0x30000+ = omni
  virtual bool setup_clearlights(eLightType type=lt_All, bool emulated=true, bool hardware=true) =0;
  virtual int  setup_addlight(eLightType type, const SLight& light, const DMatrix4& T) =0;
  virtual bool setup_setlight(int idx, eLightType type, const SLight& light, const DMatrix4& T) =0;
  virtual bool setup_setlight(int idx, bool enabled) =0;
  virtual void setup_T(const DMatrix4& T) =0;
  virtual bool setup_material(const SMaterial *mat) =0; // NULL to disable materials
  virtual bool setup_array(const IVertexArray *va, int targets=~0, int layers=~0) =0;
  virtual bool setup_array(const IVertexArray *va, eVertexClass target, int layer) =0;
  virtual bool setup_array_none(void) =0;
  //
  // -- render primitives by lists
  virtual bool render_primitive(eRenderPrimitiveType primitive, int count, int start=0) =0;
  //
  virtual bool setup_state(const SRenderState& S) =0; // discarded on phase end or render start/stop
  virtual bool setup_2d(const DPoint2i* vsize, const DPoint2i* voffset=NULL,
                        const DTexPoint* relsize=NULL, const DTexPoint *reloffset=NULL) =0; // window coordinates
  // -- render pictures
  virtual void render_picture(const SPicture& A) =0;
  virtual void render_picture(const SPicture& A, const SPicture& B, DColorElem trans) =0;
  virtual void render_particles(const SObjectParticle* op) =0;
  // -- render text
  virtual void render_text_3d(BStrL piclist, int count) =0;
  //
  // -- conversion
  virtual bool map_2d_to_3d(DTexPoint &p3d, const DPoint2i &p2d) =0;
  virtual bool map_2d_to_3d(DPoint &p3d, const DPoint2i &p2d) =0;
  virtual bool map_3d_to_2d(DPoint2i &p2d, const DPoint &p3d) =0;
  virtual bool map_3d_to_2d(DPoint2i &p2d, const DTexPoint &p3d) =0;
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IRenderDriver, 2, lib2d::IRenderDriver);
};

#if 0

//***************************************
// IRenderConsole::

struct IRenderConsole : public IConsoleHFCSDisplay {
  virtual bool set_font(ITextureCells* font) =0;
  virtual bool set_driver(IRenderDriver* drv) =0;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IRenderConsole, IConsoleHFCSDisplay)
};

#endif

//***************************************
// END
//***************************************

} // namespace lib3d
} // namespace UModSys

#endif // __UMODSYS_LIB3D_DRIVER_H
