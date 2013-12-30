// 
// RSDLTerminal::
// 

bool RSDLTerminal::next(void) {
  do {
//    SDL_EnableUNICODE(1);
    event.type = SDL_NOEVENT;
    int r = SDL_PollEvent(&event);
    if(r==0)
      return false;
    //
    if(next1(event))
      return true;
    //
  } while(true);
}

// **********

bool RSDLTerminal::mouse_reset(void)
{
  SDL_Event e;
  while(SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_MOUSEMOTIONMASK)>0)
    ;
//  SDL_WM_GrabInput(SDL_GRAB_ON);
  memset(&ms, 0, sizeof ms);
  ms.button_count = 8;
  return true;
}

bool RSDLTerminal::mouse_setvisible(bool visible)
{
  SDL_ShowCursor(visible);
  if((saved_flags & SDL_FULLSCREEN) || !visible) {
    SDL_WM_GrabInput(SDL_GRAB_ON);
  } else {
    SDL_WM_GrabInput(SDL_GRAB_OFF);
  }
#if defined(SDLV13)
  SDL_SetRelativeMouseMode(SDL_SelectMouse(-1), SDL_bool(!visible));
#elif defined(SDLV12)
//  SDL_SetCursor(NULL);
#endif
  mouse1 = true;
  return true;
}

bool RSDLTerminal::mouse_setposition(int x, int y)
{
  return true;
}

bool RSDLTerminal::mouse_unlink(int order) 
{
  cmouses.remove(-order);
  return true;
}

bool RSDLTerminal::mouse_link(int order, IRefObject* combo, UI::IMouseClient* target, int memask) 
{
  MouseClient* m = cmouses[-order];
  if(m==NULL)
    return false;
  m->set(combo, target);
  m->extra = memask;
  return true;
}

// **********

bool RSDLTerminal::key_unlink(int order) 
{
  ckeys.remove(-order);
  return true;
}

bool RSDLTerminal::key_link(int order, IRefObject* combo, UI::IKeyboardClient* target) 
{
  KeyboardClient* k = ckeys[-order];
  if(k==NULL)
    return false;
  k->set(combo, target);
  return true;
}

bool RSDLTerminal::key_reset(void)
{
  SDL_Event e;
  while(SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_KEYEVENTMASK)>0)
    ;
  SDL_WM_GrabInput(SDL_GRAB_ON);
  return true;
}

bool RSDLTerminal::key_setrepeat(float firsttime, float nexttime)
{
  key_repeat_1 = firsttime;
  key_repeat_2 = nexttime;
  return true;
}

bool RSDLTerminal::key_setmodifier(int modifier, int code, int statuson) 
{
  if(modifier<0 || modifier>=UI::max_key_modifiers)
    return false;
  kmod[modifier][0] = code;
  kmod[modifier][1] = statuson;
  return true;
}

// **********

bool RSDLTerminal::set_terminal_caption(BCStr caption)
{
  set_title(caption);
  return true;
}

bool RSDLTerminal::terminal_reset(void)
{
  cmouses.clear();
  ckeys.clear();
  close();
  termstate = UI::ts_Inactive;
  memset(&ms, 0, sizeof ms);
  ms.button_count = 8;
  return true;
}

bool RSDLTerminal::set_terminal_state(UI::eTerminalState state)
{
  if(state==termstate)
    return true;
  if(termstate==UI::ts_Terminated)
    return false; // terminated must be reset!
  termstate = state;
  if(termstate==UI::ts_Terminated)
    close();
  return true;
}

