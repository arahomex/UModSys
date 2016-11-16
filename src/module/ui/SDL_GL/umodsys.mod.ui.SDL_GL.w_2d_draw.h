
//***************************************
// RRenderDriver3D::
//***************************************

// -- render text
//

bool RRenderDriver3D::render_text_advanced(TextInfo &info, BCStrL piclist, int count) 
{
  if(!IsPhmV(phm_2D))
    return false;
  if(piclist==NULL || *piclist==0)
    return true;
  if(cur_font_ttf.valid()) {
    if(count==-1)
      count = tl::su::slen(piclist);
    bool is_caretsize = (info.options & lib2d::to_CalcCaretSize)!=0;
    int x=info.a->v[0], y=info.a->v[1];
    for(int i=0; i<count; i++) {
      int pic = piclist[i];
      const RMultiImage2D_SDL_ttf::Glyph *g = cur_font_ttf->get_glyph(pic);
      if(g==NULL || g->tex.valid())
        continue; // skip dummy glyph
      if(is_caretsize && i>=info.caret_pos) {
        info.calc_caret.set(x+g->x, y+g->y, g->w, g->h);
        is_caretsize = false;
      }
      //
      gl.h_glColor4fv(cur_color.v);
      gl.set_stage(0);
      gl.set_tex2d(true);
      gl.set_bound(g->tex.tinfo.name);
      gl.h_Rect(x+g->x, y+g->y, g->w, g->h);
      gl.set_tex2d(false);
      //
      x += g->spacing;
    }
    return true;
  }
  return false;
}

//
// -- render quad/box/tri

void RRenderDriver3D::render_box(const lib2d::DPoint& a, const lib2d::DPoint& b) 
{
  if(!IsPhmV(phm_2D))
    return;
  gl.glBegin(GL_QUADS);
  gl.glVertex2i(a.v[0], a.v[1]);
  gl.glVertex2i(b.v[0], a.v[1]);
  gl.glVertex2i(b.v[0], b.v[1]);
  gl.glVertex2i(a.v[0], b.v[1]);
  gl.glEnd();
}

void RRenderDriver3D::render_tri(const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPoint& c) 
{
  if(!IsPhmV(phm_2D))
    return;
  gl.glBegin(GL_TRIANGLES);
  gl.glVertex2i(a.v[0], a.v[1]);
  gl.glVertex2i(b.v[0], b.v[1]);
  gl.glVertex2i(c.v[0], c.v[1]);
  gl.glEnd();
}

void RRenderDriver3D::render_quad(const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPoint& c, const lib2d::DPoint& d) 
{
  if(!IsPhmV(phm_2D))
    return;
  gl.glBegin(GL_QUADS);
  gl.glVertex2i(a.v[0], a.v[1]);
  gl.glVertex2i(b.v[0], b.v[1]);
  gl.glVertex2i(c.v[0], c.v[1]);
  gl.glVertex2i(d.v[0], d.v[1]);
  gl.glEnd();
}

// -- render quad/box/tri texture

void RRenderDriver3D::render_box(lib2d::IImage* image, const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPointf& ta, const lib2d::DPointf& tb) 
{
  if(!IsPhmV(phm_2D))
    return;
}

// -- render line/rect

void RRenderDriver3D::render_line(const lib2d::DPoint& a, const lib2d::DPoint& b) 
{
  if(!IsPhmV(phm_2D))
    return;
  gl.glBegin(GL_LINES);
  gl.glVertex2i(a.v[0], a.v[1]);
  gl.glVertex2i(b.v[0], b.v[1]);
  gl.glEnd();
}

void RRenderDriver3D::render_rect(const lib2d::DPoint& a, const lib2d::DPoint& b) 
{
  if(!IsPhmV(phm_2D))
    return;
  gl.glBegin(GL_LINES);
  gl.glVertex2i(a.v[0], a.v[1]);
  gl.glVertex2i(b.v[0], a.v[1]);
  //
  gl.glVertex2i(b.v[0], a.v[1]);
  gl.glVertex2i(b.v[0], b.v[1]);
  //
  gl.glVertex2i(b.v[0], b.v[1]);
  gl.glVertex2i(a.v[0], b.v[1]);
  //
  gl.glVertex2i(a.v[0], b.v[1]);
  gl.glVertex2i(a.v[0], a.v[1]);
  gl.glEnd();
}

//***************************************
// END
//***************************************
