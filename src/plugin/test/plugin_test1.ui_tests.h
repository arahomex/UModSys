// RTest1_Shell::

void RTest1_Shell::ui_test1(void)
{
  UI_Info ui(*this);
  {
    if(0) {
      if(!ui.font_pure())
        return;
    }
    if(TEST_SDL_CORE) {
      if(!ui.init_term("*::SDL_core::*"))
        return;
    } else if(TEST_SDL_GL) {
      if(!ui.init_term("*::SDL_GL::*"))
        return;
    }
    if(TEST_SDL_CORE) {
      if(!ui.create_2d("Test #1"))
        return;
    } else if(TEST_SDL_GL) {
      if(!ui.create_3d("Test #1"))
        return;
    }
    if(1) {
      if(!ui.font_2d())
        return;
    }
    //
    if(TEST_FRAMES) {
      if(!ui.new_frames(this))
        return;
    }
    //
    ui.keyc->key_link(0, NULL, this);
    ui.mouc->mouse_link(0, NULL, this, ~0);
    ui.term->set_terminal_state(libui::ts_Keyboard, true);
    //
    ui.cycle();
    //
    ui.term->set_terminal_state(libui::ts_Keyboard, false);
    //
    ui.shutdown();
  }
  ui.rd2d.clear();
}

void RTest1_Shell::ui_test2(void)
{
  UI_Info ui(*this);
  {
    if(!ui.init_term("*::SDL_GL::*"))
      return;
    if(!ui.create_3d("Test #1"))
      return;
    if(!ui.font_2d())
      return;
    //
    if(1) {
      if(!ui.new_frames(this))
        return;
    }
    //
    ui.keyc->key_link(0, NULL, this);
    ui.mouc->mouse_link(0, NULL, this, ~0);
    ui.term->set_terminal_state(libui::ts_Keyboard, true);
    //
//    ui.cycle3d_a();
    ui.cycle3d_b();
    //
    ui.term->set_terminal_state(libui::ts_Keyboard, false);
    //
    ui.shutdown();
  }
  ui.rd2d.clear();
}

