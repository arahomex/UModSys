//
// RSDLVideoSurfaceGL::
//

// -- render primitives ordered

bool RSDLVideoSurfaceGL::render_primitive(eRenderPrimitiveType primitive, int count, int start0, ...)
{
  int starts[MAXLISTS];
  starts[0] = start0;
  va_list va;
  va_start(va, start0);
  for(int i=1; i<list_count; i++)
    starts[i] = va_arg(va, int);
  va_end(va);
  return render_primitive(primitive, count, starts);
}

bool RSDLVideoSurfaceGL::render_primitive(eRenderPrimitiveType primitive, int count, const int* starts)
{
  if(disabled_primitives || disabled_render)
    return false;
  //
  int mode = 0;
  switch(primitive) {
    case rp_Point: mode = GL_POINTS; fv_1.npolys+=count; break;
    case rp_Line: mode = GL_LINES; count*=2; fv_1.npolys+=count; break;
    case rp_Tri: mode = GL_TRIANGLES; count*=3; fv_1.npolys+=count; break;
    case rp_Quad: mode = GL_QUADS; count*=4; fv_1.npolys+=count; break;
    case rp_Fan: mode = GL_TRIANGLE_FAN; fv_1.npolys+=count-2; break;
    case rp_Strip: mode = GL_TRIANGLE_STRIP; fv_1.npolys+=count-2; break;
    case rp_Poly: mode = GL_POLYGON; fv_1.npolys++; break;
    case rp_MultiLine: mode = GL_LINE_STRIP; fv_1.npolys+=count-1; break;
    default: return false;
    // GL_QUAD_STRIP GL_LINE_LOOP
  }
  ru();
  if(list_count==1) {
    if(lists[0].list==NULL) {
      gl.glDrawArrays(mode, starts[0], count);
    } else {
      _glDrawElements(mode, count, GL_UNSIGNED_INT, lists[0].list+starts[0]);
    }
  } else if(list_count>1) {
    gl.glBegin(mode);
    if(lists[0].list==NULL) {
      for(int i=0; i<count; i++) {
        for(int k=1; k<list_count; k++) {
          if(lists[k].list!=NULL) {
            emul_glArrayElement(lists[k].list[i + starts[k]], lists[k].rm);
          } else {
            emul_glArrayElement(starts[k] + i, lists[k].rm);
          }
        }
        _glArrayElement(starts[0]+i);
      }
    } else {
      for(int i=0; i<count; i++) {
        for(int k=1; k<list_count; k++) {
          if(lists[k].list!=NULL) {
            emul_glArrayElement(lists[k].list[i + starts[k]], lists[k].rm);
          } else {
            emul_glArrayElement(starts[k] + i, lists[k].rm);
          }
        }
        _glArrayElement(lists[0].list[i + starts[0]] );
      }
    }
    gl.glEnd();
  } else {
    return false; // no lists at all
  }
  return true;
}

#define COOR(P, pa, pb, n) (P.p##pa.v[0]*Ox.v[n] + P.p##pb.v[1]*Oy.v[n] + add[n])
#define VERTEX(P, pa, pb) gl.glVertex3d(COOR(P, pa, pb, 0), COOR(P, pa, pb, 1), COOR(P, pa, pb, 2))

