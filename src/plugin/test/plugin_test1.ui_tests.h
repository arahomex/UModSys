// RTest1_Shell::

libui::ITerminal::P RTest1_Shell::ui_newterm(const DCString &mask, const SParameters& args)
{
  if(TypeId found=M.t_firstobjname<libui::ITerminal>(mask)) {
    libui::ITerminal::P rv;
    if(M.t_generate(rv, found, args)) {
      M.con().put(0, "  found terminal %s: %s => %p\n", mask(), found->name, rv());
      return rv;
    }
  }
  M.con().put(0, "  fail terminal %s\n", mask());
  return NULL;
}

lib2d::IMultiImage::P RTest1_Shell::ui_newfont(const DCString &mask, const SParameters& args)
{
  if(TypeId found=M.t_firstobjname<lib2d::IMultiImage>(mask)) {
    lib2d::IMultiImage::P rv;
    if(M.t_generate(rv, found, args)) {
      M.con().put(0, "  found font %s: %s => %p\n", mask(), found->name, rv());
      return rv;
    }
  }
  M.con().put(0, "  fail font %s\n", mask());
  return NULL;
}

void RTest1_Shell::ui_test1(void)
{
  lib2d::IRenderDriver::P rd2d;
  libui::ITerminal::P term;
  lib2d::IMultiImage::P font;
  {
    TParametersA<1024> pars;
    term = ui_newterm("*::SDL_core::*", pars);
    if(!term.valid())
      return;
  }
  if(0) {
    TParametersA<1024> pars;
    pars.add("filename", "Neocyr.ttf");
    font = ui_newfont("*::SDL_core::*ttf*", pars);
    if(!font.valid())
      return;
  }
  {
    TParametersA<1024> pars;
    pars.add("caption", "Test #1");
    if(!term->t_create_handler(rd2d, "", &pars)) {
      M.con().put(0, "  not created lib2d::IRenderDriver\n");
      return;
    }
    M.con().put(0, "  created lib2d::IRenderDriver: %s => %p\n", rd2d()->root_get_interface_type()->name, rd2d());
  }
  if(1) {
    TParametersA<1024> pars;
    pars.add("filename", "Neocyr.ttf");
    pars.add("max_glyph", 0x1000);
    font = rd2d->new_font(pars);
    if(!font.valid())
      return;
    M.con().put(0, "  created font: %s => %p\n", font()->root_get_interface_type()->name, font());
  }
  while(!term->get_terminal_state(libui::ts_Terminated) && !term->get_terminal_state(libui::ts_SoftTerminate)) {
    if(!term->poll_events()) {
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
      rd2d->end();
      term->wait(0.010f);
    }
  }
  term->set_terminal_state(libui::ts_Terminated);
}
