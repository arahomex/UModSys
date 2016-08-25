#ifndef __UMS_LIB3D_LIB3D_H
#define __UMS_LIB3D_LIB3D_H 1
//*****************************************************
// lib3d/lib3d.h: common 3d library interfaces
//*****************************************************

#include <ums/lib3d/types.h>

#include <ums/common/objects/console_help.h>

namespace UMS {
namespace lib3d {

//***************************************

struct IObject : public IRefObject {
public:
  typedef IObjectRenderState IRenderState;
  typedef IObjectVisualState IVisualState;
  typedef IObjectShapeState IShapeState;
public:
  // state generation functions
  virtual IObjectClass* get_class(void) =0;
  virtual PObjectShapeState state_shape(ISharedLibrary* lib) =0;
  virtual PObjectVisualState state_visual(IRenderer* r, IObject* preal, PObjectRenderState& state) =0; // initalize state if required
  virtual int get_default_phases(HSceneNode node=NULL) = 0;
  virtual IObject* get_render_root(void) =0; // return root render object for state
  // return object-specific values
  virtual const SFixedParameters& get_parameters(void) =0;
  virtual bool get_parami(const DHString& kind, const DHString& name, int &value) =0;
  virtual bool get_params(const DHString& kind, const DHString& name, BCStr &value) =0;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObject, IRefObject)
};

//***************************************

struct ISharedLibrary : public IRefObject {
  // -- support functions, general
  virtual PScene new_scene(BCStr hont=NULL, const SParameters* args=NULL) =0;
  virtual PRenderer new_renderer(BCStr hont=NULL, const SParameters* args=NULL) =0;
  virtual PSceneController new_controller(const DHString& kind, const SParameters& args) =0;
  virtual PRenderConsole new_console(IRenderDriver* drv, const SIntPoint& pos, const SIntPoint& size, const SParameters* par) =0;
  // -- support functions, visual/driver states & object adapting
  virtual bool register_scene(IScene* s) =0;
  virtual bool unregister_scene(IScene* s) =0;
  //
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::ISharedLibrary, IRefObject)
};

struct IRenderer : public IRefObject {
  // -- support functions, library
  virtual ISharedLibrary* get_library(void) =0;
  // -- support functions, driver
  virtual IRenderDriver* get_driver(void) =0;
  virtual bool set_driver(IRenderDriver* driver, IMediaGroup *mg) =0;
  // -- support functions, visual/driver states and object adapting
  virtual PObjectVisualState new_object_state(IObject* obj) =0; // automatic generate base parameters
  virtual bool cleanup_object_states(void) =0; // delete all un-referred
  virtual bool update_states(void) =0;
  // -- support functions, texture id
  virtual ITexture* load_texture(const DHString& name, const DHString& objname, int flags=tf_Normal) =0;
  virtual ITexture* alloc_texture(SRenderDriverTexBox* box, const SIntPoint& size, lib2d::eImageType type) =0;
  virtual ITextureCells* load_font(const DHString& name, const DHString& objname, int flags=tf_Normal) =0;
  // -- support functions, convert
  virtual bool ray_cast(IScene* scene, const DTexPoint &screen, DPoint &start, DPoint &dir) =0; // in relative roord
  virtual bool ray_cast(IScene* scene, const SIntPoint &screen, DPoint &start, DPoint &dir) =0; // in driver coord
  virtual bool ray_cast_center(const SIntPoint &screen, DPoint &centered) =0; // in screen coord
  // -- support functions, opid functionals and hints
  virtual int get_opid(void) =0;
  virtual bool set_hint(const DHString&  hint, double value) =0;
  virtual bool set_hint(const DHString& hint, BCStr value) =0;
  virtual double get_hint(const DHString&  hint, double defvalue) =0;
  virtual BCStr get_hint(const DHString& hint, BCStr defvalue) =0;
  //
  // -- top-level functions
  virtual bool render_begin(void) =0;
  virtual bool render_scene(IScene* scene, bool nocamera=false) =0;
  virtual bool render_extra(const SRenderState& state, int comps) =0;
  virtual bool render_2d(void) =0;
  virtual bool render_end(void) =0;
  //
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IRenderer, IRefObject)
};

//***************************************

} // namespace lib3d
} // namespace UMS

#endif