void RSDLVideoSurfaceGL::render_picture(const SPicture& A)
{
  const SRenderState& S = *svstate;
//  return;
//  set_transmode(tm_Glow);
  c_picture.set_texture(A.tex);
  setup_material(&c_picture);
//  gl.glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  gl.glEnable(GL_ALPHA_TEST);
  //
  const DPoint& Ox = S.ortx;
  const DPoint& Oy = S.orty;
  const double* add = &S.T_local_camera.v[3*4+0];
  //
  gl.glBegin(GL_QUADS);
    // 0,0
    gl.glMultiTexCoord2fvARB(gl_texture_bylevel_ARB[0], A.t1.v);
    VERTEX(A, 1, 1);
    // 0,1
    gl.glMultiTexCoord2fARB(gl_texture_bylevel_ARB[0], A.t1.v[0], A.t2.v[1]);
    VERTEX(A, 1, 2);
    // 1,1
    gl.glMultiTexCoord2fvARB(gl_texture_bylevel_ARB[0], A.t2.v);
    VERTEX(A, 2, 2);
    // 1,0
    gl.glMultiTexCoord2fARB(gl_texture_bylevel_ARB[0], A.t2.v[0], A.t1.v[1]);
    VERTEX(A, 2, 1);
  gl.glEnd();
//  gl.glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void RSDLVideoSurfaceGL::render_picture(const SPicture& A, const SPicture& B, float trans)
{
  const SRenderState& S = *svstate;
  const DPoint& Ox = S.ortx;
  const DPoint& Oy = S.orty;
  const double* add = &S.T_local_camera.v[3*4+0];
  //
//  set_transmode(tm_Glow);
  gl.glAlphaFunc(GL_GREATER, 0);
  gl.glEnable(GL_ALPHA_TEST);
  gl.glDepthFunc(GL_LEQUAL);
  //
  c_picture.set_texture(A.tex);
  setup_material(&c_picture);
  gl.glColor4f(1, 1, 1, 1-trans);
  gl.glBegin(GL_QUADS);
    // 0,0
    gl.glMultiTexCoord2fvARB(gl_texture_bylevel_ARB[0], A.t1.v);
    VERTEX(A, 1, 1);
    // 0,1
    gl.glMultiTexCoord2fARB(gl_texture_bylevel_ARB[0], A.t1.v[0], A.t2.v[1]);
    VERTEX(A, 1, 2);
    // 1,1
    gl.glMultiTexCoord2fvARB(gl_texture_bylevel_ARB[0], A.t2.v);
    VERTEX(A, 2, 2);
    // 1,0
    gl.glMultiTexCoord2fARB(gl_texture_bylevel_ARB[0], A.t2.v[0], A.t1.v[1]);
    VERTEX(A, 2, 1);
  gl.glEnd();
  //
  c_picture.set_texture(B.tex);
  setup_material(&c_picture);
  gl.glColor4f(1, 1, 1, trans);
  gl.glBegin(GL_QUADS);
    // 0,0
    gl.glMultiTexCoord2fvARB(gl_texture_bylevel_ARB[0], B.t1.v);
    VERTEX(B, 1, 1);
    // 0,1
    gl.glMultiTexCoord2fARB(gl_texture_bylevel_ARB[0], B.t1.v[0], B.t2.v[1]);
    VERTEX(B, 1, 2);
    // 1,1
    gl.glMultiTexCoord2fvARB(gl_texture_bylevel_ARB[0], B.t2.v);
    VERTEX(B, 2, 2);
    // 1,0
    gl.glMultiTexCoord2fARB(gl_texture_bylevel_ARB[0], B.t2.v[0], B.t1.v[1]);
    VERTEX(B, 2, 1);
  gl.glEnd();
  //
//  gl.set_composite_ARB_all(GL_REPLACE);
  _set_color();
  gl.glDepthFunc(GL_LESS);
  gl.glEnable(GL_DEPTH_TEST);
  gl.glDisable(GL_ALPHA_TEST);
}

void RSDLVideoSurfaceGL::render_particles(const SObjectParticle* op) {
  const SRenderState& S = *svstate;
  set_transmode(lib2d::tm_Glow);
  for(; op; op=op->next) {
    if(op->pic==NULL || op->pic->tex==NULL || op->color.v[3]<0.005)
      continue; // skip pic
    //
    const SPicture& A = *op->pic;
    c_picture.set_texture(A.tex);
    setup_material(&c_picture);
    //
    gl.glColor4fv(op->color.v);
    gl.glAlphaFunc(GL_GREATER, 0);
    gl.glEnable(GL_ALPHA_TEST);
    gl.glDepthFunc(GL_LEQUAL);
    //
    const DPoint& Ox = S.ortx;
    const DPoint& Oy = S.orty;
    DPoint padd = S.T_local_camera.get_translate() + S.T_local_camera.mult3(p2p(op->pos));
    const double* add = padd.v;
    //
    gl.glBegin(GL_QUADS);
      // 0,0
      gl.glMultiTexCoord2fvARB(gl_texture_bylevel_ARB[0], A.t1.v);
      VERTEX(A, 1, 1);
      // 0,1
      gl.glMultiTexCoord2fARB(gl_texture_bylevel_ARB[0], A.t1.v[0], A.t2.v[1]);
      VERTEX(A, 1, 2);
      // 1,1
      gl.glMultiTexCoord2fvARB(gl_texture_bylevel_ARB[0], A.t2.v);
      VERTEX(A, 2, 2);
      // 1,0
      gl.glMultiTexCoord2fARB(gl_texture_bylevel_ARB[0], A.t2.v[0], A.t1.v[1]);
      VERTEX(A, 2, 1);
    gl.glEnd();
  }
  _set_color();
}

void RSDLVideoSurfaceGL::render_text_3d(const int *piclist, int count)
{
/*
  const SRenderState& S = *svstate;
  if(sfont) {
    setup_mmap(1);
    //
    gl.glAlphaFunc(GL_GREATER, 0);
    gl.glEnable(GL_ALPHA_TEST);
    gl.glDepthFunc(GL_LEQUAL);
    //
    setup_map(0, sfont->tex.tex.tinfo.name,
              &SRenderMapFlags(rmff_Linear, rmfc_Modulate, rmfw_Disabled));

    //
    if(mode2d) {
      for(;count>0;count--,piclist++) {
        gl.glBegin(GL_QUADS);
          // 0,0
          gl.glMultiTexCoord2fvARB(gl_texture_bylevel_ARB[0], A.t1.v);
          VERTEX(A, 1, 1);
          // 0,1
          gl.glMultiTexCoord2fARB(gl_texture_bylevel_ARB[0], A.t1.v[0], A.t2.v[1]);
          VERTEX(A, 1, 2);
          // 1,1
          gl.glMultiTexCoord2fvARB(gl_texture_bylevel_ARB[0], A.t2.v);
          VERTEX(A, 2, 2);
          // 1,0
          gl.glMultiTexCoord2fARB(gl_texture_bylevel_ARB[0], A.t2.v[0], A.t1.v[1]);
          VERTEX(A, 2, 1);
        gl.glEnd();
      }
    }
  }
*/
}

#undef VERTEX
#undef COOR

bool RSDLVideoSurfaceGL::setup_2d(const lib2d::DPoint* vsize, const lib2d::DPoint* voffset, const DTexPoint* relsize, const DTexPoint *reloffset)
{
  int m2d = mode2d;
  if(m2d) mode_2d_end();
  //
  if(vsize) screen2d_vsize = *vsize;
  if(voffset) screen2d_voffset = *voffset;
  if(relsize) screen2d_relsize = *relsize;
  if(reloffset) screen2d_reloffset = *reloffset;
  //
  param_update_2d();
  //
  if(m2d) mode_2d_begin();
  return true;
}

bool RSDLVideoSurfaceGL::map_2d_to_3d(DTexPoint &p3d, const lib2d::DPoint &p2d)
{
  lib2d::DPoint pp = p2d - screen2d_voffset, pp2;
  DTexPoint rv = screen2d_relsize;
  rv.mult(BScalar(pp(0))/screen2d_vsize(0), BScalar(pp(1))/screen2d_vsize(1));
  rv += screen2d_reloffset;
  p3d = rv*2 + DTexPoint(-1);
  return true;
}

bool RSDLVideoSurfaceGL::map_2d_to_3d(DPoint &p3d, const lib2d::DPoint &p2d)
{
  return false;
}

bool RSDLVideoSurfaceGL::map_3d_to_2d(lib2d::DPoint &p2d, const DPoint &p3d)
{
  return false;
}

bool RSDLVideoSurfaceGL::map_3d_to_2d(lib2d::DPoint &p2d, const DTexPoint &p3d)
{
  return false;
}
