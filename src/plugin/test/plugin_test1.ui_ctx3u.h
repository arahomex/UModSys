// RTest1_Shell::

bool RTest1_Shell::UI_Info::new_va_1(void)
{
  lib3d::SVertexElemInfo s_lys[2] = {
    lib3d::SVertexElemInfo::based_on<lib3d::DPoint>(lib3d::vc_Coord),
    lib3d::SVertexElemInfo::based_on<lib3d::DColor>(lib3d::vc_Color)
  };
  lib3d::SVertexElemInfo s_lys2[2] = {
    lib3d::SVertexElemInfo::based_on_a<float32>(lib3d::vc_Coord, 3),              
    lib3d::SVertexElemInfo::based_on_a<uint8>(lib3d::vc_Color, 4)
  };
  {
    va_tri = rd3d->create_array(2, s_lys, 3);
    if(!va_tri.valid())
      return false;
    //  
    va_tri->set_layer_elem(lib3d::DPoint(-0.50, 0.50, 0.8),  2, 0); va_tri->set_layer_elem(lib3d::DColor(1, 0, 0), 2, 1);
    va_tri->set_layer_elem(lib3d::DPoint(0.50, 0.50, 0.8),   1, 0); va_tri->set_layer_elem(lib3d::DColor(0, 1, 0), 1, 1);
    va_tri->set_layer_elem(lib3d::DPoint(-0.00, -0.50, 0.8), 0, 0); va_tri->set_layer_elem(lib3d::DColor(0, 0, 1), 0, 1);
  }
  //
  {
    VertexPC tri[3];
    tri[2].set(lib3d::DPoint(-0.80, 0.80, 0.9), lib3d::DColor(0, 1, 1)); 
    tri[1].set(lib3d::DPoint(0.80, 0.80, 0.9), lib3d::DColor(1, 0, 1)); 
    tri[0].set(lib3d::DPoint(-0.00, -0.80, 0.9), lib3d::DColor(1, 1, 0)); 
    vas_tri = rd3d->create_array(2, s_lys2, 3, &tri, sizeof(tri));
    if(!vas_tri.valid())
      return false;
  }
  //
  {
    VertexPC q[6][4];
    for(int i=0; i<6; i++) {
      for(int j=0; j<4; j++) {
        VertexPC& v = q[i][j];
        const uint8 *xyz = lib3d::cube_quad_points[i][j];
        v.set_xyz(xyz[0]*0.3f+0.3f, xyz[1]*0.3f+0.3f, xyz[2]*0.3f+0.3f);
        v.set_rgba(i&4 ? 255 : 127, i&2 ? 255 : 127, i&1 ? 255 : 127);
      }
    }
    vas_cube = rd3d->create_array(2, s_lys2, 6*4, &q, sizeof(q));
    if(!vas_cube.valid())
      return false;
  }
  return true;
}

bool RTest1_Shell::UI_Info::new_va_qc(void)
{
  lib3d::SVertexElemInfo s_lys2[2] = {
    lib3d::SVertexElemInfo::based_on_a<float32>(lib3d::vc_Coord, 3),              
    lib3d::SVertexElemInfo::based_on_a<uint8>(lib3d::vc_Color, 4)
  };
  lib3d::DColorb clrs[6];
  for(int i=0; i<6; i++) {
    clrs[i].set(i&4 ? 255 : 127, i&2 ? 255 : 127, i&1 ? 255 : 127);
  }
  //
//  const int dx = 400, dy = 400, dz = 256;
  const int dx = 32*5, dy = 32*5, dz = 32*8;
  const int npoints = (dx*dy*2 + dy*dz*2 + dx*dz*2)*4; 
  libutils::STimeMeasureNum tmn1(npoints), tmn2(npoints); 
  //
  tmn1.start();
  tl::TDynarrayDynamic<VertexPC> qc;
  if(!qc.Reserve(npoints))
    return false;
  for(int x=0; x<dx; x++) {
    for(int y=0; y<dy; y++) {
      for(int z=0; z<dz; z++) {
        for(int f=0; f<6; f++) {
          const lib3d::DColorb& clr = clrs[f];
          const sint8 *norm = lib3d::cube_faceshift[f];
          int xx = norm[0] + x;
          int yy = norm[1] + y;
          int zz = norm[2] + z;
          if(xx<0 || xx>=dx) { // x-face
          } else if(yy<0 || yy>=dy) { // y-face
          } else if(zz<0 || zz>=dz) { // z-face
          } else {
            continue;
          }
          int n = ~qc;
          if(!qc.ResizeRel(4))
            return false;
          VertexPC *p = &qc[n];
          for(int i=0; i<4; i++, p++) {
            const uint8 *xyz = lib3d::cube_quad_points[f][i];
            p->set_rgba(clr.v[0], clr.v[1], clr.v[2]);
            p->set_xyz(xyz[0] + x, xyz[1] + y, xyz[2] + z);
          }
        }
      }
    }
  }
  if(~qc!=npoints)
    return false; // bad algorithm
  tmn1>>tmn2;
  vas_cubechunk = rd3d->create_array(2, s_lys2, ~qc, qc.All(), ~qc*sizeof(VertexPC));
  if(!vas_cubechunk.valid())
    return false;
  tmn2.stop();
  s_dbg.put(
    d_SubGen, cl_Info, 
    "  Cube chunk array [%d,%d,%d]/%d is generated at: %.3f/%.3f sec, %.3f/%.3f usec per vertex, %.3f/%.3f Mvertex/sec\n", 
    dx, dy, dz, (int)~qc, 
    tmn1.sec(), tmn2.sec(),
    tmn1.spn()*1e6, tmn2.spn()*1e6,
    tmn1.nps()/1e6, tmn2.nps()/1e6
  );
  return true;
}

