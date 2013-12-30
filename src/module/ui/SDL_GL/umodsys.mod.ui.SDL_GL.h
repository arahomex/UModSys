// SDLV
namespace SDL {

using namespace lib3d;

struct SSRect : public SDL_Rect {
  //
  SSRect(int xx, int yy, int dx, int dy) { x=xx; y=yy; w=dx; x=dy; }
//  SSRect(const SRect& r) { x=r.x; y=r.y; w=r.dx; h=r.dy; }
  SSRect(const SDL_Rect& r) { x=r.x; y=r.y; w=r.w; h=r.h; }
//  SSRect(const SRect* r) { x=r->x; y=r->y; w=r->dx; h=r->dy; }
  SSRect(const SDL_Rect* r) { x=r->x; y=r->y; w=r->w; h=r->h; }
  //
  operator const SDL_Rect*(void) const { return this; }
  operator SDL_Rect*(void) { return this; }
  //
};
struct SBitFontInfo {
  int ch1, ch2, ch_num;
  int dx, dy;
  int bpp, cbytes, clbytes, size;
};

} // namespace SDL

using namespace SDL;


// 
// RSDLVideo class
// 
struct RSDLTerminal : public UI::ITerminal, public UI::IKeyboardController, public UI::IMouseController {
  UMS_ROOT_IMPLEMENT3(RSDLTerminal, UI::ITerminal, UI::IKeyboardController, UI::IMouseController)
  UMS_REFOBJECT_CIMPLEMENT_FOOTDEF(RSDLTerminal, UI::ITerminal, RGenerator)
  //
  // terminal
  // get or create inputs
  UI::TTerminalConnection<UI::IKeyboardController>& get_input_keyboard(void) { return tc_key; }
  UI::TTerminalConnection<UI::IMouseController>& get_input_mouse(void) { return tc_mouse; }
  UI::TTerminalConnection<UI::IJoystickController>& get_input_joystick(void) { return tc_joy; }
  // get or create outputs
  UI::TTerminalConnection<lib3d::IRenderDriver>& get_output_3d(void) { return tc_3d; }
  UI::TTerminalConnection<lib2d::IRenderDriver>& get_output_2d(void) { return tc_2d; }
  //
  // video
  RSDLVideoSurfaceGL* screen_gl;
  int saved_width, saved_height, saved_bpp, saved_flags;
  bool video_lost;
  int video_init_counter;
  //
  inline void close_gl(void) { screen_gl = NULL; }
  //
  void stop_screen(void);
  //
  int open(BCStr driver_name, int flags);
  int close(void);
  //
  void set_title(BCStr title);
  //
  RSDLVideoSurfaceGL* open_screen_gl(int width, int height, int bpp, int flags);
  RSDLVideoSurfaceGL* get_screen_gl(void);
  //
  bool resize_screen(int width, int height);
  //
//  ISDLVideoFont* build_bitfont(const UMS_CSMem& mem, int bpp, int cx, int cy, int ch1, int ch2);
  //
  RSDLTerminal(RGenerator* s);
  ~RSDLTerminal(void);
  //
  //
  // -- events
  SDL_Event event;
  bool mouse1;
  //
  //
  // UI interfaces
#if defined(SDLV13)
  int km_scan[SDL_NUM_SCANCODES];
  int km_key[MAX_UKEYS];
  float keydowntime[SDL_NUM_SCANCODES+MAX_UKEYS];
  char keydown[SDL_NUM_SCANCODES+MAX_UKEYS];
  SDLKey keydownkey[SDL_NUM_SCANCODES+MAX_UKEYS];
  SDL_scancode keydownscan[SDL_NUM_SCANCODES+MAX_UKEYS];
  int keydownchar[SDL_NUM_SCANCODES+MAX_UKEYS];
#elif defined(SDLV12)
  int km_key[SDLK_LAST];
  float keydowntime[SDLK_LAST];
  char keydown[SDLK_LAST];
  SDLKey keydownkey[SDLK_LAST];
  int keydownscan[SDLK_LAST];
  int keydownchar[SDLK_LAST];
#endif
  int kmod[UI::max_key_modifiers][4];
  float key_repeat_1, key_repeat_2;
  UI::SMouseInput ms;
  //
  typedef types::TRefObjectComposite<UI::IKeyboardClient> KeyboardClient;
  typedef types::TRefObjectCompositeExtra<UI::IMouseClient, int> MouseClient;
  //
  types::TScatterArray< KeyboardClient > ckeys;
  types::TScatterArray< MouseClient > cmouses;
  //
  //
  inline static bool fhandle(const KeyboardClient& H, const UI::SKeyboardInputRaw& ev) { return H->key_pressed(ev); }
  inline static bool fhandle(const KeyboardClient& H, const UI::SKeyboardInputC& ev) { return H->key_charc(ev); }
  inline static bool fhandle(const KeyboardClient& H, const UI::SKeyboardInputW& ev) { return H->key_charw(ev); }
  inline static bool fhandle(const KeyboardClient& H, const UI::SKeyboardInputU& ev) { return H->key_charu(ev); }
  inline static bool fhandle(const MouseClient& H, const UI::SMouseInput& ev) { return H->mouse_event(ev); }
  //
  template<typename Arr, typename Ev>
  inline static bool t_invoke_handler(const Arr& handlers, const Ev& ev) {
    typename Arr::Index idx;
    if(!handlers.first(idx))
      return false;
    do {
      bool rv = fhandle( *handlers(idx), ev);
      if(rv)
        return rv;
    } while(handlers.next(idx));
    return false;
  }
  //
  UI::eTerminalState termstate;
  const ICharCodePage* def_cpc;
  //
  double clock(void);
  bool wait(float sec);
  bool poll_events(int maxcount);
  UI::eTerminalState get_terminal_state(void) { return termstate; }
  bool set_terminal_state(UI::eTerminalState state);
  bool terminal_reset(void);
  bool set_terminal_caption(BCStr caption);
  //
  bool mouse_unlink(int order);
  bool mouse_link(int order, IRefObject* combo, UI::IMouseClient* target, int memask);
  bool mouse_reset(void);
  bool mouse_setvisible(bool visible);
  bool mouse_setposition(int x, int y);
  //
  bool key_unlink(int order);
  bool key_link(int order, IRefObject* combo, UI::IKeyboardClient* target);
  bool key_setrepeat(float firsttime, float nexttime);
  bool key_setmodifier(int modifier, int code, int statuson);
  bool key_reset(void);
  //
  bool next1(const SDL_Event &event);
  int translate_mod(int sdl_mod);
  bool next(void);
  //
  void init_keymap(void);
  bool get_keymap(unsigned kmi, unsigned scan, int& kcode, int &ucode);
  bool reset_mouse(void);
  //
  //
  UI::TTerminalConnectionComposite<UI::IKeyboardController, RSDLTerminal> tc_key;
  UI::TTerminalConnectionComposite<UI::IMouseController, RSDLTerminal> tc_mouse;
  UI::TTerminalConnectionComposite<UI::IJoystickController, RSDLTerminal> tc_joy;
  UI::TTerminalConnectionComposite<lib2d::IRenderDriver, RSDLTerminal> tc_2d;
  UI::TTerminalConnectionComposite<lib3d::IRenderDriver, RSDLTerminal> tc_3d;
  //
  inline lib3d::IRenderDriver* get_driver(void);
  //
  inline int connection_get_count(UI::IKeyboardController* hint) { return 1; }
  inline UI::IKeyboardController* connection_get_handler_i(UI::IKeyboardController* hint, int id) { return this; }
  inline UI::IKeyboardController* connection_get_handler_s(UI::IKeyboardController* hint, BCStr id) { return id==NULL ? this : NULL; }
  inline UI::IKeyboardController* connection_create_handler(UI::IKeyboardController* hint, BCStr name, const SParameters* params) { return this; }
  //
  inline int connection_get_count(UI::IMouseController* hint) { return 1; }
  inline UI::IMouseController* connection_get_handler_i(UI::IMouseController* hint, int id) { return this; }
  inline UI::IMouseController* connection_get_handler_s(UI::IMouseController* hint, BCStr id) { return id==NULL ? this : NULL; }
  inline UI::IMouseController* connection_create_handler(UI::IMouseController* hint, BCStr name, const SParameters* params) { return this; }
  //
  inline int connection_get_count(UI::IJoystickController* hint) { return 0; }
  inline UI::IJoystickController* connection_get_handler_i(UI::IJoystickController* hint, int id) { return NULL; }
  inline UI::IJoystickController* connection_get_handler_s(UI::IJoystickController* hint, BCStr id) { return NULL; }
  inline UI::IJoystickController* connection_create_handler(UI::IJoystickController* hint, BCStr name, const SParameters* params) { return NULL; }
  //
  inline int connection_get_count(lib3d::IRenderDriver* hint) { return get_driver()!=NULL ? 1 : 0; }
  inline lib3d::IRenderDriver* connection_get_handler_i(lib3d::IRenderDriver* hint, int id) { return get_driver(); }
  inline lib3d::IRenderDriver* connection_get_handler_s(lib3d::IRenderDriver* hint, BCStr id) { return id==NULL ? get_driver() : NULL; }
  inline lib3d::IRenderDriver* connection_create_handler(lib3d::IRenderDriver* hint, BCStr name, const SParameters* params);
  //
  inline int connection_get_count(lib2d::IRenderDriver* hint) { return get_driver()!=NULL ? 1 : 0; }
  inline lib2d::IRenderDriver* connection_get_handler_i(lib2d::IRenderDriver* hint, int id) { return get_driver(); }
  inline lib2d::IRenderDriver* connection_get_handler_s(lib2d::IRenderDriver* hint, BCStr id) { return id==NULL ? get_driver() : NULL; }
  inline lib2d::IRenderDriver* connection_create_handler(lib2d::IRenderDriver* hint, BCStr name, const SParameters* params) 
    { return connection_create_handler((lib3d::IRenderDriver*)0, name, params); }
  //
};

// 
// RGenerator class
// 
struct RGenerator : public IGenerator {
  UMS_GENERATOR_IMPLEMENT(RGenerator)
  //
  RSDLTerminal term;
  //
  int get_generated_types(DListStr& list, const DHString& type) const;
  int get_generated_names(DListStr& list) const;
  bool generate(IRefObject::P& obj, const DHString& type, const SParameters& args);
  //
  RGenerator(void);
};

using namespace lib3d;

