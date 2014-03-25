#ifndef __UMODSYS_LIB3D_SCENE_H
#define __UMODSYS_LIB3D_SCENE_H 1
/*************************************************************/
// file: umodsys/lib/lib3d/lib3d.scene.h
// info: scene
/*************************************************************/

#include <umodsys/lib/2d/lib2d.typedefs.h>

namespace UModSys {
namespace lib3d {

//***************************************
// TYPES
//***************************************

//***************************************
// ISceneController::

struct ISceneController 
: public IRefObject, public IPropNamedAdapter {
  virtual bool set_parameters(const SParameters& P) =0;
  //
  virtual bool ctrl_frame(HSceneNode node, DScalar dt) =0;
  virtual bool ctrl_command(HSceneNode node, HUniquePointer id, const SPropValueR& value) =0;
  //
  inline bool ctrl_commandi(HSceneNode node, HUniquePointer id, sint32 value=0)
    { return ctrl_command(node, id, TPropValueRV<sint32>(value)); }
  bool ctrl_commandf(HSceneNode node, HUniquePointer id, float32 value=1)
    { return ctrl_command(node, id, TPropValueRV<float32>(value)); }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::ISceneController, 2, IRefObject);
};

//***************************************
// IScene::

struct IScene : public IRefObject {
public:
  typedef bool (*clone_func)(IScene *scene, void* context, HSceneNode dest, HSceneNode src);
public:
  virtual HSceneNode add_node_null(HSceneNode parent, const DCString& name, int ord) =0;
  virtual HSceneNode add_node_sky(HSceneNode parent, const DCString& name, int ord) =0; // root sky node
//  virtual HSceneNode add_node_multi(HSceneNode parent, const DCString& name, int ord, IObjectComplex *c) =0; // complex
  virtual HSceneNode add_node_clone(HSceneNode parent, const DCString& name, int ord, HSceneNode clonefrom, 
                                    clone_func after_clone=NULL, void *context=NULL) =0; // clone exist node
  virtual HSceneNode add_node_brush(HSceneNode parent, const DCString& name, int ord, IObject* obj) =0;
  virtual HSceneNode add_node_camera(HSceneNode parent, const DCString& name, int ord, IObject* obj) =0;
  virtual HSceneNode add_node_light(HSceneNode parent, const DCString& name, int ord, IObject* obj) =0;
  //
  virtual HSceneNode get_root_node(void) =0;
  virtual HSceneNode get_camera_node(void) =0;
  virtual bool set_camera_node(HSceneNode node) =0;
  virtual HSceneNode get_sky_node(void) =0;
  virtual bool set_sky_node(HSceneNode node) =0;
  //
  virtual bool renderer_add(IRenderer* R, HSceneNode parent=NULL) = 0; // generate visual states
  virtual bool renderer_remove(IRenderer* R, HSceneNode parent=NULL) = 0; // lost visual states
  virtual ISharedLibrary* get_library(void) =0;
  virtual bool ctrl_frame(HSceneNode node, DScalar deltaT) =0;
  //
  // only for renderer usage, not recommended to use it directly
  virtual bool gather(void) =0; // clear dead nodes
  virtual bool update_states(int opid) =0;
  virtual bool calc_transform(HSceneNode node=NULL) = 0;
  virtual bool render(IRenderer* R, HSceneNode parent=NULL) = 0;
  virtual bool render_camera(IRenderer* R, IRenderDriver *driver, HSceneNode parent=NULL) = 0;
  virtual bool render_extra(const SRenderState& state, int comps) =0;
  virtual bool render_cast(IRenderer* R, const DTexPoint &screen, DPoint &start, DPoint &dir) =0;
protected:
  friend struct ISceneNode;
  virtual bool mark_phases(HSceneNode node) =0;
  virtual bool mark_color(HSceneNode node, const DColorAlpha* c, const lib2d::eTransparrentMode *transmode) =0;
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IScene, 2, IRefObject);
};

//***************************************
//***************************************

//***************************************
// ISceneNodeProcessor::

struct ISceneNodeProcessor {
  virtual int node_process(HSceneNode node) =0;
};

//***************************************
// ISceneNode::

struct ISceneNode {
public:
  // node only entries
  DMatrix4 T, G;
  eSceneNodeKind kind;
  DBlendColor color;
  int phase;
  //
  // -- states
//  PNodeState visual;
  //
  // -- tree only entries
  int phase_childs;
  DBlendColor color_combined;
  //
  // -- tree related info
  HSceneNode parent;
  PWScene scene;
  DCString name; // sort first by name, then by order
  int order;
  bool T_changed, phase_changed, color_changed, r_changed;
  //
  // -- extra (change-able) values
  mutable bool enabled, visible, subid, special;
  //
public:
  // node processing
  virtual int node_for_each(ISceneNodeProcessor& proc, bool recursive=true) const =0;
  virtual HSceneNode find_node(int ord, const DCString& name) const =0;
  virtual bool first_node(HSceneNode &N) const =0;
  virtual bool next_node(HSceneNode &N) const =0;
  virtual int get_subnodes(HSceneNode* list, int maxnum) const =0;
  //
  virtual int get_default_phases_mesh(void) const =0;
  virtual void alter_phase(int pm) const =0;
  virtual bool alter_color(const DColorAlpha* c, const lib2d::eTransparrentMode *transmode=NULL) const =0;
  virtual void die(bool flag=true) const =0;
  virtual bool is_dead(void) const =0;
  virtual void suicide(void) const =0;
  virtual bool add_controller(const ISceneController::P& ctrl) const =0;
  virtual bool remove_controller(ISceneController* ctrl) const =0;
  virtual bool gather_controllers(void) const =0;
  virtual INodeObject* get_visual(IRenderer* r) const =0;
public:
  inline bool is_trans(void) const { 
    return color->v[3]!=1; 
  }
  //
  inline void alter_phase_add(int pm) const { 
    alter_phase(phase | pm); 
  }
  inline void alter_phase_remove(int pm) const { 
    alter_phase(phase & ~pm); 
  }
  //
  inline bool alter_color(const DColorAlpha& c, lib2d::eTransparrentMode tm) const { 
    return alter_color(&c, &tm); 
  }
  inline bool alter_color(const DColorAlpha& c) const { 
    return alter_color(&c, NULL); 
  }
  inline bool alter_color(const DBlendColor &c) const { 
    return alter_color(&c.color, &c.transmode); 
  }
  inline DMatrix4& alter_T(void) const {
    const_cast<ISceneNode*>(this)->T_changed = true;
    return const_cast<DMatrix4&>(T);
  }
  //
  inline void ref_add(void) const { 
    refs++; 
  }
  inline void ref_remove(void) const { 
    if(--refs==0) suicide(); 
  }
  inline int  ref_get(void) const { 
    return refs; 
  }
  //
protected:
  mutable int refs;
  //
  ISceneNode(void);
  ~ISceneNode(void);
};

//***************************************

//***************************************
// END
//***************************************

} // namespace lib3d
} // namespace UModSys

#endif // __UMODSYS_LIB3D_SCENE_H
