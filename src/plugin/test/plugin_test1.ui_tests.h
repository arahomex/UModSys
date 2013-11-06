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

void RTest1_Shell::ui_test1(void)
{
  lib2d::IRenderDriver::P rd2d;
  libui::ITerminal::P term;
  {
    TParametersA<1024> pars;
    term = ui_newterm("*::SDL_core::*", pars);
    if(!term.valid())
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
  while(!term->get_terminal_state(libui::ts_Terminated) && !term->get_terminal_state(libui::ts_SoftTerminate)) {
    if(!term->poll_events()) {
      rd2d->begin();
      rd2d->setup_color(lib2d::DColorf(1, 1, 0));
      rd2d->render_box(lib2d::DPoint(10, 10), lib2d::DPoint(50, 50));
      rd2d->end();
      term->wait(0.010f);
    }
  }
  term->set_terminal_state(libui::ts_Terminated);
}
