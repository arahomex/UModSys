#ifndef __UMS_LIB3D_PHYSICS_LOGIC_H
#define __UMS_LIB3D_PHYSICS_LOGIC_H 1
//*****************************************************
// lib3d/physics.h: common 3d library interfaces, physics logic
//*****************************************************

#include <ums/lib3d/physics.h>

namespace UMS {
namespace lib3d {

//***************************************
// Physics Logic Node
//***************************************

struct SPhysicsLogicNode {
  HPhysicsNode wnode;
  int change_id;
};

//***************************************
// Physics Logic State
//***************************************

struct IPhysicsLogicState : public IRefObject {
  IPhysics::P world;
  int change_id;
  //
  virtual bool attach(IPhysics* w) { clear_nodes(); world=w; change_id=0; return true; }
  virtual bool update_begin(void) { change_id++; return true; } // begin update cache
  virtual bool update_end(void) { return true; } // end update cache
  virtual bool update_cleanup(void) =0; // clear unused nodes
  virtual bool update_scan_untouched(SPhysicsLogicNode*& node) =0; // clear unused nodes
  //
  virtual SPhysicsLogicNode* get_node(HPhysicsNode x) =0; // correct change_id
  virtual SPhysicsLogicNode* create_node(HPhysicsNode x, int xsize=0, HTagType tagtype=NULL) =0; // correct change_id
  virtual void* get_node_userdata(SPhysicsLogicNode* x, HTagType tagtype=NULL) =0;
  virtual bool kill_node(SPhysicsLogicNode* node) =0;
  virtual bool clear_nodes(void) =0;
  //
  template<typename T>
  inline T* tget_node_userdata(HPhysicsNode x, HTagType tagtype=NULL) { return static_cast<T*>(get_node_userdata(x, tagtype)); }
  //
  inline IPhysicsLogicState(void) : change_id(0) {}
  inline ~IPhysicsLogicState(void) {}
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IPhysicsLogicState, IRefObject)
};

//***************************************
// Physics Logic (World Logic)
//***************************************

struct IPhysicsLogics {
  struct Context {
    IPhysicsLogicState &state;
    IPhysics *world;
    BScalar delta_time;
    HPhysicsNode wnode;
    SPhysicsLogicNode *lnode;
  };
  //
  virtual bool logics_step(IPhysicsLogicState &state, BScalar delta_time);
  virtual bool can_map_entity(Context& ctx);
  virtual bool map_entity(Context& ctx);
  virtual bool unmap_entity(Context& ctx);
  virtual bool step_entity(Context& ctx) =0;
  //
  IPhysicsLogics(void);
  ~IPhysicsLogics(void);
};

//***************************************
// / physics logic
//***************************************

} // namespace lib3d
} // namespace UMS

#endif
