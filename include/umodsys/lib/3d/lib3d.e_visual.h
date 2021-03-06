#ifndef __UMODSYS_LIB3D_EMUL_VISUAL_H
#define __UMODSYS_LIB3D_EMUL_VISUAL_H 1
/*************************************************************/
// file: umodsys/lib/lib3d/lib3d.e_visual.h
// info: visualizer helpers
/*************************************************************/

#include <umodsys/lib/3d/lib3d.visual.h>

namespace UModSys {
namespace lib3d {

//***************************************
// TYPES
//***************************************


//***************************************
// SVisualizerQueue::

struct SVisualizerQueue : public IRefObject {
public:
  // links
  virtual IRenderer* renderer_get(void) const =0;
  virtual bool renderer_set(IRenderer* r) =0;
  // visualize
  virtual bool render_3d(int phaseid, bool isalpha, IRenderDriver* drv) =0;
  virtual bool render_2d(IRenderDriver* drv) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IVisualizer, 2, IRefObject);
};

//***************************************
// IVisualizer3D::

struct IVisualizer3D : public IVisualizer {
public:
  typedef tl::
public:
  virtual bool 
  // links
  virtual IRenderer* renderer_get(void) const =0;
  virtual bool renderer_set(IRenderer* r) =0;
  // visualize
  virtual bool render_3d(int phaseid, bool isalpha, IRenderDriver* drv) =0;
  virtual bool render_2d(IRenderDriver* drv) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IVisualizer, 2, IRefObject);
};

//***************************************
// IVisualizerScene::

struct IVisualizerScene : public IVisualizer, public ISceneProcessor {
public:
  // scene
  virtual size_t scene_count(void) const =0;
  virtual IScene* scene_get(size_t idx) const =0;
  virtual size_t scene_index(IScene* s) const =0;
  virtual bool scene_add(IScene* s) =0;
  virtual bool scene_remove(IScene* s) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IVisualizerScene, 2, IVisualizer);
};

//***************************************
// END
//***************************************

} // namespace lib3d
} // namespace UModSys

#endif // __UMODSYS_LIB3D_VISUAL_H
