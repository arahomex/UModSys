
struct RTerminal;
struct RKeyboardController;
struct RMouseController;

//***************************************
// sdlWindow::
//***************************************

struct SWindow {
  BCStr caption;
  int x, y, dx, dy, flags; 
  SDL_Window *wnd;
  //
  SWindow(void);
  ~SWindow(void);
  //
  inline operator SDL_Window* (void) { return wnd; }
  //
  void close(void);
  bool open(const SParameters& args);
private:
  SWindow(const SWindow& R);
  void operator=(const SWindow& R);
};

//***************************************
// RKeyboardController::
//***************************************

struct RKeyboardController : public IKeyboardController {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RKeyboardController, 2, IKeyboardController)
  UMODSYS_REFOBJECT_COMPOSITE(RTerminal)
public:  
  typedef tl::TRefObjectComposite<IKeyboardClient> PKeyboardClient;
  typedef tl::TScatterArray<PKeyboardClient> DClients;
  DClients clients;
public:
  RKeyboardController(DOwner *own);
  ~RKeyboardController(void);
  //
  bool validate_construction(void) { return true; }
  bool valid(void) const { return true; }
  //
  bool process(SDL_KeyboardEvent& ev);
  bool process(SDL_TextInputEvent& text);
  //
  eKeyCode map_keycode(SDL_Keycode sym);
  Bsint16 map_keystatus(Uint8 s, Uint8 rep);
  Bsint16 map_keyoscode(SDL_Scancode scancode);
  Bsint16 map_keyspecial(Uint16 mod);
public:
  bool key_unlink(int order);
  bool key_link(int order, IRefObject* combo, IKeyboardClient* target);
  bool key_setmodifier(int modifier, int code, int statuson);
  bool key_setrepeat(float firsttime, float nexttime);
  bool key_reset(void);
  bool key_setvisible(bool visible);
};

//***************************************
// RMouseController::
//***************************************

struct RMouseController : public IMouseController {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RMouseController, 2, IMouseController)
  UMODSYS_REFOBJECT_COMPOSITE(RTerminal)
public:  
  typedef tl::TRefObjectCompositeExtra<IMouseClient, int> PMouseClient;
  typedef tl::TScatterArray<PMouseClient> DClients;
  DClients clients;
public:
  RMouseController(DOwner *own);
  ~RMouseController(void);
  //
  bool validate_construction(void) { return true; }
  bool valid(void) const { return true; }
  //
  bool process(SDL_MouseMotionEvent& ev);
  bool process(SDL_MouseButtonEvent& ev);
  bool process(SDL_MouseWheelEvent& ev);
  //
public:
  bool mouse_unlink(int order);
  bool mouse_link(int order, IRefObject* combo, IMouseClient* target, int memask);
  bool mouse_reset(void);
  bool mouse_setvisible(bool visible);
  bool mouse_setposition(int x, int y);
};

//***************************************
// RTerminalBase::
//***************************************

struct RTerminal : public ITerminal {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RTerminal, 2, ITerminal)
  UMODSYS_REFOBJECT_REFMODULE()
public:  
  typedef tl::TRefObject<RKeyboardController, tl::TRefObjectWeakFunc<RKeyboardController> > PWKeyboardController;
  typedef tl::TRefObject<RMouseController, tl::TRefObjectWeakFunc<RMouseController> > PWMouseController;
  //
  mutable RKeyboardController keybd;
  mutable RMouseController mouse;
  //
  int states;
public:  
  RTerminal(DOwner *own);
  ~RTerminal(void);
  //
  virtual lib2d::IRenderDriver* get_drv2d(void) const =0;
  virtual lib3d::IRenderDriver* get_drv3d(void) const =0;
  virtual lib2d::IRenderDriver* open_drv2d(const SParameters* params) =0;
  virtual lib3d::IRenderDriver* open_drv3d(const SParameters* params) =0;
  virtual bool close_drvxd(void) =0;
  //
  inline bool validate_construction(void) { return true; }
  //
public:  
  // special functions
  bool poll_events(int maxcount);
  bool get_terminal_state(eTerminalState state);
  bool set_terminal_state(eTerminalState state, bool flag);
  double get_clock(void);
  bool wait(float sec);
  //
  size_t get_count(TypeId tid) const;
  IRefObject* get_handler(TypeId tid, size_t id) const;
  IRefObject* get_handler(TypeId tid, BCStr name) const;
  IRefObject* get_friend_handler(TypeId tid, IRefObject *obj) const;
  bool create_handler(TypeId tid, IRefObject::P &rv, BCStr name, const SParameters* params);
public:  
};

//***************************************
// END
//***************************************
