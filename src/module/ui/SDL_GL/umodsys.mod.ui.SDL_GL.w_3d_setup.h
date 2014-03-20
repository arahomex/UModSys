
//***************************************
// RRenderDriver3D::
//***************************************

void RRenderDriver3D::SetColor(void) 
{
  gl.glColor4fv(cur_color.v);
}

void RRenderDriver3D::Update(void) 
{
  gl.glFinish();
  gl.e();
  SDL_GL_SwapWindow(wnd);
  gl.e();
}

void RRenderDriver3D::SetT(const DMatrix4& T, bool isGet)
{
  gl.glMatrixMode(GL_MODELVIEW);
  //
  if(isGet) {
    double im[16];
    gl.glGetDoublev(GL_MODELVIEW_MATRIX, im);
    double tt[16] = {
      T.v[0*4+0], T.v[0*4+1], T.v[0*4+2], 0,
      T.v[1*4+0], T.v[1*4+1], T.v[1*4+2], 0,
      T.v[2*4+0], T.v[2*4+1], T.v[2*4+2], 0,
      T.v[3*4+0], T.v[3*4+1], T.v[3*4+2], 1
    };
    gl.glMultMatrixd(tt);
    gl.glGetDoublev(GL_MODELVIEW_MATRIX, im);
    gl.e();
  } else {
    double tt[16] = {
      T.v[0*4+0], T.v[0*4+1], T.v[0*4+2], 0,
      T.v[1*4+0], T.v[1*4+1], T.v[1*4+2], 0,
      T.v[2*4+0], T.v[2*4+1], T.v[2*4+2], 0,
      T.v[3*4+0], T.v[3*4+1], T.v[3*4+2], 1
    };
    gl.glMultMatrixd(tt);
  }
}

bool RRenderDriver3D::SetFrustum(const DPoint &center, DScalar dx, DScalar dy, DScalar znear, DScalar zfar, bool isGet)
{
  if(znear<=0 || zfar<=0 || znear>=zfar)
    return false; // bad min/max
  gl.glMatrixMode(GL_PROJECTION);
  gl.glLoadIdentity();
  bool rv1, rv2;
  gl.glFrustum(center.v[0]-dx, center.v[0]+dx, 
               center.v[1]-dy, center.v[1]+dy, 
               znear, zfar);
  rv1 = gl.glGetError()==GL_NO_ERROR;
//  gl.glScaled(1, 1, -1);
//  gl.glRotated(180, 0, 1, 0);
  rv2 = gl.glGetError()==GL_NO_ERROR;
//  gl.glTranslated(0, 0, -min);
  double im[16];
  if(debug_matrix) {
    gl.glGetDoublev(GL_PROJECTION_MATRIX, im);
  }
  return rv1 && rv2;
}

//***************************************

// -- camera setup

bool RRenderDriver3D::camera_ortho(const DPoint &center, const DPoint &size)
{
  gl.glMatrixMode(GL_PROJECTION);
  gl.glLoadIdentity();
  gl.glOrtho(center.v[0]-size.v[0]/2, center.v[0]+size.v[0]/2, 
             center.v[1]-size.v[1]/2, center.v[1]+size.v[1]/2, 
             center.v[2]-size.v[2]/2, center.v[2]+size.v[2]/2);
  bool rv = gl.glGetError()!=GL_NO_ERROR;
  double im[16];
  if(debug_matrix) {
    gl.glGetDoublev(GL_PROJECTION_MATRIX, im);
  }
  return rv;
}

bool RRenderDriver3D::camera_frustum(const DPoint &center, const DPoint &size)
{
  return SetFrustum(
    center, 
    size.v[0]/2, size.v[1]/2, 
    center.v[2], center.v[2]+size.v[2], 
    debug_matrix
  );
}

bool RRenderDriver3D::camera_frustum(const DPoint &center, const DTexPoint &fov, DScalar min, DScalar max)
{
  if(fov.v[0]<0.01 || fov.v[0]>math3d::pi-0.01
    || fov.v[1]<0.01 || fov.v[1]>math3d::pi-0.01) {
    return false; // bad FOV
  }
  return SetFrustum(
    center, 
    tan(fov.v[0]/2)*min/2, tan(fov.v[1]/2)*min/2, 
    min, max, 
    debug_matrix
  );
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
  if(!IsPhmV(phm_3D)/*|| disabled_setup || disabled_render*/)
    return;
  SetT(T, debug_matrix);
}

bool RRenderDriver3D::setup_material(const SMaterial *mat) // NULL to disable materials
{
  return false;
}

bool RRenderDriver3D::setup_array(IVertexArray *va, int targets, int layers)
{
  if(!IsPhmV(phm_3D) || va==NULL)
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
