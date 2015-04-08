// RTest1_Shell::

bool RTest1_Shell::UI_Info::create_3d(const char *cap)
{
  {
    libmedia::IUtilities::P utils = shell.media_utils();
    libmedia::ILibraryLayered::P lib = shell.media_lay();
    if(!utils.valid() || !lib.valid())
      return false;
    {
      libmedia::ILibraryBinTree::P lib_vfs = shell.media_vfs();
      if(!lib_vfs.valid())
        return false;
      lib_vfs->mount_add("/", shell.media_arch_stdio("."), libmedia::mp_Read);
      lib_vfs->mount_add("/", 1, shell.media_arch_stdio("./write-dir"), libmedia::mp_RWL);
      lib->layer_push( lib_vfs );
      //
      lib_vfs->mount_add("/mc/", 5, shell.media_arch_zip(lib, utils, "/mctest.zip"), libmedia::mp_RL);
    }
    //
    {
      libmedia::ILibraryObjFilter::P lib_flt = shell.media_flt();
      if(!lib_flt.valid())
        return false;
      TParametersA<1024> params;
      libmedia::IBinObjFilter::P flt = shell.media_filter_new("*::images_std::*", params);
      lib_flt->filters_add(flt, &params, 0);
      lib->layer_push( lib_flt );
    }
    {
      lib->layer_push( shell.media_cache(false) );
      lib->layer_push( shell.media_cache(true) );
    }
    media = lib();
  }
  {
    TParametersA<1024> pars;
    rdr = generate_type<lib3d::IRenderer>("*::stdlib::*", pars, "IRenderer");
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
  //
  {
    TParametersA<1024> pars;
    visis = generate_type<lib3d::IVisualizerScene>("*::stdlib::RVisualizerScene", pars, "IVisualizerScene");
    if(!visis.valid())
      return false;
  }
  {
    TParametersA<1024> pars;
//    pars.add("fov", mathc::torad(120));
    pars.add("fov", mathc::torad(90));
    pars.add("near", 0.1);
    pars.add("far", 10.0);
    lib3d::IGeneralObject::P gcam = generate_type<lib3d::IGeneralObject>("*::stdlib::object_camera::RGeneral", pars, "IGeneralObject");
    if(!gcam.valid())
      return false;
    if(!gcam->visual_new(rdr, NULL)->t_ref_get_other_interface(cam))
      return false;
  }
  {
    TParametersA<1024> pars;
    pars.add("kind", "cube");
    lib3d::IGeneralObject::P gcube = generate_type<lib3d::IGeneralObject>("*::stdlib::object_primitive::RGeneral", pars, "IGeneralObject");
    if(!gcube.valid())
      return false;
    lib3d::IRenderObject::P rcube = gcube->render_new(rdr);
    if(!rcube.valid())
      return false;
    rcube->render_update();
    vocube = rcube->visual_new();
    if(!vocube.valid())
      return false;
    vocube->visual_update();
    vocube->T.set_ER(1);
    vocube->T.set_translate(0.5, 0.5, 1.2);
  }
  {
    lib3d::PTexture tex = rd3d->create_tex();
    if(!tex.valid()) {
      s_dbg.put(d_SubGen, cl_Info, "  Can't create texture\n");
      return false;
    }
    s_dbg.put(d_SubGen, cl_Info, "  created texture: %s => %p\n", tex->root_get_interface_type()->name, tex());
    mat_cube.textures[0].texture = tex;
    //
//    lib2d::IImageFactory::P factory = new_mem_imagefactory();
//    lib2d::IImage::P src = factory->image_new();
    if(!media->obj_load("/atlas1.png", tex, "lib2d::IImage")) {
      s_dbg.put(d_SubGen, cl_Info, "  Can't load texture from file\n");
      return false;
    }
  }
  return true;
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
    if(cam.valid()) {
      cam->T = camera_T;
    }
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
      {
        lib3d::SRenderState S(rdr, rd3d);
        S << NULL << cam() << vocube;
      }
      //
      if(0) {
        cam->camera_setup(rd3d);
//        rd3d->camera_frustum(lib3d::DPoint(0, 0, 0), lib3d::DTexPoint(math3d::torad(90), math3d::torad(90)), 0.1, 10);
      }
      {
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
        rd3d->setup_material(&mat_cube);
        rd3d->setup_array(vas_cube);
        rd3d->render_primitive(lib3d::rp_Quad, 4*6);
        rd3d->setup_material(NULL);
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
