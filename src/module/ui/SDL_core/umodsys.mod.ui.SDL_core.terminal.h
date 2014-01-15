
//***************************************
// RTerminal::
//***************************************

RTerminal::RTerminal(DOwner *own)
: refs(own), keybd(this), mouse(this)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  states = 1<<ts_Active;
}

RTerminal::~RTerminal(void) 
{
  TTF_Quit();
  SDL_Quit();
}

//
// special functions
bool RTerminal::poll_events(int maxcount) 
{ 
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
        keybd.process(ev.key);
      }
    } else if(ev.common.type==SDL_TEXTINPUT) {
      if(keybd.valid()) {
        keybd.process(ev.text);
      }
    } else if(ev.common.type==SDL_MOUSEMOTION) {
      if(mouse.valid()) {
        mouse.process(ev.motion);
      }
    } else if(ev.common.type==SDL_MOUSEBUTTONDOWN || ev.common.type==SDL_MOUSEBUTTONUP) {
      if(mouse.valid()) {
        mouse.process(ev.button);
      }
    } else if(ev.common.type==SDL_MOUSEWHEEL) {
      if(mouse.valid()) {
        mouse.process(ev.wheel);
      }
    }
  }
  return true; 
}

bool RTerminal::get_terminal_state(eTerminalState state) 
{
  return (states & (1<<state))!=0;
}

bool RTerminal::set_terminal_state(eTerminalState state, bool flag) 
{
  if(state==ts_Terminated) {
    close_drvxd();
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

double RTerminal::get_clock(void) 
{ 
  return SDL_GetTicks(); 
}

bool RTerminal::wait(float sec) 
{ 
  SDL_Delay(sec*1000);
  return true; 
}

size_t RTerminal::get_count(TypeId tid) const 
{
  if(tid==lib2d::IRenderDriver::_root_get_interface_type()) {
    return get_drv2d()!=NULL;
  }
  if(tid==lib3d::IRenderDriver::_root_get_interface_type()) {
    return get_drv3d()!=NULL;
  }
  if(tid==libui::IKeyboardController::_root_get_interface_type()) {
    return 1;
  }
  if(tid==libui::IMouseController::_root_get_interface_type()) {
    return 1;
  }
  return 0;
}

IRefObject* RTerminal::get_handler(TypeId tid, size_t id) const 
{
  if(tid==lib2d::IRenderDriver::_root_get_interface_type()) {
    return id==0 ? get_drv2d() : NULL;
  }
  if(tid==lib3d::IRenderDriver::_root_get_interface_type()) {
    return id==0 ? get_drv3d() : NULL;
  }
  if(tid==libui::IKeyboardController::_root_get_interface_type()) {
    return id==0 ? &keybd : NULL;
  }
  if(tid==libui::IMouseController::_root_get_interface_type()) {
    return id==0 ? &mouse : NULL;
  }
  return NULL;
}

IRefObject* RTerminal::get_handler(TypeId tid, BCStr name) const 
{
  if(tid==lib2d::IRenderDriver::_root_get_interface_type()) {
    return NULL;
  }
  if(tid==lib3d::IRenderDriver::_root_get_interface_type()) {
    return NULL;
  }
  if(tid==libui::IKeyboardController::_root_get_interface_type()) {
    return NULL;
  }
  if(tid==libui::IMouseController::_root_get_interface_type()) {
    return NULL;
  }
  return NULL;
}

IRefObject* RTerminal::get_friend_handler(TypeId tid, IRefObject *obj) const 
{
  if(tid==lib2d::IRenderDriver::_root_get_interface_type()) {
    return NULL;
  }
  if(tid==lib3d::IRenderDriver::_root_get_interface_type()) {
    return NULL;
  }
  return NULL;
}

bool RTerminal::create_handler(TypeId tid, IRefObject::P &rv, BCStr name, const SParameters* params) 
{
  if(tid==lib2d::IRenderDriver::_root_get_interface_type()) {
    lib2d::IRenderDriver *x = open_drv2d(params);
    if(x==NULL)
      return false;
    rv = x;
    return true;
  } 
  if(tid==lib3d::IRenderDriver::_root_get_interface_type()) {
    lib3d::IRenderDriver *x = open_drv3d(params);
    if(x==NULL)
      return false;
    rv = x;
    return true;
  } 
  if(tid==libui::IKeyboardController::_root_get_interface_type()) {
    rv = &keybd;
    return true;
  } 
  if(tid==libui::IMouseController::_root_get_interface_type()) {
    rv = &mouse;
    return true;
  }
  return false;
}

//***************************************
// END
//***************************************
