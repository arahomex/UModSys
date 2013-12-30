// 
// RSDLVideoSurfaceGL::
// 

UI::ITerminal* RSDLVideoSurfaceGL::get_terminal(void)
{
  return parent;
}

UI::IKeyboardController* RSDLVideoSurfaceGL::get_controller_keyboard(void)
{
  return parent;
}

UI::IMouseController* RSDLVideoSurfaceGL::get_controller_mouse(void)
{
  return parent;
}

// ----------------------------------------------------------------------
// -- begin/end
// ----------------------------------------------------------------------

void RSDLVideoSurfaceGL::begin(void) 
{
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
  gl.glViewport(0, 0, surface->w, surface->h);
  //
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
  _set_color();
  //
  for(int i=0; i<gl.max_tex_level; i++) {
    gl.set_stage_ARB(0);
    gl.set_tex2d_ARB(false);
    gl.set_composite_ARB(GL_REPLACE);
  }
  gl.set_stage_ARB(0);
  gl.e();
  deep_mode = dm_Frame;
}

void RSDLVideoSurfaceGL::end(void) 
{
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
  update();
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
}

bool RSDLVideoSurfaceGL::set_parameters(BCStr kind, const SParameters& P) 
{
  if(su::scmp(kind, "disabled")==0) {
    P.get("render", disabled_render);
    P.get("setup", disabled_setup);
    P.get("primitives", disabled_primitives);
    return true;
  }
  return false;
}


// ----------------------------------------------------------------------
// -- setup next primitives
// ----------------------------------------------------------------------

bool RSDLVideoSurfaceGL::setup_lists2(int count, int render_primitive_components, const int* list, ...)
{
  if(disabled_setup || disabled_render || count<0 || count>4)
    return false;

  va_list va;
  va_start(va, list);
  lists[0].rm = render_primitive_components | rpc_Point;
  lists[0].list = list;
  for(int i=1; i<count; i++) {
    lists[i].rm = va_arg(va, int);
    lists[i].list = va_arg(va, int*);
  }
  list_count = count;
  list_updated = true;
  va_end(va);
  return true;
}

bool RSDLVideoSurfaceGL::setup_lists(int render_primitive_components, const int* list, int listid) 
{
  if(disabled_setup || disabled_render || listid<0 || listid>4)
    return false;
  if(listid==0) render_primitive_components |= rpc_Point; else render_primitive_components &= ~rpc_Point;
  lists[listid].rm = render_primitive_components;
  lists[listid].list = list;
  list_count = listid+1;
  list_updated = true;
  return true;
}

void RSDLVideoSurfaceGL::emul_coloralpha(const DColor *color, const float *alpha, const int *tm)
{
  if(disabled_setup || disabled_render)
    return;
  bool update = false;
  if(color && *color!=es_color) {
    es_color = *color;
    update = true;
  }
  if(alpha && *alpha!=es_alpha) {
    es_alpha = *alpha;
    update = true;
  }
  if(update) {
    gl.glColor4f(es_color.v[0], es_color.v[1], es_color.v[2], es_alpha);
  }
  if(tm && *tm!=es_transmode) {
    int tm2 = *tm;
    if(es_alpha!=1 && tm2<=lib2d::tm_Opaque) {
      tm2 = lib2d::tm_Glass;
    }
    if(tm2!=es_transmode) {
      es_transmode = *tm;
      set_transmode(*tm);
    }
  }
//  set_transmode(saved_alpha==0 ? -1 : transmode);
}

void RSDLVideoSurfaceGL::setup_color(const DColor& c) 
{
  emul_coloralpha(&c, NULL, lib2d::tm_Opaque);
  saved_color = c;
}

void RSDLVideoSurfaceGL::setup_coloralpha(const DColorAlpha& ca, int transmode) 
{
  DColor c(ca.v);
  emul_coloralpha(&c, &ca.v[3], transmode);
  saved_color = c;
  saved_alpha = ca.v[3];
  saved_transmode = transmode;
}


void RSDLVideoSurfaceGL::setup_alpha(float alpha, int transmode) 
{
  emul_coloralpha(NULL, &alpha, &transmode);
  saved_alpha = alpha;
  saved_transmode = transmode;
}

void RSDLVideoSurfaceGL::setup_T(const DMatrix4& T) 
{
  if(disabled_setup || disabled_render)
    return;
  set_T(T, false);
}


// ----------------------------------------------------------------------
// -- render bounds
// ----------------------------------------------------------------------

void RSDLVideoSurfaceGL::phase_start(int phaseid, int alpha_mode) 
{
  if(deep_mode!=dm_Frame) {
    deep_mode = dm_Error;
    M.debug_put(dps_Error, "R3D:: phase_start() without begin()\n");
  }
  if(mode2d) {
    mode_2d_end();
    mode2d = 0;
  }
  if(alpha_mode) {
    gl.set_stage_ARB(0);
    gl.set_composite_ARB_all(GL_MODULATE);
    //
    gl.set_blend(true);
    set_transmode(alpha_mode);
    //
    gl.glEnable(GL_ALPHA_TEST);
    gl.glAlphaFunc(GL_GREATER, 0);
    gl.glEnable (GL_BLEND);
    gl.glDepthMask(GL_FALSE);
  } else {
    gl.set_stage_ARB(0);
    gl.set_composite_ARB_all(GL_REPLACE);
    //
    gl.set_blend(false);
    //
    gl.glDisable(GL_ALPHA_TEST);
    gl.glDepthMask(GL_TRUE);
  }
  deep_mode = dm_Frame;
}

