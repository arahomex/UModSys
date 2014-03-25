// RTest1_Shell::

bool RTest1_Shell::UI_Info::font_pure(void)
{
  TParametersA<1024> pars;
  pars.add("filename", "Neocyr.ttf");
  font = ui_newfont("*::SDL_core::*ttf*", pars);
  if(!font.valid())
    return false;
  return true;
}
bool RTest1_Shell::UI_Info::font_2d(void)
{
  TParametersA<1024> pars;
  pars.add("filename", "Neocyr.ttf");
  pars.add("max_glyph", 0x1000);
  font = rd2d->new_font(pars);
  if(!font.valid()) {
    s_dbg.put(d_SubGen, cl_Error, "  error create font\n");
    return false;
  }
  s_dbg.put(d_SubGen, cl_Info, "  created font: %s => %p\n", font()->root_get_interface_type()->name, font());
  return true;
}

bool RTest1_Shell::UI_Info::init_term(const char *name)
{
  TParametersA<1024> pars;
  term = ui_newterm(name, pars);
  if(!term.valid())
    return false;
  //
  {
    TParametersA<1024> pars;
    if(!term->t_create_handler(keyc, NULL, &pars)) {
      s_dbg.put(d_SubGen, cl_Error, "  error create IKeyboardController\n");
      return false;
    }
    s_dbg.put(d_SubGen, cl_Info, "  created IKeyboardController: %s => %p\n", keyc()->root_get_interface_type()->name, keyc());
  }
  {
    TParametersA<1024> pars;
    if(!term->t_create_handler(mouc, NULL, &pars)) {
      s_dbg.put(d_SubGen, cl_Error, "  error create IMouseController\n");
      return false;
    }
    s_dbg.put(d_SubGen, cl_Info, "  created IMouseController: %s => %p\n", mouc()->root_get_interface_type()->name, mouc());
  }
  return true;
}

bool RTest1_Shell::UI_Info::create_2d(const char *cap)
{
  TParametersA<1024> pars;
  pars.add("caption", cap);
  if(!term->t_create_handler(rd2d, "", &pars)) {
    s_dbg.put(d_SubGen, cl_Error, "  not created lib2d::IRenderDriver\n");
    return false;
  }
  s_dbg.put(d_SubGen, cl_Info, "  created lib2d::IRenderDriver: %s => %p\n", rd2d()->root_get_interface_type()->name, rd2d());
  return true;
}
bool RTest1_Shell::UI_Info::create_3d(const char *cap)
{
  {
    TParametersA<1024> pars;
    rdr = ui_newrenderer("*::stdlib::*", pars);
    if(!rdr.valid())
      return false;
  }
  {
    TParametersA<1024> pars;
    pars.add("caption", cap);
    if(!term->t_create_handler(rd3d, "", &pars)) {
      s_dbg.put(d_SubGen, cl_Error, "  not created lib2d::IRenderDriver\n");
      return false;
    }
    s_dbg.put(d_SubGen, cl_Info, "  created lib2d::IRenderDriver: %s => %p\n", rd3d()->root_get_interface_type()->name, rd3d());
    rd2d = rd3d();
    rdr->driver_set(rd3d, NULL);
  }
  return true;
}

bool RTest1_Shell::UI_Info::new_frames(libui::IController *ctrl)
{
  TParametersA<1024> pars;
  pars.add("driver", rd2d);
  pars.add("font", font);
  frames = ui_newframes("*::frames::*", pars);
  if(!frames.valid())
    return false;
  {
    libui::SFrameCreateParameters fcp;
    fcp.ctrl = ctrl;
    fcp.size = libui::SFrameSize(lib2d::DPoint(0, 150), lib2d::DPoint(400, 200), lib2d::a_Left, lib2d::a_Left);
    libui::IFrame* ft = frames->frame_create_s("frame", fcp, NULL);
    fcp.parent = ft;
    fcp.size = libui::SFrameSize(lib2d::DPoint(0, 0), lib2d::DPoint(400, 30), lib2d::a_Left, lib2d::a_Left);
    fcp.text = "Caption";
    fcp.ctrl_context = 1;
    frames->frame_create_s("text", fcp, NULL);
    fcp.size = libui::SFrameSize(lib2d::DPoint(0, 50), lib2d::DPoint(100, 40), lib2d::a_Left, lib2d::a_Left);
    fcp.text = "Button 1";
    fcp.ctrl_context = 2;
    frames->frame_create_s("button", fcp, NULL);
    fcp.size = libui::SFrameSize(lib2d::DPoint(0, 100), lib2d::DPoint(100, 40), lib2d::a_Left, lib2d::a_Left);
    fcp.text = "Button 2";
    fcp.ctrl_context = 3;
    frames->frame_create_s("button", fcp, NULL);
    fcp.size = libui::SFrameSize(lib2d::DPoint(0, 150), lib2d::DPoint(100, 40), lib2d::a_Left, lib2d::a_Left);
    fcp.text = "Button 3";
    fcp.ctrl_context = 4;
    frames->frame_create_s("button", fcp, NULL);
    {
      fcp.size = libui::SFrameSize(lib2d::DPoint(100, 50), lib2d::DPoint(100, 40), lib2d::a_Left, lib2d::a_Left);
      fcp.text = "value";
      fcp.ctrl_context = 5;
      TParametersA<1024> pars;
      pars.add("max_len", 50);
      frames->frame_create_s("edit", fcp, &pars);
    }
    {
      fcp.size = libui::SFrameSize(lib2d::DPoint(200, 50), lib2d::DPoint(100, 150), lib2d::a_Left, lib2d::a_Left);
      fcp.text = "value";
      fcp.ctrl_context = 6;
      TParametersA<1024> pars;
      pars.add("dy", 16);
      pars.add("user_db", true);
      frames->frame_create_s("list", fcp, &pars);
    }
  }
  frames->output_attach_2d(rd2d);
  frames->input_attach_m(mouc, 10);
  frames->input_attach_k(keyc, 10);
  return true;
}

