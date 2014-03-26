//------------------------------------
// RGeneralObject_Camera::
//------------------------------------

//------------------------------------
//------------------------------------
// IGeneralObject

bool RGeneralObject_Camera::valid(void) const
{
  return true;
}

PVisualObject RGeneralObject_Camera::visual_new(IRenderer* r, IRenderObject* ro) // generate visual dynamic data
{
  return new VisualState(this, r);
}

PRenderObject RGeneralObject_Camera::render_new(IRenderer* r) // generate visual static data
{
  return NULL;
}

PLogicObject RGeneralObject_Camera::logic_new(void) // generate logical (scene) data
{
  return NULL;
}

/*
PPhysicsObject RGeneralObject_Camera::physics_new(void) // generate physics data
{
  return NULL;
}
*/

IGeneralObject* RGeneralObject_Camera::get_root(void) // return root render object for states
{
  return this;
}

//------------------------------------
//------------------------------------

RGeneralObject_Camera::RGeneralObject_Camera(DOwner *pv, const SParameters& args)
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

RGeneralObject_Camera::~RGeneralObject_Camera(void)
{
}

//------------------------------------
// RGeneralObject_Camera::VisualState::
//------------------------------------

//------------------------------------
//------------------------------------
// IVisualObject

bool RGeneralObject_Camera::VisualState::valid(void) const
{
  return true;
}

PGeneralObject RGeneralObject_Camera::VisualState::object_get(void) const // get object
{
  return *refs;
}

PRenderObject RGeneralObject_Camera::VisualState::render_get(void) const // get object
{
  return NULL;
}

bool RGeneralObject_Camera::VisualState::visual_update(void) // re-initalize links
{
  return true;
}

bool RGeneralObject_Camera::VisualState::logic_sync(void) // update data
{
  return false;
}

bool RGeneralObject_Camera::VisualState::render_state(SRenderState& state)
{
//  return camera_render_state(state);
  return false;
}

//------------------------------------
//------------------------------------
// IVisualObject_Camera

DTexPoint RGeneralObject_Camera::VisualState::camera_get_fov(void) const
{
  return fov;
}

bool RGeneralObject_Camera::VisualState::camera_get_T(DMatrix4 &T) const
{
  return false;
}

bool RGeneralObject_Camera::VisualState::camera_get_xyz(DPoint* ox, DPoint* oy, DPoint* oz) const
{
  if(ox) *ox = right;
  if(oy) *oy = up;
  if(oz) *oz = target;
  return true;
}

int RGeneralObject_Camera::VisualState::camera_get_planes(DPlane* planes) const
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
bool RGeneralObject_Camera::VisualState::camera_cast_ray(const DTexPoint &screen, DPoint &start, DPoint &dir) const
{
  DScalar x = tan(fov.v[0]/2)*this->screen/2, y = tan(fov.v[1]/2)*this->screen/2;
//  start.set(0, screen.v[1] * y, screen.v[0] * x);
  start.set(-screen.v[0] * x, screen.v[1] * y, this->screen);
  dir = start.normalized();
  return true;
}

bool RGeneralObject_Camera::VisualState::camera_setup(IRenderDriver *driver) 
{
  return driver->camera_frustum(DPoint(0,0,0), fov, screen, fade);
}

//------------------------------------
//------------------------------------

RGeneralObject_Camera::VisualState::VisualState(DOwner *pv, IRenderer *rr)
: refs(pv) 
{
  fov = pv->def_fov;
  target.set(0,0,1);
  up.set(0,1,0);
  right.set(1,0,0);
  screen = pv->screen;
  fade = pv->fade;
}

RGeneralObject_Camera::VisualState::~VisualState(void)
{
}

//------------------------------------
// 
//------------------------------------
