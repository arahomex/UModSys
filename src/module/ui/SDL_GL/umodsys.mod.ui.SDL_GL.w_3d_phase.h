
//***************************************
// RRenderDriver3D::
//***************************************

void RRenderDriver3D::phase_start(int phaseid, int alpha_mode) // begin desired phase
{
  if(!NextPhmV(phm_3D))
    return;
  if(mode2d) {
    Mode_2d_end();
    mode2d = 0;
  }
  if(alpha_mode) {
    gl.set_stage(0);
    gl.set_composite_all(GL_MODULATE);
    //
    gl.set_blend(true);
//    set_transmode(alpha_mode);
    //
    gl.glEnable(GL_ALPHA_TEST);
    gl.glAlphaFunc(GL_GREATER, 0);
    gl.glEnable (GL_BLEND);
    gl.glDepthMask(GL_FALSE);
  } else {
    gl.set_stage(0);
    gl.set_composite_all(GL_REPLACE);
    //
    gl.set_blend(false);
    //
    gl.glDisable(GL_ALPHA_TEST);
    gl.glDepthMask(GL_TRUE);
  }
}

void RRenderDriver3D::phase_2d(bool nearmode) // begin 2d phase (after or before all other)
{
  if(!NextPhmV(phm_2D))
    return;
  if(!mode2d) {
    Mode_2d_begin();
  }
  mode2d = nearmode ? -1 : 1;
}

void RRenderDriver3D::render_start(void) // caches is filled, begin render elements
{
/*
  if(disabled_render)
    return;
  if(deep_mode!=dm_Frame) {
    deep_mode = dm_Error;
    M.debug_put(dps_Error, "R3D:: render_start() without begin()\n");
  }
*/
  gl.glEnable(GL_DEPTH_TEST);
  gl.glDepthFunc(GL_LESS);
  //
  gl.glEnable(GL_CULL_FACE);
  gl.glCullFace(GL_BACK);
  gl.glFrontFace(GL_CCW);
  gl.glEnable(GL_AUTO_NORMAL);
  //
  gl.glDisableClientState(GL_VERTEX_ARRAY);
  gl.glDisableClientState(GL_COLOR_ARRAY);
  gl.glDisableClientState(GL_NORMAL_ARRAY);
  for(int i=0; i<gl.tex_levels; i++) {
    gl.set_stage(i);
    gl.set_ctc(false);
    gl.set_tex2d(false);
  }
  gl.set_stage(0);
  //
  gl.glMatrixMode(GL_MODELVIEW);
  gl.glPushMatrix();
/*
  _set_color();
  //
  lightsys.prepare();
  deep_mode = dm_Draw;
  setup_lists2(0, 0, NULL);
*/
}

void RRenderDriver3D::render_stop(void) // caches is not required anymore
{
/*
  if(disabled_render)
    return;
  if(deep_mode!=dm_Draw) {
    deep_mode = dm_Error;
    M.debug_put(dps_Error, "R3D:: render_stop() without render_start()\n");
  }
  c_points.clear();
  c_colors.clear();
  c_normals.clear();
*/
  gl.glDisableClientState(GL_VERTEX_ARRAY);
  gl.glDisableClientState(GL_COLOR_ARRAY);
  gl.glDisableClientState(GL_NORMAL_ARRAY);
  gl.set_composite_all(GL_REPLACE);
  //
  for(int i=0; i<gl.tex_levels; i++) {
    gl.set_stage(i);
    gl.set_ctc(false);
    gl.set_tex2d(false);
//    c_texpoints[i].clear();
  }
  gl.set_stage(0);
  //
  gl.glMatrixMode(GL_MODELVIEW);
  gl.glPopMatrix();
/*
  _set_color();
  deep_mode = dm_Frame;
  setup_lists2(0, 0, NULL);
*/
}

//***************************************

bool RRenderDriver3D::setup_state(const SRenderState& S) // discarded on phase end or render start/stop
{
  return false;
}

bool RRenderDriver3D::setup_2d(const DPoint2i* vsize, const DPoint2i* voffset, const DTexPoint* relsize, const DTexPoint *reloffset) // window coordinates
{
  sint8 m2d = mode2d;
  if(m2d) {
    Mode_2d_end();
  }
  //
  if(vsize) 
    screen2d_vsize = *vsize;
  if(voffset) 
    screen2d_voffset = *voffset;
  if(relsize) 
    screen2d_relsize = *relsize;
  if(reloffset) 
    screen2d_reloffset = *reloffset;
  //
//  param_update_2d();
  //
  if(m2d) {
    Mode_2d_begin();
  }
  return true;
}


//***************************************
// END
//***************************************