void RTest1_Shell::UI_Info::shutdown(void)
{
  frames->shutdown();
  term->set_terminal_state(libui::ts_Keyboard, false);
  keyc->key_unlink(0);
  mouc->mouse_unlink(0);
  term->set_terminal_state(libui::ts_Terminated);
}

void RTest1_Shell::UI_Info::cycle(void)
{
  f_quit = false;
  while(!term->get_terminal_state(libui::ts_Terminated) && !term->get_terminal_state(libui::ts_SoftTerminate)) {
    if(f_quit)
      break;
    if(!term->poll_events()) {
      frames->input_process();
      rd2d->begin();
      rd2d->setup_color(lib2d::DColorf(1, 1, 0));
      rd2d->setup_font(font);
      rd2d->render_box(lib2d::DPoint(10, 10), lib2d::DPoint(50, 50));
      rd2d->render_tri(lib2d::DPoint(100, 10), lib2d::DPoint(150, 50), lib2d::DPoint(150, 5));
      {
        tl::su::utf32 text[256];
        static const char txt[] = "Text \xD0\xA0\xD1\x83\xD1\x81\xD1\x81\xD0\xBA\xD0\xB8\xD0\xB9";
        size_t n = tl::su::utf_8to32(text, 256, txt, sizeof(txt), true);
        rd2d->render_text(lib2d::DPoint(10, 100), text, n);
      }
      frames->output_process();
      rd2d->end();
      term->wait(0.010f);
    }
  }
}

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

