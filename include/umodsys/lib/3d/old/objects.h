#ifndef __UMS_LIB3D_OBJECTS_H
#define __UMS_LIB3D_OBJECTS_H 1
//*****************************************************
// lib3d/objects.h: common 3d library interfaces, objects
//*****************************************************

#include <ums/lib3d/lib3d.h>

namespace UMS {
namespace lib3d {

//***************************************

enum eObjectBrushOutline {
  obo_Null,
  obo_Point,
  obo_Line,
  obo_Plane,
  obo_Sphere,
  obo_Box,
  obo_Cylinder,
  obo_Cone,
  obo_ConvexMesh,
  obo_ConvexMultiMesh,
  obo_ConvexCloud,
  obo_ConcaveMesh,
  obo_Unknown,
  obo_User // above is user-defined outlines
};

//***************************************
//***************************************

struct SObjectParticle {
  // next free or next allocated
  SObjectParticle* next; 
  //
  const SPicture* pic;
  DColorAlpha color;
  //
  DPointM pos;
  BScalarM time, end_time;
};

struct SObjectParticleStatic : public SObjectParticle {
  DColorAlpha c_speed;
  DPointM speed, accel;
};

//***************************************
// IObjectClass::
//***************************************

struct IObjectClass : public IModObject {
  DHString object_class;
  SFixedParametersBase param_object;
  SFixedParametersBase param_shape;
  SFixedParametersBase param_render;
  SFixedParametersBase param_visual;
  //
  IObjectClass(BCStr name, int no=1024, int ns=1024, int nr=1024, int nv=1024);
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObjectClass, IModObject)
};

//***************************************
// IObjectComplex::
//***************************************

struct IObjectComplex : public IRefObject {
  virtual int get_sub_count(void) =0;
  virtual IObject* get_sub(int id) =0;
  virtual BCStr get_sub_name(int id) =0;
  virtual bool fix_sub_node_T(int id, HSceneNode node) = 0;
  virtual bool fix_sub_node_physics(int id, HSceneNode node) = 0;
  virtual bool fix_sub_node_controller(int id, HSceneNode node) = 0;
  //
  virtual bool build_scenenode(HSceneNode holder) =0;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObjectComplex, IRefObject)
};

//***************************************
// IObjectShapeState::
//***************************************

struct IObjectShapeState : public IRefObject {
  PObject object;
  SFixedParameters params;
  int seqno;
  //
  virtual bool valid(void) const =0;
  virtual bool clone_copy(const IObjectShapeState *R);
  inline virtual IObjectShapeState* v_as_object(eSceneNodeKind kind) { return NULL; }
  //
  template<typename T>
  inline T* t_as_object(void) {
    IObjectShapeState* rv = v_as_object(T::ObjectKind);
    return rv ? static_cast<typename T::IShapeState*>(rv) : NULL;
  }
  //
  IObjectShapeState(IObject* obj);
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObjectShapeState, IRefObject)
};

//***************************************
// IObjectRenderState::
//***************************************

struct IObjectRenderState : public IRefObject {
  PObject object;
  PObject real_object;
  SFixedParameters params;
  PRenderer renderer;
  int seqno, vlinks;
  //
  virtual bool valid(void) const =0;
  virtual bool clone_copy(const IObjectRenderState *R);
  inline virtual IObjectRenderState* v_as_object(eSceneNodeKind kind) { return NULL; }
  //
  inline bool is_unused(void) const { return vlinks==0; }
  inline void use(void) { vlinks++; }
  inline void unuse(void) { vlinks--; }
  //
  template<typename T>
  inline T* t_as_object(void) {
    IObjectRenderState* rv = v_as_object(T::ObjectKind);
    return rv ? static_cast<typename T::IRenderState*>(rv) : NULL;
  }
  //
  IObjectRenderState(IObject* obj, IObject *robj, IRenderer* r);
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObjectRenderState, IRefObject)
};

//***************************************
// IObjectVisualState::
//***************************************

struct IObjectVisualState : public IRefObject {
  PObjectRenderState rstate;
  SFixedParameters params;
  int seqno;
  //
  virtual bool update(void) =0; // re-initalize links
  virtual bool valid(void) const =0;
  virtual bool clone_copy(const IObjectVisualState *R);
  inline virtual IObjectVisualState* v_as_object(eSceneNodeKind kind) { return NULL; }
  //
  template<typename T>
  inline T* t_as_object(void) {
    IObjectVisualState* rv = v_as_object(T::ObjectKind);
    return rv ? static_cast<typename T::IVisualState*>(rv) : NULL;
  }
  //
  IObjectVisualState(IObjectRenderState* b);
  ~IObjectVisualState(void);
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObjectVisualState, IRefObject)
};

//***************************************
// IObjectBrushOutline::
//***************************************

struct IObjectBrushOutline {
  virtual eObjectBrushOutline get_type(void) =0; // eObjectBrushOutline
  //
  template<typename OutlineEx>
  inline bool t_as(OutlineEx* &outline) {
    if(get_type()!=OutlineEx::type)
      return NULL;
    outline = static_cast<OutlineEx*>(this);
    return true;
  }
  template<typename OutlineEx>
  inline bool t_asz(OutlineEx* &outline) {
    if(this==NULL)
      return NULL;
    if(get_type()!=OutlineEx::type)
      return NULL;
    outline = static_cast<OutlineEx*>(this);
    return true;
  }
};

//***************************************
// ** IObject born **
//***************************************

//***************************************
// IObject_Brush::
//***************************************

struct IObject_Brush : public IObject {
public:
  enum { ObjectKind = snk_Brush };
  //
  struct IVisualState;
  struct IShapeState;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObject_Brush, IObject)
};