bool RSDLTerminal::poll_events(int maxcount)
{
  if(termstate==UI::ts_Terminated || termstate==UI::ts_Offline)
    return false; // forced off terminal

  if(termstate!=UI::ts_Active) {
    SDL_Event e;
    while(SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_KEYEVENTMASK|SDL_MOUSEEVENTMASK)>0)
      ;
  }

  if(maxcount==-1)
    maxcount = INT_MAX;
  int evno;
  // main loop
  for(evno=0; evno<maxcount; evno++) {
    if(!next())
      break;
  }
  // manual keyboard events
  if(key_repeat_1>0 && key_repeat_2>0) {
    SDL_Event ev;
    ev.key.type = SDL_KEYDOWN;
    ev.key.state = SDL_PRESSED;
    ev.key.which = SDL_KEYDOWN;
    ev.key.keysym.mod = KMOD_NONE;
#if defined(SDLV13)
    ev.key.keysym.scancode = SDL_SCANCODE_UNKNOWN;
#elif defined(SDLV12)
    ev.key.keysym.scancode = 0;
#endif
    float time = clock();
    //
    for(bool tried = true; tried; ) {
      tried = false;
      for(int i=0; i<MAX_ALLKEYS; i++) {
        if(key_repeat_1<=0 && key_repeat_2<=0) {
          // turned off repeat mode
          tried = false;
          break;
        }
        char &k = keydown[i];
        switch(k) {
          case 0:
            break; // non-pressed
          case 1:
            if(keydowntime[i]+key_repeat_1<time) {
              k++;
              ev.key.keysym.scancode = keydownscan[i];
              ev.key.keysym.sym = keydownkey[i];
              ev.key.keysym.unicode = keydownchar[i];
              evno+= next1(ev);
              keydowntime[i] += key_repeat_1;
              tried = true;
            }
            break; // pressed once
          case 2:
            if(keydowntime[i]+key_repeat_2<time) {
              ev.key.keysym.scancode = keydownscan[i];
              ev.key.keysym.sym = keydownkey[i];
              ev.key.keysym.unicode = keydownchar[i];
              evno+= next1(ev);
              keydowntime[i] += key_repeat_2;
              tried = true;
            }
            break; // pressed twice or more
        }
      }
    }
  }
  //
  return evno>0;
}

double RSDLTerminal::clock(void)
{
  return SDL_GetTicks()/1000.0;
}

bool RSDLTerminal::wait(float sec)
{
  SDL_Delay(int(sec*1000));
  return true;
}

// **********

inline int RSDLTerminal::translate_mod(int sdl_mod)
{
  int rv = 0;
  if(sdl_mod & KMOD_LSHIFT)
    rv |= UI::km_l_shift;
  if(sdl_mod & KMOD_RSHIFT)
    rv |= UI::km_r_shift;
  if(sdl_mod & KMOD_LCTRL)
    rv |= UI::km_l_ctrl;
  if(sdl_mod & KMOD_RCTRL)
    rv |= UI::km_r_ctrl;
  if(sdl_mod & KMOD_LALT)
    rv |= UI::km_l_alt;
  if(sdl_mod & KMOD_RALT)
    rv |= UI::km_r_alt;
  if(sdl_mod & KMOD_NUM)
    rv |= UI::km_numlock;
  if(sdl_mod & KMOD_CAPS)
    rv |= UI::km_capslock;
  if(sdl_mod & KMOD_MODE)
    rv |= UI::km_scrollock;
  if(sdl_mod & KMOD_LMETA)
    rv |= UI::km_l_super;
  if(sdl_mod & KMOD_RMETA)
    rv |= UI::km_r_super;
  return rv;
}

