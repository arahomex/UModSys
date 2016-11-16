// 
// RSDLVideoSurfaceGL::
// 

//
// gl 2d primitives

//
// -- render quad/box/tri
//

void RSDLVideoSurfaceGL::render_box(const lib2d::DPoint& a, const lib2d::DPoint& b) {
  emul_mmap(0);
  render_update();
  gl.glBegin(GL_QUADS);
  gl.glVertex2i(a(0), a(1));
  gl.glVertex2i(b(0), a(1));
  gl.glVertex2i(b(0), b(1));
  gl.glVertex2i(a(0), b(1));
  gl.glEnd();
}

void RSDLVideoSurfaceGL::render_tri(const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPoint& c) {
  emul_mmap(0);
  render_update();
  gl.glBegin(GL_TRIANGLES);
  gl.glVertex2i(a(0), a(1));
  gl.glVertex2i(b(0), b(1));
  gl.glVertex2i(c(0), c(1));
  gl.glEnd();
}

void RSDLVideoSurfaceGL::render_quad(const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPoint& c, const lib2d::DPoint& d) {
  emul_mmap(0);
  render_update();
  gl.glBegin(GL_TRIANGLES);
  gl.glVertex2i(a(0), a(1));
  gl.glVertex2i(b(0), b(1));
  gl.glVertex2i(c(0), c(1));
  gl.glVertex2i(d(0), d(1));
  gl.glEnd();
}

//
  // -- render quad/box/tri textured
//

void RSDLVideoSurfaceGL::render_box(lib2d::IImage* aimg, const lib2d::DPoint& a, const lib2d::DPoint& b, const DTexPoint& ta, const DTexPoint& tb) 
{
  RSDLVideoTextureGL *img;    
  if(aimg==NULL || !aimg->t_get_other_interface(img))
    return; // no image
  //
  emul_mmap(1);
  gl.set_stage_ARB(0);
  gl.set_ctc_ARB(false);
  gl.set_tex2d_ARB(true);
  img->tex.bind();
  img->tex.set_flags(SRenderMapFlags(rmff_Unknown, rmfc_Modulate, rmfw_UVWrap));
  //
  gl.glBegin(GL_QUADS);
  gl.glTexCoord2f(ta(0), ta(1));
  gl.glVertex2i(a(0), a(1));
  gl.glTexCoord2f(tb(0), ta(1));
  gl.glVertex2i(b(0), a(1));
  gl.glTexCoord2f(tb(0), tb(1));
  gl.glVertex2i(b(0), b(1));
  gl.glTexCoord2f(ta(0), tb(1));
  gl.glVertex2i(a(0), b(1));
  gl.glEnd();
}

//
// -- render line/rect
//

void RSDLVideoSurfaceGL::render_line(const lib2d::DPoint& a, const lib2d::DPoint& b) {
  emul_mmap(0);
  render_update();
  gl.glBegin(GL_LINES);
  gl.glVertex2i(a(0), a(1));
  gl.glVertex2i(b(0), b(1));
  gl.glEnd();
}

void RSDLVideoSurfaceGL::render_rect(const lib2d::DPoint& a, const lib2d::DPoint& b) {
  emul_mmap(0);
  render_update();
  gl.glBegin(GL_LINES);
  gl.glVertex2i(a(0), a(1));
  gl.glVertex2i(b(0), a(1));
  //
  gl.glVertex2i(b(0), a(1));
  gl.glVertex2i(b(0), b(1));
  //
  gl.glVertex2i(b(0), b(1));
  gl.glVertex2i(a(0), b(1));
  //
  gl.glVertex2i(a(0), b(1));
  gl.glVertex2i(a(0), a(1));
  gl.glEnd();
}

inline bool RSDLVideoSurfaceGL::x_drawimage(SSDLVideoTextureGL* tex, float x, float y, float dx, float dy, float u, float v, float du, float dv)
{
  gl.set_stage_ARB_t(0);
  gl.set_tex2d_ARB(true);
  tex->bind();
  gl.glBegin(GL_QUADS);
  //
  gl.glTexCoord2f(u, v);
  gl.glVertex2f(x, y);
  //
  gl.glTexCoord2f(u, v+dv);
  gl.glVertex2f(x, y+dy);
  //
  gl.glTexCoord2f(u+du, v+dv);
  gl.glVertex2f(x+dx, y+dy);
  //
  gl.glTexCoord2f(u+du, v);
  gl.glVertex2f(x+dx, y);
  //
  gl.glEnd();
  //
  return true;
}

