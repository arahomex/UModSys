#ifndef __UMS_LIB3D_CONTROL_H
#define __UMS_LIB3D_CONTROL_H 1
//*****************************************************
// lib3d/control.h: common 3d library interfaces, scene controllers
//*****************************************************

#include <ums/core/common.h>
#include <ums/core/types/math3d.h>

#include <ums/lib3d/lib3d.h>
#include <ums/lib3d/scene.h>
#include <ums/lib3d/objects.h> // for ISceneControllerParticle

namespace UMS {
namespace lib3d {

//***************************************

struct SSceneControlNode {
  HSceneNode node;
  ISceneController::P ctrl;
  //
  inline SSceneControlNode(void) : node(NULL), ctrl(NULL) {}
  //
  inline bool frame(BScalar dt) { return ctrl && node ? ctrl->ctrl_frame(node, dt) : false; }
  inline bool command(int c, int v=0) { return ctrl && node ? ctrl->ctrl_command(node, c, v) : false; }
  inline bool commandf(int c, float v=0) { return ctrl && node ? ctrl->ctrl_commandf(node, c, v) : false; }
  //
  inline void init(HSceneNode n, ISceneController* c=NULL) 
    { node=n; ctrl=c; if(n) n->add_controller(c); }
  inline void free(void) { ctrl.clear(); }
};

//***************************************

struct IObjectParticleAffector : public IRefObject {
  typedef IObject_Particles::IVisualState IVisualState;
  //
  mathc::SRandomGenerator rnd;
  int valid;
  BScalar old_time, time;
  int particle_size, quantity, pics;
  //
  inline IObjectParticleAffector(void) {
    valid = 0; // not initalized
  }
  //
  bool step(HSceneNode node, BScalar dt);
  //
  virtual bool cmdi(int c, int v) =0;
  virtual bool cmdf(int c, float v) =0;
  virtual bool cmdp(const SParameters& P) =0;
  //
  virtual bool begin(IVisualState* r) =0;
  //
  virtual bool step1(IVisualState* r, BScalar dt) =0;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::IObjectParticleAffector, IRefObject)
};


//***************************************

struct ISceneController_Particle : public ISceneController {
  virtual bool set_affector(IObjectParticleAffector* a) =0;
  virtual IObjectParticleAffector* get_affector(void) =0;
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::ISceneController_Particle, ISceneController)
};

//***************************************
// INLINES
//***************************************

inline bool IObjectParticleAffector::step(HSceneNode node, BScalar dt) 
{
  bool ok = true;
  for(int i=0; i<~node->visuals; i++) {
    IObjectVisualState* s = node->visuals(i);
    IVisualState* pe = NULL;
    if(!node->object->t_get_other_interface(pe))
      continue;
    if(pe->setup_ok!=0)
      continue;
    if(!pe->particle_setup(particle_size, quantity, pics)) {
      pe->setup_ok = -1;
      return false;
    }
    if(!begin(pe)) {
      pe->setup_ok = -1;
      return false;
    }
    ok = step1(pe, dt) || ok;
  }
  return ok;
}

//***************************************
// 
//***************************************

} // namespace lib3d
} // namespace UMS

#endif
