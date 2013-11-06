
//***************************************
// RTerminal_SDL_core::
//***************************************

struct RTerminal_SDL_core : public ITerminal {
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(ui,SDL_core)::RTerminal_SDL_core, 2, ITerminal)
  UMODSYS_REFOBJECT_REFMODULE()
public:  
  typedef tl::TRefObject<RRenderDriver2D_SDL_core, tl::TRefObjectWeakFunc<RRenderDriver2D_SDL_core> > PWDriver2D;
  //
  tl::TScatterArray<PWDriver2D> list_2d;
  int states;
public:  
  RTerminal_SDL_core(DOwner *own, const SParameters& args)
  : refs(own) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    states = 1<<ts_Active;
  }
  ~RTerminal_SDL_core(void) {
    TTF_Quit();
    SDL_Quit();
  }
  inline bool validate_construction(void) { return true; }
  //
public:  
  // special functions
  bool poll_events(int maxcount) { 
    SDL_Event ev;
    if(maxcount==-1)
      maxcount = 100;
    for(int i=0; i<maxcount; i++) {
      SDL_PumpEvents();
      int n = SDL_PeepEvents(&ev, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
      if(n==0)
        return false;
      if(ev.common.type==SDL_APP_TERMINATING) {
        states |= 1<<ts_Terminated;
      } else if(ev.common.type==SDL_QUIT) {
        states |= 1<<ts_SoftTerminate;
      } else if(ev.common.type==SDL_WINDOWEVENT && ev.window.event==SDL_WINDOWEVENT_CLOSE) {
        states |= 1<<ts_SoftTerminate;
      }
    }
    return true; 
  }
  //
  bool get_terminal_state(eTerminalState state) {
    return (states & (1<<state))!=0;
  }
  bool set_terminal_state(eTerminalState state, bool flag) {
    if(state==ts_Terminated) {
      if(flag) {
        t_close(list_2d);
      }
    }
    if(flag) {
      states |= 1<<state;
    } else {
      states &= ~(1<<state);
    }
    return true;
  }
  //
  double get_clock(void) { 
    return SDL_GetTicks(); 
  }
  bool wait(float sec) { 
    SDL_Delay(sec*1000);
    return true; 
  }
  //
  size_t get_count(TypeId tid) const {
    if(tid==lib2d::IRenderDriver::_root_get_interface_type()) {
      return t_count(list_2d);
    }
    return 0;
  }
  IRefObject* get_handler(TypeId tid, size_t id) const {
    if(tid==lib2d::IRenderDriver::_root_get_interface_type()) {
      return t_get(list_2d, id);
    }
    return NULL;
  }
  IRefObject* get_handler(TypeId tid, BCStr name) const {
    if(tid==lib2d::IRenderDriver::_root_get_interface_type()) {
      return t_get(list_2d, name);
    }
    return NULL;
  }
  IRefObject* get_friend_handler(TypeId tid, IRefObject *obj) const {
    if(tid==lib2d::IRenderDriver::_root_get_interface_type()) {
      return t_getfr(list_2d, obj);
    }
    return NULL;
  }
  bool create_handler(TypeId tid, IRefObject::P &rv, BCStr name, const SParameters* params) {
    if(tid==lib2d::IRenderDriver::_root_get_interface_type()) {
      RRenderDriver2D_SDL_core::SelfP val = new RRenderDriver2D_SDL_core(this, *params);
      if(!ValidateConstruction(rv, val))
        return false;
      return t_add(list_2d, val());
    }
    return false;
  }
public:  
};

//***************************************
// END
//***************************************
