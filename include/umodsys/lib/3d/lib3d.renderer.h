#ifndef __UMODSYS_LIB3D_RENDERER_H
#define __UMODSYS_LIB3D_RENDERER_H 1
/*************************************************************/
// file: umodsys/lib/lib3d/lib3d.renderer.h
// info: renderer and shared library
/*************************************************************/

#include <umodsys/lib/3d/lib3d.typedefs.h>
#include <umodsys/lib/media/libmedia.typedefs.h>

namespace UModSys {
namespace lib3d {

//***************************************
// TYPES
//***************************************

struct ISharedLibrary : public IRefObject {
  // -- support functions, general
  virtual PScene new_scene(BCStr hont=NULL, const SParameters* args=NULL) =0;
  virtual PRenderer new_renderer(BCStr hont=NULL, const SParameters* args=NULL) =0;
  virtual PSceneController new_controller(const DCString& kind, const SParameters& args) =0;
  virtual PRenderConsole new_console(IRenderDriver* drv, const DPoint2i& pos, const DPoint2i& size, const SParameters* par) =0;
  // -- support functions, visual/driver states & object adapting
  virtual bool register_scene(IScene* s) =0;
  virtual bool unregister_scene(IScene* s) =0;
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::ISharedLibrary, 2, IRefObject);
};

struct IRenderer : public IRefObject {
  // -- support functions, library
  virtual ISharedLibrary* get_library(void) =0;
  // -- support functions, driver
  virtual IRenderDriver* get_driver(void) =0;
  virtual bool set_driver(IRenderDriver* driver, libmedia::ILibrary *mg) =0;
  // -- support functions, visual/driver states and object adapting
  virtual PNodeObject new_nodeobject(IObject* obj) =0; // automatic generate base parameters
  virtual bool cleanup_object_states(void) =0; // delete all un-referred
  virtual bool update_states(void) =0;
  // -- support functions, texture id
  virtual ITexture* load_texture(const DCString& name, const DCString& objname, int flags=tf_Normal) =0;
  virtual ITexture* alloc_texture(SRenderDriverTexBox* box, const DPoint2i& size, lib2d::eImageType type) =0;
  virtual ITextureCells* load_font(const DCString& name, const DCString& objname, int flags=tf_Normal) =0;
  // -- support functions, convert
  virtual bool ray_cast(IScene* scene, const DTexPoint &screen, DPoint &start, DPoint &dir) =0; // in relative roord
  virtual bool ray_cast(IScene* scene, const DPoint2i &screen, DPoint &start, DPoint &dir) =0; // in driver coord
  virtual bool ray_cast_center(const DPoint2i &screen, DPoint &centered) =0; // in screen coord
  // -- support functions, opid functionals and hints
  virtual int get_opid(void) =0;
  virtual bool set_hint(const DCString&  hint, double value) =0;
  virtual bool set_hint(const DCString& hint, BCStr value) =0;
  virtual double get_hint(const DCString&  hint, double defvalue) =0;
  virtual BCStr get_hint(const DCString& hint, BCStr defvalue) =0;
  //
  // -- top-level functions
  virtual bool render_begin(void) =0;
  virtual bool render_scene(IScene* scene, bool nocamera=false) =0;
  virtual bool render_extra(const SRenderState& state, int comps) =0;
  virtual bool render_2d(void) =0;
  virtual bool render_end(void) =0;
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IRenderer, 2, IRefObject);
};

//***************************************
// END
//***************************************

} // namespace lib3d
} // namespace UModSys

#endif // __UMODSYS_LIB3D_RENDERER_H
