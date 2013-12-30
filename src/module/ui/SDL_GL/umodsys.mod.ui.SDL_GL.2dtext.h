// 
// SFontContextCommon::
// 

//
// -- text helpers, initalize
//
inline bool RSDLVideoSurfaceGL::font_init(DFontContext& CC)
{
  emul_mmap(1);
  render_update();
/*
  M.debug_put(dps_Info+12, "F %d:", CC.count);
  for(int i=0; i<CC.count; i++) {
    M.debug_put(dps_Info+12, " %x", CC.piclist[i]);
  }
  M.debug_put(dps_Info+12, "\n");
*/
  return true;
}

//
// -- text helpers, reset
//

//
// -- text helpers, draw char by id
//

inline bool RSDLVideoSurfaceGL::font_draw(DFontContext& CC)
{
  const SPicture *cd = &CC.picture;
  if(cd->tex==NULL)
    return true;
  SSDLVideoTextureGL* tex = cd->tex->tget_hwinfo<SSDLVideoTextureGL>(&gl);
  if(tex==NULL)
    return true;
  //
//  M.debug_put(dps_Info+8, "C %x {%g %g} {%g %g}\n", CC.current, CC.ax, CC.ay, CC.bx, CC.by);
  tex->bind();
  tex->set_flags(SRenderMapFlags(rmff_Unknown, rmfc_Modulate, rmfw_Unknown));
  gl.e();
  //
  gl.glBegin(GL_QUADS);
  gl.glTexCoord2f(cd->t1.v[0], cd->t1.v[1]);
  gl.glVertex2f(CC.ax, CC.ay);
  gl.glTexCoord2f(cd->t2.v[0], cd->t1.v[1]);
  gl.glVertex2f(CC.bx, CC.ay);
  gl.glTexCoord2f(cd->t2.v[0], cd->t2.v[1]);
  gl.glVertex2f(CC.bx, CC.by);
  gl.glTexCoord2f(cd->t1.v[0], cd->t2.v[1]);
  gl.glVertex2f(CC.ax, CC.by);
  gl.glEnd();
  //
  gl.e();
  return true;
}


//
// gl 2d primitives

//
// -- render text
//

bool RSDLVideoSurfaceGL::render_text_advanced(TextInfo &info, BStrU piclist, int count)
{
  if(sfont) {
    DFontContext cc(this, sfont, info, piclist, count);
    return cc.draw();
  }
  return false;
}