void RTest1_Shell::UI_Info::fps_view_matrix(void)
{
  if(is_upz) {
    static lib3d::DPoint3 up(0,0,1);
    lib3d::DScalar cosy = cos(fps_ori.v[1]);
    lib3d::DPoint3 lookat( sin(fps_ori.v[0]) * cosy, cos(fps_ori.v[0]) * cosy, sin(fps_ori.v[1]) );
    view_matrix.set_look_at(fps_pos, fps_pos+lookat, up);
  } else {
    static lib3d::DPoint3 up(0,1,0);
    lib3d::DScalar cosy = cos(fps_ori.v[1]);
    lib3d::DPoint3 lookat( sin(fps_ori.v[0]) * cosy, sin(fps_ori.v[1]), cos(fps_ori.v[0]) * cosy );
    view_matrix.set_look_at(fps_pos, fps_pos+lookat, up);
  }
}

void RTest1_Shell::UI_Info::fps_move(lib3d::DScalar x, lib3d::DScalar y, lib3d::DScalar z)
{
  fview_matrix = true;
  lib3d::DPoint3 offset;
  if(is_upz) {
    lib3d::DPoint3 forward( sin(fps_ori.v[0]), cos(fps_ori.v[0]), 0 );
    lib3d::DPoint3 right( forward.v[1], forward.v[0], 0 );
    //
    offset = right*x + forward*y;
    offset.v[2] += z;
  } else {
    lib3d::DPoint3 forward( sin(fps_ori.v[0]), 0, cos(fps_ori.v[0]) );
    lib3d::DPoint3 right( -forward.v[2], 0, forward.v[0] );
    //
    offset = right*x + forward*y;
    offset.v[1] += z;
  }
  offset.normalize();
  offset *= move_speed;
  //
  fps_pos += offset;
  //
  s_dbg.put(d_SubGen, cl_Info, "fps_pos: { %6.2f %6.2f %6.2f }\n", fps_pos(0), fps_pos(1), fps_pos(2));
}

void RTest1_Shell::UI_Info::fps2_move(int ort, lib3d::DScalar value)
{
  lib3d::DPoint3 p_speed(move_speed);
  camera_T.add_translate( camera_T.mult3_ortv(p_speed, ort, value) );
}

void RTest1_Shell::UI_Info::fps2_add_rot(lib3d::DScalar x, lib3d::DScalar y)
{
  if(x==0 && y==0)
    return;
  lib3d::DPoint3 a_speed(mou_sensitivity);
  lib3d::DMatrix4 T, oldT = camera_T;
  T.set_translate(0,0,0);
  T.set_rotate_about( oldT.mult3_ort(-1, 1), a_speed.v[1]*x);
  camera_T.mult3(oldT, T);
  oldT = camera_T;
  T.set_rotate_about( oldT.mult3_ort(1, 0), a_speed.v[0]*y);
  camera_T.mult3(oldT, T);
}

void RTest1_Shell::UI_Info::fps_add_rot(lib3d::DScalar x, lib3d::DScalar y)
{
  fview_matrix = true;
  x *= mou_sensitivity; y *= mou_sensitivity;
  //
  fps_ori.v[0] = fmod(fps_ori.v[0] + x, mathc::pi * 2);
  fps_ori.v[1] = scalar_max(scalar_min(fps_ori.v[1] + y, mathc::pi / 2 - 0.1), -mathc::pi / 2.0 + 0.1);
  s_dbg.put(d_SubGen, cl_Info, "fps_rot: { %6.2f %6.2f %6.2f }\n", math3d::todeg(fps_ori(0)), math3d::todeg(fps_ori(1)), math3d::todeg(fps_ori(2)));
}

bool RTest1_Shell::UI_Info::key_pressed(const libui::SKeyboardInputRaw& key)
{
  switch(key.code) {
    case libui::k_escape:
      f_quit = true;
      return true;
    case libui::k_w: // forward
      fps2_move(2, -1);
      return true;
    case libui::k_s: // backward
      fps2_move(2, 1);
      return true;
    case libui::k_a: // left
      fps2_move(0, -1);
      return true;
    case libui::k_d: // right
      fps2_move(0, 1);
      return true;
    case libui::k_q: // down
      fps2_move(1, 1);
      return true;
    case libui::k_e: // up
      fps2_move(1, -1);
      return true;
    case libui::k_space:
      fmouvis = !fmouvis;
      mouc->mouse_setvisible(fmouvis);
      return true;
  }
  return false;
}

bool RTest1_Shell::UI_Info::key_text(const libui::SKeyboardInputText& key)
{
  return false;
}

bool RTest1_Shell::UI_Info::mouse_event(const libui::SMouseInput& ms)
{
  if(fmouvis)
    return false;
  unsigned mm = ms.get_buttonmask();
  if(mm & 1)
    return false;
  if(mm & 2) {
//    fmouvis = !fmouvis;
//    mouc->mouse_setvisible(fmouvis);
    return false;
  }
  fps2_add_rot(ms.rel(0), -ms.rel(1));
  return false;
}

