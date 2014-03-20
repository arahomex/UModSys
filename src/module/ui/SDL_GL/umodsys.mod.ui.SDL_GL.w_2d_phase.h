
//***************************************
// RRenderDriver3D::
//***************************************

//
// -- main ones

void RRenderDriver3D::begin(void) 
{
  if(!NextPhmV(phm_Begin))
    return;
  //
/*
  double T = parent->clock();
  fv_1.begin( T );
  if(fv_N.time<0)
    fv_N.begin( T );
  //
*/
  //
  gl.glViewport(0, 0, wnd.dx, wnd.dy);
  //
  gl.glClearColor(clear_color.v[0], clear_color.v[1], clear_color.v[2], clear_color.v[3]);
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
//  gl.glOrtho(-100.0,100.0, -100.0,100.0, -1000.0,1000.0);
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
  //
#if 0
  gl.glBegin(GL_TRIANGLES);
  gl.glColor3f(0, 0, 1); gl.glVertex3d(-50, 50, 0);
  gl.glColor3f(0, 1, 0); gl.glVertex3d(50, 50, 0);
  gl.glColor3f(1, 0, 0); gl.glVertex3d(0, -50, 0);
  gl.glEnd();
#endif
  //
  phase_2d(false);
}

void RRenderDriver3D::end(void) 
{
  if(!NextPhmV(phm_End))
    return;
  //
  if(mode2d) {
    Mode_2d_end();
    mode2d = 0;
  }
/*
  sfont.clear();
  c_picture.set_texture(NULL);
*/
  Update();
/*
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

//***************************************
//***************************************

bool RRenderDriver3D::Mode_2d_begin(void)
{
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
  gl.set_stage(0);
  gl.set_tex2d(true);
  gl.set_composite_all(GL_MODULATE);
  //
  gl.glViewport(0, 0, wnd.dx, wnd.dy);
  //
  gl.glMatrixMode(GL_PROJECTION);
  gl.glPushMatrix();
  gl.glLoadIdentity();
//  gl.glOrtho(0.0, screen2d_vsize(0), screen2d_vsize(1), 0.0, 0.0, 1.0);
  gl.glOrtho(screen2d_voffset(0), screen2d_voffset(0) + screen2d_vsize(0), 
             screen2d_voffset(1) + screen2d_vsize(1), screen2d_voffset(1), 
             0.0, 1.0);
  //
  gl.glMatrixMode(GL_MODELVIEW);
  gl.glPushMatrix();
  gl.glLoadIdentity();
  //
//  disable_lights();
  return true;
}

bool RRenderDriver3D::Mode_2d_end(void) 
{
  gl.glMatrixMode(GL_MODELVIEW);
  gl.glPopMatrix();
  gl.glMatrixMode(GL_PROJECTION);
  gl.glPopMatrix();
  gl.glPopAttrib();
  return true;
}

//***************************************
// END
//***************************************
