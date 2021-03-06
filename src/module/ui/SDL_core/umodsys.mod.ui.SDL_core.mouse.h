
//***************************************
// RMouseController::
//***************************************

RMouseController::RMouseController(DOwner *own)
: refs(own) 
{
}

RMouseController::~RMouseController(void) 
{
}

bool RMouseController::process(SDL_MouseMotionEvent& ev) 
{
  SMouseInput inp = {
    0, 1,
    lib2d::DPoint(ev.x, ev.y), lib2d::DPoint(ev.xrel, ev.yrel), lib2d::DPoint(0, 0)
  };
  if(ev.x || ev.y)
    inp.eventid |= me_MoveAbs;
  if(ev.xrel || ev.yrel)
    inp.eventid |= me_MoveRelative;
  for(int i=0; i<max_mouse_buttons; i++) {
    inp.buttons[i] = 0;
    if(ev.state & (1<<i)) {
      inp.buttons[i] |= bis_Pressed;
      inp.eventid |= me_MoveButton;
      if(inp.button_count<i+1)
        inp.button_count = i+1;
    }
  }
  for(DClients::CIter x = clients.Last(); x; --x) {
    if(x->second->mouse_event(inp))
      return true;
  }
  return false;
}

bool RMouseController::process(SDL_MouseButtonEvent& ev) 
{
  SMouseInput inp = {
    0, 1,
    lib2d::DPoint(ev.x, ev.y), lib2d::DPoint(0, 0), lib2d::DPoint(0, 0)
  };
  for(int i=0; i<max_mouse_buttons; i++) {
    inp.buttons[i] = 0;
    if(ev.button==i+1) {
      inp.buttons[i] |= ev.state==SDL_PRESSED ? bis_Pressed : bis_Released;
      inp.eventid |= me_Button;
      if(inp.button_count<i+1)
        inp.button_count = i+1;
    }
  }
  for(DClients::CIter x = clients.Last(); x; --x) {
    if(x->second->mouse_event(inp))
      return true;
  }
  return false;
}

bool RMouseController::process(SDL_MouseWheelEvent& ev) 
{
  SMouseInput inp = {
    me_MoveRelative, 1,
    lib2d::DPoint(0, 0), lib2d::DPoint(0, 0), lib2d::DPoint(ev.x, ev.y)
  };
  for(int i=0; i<max_mouse_buttons; i++) {
    inp.buttons[i] = 0;
  }
  for(DClients::CIter x = clients.Last(); x; --x) {
    if(x->second->mouse_event(inp))
      return true;
  }
  return false;
}

bool RMouseController::mouse_unlink(int order) 
{
  return clients.Remove(order);
}

bool RMouseController::mouse_link(int order, IRefObject* combo, IMouseClient* target, int memask) 
{
  PMouseClient* c = clients(order, void_null());
  if(c==NULL)
    return false;
  c->set(combo, target);
  c->extra = memask;
  return true;
}

bool RMouseController::mouse_reset(void) 
{
  mouse_setvisible(true);
  return true;
}

bool RMouseController::mouse_setvisible(bool visible) 
{
  if(visible) {
    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_ShowCursor(1);
  } else {
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_ShowCursor(0);
  }
  return true;
}

bool RMouseController::mouse_setposition(int x, int y) 
{
//  SDL_WarpMouseInWindow(wnd, x, y);
  return false;
}

//***************************************
// END
//***************************************
