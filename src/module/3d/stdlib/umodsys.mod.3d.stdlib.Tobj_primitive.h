//------------------------------------
// object_primitive
//------------------------------------

namespace object_primitive {

//------------------------------------
// RGeneral::
//------------------------------------

//------------------------------------
//------------------------------------
// IGeneralObject

bool RGeneral::valid(void) const
{
  return true;
}

PVisualObject RGeneral::visual_new(IRenderer* r, IRenderObject* ro) // generate visual dynamic data
{
  if(ro==NULL)
    return NULL;
  RRender* ror;
  if(!ro->t_root_get_other_interface(ror))
    return NULL;
  return new RVisual(ror);
}

PRenderObject RGeneral::render_new(IRenderer* r) // generate visual static data
{
  return new RRender(this, r);
}

PLogicObject RGeneral::logic_new(void) // generate logical (scene) data
{
  return NULL;
}

/*
PPhysicsObject RGeneralObject_Camera::physics_new(void) // generate physics data
{
  return NULL;
}
*/

IGeneralObject* RGeneral::get_root(void) // return root render object for states
{
  return this;
}

//------------------------------------
//------------------------------------

RGeneral::RGeneral(DOwner *pv, const SParameters& args)
: refs(pv), kind(ok_Null)
{
  DCString skind;
  if(args.get("kind", skind)) {
    if(skind=="cube") {
      kind = ok_Cube;
    } else if(skind=="sphere") {
      kind = ok_Sphere;
    } else if(skind=="pyramid") {
      kind = ok_Pyramid;
    } else if(skind=="cone") {
      kind = ok_Cone;
    } else if(skind=="cylinder") {
      kind = ok_Cylinder;
    }
  }
}

RGeneral::~RGeneral(void)
{
}

//------------------------------------
// RRender::
//------------------------------------

//------------------------------------
//------------------------------------
// IRenderObject

bool RRender::valid(void) const
{
  return true;
}

bool RRender::render_update(void) // re-initalize links
{
  vamain.clear();
  vatex.clear();
  nvertices = 0;
  //
  // rebuild vertex arrays
  switch(g()->kind) {
    case ok_Null:
      return true;
    case ok_Cube:
      return build_cube();
  }
  return false;
}

PGeneralObject RRender::object_get(void) const // get object
{
  return *refs;
}

PVisualObject RRender::visual_new(void) // generate dynamic data
{
  return new RVisual(this);
}

//------------------------------------
//------------------------------------

bool RRender::render(const SRenderState& state)
{
  if(nvertices==0)
    return true;
  if(!state.refill(rsc_TLocalCamera))
    return false;
  //
  IRenderDriver *driver = state.driver;
  driver->render_start();
  driver->setup_T(state.T_local_camera);
  if(vamain.valid())
    state.driver->setup_array(vamain);
  if(vatex.valid())
    state.driver->setup_array(vatex);
  state.driver->render_primitive(ptype, nvertices);
  driver->render_stop();
  //
  return true;
}

bool RRender::build_cube(void)
{
  VertexPC q[6][4];
  for(int i=0; i<6; i++) {
    for(int j=0; j<4; j++) {
      VertexPC& v = q[i][j];
      const uint8 *xyz = cube_quad_points[i][j];
      v.set_xyz(xyz[0]*0.3f+0.3f, xyz[1]*0.3f+0.3f, xyz[2]*0.3f+0.3f);
      v.set_rgba(i&4 ? 255 : 127, i&2 ? 255 : 127, i&1 ? 255 : 127);
    }
  }
  vamain = renderer->driver_get()->create_array(2, VertexPC::s_inf, 6*4, &q, sizeof(q));
  if(!vamain.valid())
    return false;
  nvertices = 6*4;
  ptype = rp_Quad;
  return true;
}

//------------------------------------
//------------------------------------

RRender::RRender(DOwner *pv, IRenderer *rr)
: refs(pv)
{
  nvertices = 0;
  renderer = rr;
}

RRender::~RRender(void)
{
}

//------------------------------------
// RVisual::
//------------------------------------

//------------------------------------
//------------------------------------
// IVisualObject

bool RVisual::valid(void) const
{
  return true;
}

PGeneralObject RVisual::object_get(void) const // get object
{
  return (*refs)->object_get();
}

PRenderObject RVisual::render_get(void) const // get object
{
  return *refs;
}

bool RVisual::visual_update(void) // re-initalize links
{
  return true;
}

bool RVisual::logic_sync(void) // update data
{
  return false;
}

bool RVisual::render_state(const SRenderState& state)
{
  bool rv = r()->render(state);
  return true;
}

//------------------------------------
//------------------------------------

RVisual::RVisual(DOwner *pv)
: refs(pv) 
{
}

RVisual::~RVisual(void)
{
}


//------------------------------------
// VertexPC::
//------------------------------------

SVertexElemInfo VertexPC::s_inf[2] = {
  SVertexElemInfo::based_on_a<float32>(vc_Coord, 3),              
  SVertexElemInfo::based_on_a<uint8>(vc_Color, 4)
};

//------------------------------------
// /object_primitive
//------------------------------------

} // namespace object_primitive

//------------------------------------
// 
//------------------------------------
