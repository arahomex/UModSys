
//***************************************
// RRenderDriver3D::
//***************************************

// -- camera setup

bool RRenderDriver3D::camera_ortho(const DPoint &center, const DPoint &size)
{
  gl.glMatrixMode(GL_PROJECTION);
  gl.glLoadIdentity();
  gl.glOrtho(center.v[0]-size.v[0]/2, center.v[0]+size.v[0]/2, 
             center.v[1]-size.v[1]/2, center.v[1]+size.v[1]/2, 
             center.v[2]-size.v[2]/2, center.v[2]+size.v[2]/2);
  return gl.glGetError()!=GL_NO_ERROR;
}

bool RRenderDriver3D::camera_frustum(const DPoint &center, const DPoint &size)
{
  gl.glMatrixMode(GL_PROJECTION);
  gl.glLoadIdentity();
  gl.glFrustum(center.v[0]-size.v[0]/2, center.v[0]+size.v[0]/2, 
               center.v[1]-size.v[1]/2, center.v[1]+size.v[1]/2, 
               center.v[2], center.v[2]+size.v[2]);
//  gl.glTranslated(0, 0, -center.v[2]);
  return gl.glGetError()!=GL_NO_ERROR;
}

bool RRenderDriver3D::camera_frustum(const DPoint &center, const DTexPoint &fov, DScalar min, DScalar max)
{
  gl.glMatrixMode(GL_PROJECTION);
  gl.glLoadIdentity();
  DScalar x, y;
  if(fov.v[0]<0.01 || fov.v[0]>math3d::pi-0.01
    || fov.v[1]<0.01 || fov.v[1]>math3d::pi-0.01) {
    return false; // bad FOV
  }
  x = tan(fov.v[0]/2)*min/2, y = tan(fov.v[1]/2)*min/2;
  gl.glFrustum(center.v[0]-x, center.v[0]+x, 
               center.v[1]-y, center.v[1]+y, 
               min, max);
//  gl.glTranslated(0, 0, -min);
  return gl.glGetError()!=GL_NO_ERROR;
}

//***************************************
// -- setup dynamic lights, light indices: <=0 =error, 0x10000+ = HW, 0x20000+ = SW, 0x30000+ = omni


bool RRenderDriver3D::setup_clearlights(eLightType type=lt_All, bool emulated=true, bool hardware=true)
{
  return false;
}

int  RRenderDriver3D::setup_addlight(eLightType type, const SLight& light, const DMatrix4& T)
{
  return -1;
}

bool RRenderDriver3D::setup_setlight(int idx, eLightType type, const SLight& light, const DMatrix4& T)
{
  return false;
}

bool RRenderDriver3D::setup_setlight(int idx, bool enabled)
{
  return false;
}

void RRenderDriver3D::setup_T(const DMatrix4& T)
{
}

bool RRenderDriver3D::setup_material(const SMaterial *mat) // NULL to disable materials
{
  return false;
}

bool RRenderDriver3D::setup_array(IVertexArray *va, int targets, int layers)
{
  if(!is_phmv(phm_3D) || va==NULL)
    return false;
  RVertexArray::SelfP arr;
  if(!va->t_ref_get_other_interface(arr))
    return false;
  if(!arr->Use(layers))
    return false;
  return true;
}

bool RRenderDriver3D::setup_array(IVertexArray *va, eVertexClass target, int layer)
{
  return false;
}

bool RRenderDriver3D::setup_array_none(void)
{
  return false;
}

//***************************************
// END
//***************************************
