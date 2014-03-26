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