void RTest1_Shell::UI_Info::cycle3d_a(void)
{
//  move_speed = 0.5*0.1;
  move_speed = 0.1;
  mou_sensitivity = 0.003;
  fps_pos.set(0,0,-1);
//  fps_ori.set(mathc::pi, 0, 0);
  fps_ori.set(0, 0, 0);
  //
  fmouvis = false;
  mouc->mouse_setvisible(fmouvis);
  //
  keyc->key_link(50, NULL, this);
  mouc->mouse_link(50, NULL, this, ~0);
  //
  is_upz = false;
  fps_add_rot(0,0);
  fps_move(0,0,0);
  //
  if(!new_va_1())
    return;
#if 0
  if(!new_va_qc())
    return;
#endif
  //
  {
    camera_T.set_translate(0, 0, -5);
    camera_T.set_rotate_about(lib3d::DPoint(0, 1, 0), math3d::torad(180));
  }
  //
  f_quit = false;
  ticks = 0;
  frame_time = 0.010f;
  while(!term->get_terminal_state(libui::ts_Terminated) && !term->get_terminal_state(libui::ts_SoftTerminate)) {
    if(f_quit)
      break;
    if(!term->poll_events()) {
      frames->input_process();
      rdr->render_begin();
      //
      rd3d->phase_start(0, false);
      //
      if(0) {
        rd3d->camera_frustum(lib3d::DPoint(0, 0, 0), lib3d::DTexPoint(math3d::torad(90), math3d::torad(90)), 0.1, 10);
        if(fview_matrix) {
          fps_view_matrix();
        }
        rd3d->setup_T(view_matrix);
      } else {
        rd3d->camera_frustum(lib3d::DPoint(0, 0, 0), lib3d::DTexPoint(math3d::torad(90), math3d::torad(90)), 0.1, 10);
        //rd3d->camera_ortho(lib3d::DPoint(0, 0, 0), lib3d::DPoint(5, 5, 5));
        lib3d::DMatrix4 T, CT, RT;
        /**/
        T.set_inversed(camera_T);
        /**/
/*
        CT.setE(); 
        CT.set_translate(0, 0, -5+ticks);
        CT.set_rotate_about(lib3d::DPoint(0, 1, 0), math3d::torad(180));
        RT.set_inversed(CT);
        T = RT;
*/
//        rd3d->setup_T(T);
        /**/
//        T.set_look_at(lib3d::DPoint(0, 0, 0.02*ticks), lib3d::DPoint(0, 0, 10), lib3d::DPoint(0, 1, 0));
//        T.set_look_at(lib3d::DPoint(0.75, 0.5, -0.5), lib3d::DPoint(0, 0, 10), lib3d::DPoint(1, 1, 0));
//        T.set_look_at(lib3d::DPoint(0.75, 0.5, 0.0), lib3d::DPoint(0, 0, 10), lib3d::DPoint(1, 1, 0));
//        T.set_look_at(lib3d::DPoint(0.0, 0.0, -5+ticks), lib3d::DPoint(0, 0, 10), lib3d::DPoint(0, 1, 0));
        /**/
        rd3d->setup_T(T);
      }
      if(va_tri.valid()) {
        rd3d->setup_array(va_tri);
        rd3d->render_primitive(lib3d::rp_Tri, 3);
      }
      if(vas_tri.valid()) {
        rd3d->setup_array(vas_tri);
        rd3d->render_primitive(lib3d::rp_Tri, 3);
      }
      if(vas_cube.valid()) {
        rd3d->setup_array(vas_cube);
        rd3d->render_primitive(lib3d::rp_Quad, 4*6);
      }
      if(vas_cubechunk.valid()) {
        rd3d->setup_array(vas_cubechunk);
        rd3d->render_primitive(lib3d::rp_Quad, vas_cubechunk->get_array_count());
      }
      //
      rd3d->phase_2d();
      rd3d->setup_color(lib2d::DColorf(1, 1, 0));
      rd3d->setup_font(font);
      rd3d->render_box(lib2d::DPoint(10, 10), lib2d::DPoint(50, 50));
      rd3d->render_tri(lib2d::DPoint(100, 10), lib2d::DPoint(150, 50), lib2d::DPoint(150, 5));
      //
      frames->output_process();
      rdr->render_end();
      term->wait(frame_time);
      ticks += frame_time;
    }
  }
}

void RTest1_Shell::UI_Info::cycle3d_b(void)
{
  move_speed = 0.1;
  mou_sensitivity = 0.001;
  //
  fmouvis = false;
  mouc->mouse_setvisible(fmouvis);
  //
  keyc->key_link(50, NULL, this);
  mouc->mouse_link(50, NULL, this, ~0);
  //
  is_upz = false;
  fps_add_rot(0,0);
  fps_move(0,0,0);
  //
  if(!new_va_1())
    return;
#if 0
  if(!new_va_qc())
    return;
#endif
  //
  {
    camera_T.set_translate(0, 0, -5);
    camera_T.set_rotate_about(lib3d::DPoint(0, 1, 0), math3d::torad(180));
  }
  //
  f_quit = false;
  ticks = 0;
  frame_time = 0.010f;
  while(!term->get_terminal_state(libui::ts_Terminated) && !term->get_terminal_state(libui::ts_SoftTerminate)) {
    if(f_quit)
      break;
    if(!term->poll_events()) {
      frames->input_process();
      rdr->render_begin();
      //
      rd3d->phase_start(0, false);
      //
      {
        rd3d->camera_frustum(lib3d::DPoint(0, 0, 0), lib3d::DTexPoint(math3d::torad(90), math3d::torad(90)), 0.1, 10);
        lib3d::DMatrix4 T;
        T.set_inversed(camera_T);
        rd3d->setup_T(T);
      }
      if(va_tri.valid()) {
        rd3d->setup_array(va_tri);
        rd3d->render_primitive(lib3d::rp_Tri, 3);
      }
      if(vas_tri.valid()) {
        rd3d->setup_array(vas_tri);
        rd3d->render_primitive(lib3d::rp_Tri, 3);
      }
      if(vas_cube.valid()) {
        rd3d->setup_array(vas_cube);
        rd3d->render_primitive(lib3d::rp_Quad, 4*6);
      }
      if(vas_cubechunk.valid()) {
        rd3d->setup_array(vas_cubechunk);
        rd3d->render_primitive(lib3d::rp_Quad, vas_cubechunk->get_array_count());
      }
      //
      rd3d->phase_2d();
      rd3d->setup_color(lib2d::DColorf(1, 1, 0));
      rd3d->setup_font(font);
      rd3d->render_box(lib2d::DPoint(10, 10), lib2d::DPoint(50, 50));
      rd3d->render_tri(lib2d::DPoint(100, 10), lib2d::DPoint(150, 50), lib2d::DPoint(150, 5));
      //
      frames->output_process();
      rdr->render_end();
      term->wait(frame_time);
      ticks += frame_time;
    }
  }
}
