#ifndef __UMS_LIB3D_PHYSICS_H
#define __UMS_LIB3D_PHYSICS_H 1
//*****************************************************
// lib3d/physics.h: common 3d library interfaces, general physics
//*****************************************************

#include <ums/common/common.h>
#include <ums/common/types/math3d.h>

#include <ums/lib3d/lib3d.h>
//#include <ums/lib3d/scene.h>

namespace UMS {
namespace lib3d {

//***************************************
// Touch Objects
//***************************************

struct SPhysicsTouch { 
  DPoint point, normal;
  BScalar ratio;
  PPhysicsNode blocker;
  //
  inline void clear(void) {
    point.set(0.0); normal.set(0.0); 
    ratio = 0; blocker = NULL;
  }
  inline SPhysicsTouch(void) { clear(); }
};

//***************************************
// Physics Collider
//***************************************

struct IObjectCollider : public IRefObject {
  virtual bool trace_ray(IObject_Brush* brush, HPhysicsNode node,
                         const DPoint &start, const DPoint &end, SPhysicsTouch& T) =0;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObjectCollider, IRefObject)
};

//***************************************
// Physics World
//***************************************

struct IPhysics : public IRefObject {
  virtual bool set_parameters(const SParameters& P) =0;
  virtual bool physics_step(BScalar delta_time) =0;
  //
  virtual bool nodes_clear(void) =0; // clear all contents
  virtual HPhysicsNode node_add(const DMatrix4& T, IObjectShapeState* state, IObject* obj=NULL) =0;
  virtual bool node_scan(HPhysicsNode& node) =0;
  //
  virtual bool trace_ray(const DPoint &start, const DPoint &end, SPhysicsTouch& T) =0;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IPhysics, IRefObject)
};

//***************************************
// Physics Entity (World Node)
//***************************************

struct IPhysicsNode {
  IPhysics *world;
  PObject object;
  PObjectShapeState state;
  void* have_logic;
  DSString name;
  //
  DMatrix4 T;
  mutable BScalar mass;
  mutable BScalar bounce, flex;
  int change_id;
  unsigned int source_mask, target_mask; // source & target masks used by collisions
  //
  virtual void push(const DPoint& force) const =0;
  virtual void push(const DPoint& force, const DPoint& relpoint) const =0;
  virtual void die(void) const =0;
  virtual void clone_copy(HPhysicsNode R) const =0;
  virtual void suicide(void) const =0;
  //
  inline IPhysicsNode(IPhysics *w=NULL);
  //
  inline void ref_add(void) const { refs++; }
  inline void ref_remove(void) const { if(--refs==0) suicide(); }
  inline int  ref_get(void) const { return refs; }
protected:
  mutable int refs;
};

//***************************************
// **INLINES**
//***************************************

inline IPhysicsNode::IPhysicsNode(IPhysics *w) 
: world(w), mass(0), bounce(0), flex(0), change_id(0), 
  source_mask(~0), target_mask(0), have_logic(NULL), T(true,true), 
  refs(0) 
{
}

//***************************************
// / physics
//***************************************

} // namespace lib3d
} // namespace UMS

#endif
