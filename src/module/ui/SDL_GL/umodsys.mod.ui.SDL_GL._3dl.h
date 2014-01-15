// 
// RSDLVideoSurfaceGL::
// 

// ----------------------------------------------------------------------
// -- setup dynamic lights, light indices: <=0 =error, 0x10000+ = HW, 0x20000+ = SW, 0x30000+ = omni
// ----------------------------------------------------------------------

void RSDLVideoSurfaceGL::disable_lights(void)
{
  light_enabled = false;
  gl.glDisable(GL_LIGHTING);
  gl.glLightModelfv(GL_LIGHT_MODEL_AMBIENT, f_color_default);
  gl.glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, f_color_default);
  gl.glMaterialfv(GL_FRONT, GL_DIFFUSE, f_color_zero);
  gl.glMaterialfv(GL_FRONT, GL_SPECULAR, f_color_zero);
}

bool RSDLVideoSurfaceGL::els_clear(void)
{
  if(disabled_setup || disabled_render)
    return true;
  gl.glDisable(GL_LIGHTING);
  for(int i=0; i<max_lights; i++) {
    gl.glDisable(GL_LIGHT0 + i);
  }
  return true;
}

bool RSDLVideoSurfaceGL::els_init_info(emulate_light::SHardwareInfo* info, int id)
{
  info->ui1 = GL_LIGHT0 + id;
  return true;
}

bool RSDLVideoSurfaceGL::els_set_omni(const DColor& color)
{
  if(disabled_setup || disabled_render)
    return true;
  DColorAlpha ca;
  ca.setp3(color.v);
  ca.set4(1);
  gl.glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ca.v);
  omni_color = color;
  gl.glEnable(GL_LIGHTING);
  light_enabled = true;
  return true;
}

bool RSDLVideoSurfaceGL::els_set_omni(emulate_light::SHardwareInfo* info, const SLight& L)
{
  if(disabled_setup || disabled_render)
    return true;
  return false;
}

bool RSDLVideoSurfaceGL::els_set_cone(emulate_light::SHardwareInfo* info, const SLightCone& L, const DMatrix4& T)
{
  if(disabled_setup || disabled_render)
    return true;
  return false;
}

bool RSDLVideoSurfaceGL::els_set_cylinder(emulate_light::SHardwareInfo* info, const SLightCylinder& L, const DMatrix4& T)
{
  if(disabled_setup || disabled_render)
    return true;
  int lid = info->ui1;
  //
  gl.glMatrixMode(GL_MODELVIEW);
//  gl.glPushMatrix();
  set_T(T, true);
  gl.glLightfv(lid, GL_SPOT_DIRECTION, color_direction);
  gl.glLightfv(lid, GL_POSITION, color_direction);
  gl.glLightf(lid, GL_CONSTANT_ATTENUATION, 1);
  gl.glLightf(lid, GL_LINEAR_ATTENUATION, 0);
  gl.glLightf(lid, GL_QUADRATIC_ATTENUATION, 0);
  gl.glLightf(lid, GL_SPOT_CUTOFF, 180);
  gl.glLightf(lid, GL_SPOT_EXPONENT, 0);
  gl.glLoadIdentity();
//  gl.glPopMatrix();
  //
  DColorAlpha ca;
  ca.setp3(L.color.v);
  ca.set4(1);
  //
  gl.glLightfv(lid, GL_AMBIENT, f_color_zero);
  gl.glLightfv(lid, GL_SPECULAR, ca.v);
  gl.glLightfv(lid, GL_DIFFUSE, ca.v);
  //
  gl.glEnable(lid);
  gl.glEnable(GL_LIGHTING);
  light_enabled = true;
  return true;
}

bool RSDLVideoSurfaceGL::els_set_sphere(emulate_light::SHardwareInfo* info, const SLightSphere& L, const DMatrix4& T)
{
  if(disabled_setup || disabled_render)
    return true;
  int lid = info->ui1;
//  return false;
  //
  gl.glMatrixMode(GL_MODELVIEW);
//  gl.glPushMatrix();
  set_T(T, true);
  gl.glLightfv(lid, GL_POSITION, color_position);
  gl.glLightfv(lid, GL_SPOT_DIRECTION, color_direction);
  if(L.sphere.power<1) {
    gl.glLightf(lid, GL_CONSTANT_ATTENUATION, 1);
    gl.glLightf(lid, GL_LINEAR_ATTENUATION, 0);
    gl.glLightf(lid, GL_QUADRATIC_ATTENUATION, 0);
  } else if(L.sphere.power<2, 1) {
    gl.glLightf(lid, GL_CONSTANT_ATTENUATION, 0);
    gl.glLightf(lid, GL_LINEAR_ATTENUATION, 1/L.sphere.base);
    gl.glLightf(lid, GL_QUADRATIC_ATTENUATION, 0);
  } else {
    gl.glLightf(lid, GL_CONSTANT_ATTENUATION, 0);
    gl.glLightf(lid, GL_LINEAR_ATTENUATION, 0);
    gl.glLightf(lid, GL_QUADRATIC_ATTENUATION, 1/(L.sphere.base*L.sphere.base));
  }
  gl.glLightf(lid, GL_SPOT_CUTOFF, 180);
  gl.glLightf(lid, GL_SPOT_EXPONENT, 0);
  gl.glLoadIdentity();
//  gl.glPopMatrix();
  //
  DColorAlpha ca;
  ca.setp3(L.color.v);
  ca.set4(1);
  //
  gl.glLightfv(lid, GL_AMBIENT, f_color_zero);
  gl.glLightfv(lid, GL_SPECULAR, ca.v);
  gl.glLightfv(lid, GL_DIFFUSE, ca.v);
  //
  gl.glEnable(lid);
  gl.glEnable(GL_LIGHTING);
  light_enabled = true;
  return true;
}

// ----------------------------------------------------------------------
// -- setup dynamic lights, light indices: <=0 =error, 0x10000+ = HW, 0x20000+ = SW, 0x30000+ = omni
// ----------------------------------------------------------------------

bool RSDLVideoSurfaceGL::setup_clearlights(eLightType type, bool emulated, bool hardware)
{
  return lightsys.light_clear(type, emulated, hardware);
}

int  RSDLVideoSurfaceGL::setup_addlight(eLightType type, const SLight& light, const DMatrix4& T)
{
  return lightsys.light_add(type, light, T, true);
}

bool RSDLVideoSurfaceGL::setup_setlight(int idx, eLightType type, const SLight& light, const DMatrix4& T)
{
  return lightsys.light_set(idx, type, light, T);
}

bool RSDLVideoSurfaceGL::setup_setlight(int idx, bool enabled)
{
  return lightsys.light_enable(idx, enabled);
}

// ----------------------------------------------------------------------
//
// ----------------------------------------------------------------------
