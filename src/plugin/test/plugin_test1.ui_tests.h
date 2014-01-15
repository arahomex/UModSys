// RTest1_Shell::

libui::ICollector::P RTest1_Shell::ui_newframes(const DCString &mask, const SParameters& args)
{
  return generate_type<libui::ICollector>(mask, args, "ICollector");
}

libui::ITerminal::P RTest1_Shell::ui_newterm(const DCString &mask, const SParameters& args)
{
  return generate_type<libui::ITerminal>(mask, args, "ITerminal");
}

lib2d::IMultiImage::P RTest1_Shell::ui_newfont(const DCString &mask, const SParameters& args)
{
  return generate_type<lib2d::IMultiImage>(mask, args, "IMultiImage");
}

bool RTest1_Shell::key_pressed(const libui::SKeyboardInputRaw& key)
{
  s_dbg.put(d_UI, cl_Info, 
    "[KEY.KEY %d,%d,%d,%d]\n",
    key.code, key.status, key.os_code, key.special
  );
  if(key.code==libui::k_escape) {
    f_quit = true;
  }
  return true;
}

bool RTest1_Shell::key_text(const libui::SKeyboardInputText& key)
{
  tl::su::utf8 text[8];
  size_t n = tl::su::utf_32to8(text, 8, &key.text, 1, true);
  text[n] = 0;
  s_dbg.put(d_UI, cl_Info, 
    "[KEY.TEXT %s]\n",
    text
  );
  return true;
}

bool RTest1_Shell::mouse_event(const libui::SMouseInput& ms)
{
  s_dbg.put(d_UI, cl_Info, 
    "[MOUSE %x B%x:%d A(%d,%d) R(%d,%d) X(%d,%d)]\n",
    ms.eventid, ms.get_buttonmask(), ms.button_count,
    ms.abs(0), ms.abs(1), ms.rel(0), ms.rel(1), ms.aux(0), ms.aux(1)
  );
  return true;
}

bool RTest1_Shell::command(const libui::SController& ci, int command, const libui::SFrameDataIn& in)
{
  s_dbg.put(d_UI, cl_Info, 
    "[COMMAND %d@%p C%d D%d]\n",
    ci.context, ci.source, command, 
    in.safe_i()
  );
  return true;
}

bool RTest1_Shell::db_get(const libui::SController& ci, const BCStr hint, const libui::SFrameDataOut& out)
{
  if(ci.context==6) {
    if(out.type==libui::fdt_Integer) {
      if(tl::su::seq(hint, "count")) {
        out.i(50);
        return true;
      } else if(tl::su::seq(hint, "dy")) {
        out.i(16);
        return true;
      }
    }
    return false;
  }
  return false;
}

bool RTest1_Shell::db_get(const libui::SController& ci, const BCStr hint, int sid, const libui::SFrameDataOut& out)
{
  if(ci.context==6) {
    if(out.type==libui::fdt_String) {
      if(tl::su::seq(hint, "text")) {
        char buf[32];
        sprintf(buf, "Item #%d", sid);
        out.s(buf);
        return true;
      }
    }
  }
  return false;
}

bool RTest1_Shell::command_draw(const libui::SController& ci, lib2d::IRenderDriver *drv, const lib2d::DBox &bounds)
{
  return false;
}

void RTest1_Shell::ui_test1(void)
{
  lib2d::IRenderDriver::P rd2d;
  libui::ITerminal::P term;
  lib2d::IMultiImage::P font;
  libui::IKeyboardController::P keyc;
  libui::IMouseController::P mouc;
  libui::ICollector::P frames;
  if(0) {
    TParametersA<1024> pars;
    pars.add("filename", "Neocyr.ttf");
    font = ui_newfont("*::SDL_core::*ttf*", pars);
    if(!font.valid())
      return;
  }
  {
    TParametersA<1024> pars;
    pars.add("driver", rd2d);
    pars.add("font", font);
    term = ui_newterm("*::SDL_core::*", pars);
    if(!term.valid())
      return;
  }
  {
    TParametersA<1024> pars;
    pars.add("caption", "Test #1");
    if(!term->t_create_handler(rd2d, "", &pars)) {
      s_dbg.put(d_SubGen, cl_Error, "  not created lib2d::IRenderDriver\n");
      return;
    }
    s_dbg.put(d_SubGen, cl_Info, "  created lib2d::IRenderDriver: %s => %p\n", rd2d()->root_get_interface_type()->name, rd2d());
  }
  if(1) {
    TParametersA<1024> pars;
    pars.add("filename", "Neocyr.ttf");
    pars.add("max_glyph", 0x1000);
    font = rd2d->new_font(pars);
    if(!font.valid()) {
      s_dbg.put(d_SubGen, cl_Error, "  error create font\n");
      return;
    }
    s_dbg.put(d_SubGen, cl_Info, "  created font: %s => %p\n", font()->root_get_interface_type()->name, font());
  }
  if(1) {
    TParametersA<1024> pars;
    if(!term->t_create_handler(keyc, NULL, &pars)) {
      s_dbg.put(d_SubGen, cl_Error, "  error create IKeyboardController\n");
      return;
    }
    s_dbg.put(d_SubGen, cl_Info, "  created IKeyboardController: %s => %p\n", keyc()->root_get_interface_type()->name, keyc());
  }
  if(1) {
    TParametersA<1024> pars;
    if(!term->t_create_handler(mouc, NULL, &pars)) {
      s_dbg.put(d_SubGen, cl_Error, "  error create IMouseController\n");
      return;
    }
    s_dbg.put(d_SubGen, cl_Info, "  created IMouseController: %s => %p\n", mouc()->root_get_interface_type()->name, mouc());
  }
  {
    TParametersA<1024> pars;
    pars.add("driver", rd2d);
    pars.add("font", font);
    frames = ui_newframes("*::frames::*", pars);
    if(!frames.valid())
      return;
  }
  //
  f_quit = false;
  keyc->key_link(0, NULL, this);
  mouc->mouse_link(0, NULL, this, ~0);
  term->set_terminal_state(libui::ts_Keyboard, true);
  frames->output_attach_2d(rd2d);
  frames->input_attach_m(mouc, 10);
  frames->input_attach_k(keyc, 10);
  //
  {
    libui::SFrameCreateParameters fcp;
    fcp.ctrl = this;
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
  //
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
  //
  frames->shutdown();
  term->set_terminal_state(libui::ts_Keyboard, false);
  keyc->key_unlink(0);
  mouc->mouse_unlink(0);
  term->set_terminal_state(libui::ts_Terminated);
}

void RTest1_Shell::ui_test2(void)
{
}
