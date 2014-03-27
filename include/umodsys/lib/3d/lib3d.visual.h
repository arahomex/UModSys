#ifndef __UMODSYS_LIB3D_VISUAL_H
#define __UMODSYS_LIB3D_VISUAL_H 1
/*************************************************************/
// file: umodsys/lib/lib3d/lib3d.visual.h
// info: visualizer
/*************************************************************/

#include <umodsys/lib/3d/lib3d.typedefs.h>
#include <umodsys/lib/3d/lib3d.common.h>
#include <umodsys/lib/3d/lib3d.scene.h>

namespace UModSys {
namespace lib3d {

//***************************************
// INTERFACES
//***************************************

//***************************************
// IVisualizer::

struct IVisualizer : public IRefObject {
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
// IVisualSceneMapper::

struct IVisualSceneMapper : public IRefObject, public ISceneProcessor {
public:
//  typedef tl::TIArraySocket<IVisualObject*> Queue;
  typedef tl::TIStackSocket<IVisualObject*> Queue;
public:
  virtual bool renderer_set(IRenderer* r) =0;
  virtual bool node_update_all(IScene* scene) =0;
  virtual bool queue_fill(Queue& q, int phaseid, bool isalpha) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IVisualSceneMapper, 2, IRefObject);
};

//***************************************
// IVisualizerScene::

struct IVisualizerScene : public IVisualizer {
public:
  struct Scene {
    IScene::P scene;
    IVisualSceneMapper::P mapper;
    //
    inline Scene(IScene* s, IVisualSceneMapper* m) : scene(s), mapper(m) {}
    inline Scene(void) {}
    inline bool operator==(const Scene& R) const { return scene()==R.scene() && mapper()==R.mapper(); }
    inline bool valid(void) const { return scene.valid() && mapper.valid(); }
  };
public:
  // scene
  virtual size_t scene_count(void) const =0;
  virtual Scene scene_get(size_t idx) const =0;
  virtual size_t scene_index(const Scene &s) const =0;
  virtual bool scene_add(const Scene &s) =0;
  virtual bool scene_remove(const Scene &s) =0;
  //
public:
  inline size_t scene_index(IScene* s, IVisualSceneMapper* m) const
    { return scene_index(Scene(s, m)); }
  inline bool scene_add(IScene* s, IVisualSceneMapper* m) 
    { return scene_add(Scene(s, m)); }
  inline bool scene_remove(IScene* s, IVisualSceneMapper* m) 
    { return scene_remove(Scene(s, m)); }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IVisualizerScene, 2, IVisualizer);
};

//***************************************
// END
//***************************************

} // namespace lib3d
} // namespace UModSys

#endif // __UMODSYS_LIB3D_VISUAL_H
