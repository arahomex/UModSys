//------------------------------------
// object_camera
//------------------------------------

namespace object_camera {

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
  return new RVisual(this, r);
}

PRenderObject RGeneral::render_new(IRenderer* r) // generate visual static data
{
  return NULL;
}

PLogicObject RGeneral::logic_new(void) // generate logical (scene) data
{
  return NULL;
}

/*
PPhysicsObject RGeneral::physics_new(void) // generate physics data
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
: refs(pv) 
{
  def_fov.set(mathc::torad(90), mathc::torad(90)); // full fov 90 deg
  screen = 1;
  fade = 100;
  //
  double fv;
  if(args.get("fov", fv)) {
    def_fov.set(fv, fv);
    def_fov.set(fv, fv);
  }
  if(args.get("fov_x", fv))
    def_fov.v[0] = fv;
  if(args.get("fov_y", fv))
    def_fov.v[1] = fv;
  //
  if(args.get("screen", fv) || args.get("min", fv))
    screen = fv;
  if(args.get("fade", fv) || args.get("max", fv))
    fade = fv;
}

RGeneral::~RGeneral(void)
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
  return *refs;
}

PRenderObject RVisual::render_get(void) const // get object
{
  return NULL;
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
  const_cast<SRenderState&>(state).link_camera(this);
  camera_setup(state.driver);
  return true;
}

//------------------------------------
//------------------------------------
// IVisualObject_Camera

DTexPoint RVisual::camera_get_fov(void) const
{
  return fov;
}

bool RVisual::camera_get_T(DMatrix4 &T) const
{
  return false;
}

bool RVisual::camera_get_xyz(DPoint* ox, DPoint* oy, DPoint* oz) const
{
  if(ox) *ox = right;
  if(oy) *oy = up;
  if(oz) *oz = target;
  return true;
}

int RVisual::camera_get_planes(DPlane* planes) const
{
  if(planes==NULL)
    return 6; // box
  //
  DScalar x = tan(fov.v[0]/2)*screen/2, y = tan(fov.v[1]/2)*screen/2;
  // 0/1 = near/far
  planes[0].from_3pr(DPoint(-x, -y, screen), DPoint(x, -y, screen), DPoint(x, y, screen));
  planes[1].from_3pr(DPoint(-x, -y, fade), DPoint(-x, y, fade), DPoint(x, y, fade));
  // 2/3 = left/right
  planes[2].from_3pr(DPoint(0, 0, 0), DPoint(-x, y, screen), DPoint(-x, -y, screen));
  planes[3].from_3pr(DPoint(0, 0, 0), DPoint(x, -y, screen), DPoint(x, y, screen));
  // 4/5 = up/down
  planes[4].from_3pr(DPoint(0, 0, 0), DPoint(x, -y, screen), DPoint(-x, -y, screen));
  planes[5].from_3pr(DPoint(0, 0, 0), DPoint(-x, y, screen), DPoint(x, y, screen));
  //
  return 6;
}

// cast ray from [-1,1],[-1,1] to world coordinates
bool RVisual::camera_cast_ray(const DTexPoint &screen, DPoint &start, DPoint &dir) const
{
  DScalar x = tan(fov.v[0]/2)*this->screen/2, y = tan(fov.v[1]/2)*this->screen/2;
//  start.set(0, screen.v[1] * y, screen.v[0] * x);
  start.set(-screen.v[0] * x, screen.v[1] * y, this->screen);
  dir = start.normalized();
  return true;
}

bool RVisual::camera_setup(IRenderDriver *driver) 
{
  return driver->camera_frustum(DPoint(0,0,0), fov, screen, fade);
}

//------------------------------------
//------------------------------------

RVisual::RVisual(DOwner *pv, IRenderer *rr)
: refs(pv) 
{
  fov = pv->def_fov;
  target.set(0,0,1);
  up.set(0,1,0);
  right.set(1,0,0);
  screen = pv->screen;
  fade = pv->fade;
}

RVisual::~RVisual(void)
{
}

//------------------------------------
// /object_camera
//------------------------------------

} // namespace object_camera

//------------------------------------
// 
//------------------------------------
