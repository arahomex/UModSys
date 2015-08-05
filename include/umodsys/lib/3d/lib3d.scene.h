#ifndef __UMODSYS_LIB3D_SCENE_H
#define __UMODSYS_LIB3D_SCENE_H 1
/*************************************************************/
// file: umodsys/lib/lib3d/lib3d.scene.h
// info: scene
/*************************************************************/

#include <umodsys/lib/2d/lib2d.common.h>
#include <umodsys/lib/3d/lib3d.typedefs.h>

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
// ISceneProcessor::

struct ISceneProcessor {
public:
  virtual bool on_scene_node_add(IScene* sc, HSceneNode node) =0;
  virtual bool on_scene_node_remove(IScene* sc, HSceneNode node) =0;
  virtual bool on_scene_node_change(IScene* sc, HSceneNode node) =0;
};

//***************************************
// IScene::

struct IScene : public IRefObject {
public:
//  typedef bool (*clone_func)(IScene *scene, void* context, HSceneNode dest, HSceneNode src);
public:
/*
  virtual HSceneNode add_node_null(HSceneNode parent, const DCString& name, int ord) =0;
  virtual HSceneNode add_node_sky(HSceneNode parent, const DCString& name, int ord) =0; // root sky node
//  virtual HSceneNode add_node_multi(HSceneNode parent, const DCString& name, int ord, IObjectComplex *c) =0; // complex
  virtual HSceneNode add_node_clone(HSceneNode parent, const DCString& name, int ord, HSceneNode clonefrom, 
                                    clone_func after_clone=NULL, void *context=NULL) =0; // clone exist node
  virtual HSceneNode add_node_brush(HSceneNode parent, const DCString& name, int ord, IObject* obj) =0;
  virtual HSceneNode add_node_camera(HSceneNode parent, const DCString& name, int ord, IObject* obj) =0;
  virtual HSceneNode add_node_light(HSceneNode parent, const DCString& name, int ord, IObject* obj) =0;
  //
*/
  virtual ISharedLibrary* library_get(void) =0;
  virtual HSceneNode root_get(void) const =0;
  virtual HSceneNode camera_get(void) const =0;
  virtual bool camera_set(HSceneNode node) =0;
  virtual HSceneNode sky_get(void) const =0;
  virtual bool sky_set(HSceneNode node) =0;
  //
  virtual bool processor_add(ISceneProcessor* proc) = 0;
  virtual bool processor_remove(ISceneProcessor* proc) = 0;
  //
  virtual bool nodes_gather(void) =0; // clear dead nodes
  //
  virtual bool calc_transform(HSceneNode node=NULL) = 0;
  virtual bool mark_phases(HSceneNode node) =0;
  virtual bool mark_color(HSceneNode node, const DColorAlpha* c, const lib2d::eTransparrentMode *transmode) =0;
/*
  virtual bool renderer_add(IRenderer* R, HSceneNode parent=NULL) = 0; // generate visual states
  virtual bool renderer_remove(IRenderer* R, HSceneNode parent=NULL) = 0; // lost visual states
  virtual bool ctrl_frame(HSceneNode node, DScalar deltaT) =0;
  virtual bool render(IRenderer* R, HSceneNode parent=NULL) = 0;
  virtual bool update_states(int opid) =0;
  virtual bool render_camera(IRenderer* R, IRenderDriver *driver, HSceneNode parent=NULL) = 0;
  virtual bool render_extra(const SRenderState& state, int comps) =0;
  virtual bool render_cast(IRenderer* R, const DTexPoint &screen, DPoint &start, DPoint &dir) =0;
*/
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
  enum eNodeChanged {
    nc_TL       = 0x0001,
    nc_TG       = 0x0002,
    nc_Color    = 0x0004,
    nc_Phase    = 0x0008,
    nc_PhaseU   = 0x0010,
    nc_ColorU   = 0x0020,
    nc_Flags    = 0x0040
  };
public:
  // node only entries
  DMatrix4 TL, TG;
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
  bool T_changed, phase_changed, color_changed, r_changed;
  //
  // node order detemined as:
  // 1. by order
  // 2. any non-NULL tag in any order, tag is unique for order
  // 3. any count of NULL tag in any order
  int order;
  DCString name;
  //
  // -- extra (change-able) values
  bool enabled, visible, subid, special;
  //
protected:
  virtual void node_suicide(void) const =0; // remove in memory
public:
  virtual HSceneNode node_next(void) const =0; // get next node
  //
  virtual HSceneNode child_find(int order, const DCString& tag) const =0; // find non-NULL tag or any NULL tag
  virtual size_t child_find(int order, HSceneNode lst[], size_t nlst) const =0; // find all NULL tag nodes
  virtual HSceneNode child_first(void) const =0; // get first child
  virtual size_t child_get(int idx, HSceneNode *lst, size_t nlst) const =0; // get all nodes starting with idx
  virtual size_t child_count(void) const =0; // get all nodes count
  virtual HSceneNode child_new(ILogicObject *lo, int order, const DCString& tag, eSceneNodeKind k) const =0;
  //
  virtual void node_die(void) const =0; // mark node as dead
  virtual bool node_is_dead(void) const =0; // check if node is dead
  virtual void node_mark(int nc) const =0; // mark changes in node
  virtual bool node_color(const DColorAlpha* c, const lib2d::eTransparrentMode *transmode=NULL) const =0;
  virtual void node_phase(int pm) const =0;
  //
  virtual bool ctrl_add(const ISceneController::P& ctrl) const =0;
  virtual bool ctrl_remove(ISceneController* ctrl) const =0;
  virtual bool ctrl_run(void) const =0;
public:
  //
  inline HSceneNode child_new_camera(ILogicObject *lo, int order=0, const DCString& tag=NULL) const
    { return child_new(lo, order, tag, snk_Camera); }
  inline HSceneNode child_new_light(ILogicObject *lo, int order=0, const DCString& tag=NULL) const
    { return child_new(lo, order, tag, snk_Light); }
  inline HSceneNode child_new_brush(ILogicObject *lo, int order=0, const DCString& tag=NULL) const
    { return child_new(lo, order, tag, snk_Brush); }
  inline HSceneNode child_new_null(int order=0, const DCString& tag=NULL) const
    { return child_new(NULL, order, tag, snk_Null); }
  //
  inline void visible_set(bool f=true) const 
    { node_mark(nc_Flags); _a()->visible = f; }
  inline void enabled_set(bool f=true) const 
    { node_mark(nc_Flags); _a()->enabled = f; }
  //
  inline DMatrix4& alter_TL(void) const 
    { node_mark(nc_TL); return _a()->TL; }
  inline bool color_set(const DColorAlpha& c, lib2d::eTransparrentMode tm) const 
    { return node_color(&c, &tm); }
  inline bool color_set(const DColorAlpha& c) const 
    { return node_color(&c, NULL); }
  inline bool color_set(const DBlendColor &c) const 
    { return node_color(&c.color, &c.transmode); }
  inline void phase_add(int pm) const 
    { node_phase(phase | pm); }
  inline void phase_remove(int pm) const 
    { node_phase(phase & ~pm); }
  //
  inline bool is_trans(void) const { return color->v[3]!=1; }
public:
  // node processing
//  virtual int node_for_each(ISceneNodeProcessor& proc, bool recursive=true) const =0;
//  virtual int get_default_phases_mesh(void) const =0;
//  virtual INodeObject* get_visual(IRenderer* r) const =0;
public:
  //
  inline void ref_add(void) const { node_refs++;  }
  inline void ref_remove(void) const { if(--node_refs==0) node_suicide(); }
  inline int  ref_get(void) const { return node_refs; }
  //
protected:
  mutable int node_refs;
  inline ISceneNode* _a(void) const { return const_cast<ISceneNode*>(this); }
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
