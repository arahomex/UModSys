
//***************************************
// RKeyboardController::
//***************************************

struct RKeyboardController : public IKeyboardController {
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(ui,SDL_core)::RKeyboardController, 2, IKeyboardController)
  UMODSYS_REFOBJECT_REFOTHER(RTerminal)
public:  
  typedef tl::TRefObjectComposite<IKeyboardClient> PKeyboardClient;
  typedef tl::TScatterArray<PKeyboardClient> DClients;
  DClients clients;
public:
  RKeyboardController(DOwner *own, const SParameters& args)
  : refs(own) {
  }
  ~RKeyboardController(void) {
  }
  bool validate_construction(void) { return true; }
  bool valid(void) const { return true; }
  //
  bool process(SDL_KeyboardEvent& ev) {
    SKeyboardInputRaw key = {
      map_keycode(ev.keysym.sym),
      map_keystatus(ev.keysym.mod),
      map_keyoscode(ev.keysym.scancode),
      0
    };
    if(ev.state!=SDL_PRESSED) {
      key.code = -key.code;
      key.os_code = -key.os_code;
    }
    for(DClients::CIter x = clients.Last(); x; --x) {
      if(x->second->key_pressed(key))
        return true;
    }
    return false;
  }
  bool process(SDL_TextInputEvent& text) {
    const tl::su::utf8* p = text.text;
    size_t len = strlen(p), n=0;
    SKeyboardInputText key;
    while(1) {
      if(!tl::su::utf_8to32(key.text, p, len))
        break;
      for(DClients::CIter x = clients.Last(); x; --x) {
        if(x->second->key_text(key)) {
          n++;
          break;
        }
      }
    }
    return n>0;
  }
  //
  eKeyCode map_keycode(SDL_Keycode sym) {
    switch(sym) {
#define B(a, b) case SDLK_##a: return k_##b;
#define C(a, b) case SDLK_##b: return k_##b;
      B(BACKSPACE, backspace) B(TAB, tab) B(CLEAR, clear) B(RETURN, return) B(PAUSE, pause) B(ESCAPE, escape) B(SPACE, space)
      B(EXCLAIM, exclaim) B(QUOTEDBL, quotedbl) B(HASH, hash) B(DOLLAR, dollar) B(AMPERSAND, ampersand) B(QUOTE, quote) 
      B(LEFTPAREN, leftparen) B(RIGHTPAREN, rightparen) B(ASTERISK, asterisk) B(PLUS, plus) B(MINUS, minus) B(PERIOD, period) B(SLASH, slash)
      C(1, 1) C(2, 2) C(3, 3) C(4, 4) C(5, 5) C(6, 6) C(7, 7) C(8, 8) C(9, 9) B(0, 0)
      B(COLON, colon) B(SEMICOLON, semicolon) B(LESS, less) B(GREATER, greater) B(QUESTION, question) B(AT, at) B(LEFTBRACKET, leftbracket) B(RIGHTBRACKET, rightbracket) 
      B(BACKSLASH, backslash) B(CARET, caret) B(UNDERSCORE, underscore) /*B(BACKSQUOTE, backsquote)*/
      C('A', a) C('B', b) C('C', c) C('D', d) C('E', e) C('F', f) C('G', g) C('H', h) C('I', i) C('J', j) C('K', k) C('L', l) C('M', m)
      C('N', n) C('O', o) C('P', p) C('Q', q) C('R', r) C('S', s) C('T', t) C('U', u) C('V', v) C('W', w) C('X', x) C('Y', y) C('Z', z)
      B(KP_1, kp1) B(KP_2, kp2) B(KP_3, kp3) B(KP_4, kp4) B(KP_5, kp5) B(KP_6, kp6) B(KP_7, kp7) B(KP_8, kp8) B(KP_9, kp9) B(KP_0, kp0)
      B(KP_DIVIDE, kp_divide) B(KP_MULTIPLY, kp_multiply) B(KP_MINUS, kp_minus) B(KP_PLUS, kp_plus) B(KP_ENTER, kp_enter)
      B(F1, f1) B(F2, f2) B(F3, f3) B(F4, f4) B(F5, f5) B(F6, f6) B(F7, f7) B(F8, f8) B(F9, f9) B(F10, f10) B(F11, f11) B(F12, f12)
      B(INSERT, insert) B(DELETE, delete) B(PAGEUP, pageup) B(PAGEDOWN, pagedown) B(HOME, home) B(END, end) B(LEFT, left) B(RIGHT, right) B(UP, up) B(DOWN, down)
#undef C
#undef B
    } return k_none;
  }
  Bsint16 map_keystatus(Uint16 mod) {
    Bsint16 rv = 0;
#define B(a, b) if(mod & KMOD_##a) rv |= km_##b;
    B(LSHIFT, l_shift) B(RSHIFT, r_shift) B(LCTRL, l_ctrl) B(RCTRL, r_ctrl) 
    B(LALT, l_alt) B(RALT, r_alt) B(LGUI, l_super) B(RGUI, r_super) 
    B(NUM, numlock) B(CAPS, capslock) B(MODE, scrollock)
#undef B
    return rv;
  }
  Bsint16 map_keyoscode(SDL_Scancode scancode) {
    return 0;
  }
public:
  bool key_unlink(int order) {
    return clients.Remove(order);
  }
  bool key_link(int order, IRefObject* combo, IKeyboardClient* target) {
    PKeyboardClient* c = clients(order, void_null());
    if(c==NULL)
      return false;
    c->set(combo, target);
    return true;
  }
  bool key_setmodifier(int modifier, int code, int statuson) {
    return false;
  }
  bool key_setrepeat(float firsttime, float nexttime) {
    return false;
  }
  bool key_reset(void) {
    return false;
  }
};


//***************************************
// END
//***************************************