bool RSDLTerminal::next1(const SDL_Event &event)
{
    int kcode, ucode;
    //
    if(mouse1 && event.type==SDL_MOUSEMOTION) {
      mouse1 = false;
      return false; // skip this event
    }
    //
    switch(event.type) {
      case SDL_QUIT:
        set_terminal_state(UI::ts_Terminated);
        return false;

      case SDL_ACTIVEEVENT:
        set_terminal_state(event.active.gain ? UI::ts_Active : UI::ts_Inactive);
        return false;

      case SDL_VIDEORESIZE:
        resize_screen(event.resize.w, event.resize.h);
        return false;

      case SDL_KEYDOWN:
      case SDL_KEYUP: 
//        M.debug_put(0, "[SDL keys K%x S%x M%x]\n", event.key.keysym.sym, event.key.keysym.scancode, event.key.keysym.mod);
        if(!get_keymap(event.key.keysym.sym, event.key.keysym.scancode, kcode, ucode))
          break;
        if(event.key.state==SDL_PRESSED) {
          if(keydown[ucode]==0) { // first time press
            keydown[ucode] = 1;
            keydownchar[ucode] = event.key.keysym.unicode;
            keydownkey[ucode] = event.key.keysym.sym;
            keydownscan[ucode] = event.key.keysym.scancode;
            keydowntime[ucode] = clock();
          }
        } else if(event.key.state==SDL_RELEASED) {
          keydown[ucode] = 0;
        }
        if(!ckeys.empty()) {
          UI::SKeyboardInputRaw ki;
          ki.code = kcode;
          if(ki.code>0) {
            ki.os_code = event.key.keysym.sym;
            ki.status = event.key.state==SDL_PRESSED ? UI::bis_Pressed
              : event.key.state==SDL_RELEASED ? UI::bis_Released
              : UI::bis_None;
            ki.special = translate_mod(event.key.keysym.mod);
            if(t_invoke_handler(ckeys, ki))
              return false;
          }
          if(event.key.state==SDL_PRESSED && event.key.keysym.unicode>=' ') {
            bool ok = false;
            UI::SKeyboardInputU kic;
            kic.keyu = event.key.keysym.unicode;
            ok = t_invoke_handler(ckeys, kic);
            //
            UI::SKeyboardInputC kicc;
            kicc.len = su::utf_32to8(kicc.keyc, su::utf_max8len, kic.keyu);
            kicc.keyc[kicc.len] = 0;
            ok = t_invoke_handler(ckeys, kicc) || ok;
            //
            UI::SKeyboardInputW kicw;
            kicw.len = su::utf_32to16(kicw.keyw, su::utf_max16len, kic.keyu);
            kicw.keyw[kicw.len] = 0;
            ok = t_invoke_handler(ckeys, kicw) || ok;
            //
            if(ok)
              return false;
          }
        } break;

      case SDL_MOUSEMOTION:
        if(!cmouses.empty()) {
          ms.abs.set(event.motion.x, event.motion.y);
          ms.rel.set(event.motion.xrel, event.motion.yrel);
          ms.eventid = UI::me_MoveRelative;
          for(int i=0; i<8; i++) {
            if(event.motion.state & SDL_BUTTON(i+1)) {
              ms.buttons[i] = UI::bis_Pressed;
              ms.eventid = UI::me_MoveButton;
            } else {
              ms.buttons[i] = UI::bis_Released;
            }
          }
          if(t_invoke_handler(cmouses, ms))
            return false;
        } 
        reset_mouse();
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        if(!cmouses.empty()) {
          ms.abs.set(event.button.x, event.button.y);
          ms.rel.set(0, 0);
          ms.eventid = UI::me_Button;
          for(int i=0; i<8; i++) {
            if(event.button.button == i+1) {
              ms.buttons[i] = event.button.state==SDL_PRESSED ? UI::bis_Pressed : UI::bis_Released;
            }
          }
          if(t_invoke_handler(cmouses, ms))
            return false;
        } 
        reset_mouse();
        break;
/*
      case SDL_MOUSEMOTION:
        return et_MouseMove;
      case SDL_MOUSEBUTTONDOWN:
        return et_MouseDown;
      case SDL_MOUSEBUTTONUP:
        return et_MouseUp;
*/
      /*
      case SDL_JOYAXISMOTION:
        return et_JoyAxis;
      case SDL_JOYBALLMOTION:
        return et_JoyBall;
      case SDL_JOYHATMOTION:
        return et_JoyHat;
      case SDL_JOYBUTTONDOWN:
        return et_JoyButtonDown;
      case SDL_JOYBUTTONUP:
        return et_JoyButtonUp;
      */
    }
    return true;
}

bool RSDLTerminal::reset_mouse(void)
{
#if defined(SDLV13)
  if(SDL_ShowCursor(-1)==0) {
    SDL_Surface *s = SDL_GetVideoSurface();
    if(s) {
      mouse1 = true;
      SDL_WarpMouse(s->w/2, s->h/2);
    }
//    int w, h;
//    SDL_GetWindowSize(0, &w, &h);
//    SDL_WarpMouseInWindow(0, w/2, h/2);
  }
#elif defined(SDLV12)
#endif
  return true;
}

bool RSDLTerminal::get_keymap(unsigned kmi, unsigned scan, int& kcode, int &ucode)
{
#if defined(SDLV13)
  if(kmi & SDLK_SCANCODE_MASK) {
    kcode = km_scan[kmi & ~SDLK_SCANCODE_MASK];
    ucode = kcode + MAX_UKEYS;
  } else if(kmi<MAX_UKEYS) {
    kcode = km_key[kmi];
    ucode = kcode;
  } else {
    kcode = km_scan[scan];
    return true;
  }
#elif defined(SDLV12)
  kcode = km_key[kmi];
  ucode = kcode;
#endif
  return true;
}

