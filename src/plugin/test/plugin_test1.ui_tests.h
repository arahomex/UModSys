// RTest1_Shell::

void RTest1_Shell::ui_test1(void)
{
  UI_Info ui;
  {
    if(0) {
      if(!ui.font_pure())
        return;
    }
    if(0) {
      if(!ui.init_term("*::SDL_core::*"))
        return;
    } else if(1) {
      if(!ui.init_term("*::SDL_GL::*"))
        return;
    }
    if(0) {
      if(!ui.create_2d("Test #1"))
        return;
    } else if(1) {
      if(!ui.create_3d("Test #1"))
        return;
    }
    if(1) {
      if(!ui.font_2d())
        return;
    }
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
  UI_Info ui;
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
    ui.cycle3d();
    //
    ui.term->set_terminal_state(libui::ts_Keyboard, false);
    //
    ui.shutdown();
  }
  ui.rd2d.clear();
}

