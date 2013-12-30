#ifndef __UMS_LIB3D_VISI_H
#define __UMS_LIB3D_VISI_H 1
//*****************************************************
// lib3d/visi.h: common 3d library interfaces, visualizer
//*****************************************************

#include <ums/lib3d/lib3d.h>
#include <ums/lib3d/scene.h>
#include <ums/lib3d/physics.h>

namespace UMS {
namespace lib3d {

//***************************************

struct SVisualizerNode {
  PPhysicsNode wnode;
  PSceneNode snode;
  int change_id, world_id;
};

//***************************************

struct IVisualizerState : public IRefObject {
  IPhysics::P world;
  int change_id;
  //
  PScene scene;
  PSceneNode root;
  //
  //
  virtual bool attach(IPhysics* w, IScene* sc, HSceneNode snr=NULL) 
    { clear_nodes(); world=w; change_id=0; scene=sc; root=snr; return true; }
  virtual bool update_begin(void) { change_id++; return true; } // begin update cache
  virtual bool update_end(void) { return true; } // end update cache
  virtual bool update_cleanup(void) =0; // clear unused nodes
  virtual bool update_scan_untouched(SVisualizerNode*& node) =0; // clear unused nodes
  //
  virtual SVisualizerNode* get_node(HPhysicsNode x) =0; // correct change_id
  virtual SVisualizerNode* create_node(HPhysicsNode x, int xsize=0, HTagType tagtype=NULL) =0; // correct change_id
  virtual void* get_node_userdata(SVisualizerNode* x, HTagType tagtype=NULL) =0;
  virtual bool kill_node(SVisualizerNode* node) =0;
  virtual bool clear_nodes(void) =0;
  //
  template<typename T>
  inline T* tget_node_userdata(HPhysicsNode x, HTagType tagtype=NULL) { return static_cast<T*>(get_node_userdata(x, tagtype)); }
  //
  inline IVisualizerState(void) : change_id(0) {}
  inline ~IVisualizerState(void) {}
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IVisualizerState, IRefObject)
};

//***************************************

struct IVisualizer {
  virtual bool map_world(IVisualizerState& state);
  virtual bool can_map_entity(IVisualizerState& state, HPhysicsNode ent);
  virtual bool map_entity(IVisualizerState& state, SVisualizerNode*& node, HPhysicsNode wnode);
  virtual bool unmap_entity(IVisualizerState& state, SVisualizerNode& node);
  virtual bool sync_entity(IVisualizerState& state, SVisualizerNode& node);
  //
  IVisualizer(void);
  ~IVisualizer(void);
};

//***************************************

} // namespace lib3d
} // namespace UMS

#endif