struct IObject_Brush::IVisualState : public IObject::IVisualState {
  //
  inline virtual IObjectVisualState* v_as_object(eSceneNodeKind kind) {
    switch(kind) {
      case ObjectKind: return this;
    }
    return NULL;
  }
  //
  virtual bool render_state(SRenderState& state) =0;
  //
  inline IVisualState(IRenderState* b)
  : IObject::IVisualState(b)
  {
  }
  inline ~IVisualState(void)
  {
  }
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObject_Brush::IVisualState, IObject::IVisualState)
};

struct IObject_Brush::IShapeState : public IObject::IShapeState {
  //
  inline virtual IObjectShapeState* v_as_object(eSceneNodeKind kind) {
    switch(kind) {
      case ObjectKind: return this;
    }
    return NULL;
  }
  //
  virtual IObjectCollider* get_collider(void) =0; // get collider, if possible
  virtual IObjectBrushOutline* get_outlineindex(int aid=0) =0; // ordered outlines list
  virtual IObjectBrushOutline* get_outline(eObjectBrushOutline id) =0; // fixed outline
  //
  inline IShapeState(IObject_Brush* obj)
  : IObject::IShapeState(obj)
  {
  }
  inline ~IShapeState(void)
  {
  }
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObject_Brush::IShapeState, IObject::IShapeState)
};

//***************************************
// IObject_Mesh::
//***************************************

struct IObject_Mesh : public IObject_Brush {
  enum { ObjectKind = snk_Mesh };
  //
//  virtual bool move_path(HSceneNode node, const IObjectShapeState* state, SSceneMoved& info) =0;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObject_Mesh, IObject_Brush)
};

//***************************************
// IObject_Picture::
//***************************************

struct IObject_Picture : public IObject_Brush {
  enum { ObjectKind = snk_Picture };
  //
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObject_Picture, IObject_Brush)
};

//***************************************
// IObject_Particles::
//***************************************

struct IObject_Particles : public IObject_Brush {
  enum { ObjectKind = snk_Particles };
  //
  struct IVisualState;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObject_Particles, IObject_Brush)
};

struct IObject_Particles::IVisualState : public IObject_Brush::IVisualState {
  int setup_ok;
  //
  inline virtual IObjectVisualState* v_as_object(eSceneNodeKind kind) {
    switch(kind) {
      case snk_Brush:
      case ObjectKind: return this;
    }
    return NULL;
  }
  //
  virtual bool particle_setup(int size, int count, int npics=1) =0;
  virtual int particle_count(void) =0; // size solid array of particles
  virtual int particle_countused(void) =0;
  virtual int particle_countfreed(void) =0;
  virtual SObjectParticle* particle_array(void) =0; // solid array of particles
  virtual SObjectParticle* particle_chain(void) =0; // first used, use next
  virtual SObjectParticle* particle_alloc(void) =0;
  virtual bool particle_free(void) =0; // free all unused particles
  //
  virtual const SPicture* pic_set(int pid,
                                  const DTexPoint &p1, const DTexPoint &p2,
                                  const DTexPoint &t1, const DTexPoint &t2,
                                  ITexture *tex) =0;
  virtual const SPicture* pic_set(int pid, ITexture *tex, BScalar s=1.0) =0;
  virtual const SPicture* pic_get(int pid) =0;
  //
  inline IVisualState(IRenderState* b)
  : IObject_Brush::IVisualState(b), setup_ok(0) 
  {
  }
  inline ~IVisualState(void)
  {
  }
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObject_Particles::IVisualState, IObject_Brush::IVisualState)
};

//***************************************
// IObject_Camera::
//***************************************

struct IObject_Camera : public IObject {
public:
  enum { ObjectKind = snk_Camera };
  //
  struct IVisualState;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObject_Camera, IObject)
};

struct IObject_Camera::IVisualState : public IObject::IVisualState {
  //
  inline virtual IObjectVisualState* v_as_object(eSceneNodeKind kind) {
    switch(kind) {
      case ObjectKind: return this;
    }
    return NULL;
  }
  //
  virtual DTexPoint get_fov(void) =0;
  virtual bool get_T(DMatrix4 &T) =0;
  virtual bool get_xyz(DPoint* ox, DPoint* oy, DPoint* oz) =0;
  virtual int get_planes(DPlane* planes) =0;
  // cast ray from [-1,1],[-1,1] to world coordinates
  virtual bool cast_ray(const DTexPoint &screen, DPoint &start, DPoint &dir) =0;
  virtual bool camera_setup(IRenderDriver *driver) =0;
  //
  inline IVisualState(IRenderState* b)
  : IObject::IVisualState(b)
  {
  }
  inline ~IVisualState(void)
  {
  }
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObject_Camera::IVisualState, IObject::IVisualState)
};

//***************************************
// IObject_Light::
//***************************************

struct IObject_Light : public IObject {
public:
  enum { ObjectKind = snk_Light };
  //
  struct IVisualState;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObject_Light, IObject)
};

struct IObject_Light::IVisualState : public IObject::IVisualState {
  //
  inline virtual IObjectVisualState* v_as_object(eSceneNodeKind kind) {
    switch(kind) {
      case ObjectKind: return this;
    }
    return NULL;
  }
  //
  virtual bool light_supported(eLightType type) =0;
  virtual bool light_setinfo(eLightType type, const SLight& info) =0;
  virtual bool light_getinfo(eLightType type, SLight& info) =0;
  //
  inline IVisualState(IRenderState* b)
  : IObject::IVisualState(b)
  {
  }
  inline ~IVisualState(void)
  {
  }
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObject_Light::IVisualState, IObject::IVisualState)
};

//***************************************
//
//***************************************

} // namespace lib3d
} // namespace UMS

#endif
