
//***************************************
// RTerminal_SDL_core::
//***************************************

struct RTerminal_SDL_core : public ITerminal {
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(ui,SDL_core)::RTerminal_SDL_core, 2, ITerminal)
  UMODSYS_REFOBJECT_REFMODULE()
public:  
  TRTerminalConnectionNull<IKeyboardController> ctrl_key;
  TRTerminalConnectionNull<IMouseController> ctrl_mouse;
  TRTerminalConnectionNull<ITouchController> ctrl_touch;
  TRTerminalConnectionNull<IJoystickController> ctrl_joy;
  //
  TRTerminalConnectionNull<lib2d::ICaptureDriver> ctrl_2in;
  TRTerminalConnectionNull<lib3d::ICaptureDriver> ctrl_3in;
  TRTerminalConnectionNull<libmovie::ICaptureDriver> ctrl_vin;
  TRTerminalConnectionNull<libsound::ICaptureDriver> ctrl_sin;
  TRTerminalConnectionNull<libmusic::ICaptureDriver> ctrl_min;
  //
  TRTerminalConnectionNull<lib3d::IRenderDriver> ctrl_3out;
  TRTerminalConnectionNull<lib2d::IRenderDriver> ctrl_2out;
  TRTerminalConnectionNull<libmovie::IRenderDriver> ctrl_vout;
  TRTerminalConnectionNull<libsound::IRenderDriver> ctrl_sout;
  TRTerminalConnectionNull<libmusic::IRenderDriver> ctrl_mout;
  //
  SDL_Window *wnd;
  eTerminalState state;
public:  
  RTerminal_SDL_core(DOwner *own, const SParameters& args)
  : refs(own), wnd(NULL) {
    state = ts_Active;
    SDL_Init(SDL_INIT_EVERYTHING);
    wnd = SDL_CreateWindow("SDL", 20, 20, 640, 480, 0);
  }
  ~RTerminal_SDL_core(void) {
    if(wnd!=NULL) {
      SDL_DestroyWindow(wnd);
      wnd = NULL;
    }
    SDL_Quit();
  }
  bool validate_construction(void) { return true; }
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
        break;
      if(ev.common.type==SDL_APP_TERMINATING) {
        state = ts_Terminated;
      } else if(ev.common.type==SDL_WINDOWEVENT && ev.window.event==SDL_WINDOWEVENT_CLOSE) {
        state = ts_Terminated;
      }
    }
    return true; 
  }
  eTerminalState get_terminal_state(void) { return state; }
  bool set_terminal_state(eTerminalState ss) {
    if(state==ts_Terminated) {
      if(wnd!=NULL) {
        SDL_DestroyWindow(wnd);
        wnd = NULL;
      }
    }
    state = ss;
    return true; 
  }
  bool set_terminal_caption(BCStr caption) { 
    return false; 
  }
  double get_clock(void) { 
    return SDL_GetTicks(); 
  }
  bool wait(float sec) { 
    SDL_Delay(sec*1000);
    return true; 
  }
  bool terminal_reset(void) { 
    return false; 
  }
  // get or create pure inputs
  TITerminalConnection<IKeyboardController>& get_input_keyboard(void) { return ctrl_key; }
  TITerminalConnection<IMouseController>& get_input_mouse(void) { return ctrl_mouse; }
  TITerminalConnection<ITouchController>& get_input_touch(void) { return ctrl_touch; }
  TITerminalConnection<IJoystickController>& get_input_joystick(void) { return ctrl_joy; }
  // get or create inputs
  TITerminalConnection<lib2d::ICaptureDriver>& get_input_2d(void) { return ctrl_2in; }
  TITerminalConnection<lib3d::ICaptureDriver>& get_input_3d(void) { return ctrl_3in; }
  TITerminalConnection<libmovie::ICaptureDriver>& get_input_movie(void) { return ctrl_vin; }
  TITerminalConnection<libsound::ICaptureDriver>& get_input_sound(void) { return ctrl_sin; }
  TITerminalConnection<libmusic::ICaptureDriver>& get_input_music(void) { return ctrl_min; }
  // get or create outputs
  TITerminalConnection<lib3d::IRenderDriver>& get_output_3d(void) { return ctrl_3out; }
  TITerminalConnection<lib2d::IRenderDriver>& get_output_2d(void) { return ctrl_2out; }
  TITerminalConnection<libmovie::IRenderDriver>& get_output_movie(void) { return ctrl_vout; }
  TITerminalConnection<libsound::IRenderDriver>& get_output_sound(void) { return ctrl_sout; }
  TITerminalConnection<libmusic::IRenderDriver>& get_output_music(void) { return ctrl_mout; }
public:  
};

//***************************************
// END
//***************************************
