
//***************************************
// RRenderDriver3D::
//***************************************

libui::ITerminal* RRenderDriver3D::get_terminal(void) const 
{
  return *refs;
}

IRefObject* RRenderDriver3D::get_controller(TypeId ctrl) const 
{
  return NULL;
}

lib2d::IMultiImage::P RRenderDriver3D::new_font(const SParameters& params, const DCString &tagname)
{
  lib2d::IMultiImage::P rv;
  BCStr fn;
  if(params.get("filename", fn) && libmedia::is_extension_case(fn, ".ttf")) {
    if(!ValidateConstruction(rv, new(local_memory()) RMultiImage2D_SDL_ttf(this, params)))
      return NULL;
    return rv;
  }
  return NULL;
}

lib2d::IImage::P RRenderDriver3D::new_picture(const SParameters& params, const DCString &tagname)
{
  return NULL;
}

//
// -- main ones

void RRenderDriver3D::begin(void) 
{
  if(!valid())
    return;
  //
/*
  double T = parent->clock();
  fv_1.begin( T );
  if(fv_N.time<0)
    fv_N.begin( T );
  //
  if(deep_mode!=dm_End && deep_mode!=dm_None) {
    deep_mode = dm_Error;
    M.debug_put(dps_Error, "R3D:: begin() again\n");
  }
  //
*/
  gl.glViewport(0, 0, wnd.dx, wnd.dy);
  //
  gl.glClearColor(0, 0, 0.1f, 0);
  gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  //
  gl.glEnable(GL_DEPTH_TEST);
  gl.glDepthFunc(GL_LESS);
  //
  gl.glEnable(GL_CULL_FACE);
  gl.glCullFace(GL_FRONT);
  //
  gl.set_blend(false);
  gl.set_blend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //
  gl.glMatrixMode(GL_PROJECTION);
  gl.glLoadIdentity();
  gl.glOrtho(-100.0,100.0, -100.0,100.0, -1000.0,1000.0);
//  gl.glOrtho(-100.0,100.0, -100.0,100.0, 0.0,1000.0);
//  gl.glFrustum(-100.0,100.0, -100.0,100.0, 3.0,100.0);
  //
  gl.glMatrixMode(GL_MODELVIEW);
  gl.glLoadIdentity();
  //
//  _set_color();
  //
  for(int i=0; i<gl.max_tex_level; i++) {
    gl.set_stage(0);
    gl.set_tex2d(false);
    gl.set_composite(GL_REPLACE);
  }
  gl.set_stage(0);
  gl.e();
//  deep_mode = dm_Frame;
  //
#if 1
  gl.glBegin(GL_TRIANGLES);
  gl.glColor3f(0, 0, 1); gl.glVertex3d(-50, 50, 0);
  gl.glColor3f(0, 1, 0); gl.glVertex3d(50, 50, 0);
  gl.glColor3f(1, 0, 0); gl.glVertex3d(0, -50, 0);
  gl.glEnd();
#endif
#if 0
  static float theta = 0;
  theta += 1;
  gl.glMatrixMode(GL_PROJECTION);
	gl.glPushMatrix();
  gl.glLoadIdentity();
	gl.glRotatef( theta, 0.0f, 0.0f, 1.0f );
	gl.glBegin( GL_TRIANGLES );
	gl.glColor3f( 1.0f, 0.0f, 0.0f ); gl.glVertex2f( 0.0f, 1.0f );
	gl.glColor3f( 0.0f, 1.0f, 0.0f ); gl.glVertex2f( 0.87f, -0.5f );
	gl.glColor3f( 0.0f, 0.0f, 1.0f ); gl.glVertex2f( -0.87f, -0.5f );
	gl.glEnd();
	gl.glPopMatrix();
#endif
}

