
//***************************************
// RTerminal::
//***************************************

struct RTerminal : public ITerminal {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RTerminal, 2, ITerminal)
  UMODSYS_REFOBJECT_REFMODULE()
public:  
  typedef tl::TRefObject<RRenderDriver2D, tl::TRefObjectWeakFunc<RRenderDriver2D> > PWDriver2D;
  typedef tl::TRefObject<RKeyboardController, tl::TRefObjectWeakFunc<RKeyboardController> > PWKeyboardController;
  typedef tl::TRefObject<RMouseController, tl::TRefObjectWeakFunc<RMouseController> > PWMouseController;
  //
  tl::TScatterArray<PWDriver2D> list_2d;
  PWKeyboardController keybd;
  PWMouseController mouse;
  int states;
public:  
  RTerminal(DOwner *own, const SParameters& args)
  : refs(own) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    states = 1<<ts_Active;
  }
  ~RTerminal(void) {
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
      } else if(ev.common.type==SDL_KEYDOWN || ev.common.type==SDL_KEYUP) {
        if(keybd.valid()) {
          keybd->process(ev.key);
        }
      } else if(ev.common.type==SDL_TEXTINPUT) {
        if(keybd.valid()) {
          keybd->process(ev.text);
        }
      } else if(ev.common.type==SDL_MOUSEMOTION) {
        if(mouse.valid()) {
          mouse->process(ev.motion);
        }
      } else if(ev.common.type==SDL_MOUSEBUTTONDOWN || ev.common.type==SDL_MOUSEBUTTONUP) {
        if(mouse.valid()) {
          mouse->process(ev.button);
        }
      } else if(ev.common.type==SDL_MOUSEWHEEL) {
        if(mouse.valid()) {
          mouse->process(ev.wheel);
        }
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
    if(state==ts_Keyboard) {
      if(flag) SDL_StartTextInput(); else SDL_StopTextInput();
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
    if(tid==libui::IKeyboardController::_root_get_interface_type()) {
      return 1;
    }
    if(tid==libui::IMouseController::_root_get_interface_type()) {
      return 1;
    }
    return 0;
  }
  IRefObject* get_handler(TypeId tid, size_t id) const {
    if(tid==lib2d::IRenderDriver::_root_get_interface_type()) {
      return t_get(list_2d, id);
    }
    if(tid==libui::IKeyboardController::_root_get_interface_type()) {
      return id==0 ? keybd() : NULL;
    }
    if(tid==libui::IMouseController::_root_get_interface_type()) {
      return id==0 ? mouse() : NULL;
    }
    return NULL;
  }
  IRefObject* get_handler(TypeId tid, BCStr name) const {
    if(tid==lib2d::IRenderDriver::_root_get_interface_type()) {
      return t_get(list_2d, name);
    }
    if(tid==libui::IKeyboardController::_root_get_interface_type()) {
      return NULL;
    }
    if(tid==libui::IMouseController::_root_get_interface_type()) {
      return NULL;
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
      RRenderDriver2D::SelfP val = new RRenderDriver2D(this, *params);
      if(!ValidateConstruction(rv, val))
        return false;
      return t_add(list_2d, val());
    }
    if(tid==libui::IKeyboardController::_root_get_interface_type()) {
      if(keybd.valid())
        return false; // already done
      RKeyboardController::SelfP val = new RKeyboardController(this, *params);
      if(!ValidateConstruction(rv, val))
        return false;
      keybd = val();
      return true;
    }
    if(tid==libui::IMouseController::_root_get_interface_type()) {
      if(mouse.valid())
        return false; // already done
      RMouseController::SelfP val = new RMouseController(this, *params);
      if(!ValidateConstruction(rv, val))
        return false;
      mouse = val();
      return true;
    }
    return false;
  }
public:  
};

//***************************************
// END
//***************************************