void RSDLTerminal::init_keymap(void)
{
  int i;
  //
#if defined(SDLV13)
  for(i=0; i<MAX_UKEYS; i++) 
    km_key[i] = -1;
  for(i=0; i<SDL_NUM_SCANCODES; i++) 
    km_scan[i] = -1;
#elif defined(SDLV12)
  for(i=0; i<SDLK_LAST; i++) 
    km_key[i] = -1;
#endif
  //
#if defined(SDLV13)
#define K(x, y) km_scan[(y)] = (x);
  //
  K(UI::k_backspace,    SDL_SCANCODE_BACKSPACE)
  K(UI::k_tab,          SDL_SCANCODE_TAB)
  K(UI::k_clear,        SDL_SCANCODE_CLEAR)
  K(UI::k_return,       SDL_SCANCODE_RETURN)
  K(UI::k_pause,        SDL_SCANCODE_PAUSE)
  K(UI::k_escape,       SDL_SCANCODE_ESCAPE)
  K(UI::k_space,        SDL_SCANCODE_SPACE)
  K(UI::k_exclaim,      SDL_SCANCODE_KP_EXCLAM)
//  K(UI::k_quotedbl,     SDL_SCANCODE_QUOTEDBL)
  K(UI::k_hash,         SDL_SCANCODE_KP_HASH)
//  K(UI::k_dollar,       SDL_SCANCODE_DOLLAR)
//  K(UI::k_ampersand,    SDL_SCANCODE_AMPERSAND)
//  K(UI::k_quote,        SDL_SCANCODE_QUOTE)
//  K(UI::k_leftparen,    SDL_SCANCODE_LEFTPAREN)
//  K(UI::k_rightparen,   SDL_SCANCODE_RIGHTPAREN)
//  K(UI::k_asterisk,     SDL_SCANCODE_ASTERISK)
  K(UI::k_plus,         SDL_SCANCODE_KP_PLUS)
  K(UI::k_comma,        SDL_SCANCODE_COMMA)
  K(UI::k_minus,        SDL_SCANCODE_MINUS)
  K(UI::k_period,       SDL_SCANCODE_PERIOD)
  K(UI::k_slash,        SDL_SCANCODE_SLASH)
  K(UI::k_0, SDL_SCANCODE_0);
  for(i=0; i<='9'-'1'; i++) {
    K(UI::k_1+i, SDL_SCANCODE_1+i);
  }
  K(UI::k_colon,        SDL_SCANCODE_KP_COLON)
  K(UI::k_semicolon,    SDL_SCANCODE_SEMICOLON)
//  K(UI::k_less,         SDL_SCANCODE_LESS)
  K(UI::k_equals,       SDL_SCANCODE_EQUALS)
//  K(UI::k_greater,      SDL_SCANCODE_GREATER)
//  K(UI::k_question,     SDL_SCANCODE_QUESTION)
  K(UI::k_at,           SDL_SCANCODE_KP_AT)
  K(UI::k_leftbracket,  SDL_SCANCODE_LEFTBRACKET)
  K(UI::k_backslash,    SDL_SCANCODE_BACKSLASH)
  K(UI::k_rightbracket, SDL_SCANCODE_RIGHTBRACKET)
//  K(UI::k_caret,        SDL_SCANCODE_CARET)
//  K(UI::k_underscore,   SDL_SCANCODE_UNDERSCORE)
//  K(UI::k_backquote,    SDL_SCANCODE_BACKQUOTE)
  K(UI::k_delete,       SDL_SCANCODE_DELETE)
  for(i=0; i<='z'-'a'; i++) {
    K(UI::k_a+i, SDL_SCANCODE_A+i);
  }
//  for(i=0; i<=95; i++) {
//    K(UI::k_world0+i, SDL_SCANCODE_WORLD_0+i);
//  }
  K(UI::k_kp0,          SDL_SCANCODE_KP_0)
  K(UI::k_kp1,          SDL_SCANCODE_KP_1)
  K(UI::k_kp2,          SDL_SCANCODE_KP_2)
  K(UI::k_kp3,          SDL_SCANCODE_KP_3)
  K(UI::k_kp4,          SDL_SCANCODE_KP_4)
  K(UI::k_kp5,          SDL_SCANCODE_KP_5)
  K(UI::k_kp6,          SDL_SCANCODE_KP_6)
  K(UI::k_kp7,          SDL_SCANCODE_KP_7)
  K(UI::k_kp8,          SDL_SCANCODE_KP_8)
  K(UI::k_kp9,          SDL_SCANCODE_KP_9)
  K(UI::k_kp_period,    SDL_SCANCODE_KP_PERIOD)
  K(UI::k_kp_divide,    SDL_SCANCODE_KP_DIVIDE)
  K(UI::k_kp_multiply,  SDL_SCANCODE_KP_MULTIPLY)
  K(UI::k_kp_minus,     SDL_SCANCODE_KP_MINUS)
  K(UI::k_kp_plus,      SDL_SCANCODE_KP_PLUS)
  K(UI::k_kp_enter,     SDL_SCANCODE_KP_ENTER)
  K(UI::k_kp_equals,    SDL_SCANCODE_KP_EQUALS)
  K(UI::k_up,           SDL_SCANCODE_UP)
  K(UI::k_down,         SDL_SCANCODE_DOWN)
  K(UI::k_right,        SDL_SCANCODE_RIGHT)
  K(UI::k_left,         SDL_SCANCODE_LEFT)
  K(UI::k_insert,       SDL_SCANCODE_INSERT)
  K(UI::k_home,         SDL_SCANCODE_HOME)
  K(UI::k_end,          SDL_SCANCODE_END)
  K(UI::k_pageup,       SDL_SCANCODE_PAGEUP)
  K(UI::k_pagedown,     SDL_SCANCODE_PAGEDOWN)
  K(UI::k_f1,           SDL_SCANCODE_F1)
  K(UI::k_f2,           SDL_SCANCODE_F2)
  K(UI::k_f3,           SDL_SCANCODE_F3)
  K(UI::k_f4,           SDL_SCANCODE_F4)
  K(UI::k_f5,           SDL_SCANCODE_F5)
  K(UI::k_f6,           SDL_SCANCODE_F6)
  K(UI::k_f7,           SDL_SCANCODE_F7)
  K(UI::k_f8,           SDL_SCANCODE_F8)
  K(UI::k_f9,           SDL_SCANCODE_F9)
  K(UI::k_f10,          SDL_SCANCODE_F10)
  K(UI::k_f11,          SDL_SCANCODE_F11)
  K(UI::k_f12,          SDL_SCANCODE_F12)
  K(UI::k_f13,          SDL_SCANCODE_F13)
  K(UI::k_f14,          SDL_SCANCODE_F14)
  K(UI::k_f15,          SDL_SCANCODE_F15)
  K(UI::k_numlock,      SDL_SCANCODE_NUMLOCKCLEAR)
  K(UI::k_capslock,     SDL_SCANCODE_CAPSLOCK)
  K(UI::k_scrollock,    SDL_SCANCODE_SCROLLLOCK)
  K(UI::k_rshift,       SDL_SCANCODE_RSHIFT)
  K(UI::k_lshift,       SDL_SCANCODE_LSHIFT)
  K(UI::k_rctrl,        SDL_SCANCODE_RCTRL)
  K(UI::k_lctrl,        SDL_SCANCODE_LCTRL)
  K(UI::k_ralt,         SDL_SCANCODE_RALT)
  K(UI::k_lalt,         SDL_SCANCODE_LALT)
  K(UI::k_rmeta,        SDL_SCANCODE_RGUI)
  K(UI::k_lmeta,        SDL_SCANCODE_LGUI)
//  K(UI::k_lsuper,       SDL_SCANCODE_LSUPER)
//  K(UI::k_rsuper,       SDL_SCANCODE_RSUPER)
  K(UI::k_mode,         SDL_SCANCODE_MODE)
//  K(UI::k_compose,      SDL_SCANCODE_COMPOSE)
  K(UI::k_help,         SDL_SCANCODE_HELP)
  K(UI::k_print,        SDL_SCANCODE_PRINTSCREEN)
  K(UI::k_sysreq,       SDL_SCANCODE_SYSREQ)
//  K(UI::k_break,        SDL_SCANCODE_BREAK)
  K(UI::k_menu,         SDL_SCANCODE_MENU)
  K(UI::k_power,        SDL_SCANCODE_POWER)
//  K(UI::k_euro,         SDL_SCANCODE_EURO)
  K(UI::k_undo,         SDL_SCANCODE_UNDO)
  //
#undef K
#endif // defined(SDLKV13)
  //
#if defined(SDLV13)
  #define K(x, y) if((y)<MAX_UKEYS) km_key[(y)&~SDLK_SCANCODE_MASK] = x;
#elif defined(SDLV12)
  #define K(x, y) km_key[(y)] = x;
#endif
  //
#if defined(SDLV13)
  K(UI::k_comma,        ','/*SDLK_COMMA*/)
  K(UI::k_period,       '.'/*SDLK_PERIOD*/)
  K(UI::k_slash,        '/'/*SDLK_SLASH*/)
  K(UI::k_semicolon,    ';'/*SDLK_SEMICOLON*/)
  K(UI::k_equals,       '='/*SDLK_EQUALS*/)
  K(UI::k_leftbracket,  '('/*SDLK_LEFTBRACKET*/)
  K(UI::k_backslash,    '\\'/*SDLK_BACKSLASH*/)
  K(UI::k_rightbracket, ')'/*SDLK_RIGHTBRACKET*/)
/*
  for(i=0; i<=95; i++) {
    K(UI::k_world0+i, SDLK_WORLD_0+i);
  }
*/
/*
  K(UI::k_rmeta,        SDLK_RMETA)
  K(UI::k_lmeta,        SDLK_LMETA)
  K(UI::k_lsuper,       SDLK_LSUPER)
  K(UI::k_rsuper,       SDLK_RSUPER)
*/
#elif defined(SDLV12)
  K(UI::k_comma,        SDLK_COMMA)
  K(UI::k_period,       SDLK_PERIOD)
  K(UI::k_slash,        SDLK_SLASH)
  K(UI::k_semicolon,    SDLK_SEMICOLON)
  K(UI::k_equals,       SDLK_EQUALS)
  K(UI::k_leftbracket,  SDLK_LEFTBRACKET)
  K(UI::k_backslash,    SDLK_BACKSLASH)
  K(UI::k_rightbracket, SDLK_RIGHTBRACKET)
  for(i=0; i<=95; i++) {
    K(UI::k_world0+i, SDLK_WORLD_0+i);
  }
  K(UI::k_rmeta,        SDLK_RMETA)
  K(UI::k_lmeta,        SDLK_LMETA)
  K(UI::k_lsuper,       SDLK_LSUPER)
  K(UI::k_rsuper,       SDLK_RSUPER)
#endif
  //
  K(UI::k_backspace,    SDLK_BACKSPACE)
  K(UI::k_tab,          SDLK_TAB)
  K(UI::k_clear,        SDLK_CLEAR)
  K(UI::k_return,       SDLK_RETURN)
  K(UI::k_pause,        SDLK_PAUSE)
  K(UI::k_escape,       SDLK_ESCAPE)
  K(UI::k_space,        SDLK_SPACE)
  K(UI::k_exclaim,      SDLK_EXCLAIM)
  K(UI::k_quotedbl,     SDLK_QUOTEDBL)
  K(UI::k_hash,         SDLK_HASH)
  K(UI::k_dollar,       SDLK_DOLLAR)
  K(UI::k_ampersand,    SDLK_AMPERSAND)
  K(UI::k_quote,        SDLK_QUOTE)
  K(UI::k_leftparen,    SDLK_LEFTPAREN)
  K(UI::k_rightparen,   SDLK_RIGHTPAREN)
  K(UI::k_asterisk,     SDLK_ASTERISK)
  K(UI::k_plus,         SDLK_PLUS)
  K(UI::k_minus,        SDLK_MINUS)
  K(UI::k_0,            SDLK_0)
  K(UI::k_1,            SDLK_1)
  K(UI::k_2,            SDLK_2)
  K(UI::k_3,            SDLK_3)
  K(UI::k_4,            SDLK_4)
  K(UI::k_5,            SDLK_5)
  K(UI::k_6,            SDLK_6)
  K(UI::k_7,            SDLK_7)
  K(UI::k_8,            SDLK_8)
  K(UI::k_9,            SDLK_9)
  K(UI::k_colon,        SDLK_COLON)
  K(UI::k_less,         SDLK_LESS)
  K(UI::k_greater,      SDLK_GREATER)
  K(UI::k_question,     SDLK_QUESTION)
  K(UI::k_at,           SDLK_AT)
  K(UI::k_caret,        SDLK_CARET)
  K(UI::k_underscore,   SDLK_UNDERSCORE)
  K(UI::k_backquote,    SDLK_BACKQUOTE)
  K(UI::k_delete,       SDLK_DELETE)
  for(i=0; i<='z'-'a'; i++) {
    K(UI::k_a+i, SDLK_a+i);
  }
  K(UI::k_kp0,          SDLK_KP0)
  K(UI::k_kp1,          SDLK_KP1)
  K(UI::k_kp2,          SDLK_KP2)
  K(UI::k_kp3,          SDLK_KP3)
  K(UI::k_kp4,          SDLK_KP4)
  K(UI::k_kp5,          SDLK_KP5)
  K(UI::k_kp6,          SDLK_KP6)
  K(UI::k_kp7,          SDLK_KP7)
  K(UI::k_kp8,          SDLK_KP8)
  K(UI::k_kp9,          SDLK_KP9)
  K(UI::k_kp_period,    SDLK_KP_PERIOD)
  K(UI::k_kp_divide,    SDLK_KP_DIVIDE)
  K(UI::k_kp_multiply,  SDLK_KP_MULTIPLY)
  K(UI::k_kp_minus,     SDLK_KP_MINUS)
  K(UI::k_kp_plus,      SDLK_KP_PLUS)
  K(UI::k_kp_enter,     SDLK_KP_ENTER)
  K(UI::k_kp_equals,    SDLK_KP_EQUALS)
  K(UI::k_up,           SDLK_UP)
  K(UI::k_down,         SDLK_DOWN)
  K(UI::k_right,        SDLK_RIGHT)
  K(UI::k_left,         SDLK_LEFT)
  K(UI::k_insert,       SDLK_INSERT)
  K(UI::k_home,         SDLK_HOME)
  K(UI::k_end,          SDLK_END)
  K(UI::k_pageup,       SDLK_PAGEUP)
  K(UI::k_pagedown,     SDLK_PAGEDOWN)
  K(UI::k_f1,           SDLK_F1)
  K(UI::k_f2,           SDLK_F2)
  K(UI::k_f3,           SDLK_F3)
  K(UI::k_f4,           SDLK_F4)
  K(UI::k_f5,           SDLK_F5)
  K(UI::k_f6,           SDLK_F6)
  K(UI::k_f7,           SDLK_F7)
  K(UI::k_f8,           SDLK_F8)
  K(UI::k_f9,           SDLK_F9)
  K(UI::k_f10,          SDLK_F10)
  K(UI::k_f11,          SDLK_F11)
  K(UI::k_f12,          SDLK_F12)
  K(UI::k_f13,          SDLK_F13)
  K(UI::k_f14,          SDLK_F14)
  K(UI::k_f15,          SDLK_F15)
  K(UI::k_numlock,      SDLK_NUMLOCK)
  K(UI::k_capslock,     SDLK_CAPSLOCK)
  K(UI::k_scrollock,    SDLK_SCROLLOCK)
  K(UI::k_rshift,       SDLK_RSHIFT)
  K(UI::k_lshift,       SDLK_LSHIFT)
  K(UI::k_rctrl,        SDLK_RCTRL)
  K(UI::k_lctrl,        SDLK_LCTRL)
  K(UI::k_ralt,         SDLK_RALT)
  K(UI::k_lalt,         SDLK_LALT)
  K(UI::k_mode,         SDLK_MODE)
  K(UI::k_compose,      SDLK_COMPOSE)
  K(UI::k_help,         SDLK_HELP)
  K(UI::k_print,        SDLK_PRINT)
  K(UI::k_sysreq,       SDLK_SYSREQ)
  K(UI::k_break,        SDLK_BREAK)
  K(UI::k_menu,         SDLK_MENU)
  K(UI::k_power,        SDLK_POWER)
  K(UI::k_euro,         SDLK_EURO)
  K(UI::k_undo,         SDLK_UNDO)
#undef K
  //
  {
    for(int i=0; i<UI::max_key_modifiers; i++) {
      kmod[i][0] = -1;
    }
  }
}

inline lib3d::IRenderDriver* RSDLTerminal::get_driver(void)
{
  return screen_gl;
}

