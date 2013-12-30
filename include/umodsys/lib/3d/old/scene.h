#ifndef __UMS_LIB3D_SCENE_H
#define __UMS_LIB3D_SCENE_H 1
//*****************************************************
// lib3d/scene.h: common 3d library interfaces, scene management
//*****************************************************

#include <ums/core/types/array.h>
#include <ums/core/types/tree_rb.h>

#include <ums/lib3d/lib3d.h>
#include <ums/lib3d/objects.h>

namespace UMS {
namespace lib3d {

//***************************************

struct ISceneController : public IRefObject {
  virtual bool set_parameters(const SParameters& P) =0;
  virtual bool ctrl_frame(HSceneNode node, BScalar dt) =0;
  virtual bool ctrl_command(HSceneNode node, int command, int value=0) =0;
  virtual bool ctrl_commandf(HSceneNode node, int command, float value) =0;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::ISceneController, IRefObject)
};

//***************************************


//***************************************

typedef types::TTreeHoldRB<ISceneNode> DSceneNodeArray;

struct ISceneNode : public types::TTreeNodeRB<ISceneNode> {
  //
  struct Cmp {
    DHString name;
    int order;
    //
    inline Cmp(const DHString &n, int o) 
    : order(o), name(n) {}
    //
    inline int operator()(const ISceneNode* r) const {
      if(order!=r->order)
        return cpu::scalar_compare(order, r->order);
      return name.cmp(r->name.gets());
    }
  };
  //
  // node only entries
  DMatrix4 T, G;
  eSceneNodeKind kind;
  DBlendColor color;
  int phase;
  //
  // -- states
  PObject object;
  PObjectShapeState shape;
  types::TArray< PObjectVisualState, SModule_AllocatorFast_Shared > visuals;
  //
  // -- tree only entries
  int phase_childs;
  DBlendColor color_combined;
  //
  // -- tree related info
  IScene* scene;
  DSString name; // sort first by name, then by order
  int order;
  HSceneNode parent;
  DSceneNodeArray childs;
  bool T_changed, phase_changed, color_changed, r_changed;
  //
  // -- extra (change-able) values
  mutable bool enabled, visible, subid, special;
  mutable types::TArray< TRefObject<ISceneController>, SModule_AllocatorFast_Shared > ctrls;
  //
  inline bool is_trans(void) const { return color->v[3]!=1; }
  inline int get_default_phases_mesh(void) const {
    if(this==NULL || color_combined.color.v[3]==1 && color_combined.transmode<=lib2d::tm_Opaque)
      return snp_RenderMain;
    return snp_RenderTran<<(color_combined.transmode-lib2d::tm_Opaque-1);
  }
  //
  void alter_phase(int pm) const;
  inline void alter_phase_add(int pm) const { alter_phase(phase | pm); }
  inline void alter_phase_remove(int pm) const { alter_phase(phase & ~pm); }
  //
  bool alter_color(const DColorAlpha* c, const lib2d::eTransparrentMode *transmode=NULL) const;
  inline bool alter_color(const DColorAlpha& c, lib2d::eTransparrentMode tm) const { return alter_color(&c, &tm); }
  inline bool alter_color(const DColorAlpha& c) const { return alter_color(&c, NULL); }
  inline bool alter_color(const DBlendColor &c) const { return alter_color(&c.color, &c.transmode); }
  inline DMatrix4& alter_T(void) const {
    const_cast<ISceneNode*>(this)->T_changed = true;
    return const_cast<DMatrix4&>(T);
  }
  //
  void die(bool flag=true) const { const_cast<ISceneNode*>(this)->_node_flag1 = flag; }
  bool is_dead(void) const { return _node_flag1!=0; }
  virtual void suicide(void) const =0;
  //
  bool add_controller(const ISceneController::P& ctrl) const;
  bool remove_controller(ISceneController* ctrl) const;
  bool gather_controllers(void) const;
  //
  IObjectVisualState* get_visual(IRenderer* r) const;
  //
  inline void ref_add(void) const { refs++; }
  inline void ref_remove(void) const { if(--refs==0) suicide(); }
  inline int  ref_get(void) const { return refs; }
  //
  HSceneNode find_node(int ord, const DHString& name) const;
  bool first_node(HSceneNode &N) const;
  bool next_node(HSceneNode &N) const;
  int get_subnodes(HSceneNode* list, int maxnum) const;
protected:
  mutable int refs;
  //
  ISceneNode(void);
  ~ISceneNode(void);
};

//***************************************

struct IScene : public IRefObject {
  typedef bool (*clone_func)(IScene *scene, HTagType context, HSceneNode dest, HSceneNode src);
  //
  virtual HSceneNode add_node_null(HSceneNode parent, const DSString& name, int ord) =0;
  virtual HSceneNode add_node_sky(HSceneNode parent, const DSString& name, int ord) =0; // root sky node
  virtual HSceneNode add_node_multi(HSceneNode parent, const DSString& name, int ord, IObjectComplex *c) =0; // complex
  virtual HSceneNode add_node_clone(HSceneNode parent, const DSString& name, int ord, HSceneNode clonefrom, 
                                    clone_func after_clone=NULL, void *context=NULL) =0; // clone exist node
  virtual HSceneNode add_node_brush(HSceneNode parent, const DSString& name, int ord, IObject_Brush* obj, IObject_Brush::IShapeState* ss=NULL) =0;
  virtual HSceneNode add_node_camera(HSceneNode parent, const DSString& name, int ord, IObject_Camera* obj, IObject_Camera::IShapeState* ss=NULL) =0;
  virtual HSceneNode add_node_light(HSceneNode parent, const DSString& name, int ord, IObject_Light* obj, IObject_Light::IShapeState* ss=NULL) =0;
  //
  virtual HSceneNode get_root_node(void) =0;
  virtual HSceneNode get_camera_node(void) =0;
  virtual bool set_camera_node(HSceneNode node) =0;
  virtual HSceneNode get_sky_node(void) =0;
  virtual bool set_sky_node(HSceneNode node) =0;
  //
  virtual bool renderer_add(IRenderer* R, IRenderDriver *driver, HSceneNode parent=NULL) = 0; // generate visual states
  virtual bool renderer_remove(IRenderer* R, IRenderDriver *driver, HSceneNode parent=NULL) = 0; // lost visual states
  virtual ISharedLibrary* get_library(void) =0;
  virtual bool set_parameters(const SParameters& P) =0;
  virtual bool ctrl_frame(HSceneNode node, BScalar deltaT) =0;
  //
  // only for renderer usage, not recommended to use it directly
  virtual bool gather(void) =0; // clear dead nodes
  virtual bool update_states(int opid) =0;
  virtual bool calc_transform(HSceneNode node=NULL) = 0;
  virtual bool render(IRenderer* R, IRenderDriver *driver, HSceneNode parent=NULL) = 0;
  virtual bool render_camera(IRenderer* R, IRenderDriver *driver, HSceneNode parent=NULL) = 0;
  virtual bool render_extra(const SRenderState& state, int comps) =0;
  virtual bool render_cast(IRenderer* R, const DTexPoint &screen, DPoint &start, DPoint &dir) =0;
protected:
  friend struct ISceneNode;
  virtual bool mark_phases(HSceneNode node) =0;
  virtual bool mark_color(HSceneNode node, const DColorAlpha* c, const lib2d::eTransparrentMode *transmode) =0;
  //
  UMS_ROOT_P_IMPLEMENT(lib3d::IScene, IRefObject)
};

//***************************************

inline bool ISceneNode::alter_color(const DColorAlpha* c, const lib2d::eTransparrentMode *transmode) const 
{
  return scene->mark_color(this, c, transmode);
}

inline void ISceneNode::alter_phase(int pm) const 
{
  if(phase!=pm) {
    const_cast<ISceneNode*>(this)->phase = pm;
    scene->mark_phases(this);
  }
}

inline HSceneNode ISceneNode::find_node(int ord, const DHString& name) const
{
  return childs.find_node_u(Cmp(name, ord));
}

inline bool ISceneNode::first_node(HSceneNode &N) const
{
  N = childs.min_node();
  return N!=NULL;
}

inline bool ISceneNode::next_node(HSceneNode &N) const
{
  if(N==NULL)
    return false;
  N = node_get_next(N);
  return N!=NULL;
}

inline int ISceneNode::get_subnodes(HSceneNode* list, int maxnum) const
{
  int rv = 0;
  for(HSceneNode n = childs.min_node(); n; node_get_next(n)) {
    if(list!=NULL) {
      if(maxnum>0) {
        *list++ = n;
        rv++;
      } else {
        break;
      }
    } else {
      rv++;
    }
  }
  return rv;
}

//***************************************

} // namespace lib3d
} // namespace UMS

#endif
