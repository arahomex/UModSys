#include <umodsys/lib/3d/lib3d.driver.h>
#include <umodsys/lib/3d/lib3d.scene.h>
#include <umodsys/lib/3d/lib3d.visual.h>

using namespace UModSys::lib3d;

//***************************************
// SRenderState::
//***************************************

SRenderState::SRenderState(IRenderer* rr, IScene* sc, IRenderDriver *drv, HSceneNode n)
: driver(drv), node(n), renderer(rr), scene(sc),
  state(NULL), 
  // shape(NULL), render(NULL),
  camera(NULL), camstate(NULL),
  //
  T_camera_world(NULL), T_local_world(NULL),
  extras(0), phase(0), outlines(0), nlights(0),
  ncameraplanes(0), cameraplanes_world(NULL), cameraplanes_local(NULL)
{
  if(n) {
    color = n->color;
//    shape = n->shape;
//    state = n->get_visual(rr);
  }
}

#if 0
//***************************************
// SMaterial::
//***************************************

bool SMaterial::update_texpoints(DTexPoint *dest, const DTexPoint *src, int count, ITexture* tex)
{
  if(!dest || !src)
    return false;
  //
  if(tex) {
    DTexPoint ss = tex->get_fsize();
    for(int i=0; i<count; i++) {
      (*dest++).mult(ss, *src++);
    }
  } else {
    for(int i=0; i<count; i++)
      *dest++ = *src++;
  }
  return true;
}

//***************************************
// IObjectClass::
//***************************************

IObjectClass::IObjectClass(BCStr name, int no, int ns, int nr, int nv)
: param_visual(nv), param_render(nr), param_shape(ns), param_object(no), 
  object_class(name) 
{
}

//***************************************
// IObjectRenderState::
//***************************************

bool IObjectRenderState::clone_copy(const IObjectRenderState *R) 
{ 
  return true; 
}

//***************************************

IObjectRenderState::IObjectRenderState(IObject* obj, IObject *robj, IRenderer* r)
: params(obj->get_class()->param_render), 
  object(obj), real_object(robj), renderer(r), seqno(0), vlinks(0)
{
}

//***************************************
// IObjectVisualState::
//***************************************

bool IObjectVisualState::clone_copy(const IObjectVisualState *R) 
{ 
  return true; 
}

//***************************************

IObjectVisualState::IObjectVisualState(IObjectRenderState* b)
: params(b->object->get_class()->param_visual), 
  rstate(b), seqno(0) {
  b->use();
}

IObjectVisualState::~IObjectVisualState(void)
{
  if(rstate.valid()) {
    rstate->unuse();
    rstate.clear();
  }
}

//***************************************
// IObjectShapeState::
//***************************************

bool IObjectShapeState::clone_copy(const IObjectShapeState *R) 
{ 
  return true; 
}

//***************************************

IObjectShapeState::IObjectShapeState(IObject* obj)
: params(obj->get_class()->param_shape), 
  seqno(0), object(obj)
{
}
#endif

//***************************************
// ::
//***************************************