void RRenderDriver3D::end(void) 
{
  if(!valid())
    return;
  //
/*
  if(deep_mode!=dm_Frame) {
    deep_mode = dm_Error;
    M.debug_put(dps_Error, "R3D:: end() again\n");
  }
  if(mode2d) {
    mode_2d_end();
    mode2d = 0;
  }
  sfont.clear();
  c_picture.set_texture(NULL);
*/
  Update();
/*
  deep_mode = dm_End;
  //
  {
    double T = parent->clock();
    fv_1.end(T);
    frame_values.add("frame.polygons", fv_1.npolys);
    frame_values.add("frame.objects", fv_1.nobjects);
    frame_values.add("frame.time", fv_1.time);
    fv_N.npolys += fv_1.npolys;
    fv_N.nobjects += fv_1.nobjects;
    fv_N.nframes++;
    if(fv_N.len(T)>fv_N_length) {
      fv_N.end(T);
      frame_values.add("average.polygons", fv_N.npolys/fv_N.time);
      frame_values.add("average.objects", fv_N.nobjects/fv_N.time);
      frame_values.add("average.fps", fv_N.nframes/fv_N.time);
      fv_N.begin(T);
    }
  }
*/
}

// -- information

const SParameters* RRenderDriver3D::get_max_values(void) const 
{
  return &max_values;
}

const SParameters* RRenderDriver3D::get_frame_values(void) const 
{
  return &frame_values;
}

bool RRenderDriver3D::set_parameters(BCStr mode, const SParameters& P) 
{
  return false;
}

//
// -- setup next primitives

bool RRenderDriver3D::setup_font(lib2d::IMultiImage* font, const lib2d::DPointf* scale) 
{
  if(!valid())
    return false;
  cur_font_ttf.clear();
  if(font==NULL)
    return true;
  if(font->t_ref_get_other_interface(cur_font_ttf))
    return true;
  return false;
}

void RRenderDriver3D::setup_color(const lib2d::DColorf& c) 
{
  if(!valid())
    return;
  cur_color.setp3(c.v);
  set_color();
}

void RRenderDriver3D::setup_color(const lib2d::DColor& c) 
{
  if(!valid())
    return;
  cur_color.set3(c(0)/255.0f, c(1)/255.0f, c(2)/255.0f);
  set_color();
}

void RRenderDriver3D::setup_coloralpha(const lib2d::DColorAlphaf& c, int transmode) 
{
  if(!valid())
    return;
  cur_color = c;
  cur_tm = transmode;
  set_color();
}

void RRenderDriver3D::setup_coloralpha(const lib2d::DColorAlpha& c, int transmode) 
{
  if(!valid())
    return;
  cur_color.set(c(0)/255.0f, c(1)/255.0f, c(2)/255.0f, c(3)/255.0f);
  cur_tm = transmode;
  set_color();
}

void RRenderDriver3D::setup_blendcolor(const lib2d::SBlendColor& c) 
{
  if(!valid())
    return;
  cur_color.set(c.color(0)/255.0f, c.color(1)/255.0f, c.color(2)/255.0f, c.color(3)/255.0f);
  cur_tm = c.transmode;
  set_color();
}

void RRenderDriver3D::setup_blendcolor(const lib2d::SBlendColorf& c) 
{
  if(!valid())
    return;
  cur_color = c.color(0);
  cur_tm = c.transmode;
  set_color();
}

void RRenderDriver3D::setup_alpha(lib2d::DColorElemi alpha, int transmode) 
{
  if(!valid())
    return;
  cur_color.set4(alpha/255.0f);
  cur_tm = transmode;
  set_color();
}

void RRenderDriver3D::setup_alpha(lib2d::DColorElemf alpha, int transmode) 
{
  if(!valid())
    return;
  cur_color.set4(alpha);
  cur_tm = transmode;
  set_color();
}

// -- render text
//

bool RRenderDriver3D::render_text_advanced(TextInfo &info, BCStrL piclist, int count) 
{
  if(!valid())
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
  if(!valid())
    return;
}

void RRenderDriver3D::render_tri(const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPoint& c) 
{
  if(!valid())
    return;
}

void RRenderDriver3D::render_quad(const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPoint& c, const lib2d::DPoint& d) 
{
  if(!valid())
    return;
}

// -- render quad/box/tri texture

void RRenderDriver3D::render_box(lib2d::IImage* image, const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPointf& ta, const lib2d::DPointf& tb) 
{
  if(!valid())
    return;
}

// -- render line/rect

void RRenderDriver3D::render_line(const lib2d::DPoint& a, const lib2d::DPoint& b) 
{
  if(!valid())
    return;
}

void RRenderDriver3D::render_rect(const lib2d::DPoint& a, const lib2d::DPoint& b) 
{
  if(!valid())
    return;
}

//***************************************
// END
//***************************************