void RSDLVideoSurfaceGL::phase_2d(bool nearmode) 
{
  if(deep_mode!=dm_Frame) {
    deep_mode = dm_Error;
    M.debug_put(dps_Error, "R3D:: phase_2d() without begin()\n");
  }
  if(!mode2d) {
    mode_2d_begin();
  }
  mode2d = nearmode ? -1 : 1;
  deep_mode = dm_Frame;
}

void RSDLVideoSurfaceGL::render_start(void) 
{
  if(disabled_render)
    return;
  if(deep_mode!=dm_Frame) {
    deep_mode = dm_Error;
    M.debug_put(dps_Error, "R3D:: render_start() without begin()\n");
  }
  gl.glEnable(GL_DEPTH_TEST);
  gl.glDepthFunc(GL_LESS);
  //
  gl.glEnable(GL_CULL_FACE);
//  gl.glCullFace(GL_BACK);
//  gl.glFrontFace(GL_CW);
  gl.glCullFace(GL_FRONT);
  gl.glFrontFace(GL_CCW);
//  gl.glEnable(GL_AUTO_NORMAL);
  //
  gl.glDisableClientState(GL_VERTEX_ARRAY);
  gl.glDisableClientState(GL_COLOR_ARRAY);
  for(int i=0; i<gl.tex_levels; i++) {
    gl.set_stage_ARB(i);
    gl.set_ctc_ARB(false);
    gl.set_tex2d_ARB(false);
  }
  gl.set_stage_ARB(0);
  //
  gl.glMatrixMode(GL_MODELVIEW);
  gl.glPushMatrix();
  _set_color();
  //
  lightsys.prepare();
  deep_mode = dm_Draw;
  setup_lists2(0, 0, NULL);
}

void RSDLVideoSurfaceGL::render_stop(void) {
  if(disabled_render)
    return;
  if(deep_mode!=dm_Draw) {
    deep_mode = dm_Error;
    M.debug_put(dps_Error, "R3D:: render_stop() without render_start()\n");
  }
  c_points.clear();
  c_colors.clear();
  c_normals.clear();
  gl.glDisableClientState(GL_VERTEX_ARRAY);
  gl.glDisableClientState(GL_COLOR_ARRAY);
  gl.glDisableClientState(GL_NORMAL_ARRAY);
  gl.set_composite_ARB_all(GL_REPLACE);
  //
  for(int i=0; i<gl.tex_levels; i++) {
    gl.set_stage_ARB(i);
    gl.set_ctc_ARB(false);
    gl.set_tex2d_ARB(false);
    c_texpoints[i].clear();
  }
  gl.set_stage_ARB(0);
  //
  gl.glMatrixMode(GL_MODELVIEW);
  gl.glPopMatrix();
  _set_color();
  deep_mode = dm_Frame;
  setup_lists2(0, 0, NULL);
}

void RSDLVideoSurfaceGL::render_update(void) 
{
  if(disabled_render)
    return;

  int i;
  //
  //glFogCoordPointer
/*
  gl.glNormalPointer(GL_FLOAT, sizeof(float), 0);
  gl.glDisableClientState(GL_NORMAL_ARRAY);

  gl.glEdgeFlagPointer(GL_INT, 0);
  gl.glDisableClientState(GL_EDGE_FLAG_ARRAY);
  //
  gl.glIndexPointer(GL_INT, sizeof(int), 0);
  gl.glDisableClientState(GL_INDEX_ARRAY);
*/
  //
/*
  if(gl.is_GL_ARB_vertex_buffer_object) {
    gl.glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    gl.glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
  }
*/
  //
  //
  if(c_colors.dirty) {
    gl.glColorPointer(3, GL_FLOAT, sizeof(DColor), c_colors.cvalues->v);
    c_colors.clear();
  } else if(c_coloralphas.dirty) {
    gl.glColorPointer(4, GL_FLOAT, sizeof(DColor), c_colors.cvalues->v);
    c_coloralphas.clear();
  }
  //
  if(c_points.dirty) {
    gl.glVertexPointer(3, GL_DOUBLE, sizeof(DPoint), c_points.cvalues->v);
    c_points.clear();
  }
  //
  if(c_normals.dirty) {
    gl.glNormalPointer(GL_FLOAT, sizeof(DNormal), c_normals.cvalues->v);
    c_normals.clear();
  }
  //
  int components = lists[0].rm;
  //
  if(components & rpc_Point) {
    gl.glEnableClientState(GL_VERTEX_ARRAY);
  } else {
    gl.glDisableClientState(GL_VERTEX_ARRAY);
  }
  //
  if(components & rpc_Color) {
    gl.glEnableClientState(GL_COLOR_ARRAY);
  } else {
    gl.glDisableClientState(GL_COLOR_ARRAY);
//    c_colors.alloc(NULL, 0);
  }
  //
  if(components & rpc_Normal) {
    gl.glEnableClientState(GL_NORMAL_ARRAY);
  } else {
    gl.glDisableClientState(GL_NORMAL_ARRAY);
  }
  //
  for(i=0; i<gl.tex_levels; i++) {
    if(gl.get_tex2d_ARB(i)) {
      gl.set_stage_ARB(i);
      gl.set_ctc_ARB((components & rpc_Texture)!=0);
      //
      TArrayMemCache<DTexPoint> &ct = c_texpoints[ texlevel_to_tex[i] ];
      if(ct.dirty) {
        gl.glTexCoordPointer(2, GL_FLOAT, sizeof(DTexPoint), ct.cvalues->v);
        ct.clear();
      }
    }
  }
  gl.set_stage_ARB(0);
  //
  list_updated = false;
}

// ----------------------------------------------------------------------
//
// ----------------------------------------------------------------------
