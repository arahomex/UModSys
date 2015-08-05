#ifndef __UMS_LIB3D_DRIVER_H
#define __UMS_LIB3D_DRIVER_H 1
//*****************************************************
// lib3d/render.h: common 3d library interfaces, driver
//*****************************************************

#include <ums/core/objects/console_help.h>

#include <ums/lib2d/lib2d.h>
#include <ums/lib2d/driver2.h>

#include <ums/lib3d/lib3d.h>
#include <ums/lib3d/material.h>

namespace UMS {
namespace lib3d {

//***************************************

enum eRenderStateComponents {
  rsc_TCameraWorld    = 0x00000001,
  rsc_TLocalWorld     = 0x00000002,
  rsc_TWorldCamera    = 0x00000004,
  rsc_TLocalCamera    = 0x00000010,
  rsc_TCameraLocal    = 0x00000020,
  rsc_TWorldLocal     = 0x00000040,
  rsc_OrtX            = 0x00001000,
  rsc_OrtY            = 0x00002000,
  rsc_OrtZ            = 0x00004000,
  rsc_Origin0         = 0x00008000,
  rsc_CameraPlanes    = 0x00010000,
  rsc_CameraPlanesW   = 0x00020000,
  rsc_CameraPlanesL   = 0x00040000,
  rsc_Camera          = 0x01000000,
  rsc_None            = 0x00000000
};

enum eRenderPrimitiveComponents {
  rpc_Point           = 0x00000001,
  rpc_Normal          = 0x00000002,
  rpc_Color           = 0x00000004,
  rpc_ColorAlpha      = 0x00000008,
  rpc_Texture1        = 0x00000010,
  rpc_Texture2        = 0x00000020,
  rpc_Texture3        = 0x00000040,
  rpc_Texture         = 0x00000070,
  rpc_None            = 0x00000000
};

enum eRenderPrimitiveType {
  rp_Null         = 0,
  rp_Point        = 1,
  rp_Line         = 2,
  rp_Tri          = 3,
  rp_Triangle     = rp_Tri,
  rp_3 = rp_Triangle,
  rp_Quad         = 4,
  rp_4 = rp_Quad,
  rp_Poly         = 10,
  rp_Strip        = 11,
  rp_Fan          = 12,
  rp_MultiLine    = 13,
  rp_Unknown      = -1
};
//***************************************

struct SRenderState {
  IRenderer *renderer;
  IScene* scene;
  IRenderDriver *driver;            // current driver
  DBlendColor color;
  const IObjectVisualState* state;  // current state
  const IObjectShapeState* shape;   // current state
  HSceneNode node;                  // current scene node
  int phase;                        // pass to go
  int outlines;                     // draw extra: outlines
  // light information
  int nlights;                      // count of dynamic lights
  // camera info
  IObject_Camera::IVisualState* camstate;
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

struct IRenderDriver : public lib2d::IRenderDriver {
  // -- camera setup
  virtual bool camera_ortho(const DPoint &center, const DPoint &size) =0;
  virtual bool camera_frustum(const DPoint &center, const DPoint &size) =0;
  virtual bool camera_frustum(const DPoint &center, const DTexPoint &fov, BScalar min, BScalar max) =0;
  // -- allocate vector-cache
  // all data will be reset after end of frame
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
  virtual ITexture::P register_tex(IMediaGroup* mg, const DHString& texname, const SRenderMapFlags& deff) =0;
  virtual ITextureCells::P register_texcells(IMediaGroup* mg, const DHString& texname, const SRenderMapFlags& deff) =0;
  // this one create a new box used for supply textures
  virtual ITexture::P register_tex(const SIntPoint& size, const SRenderMapFlags& deff, lib2d::eImageType type) =0;
  // -- setup dynamic lights, light indices: <=0 =error, 0x10000+ = HW, 0x20000+ = SW, 0x30000+ = omni
  virtual bool setup_clearlights(eLightType type=lt_All, bool emulated=true, bool hardware=true) =0;
  virtual int  setup_addlight(eLightType type, const SLight& light, const DMatrix4& T) =0;
  virtual bool setup_setlight(int idx, eLightType type, const SLight& light, const DMatrix4& T) =0;
  virtual bool setup_setlight(int idx, bool enabled) =0;
  virtual void setup_T(const DMatrix4& T) =0;
  virtual bool setup_material(const SMaterial *mat) =0; // NULL to disable materials
  //
  // -- render primitives by lists
  // enable lists, up to 4, first list always have rpc_Point, other never have it
  virtual bool setup_lists(int render_primitive_components, const int* list, int listid=0) =0;
  virtual bool setup_lists2(int count, int render_primitive_components, const int* list, ...) =0;
  virtual bool render_primitive(eRenderPrimitiveType primitive, int count, int starts=0, ...) =0;
  virtual bool render_primitive(eRenderPrimitiveType primitive, int count, const int* starts) =0;
  //
  virtual bool setup_state(const SRenderState& S) =0; // discarded on phase end or render start/stop
  virtual bool setup_2d(const SIntPoint* vsize, const SIntPoint* voffset=NULL,
                        const DTexPoint* relsize=NULL, const DTexPoint *reloffset=NULL) =0; // window coordinates
  // -- render pictures
  virtual void render_picture(const SPicture& A) =0;
  virtual void render_picture(const SPicture& A, const SPicture& B, BColorElem trans) =0;
  virtual void render_particles(const SObjectParticle* op) =0;
  // -- render text
  virtual void render_text_3d(BStrU piclist, int count) =0;
  //
  // -- conversion
  virtual bool map_2d_to_3d(DTexPoint &p3d, const SIntPoint &p2d) =0;
  virtual bool map_2d_to_3d(DPoint &p3d, const SIntPoint &p2d) =0;
  virtual bool map_3d_to_2d(SIntPoint &p2d, const DPoint &p3d) =0;
  virtual bool map_3d_to_2d(SIntPoint &p2d, const DTexPoint &p3d) =0;
  //
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IRenderDriver, lib2d::IRenderDriver)
};

//***************************************

struct IRenderConsole : public IConsoleHFCSDisplay {
  virtual bool set_font(ITextureCells* font) =0;
  virtual bool set_driver(IRenderDriver* drv) =0;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IRenderConsole, IConsoleHFCSDisplay)
};

//***************************************
} // namespace lib3d
} // namespace UMS

#endif
