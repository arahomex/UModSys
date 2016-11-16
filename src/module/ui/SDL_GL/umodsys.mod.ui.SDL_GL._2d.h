// 
// RSDLVideoSurfaceGL::
// 


//
// gl 2d primitives

bool RSDLVideoSurfaceGL::setup_font(lib2d::IMultiImage* font, const DTexPoint *size) 
{
  if(font==sfont)
    return true; // same font already set
  //
  sfont.clear();
  //
  kfont = size ? *size : DTexPoint(1);
  //
  if(font && font->t_get_other_interface_n(sfont)) {
    // update font cache info
    return true;
  }
  return false;
}

void RSDLVideoSurfaceGL::mode_2d_begin(void) {
  /* Note, there may be other things you need to change,
     depending on how you have your OpenGL state set up. */
  gl.glPushAttrib(GL_ENABLE_BIT|GL_VIEWPORT_BIT|GL_TEXTURE_BIT|GL_COLOR_BUFFER_BIT);
  //
  gl.glDisable(GL_DEPTH_TEST);
  gl.glDisable(GL_CULL_FACE);
  //
  gl.set_blend(true);
  gl.set_blend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //
  gl.set_stage_ARB(0);
  gl.set_tex2d_ARB(true);
  gl.set_composite_ARB_all(GL_MODULATE);
  //
  gl.glViewport(0, 0, surface->w, surface->h);
  //
  gl.glMatrixMode(GL_PROJECTION);
  gl.glPushMatrix();
  gl.glLoadIdentity();
//  gl.glOrtho(0.0, screen2d_vsize(0), screen2d_vsize(1), 0.0, 0.0, 1.0);
  gl.glOrtho(screen2d_voffset(0), screen2d_voffset(0)+screen2d_vsize(0), 
             screen2d_voffset(1)+screen2d_vsize(1), screen2d_voffset(1), 
             0.0, 1.0);
  //
  gl.glMatrixMode(GL_MODELVIEW);
  gl.glPushMatrix();
  gl.glLoadIdentity();
  //
  disable_lights();
}

void RSDLVideoSurfaceGL::mode_2d_end(void) {
  gl.glMatrixMode(GL_MODELVIEW);
  gl.glPopMatrix();
  gl.glMatrixMode(GL_PROJECTION);
  gl.glPopMatrix();
  gl.glPopAttrib();
}

void RSDLVideoSurfaceGL::draw2d_rect(const DTexPoint& a, const DTexPoint& b) {
   gl.glBegin(GL_QUADS);
   gl.glVertex2f(a.v[0], a.v[1]);
   gl.glVertex2f(b.v[0], a.v[1]);
   gl.glVertex2f(b.v[0], b.v[1]);
   gl.glVertex2f(a.v[0], b.v[1]);
   gl.glEnd();
}

void RSDLVideoSurfaceGL::draw2d_rect_m(const DTexPoint& a, const DTexPoint& b, const DTexPoint& ma, const DTexPoint& mb) {
   gl.glBegin(GL_QUADS);
   gl.glTexCoord2f(ma.v[0], ma.v[1]);
   gl.glVertex2f(a.v[0], a.v[1]);
   gl.glTexCoord2f(mb.v[0], ma.v[1]);
   gl.glVertex2f(b.v[0], a.v[1]);
   gl.glTexCoord2f(mb.v[0], mb.v[1]);
   gl.glVertex2f(b.v[0], b.v[1]);
   gl.glTexCoord2f(ma.v[0], mb.v[1]);
   gl.glVertex2f(a.v[0], b.v[1]);
   gl.glEnd();
}


IConsoleHFCSDisplay::P RSDLVideoSurfaceGL::new_console_HFCSD(const lib2d::DPoint& pos, 
                                                             const lib2d::DPoint& size, 
                                                             const SParameters* par) 
{
//  return new RSDLConsoleFCS(this, font, pos, size);
  return NULL;
}
