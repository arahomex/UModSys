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
  TParametersA<1024> pars;
  libui::ITerminal::P term = ui_newterm("*::SDL_core::*", pars);
  if(!term.valid())
    return;
  if(!term->set_terminal_state(libui::ts_Active))
    return;
  while(term->get_terminal_state()!=libui::ts_Terminated) {
    if(!term->poll_events()) {
      term->wait(0.001f);
    }
  }
}
