// 
// RSDLSystem::
// 
/*
int RSDLSystem::open(int flags) {
  int f = 0;
#define F(x, y) if((flags & x)==x) f |= y;
  F(sc_Everything,  SDL_INIT_EVERYTHING)
  F(sc_Timer,       SDL_INIT_TIMER)
  F(sc_Audio,       SDL_INIT_AUDIO)
  F(sc_Video,       SDL_INIT_VIDEO)
  F(sc_CDRom,       SDL_INIT_CDROM)
  F(sc_Joystick,    SDL_INIT_JOYSTICK)
  F(sc_NoParachute, SDL_INIT_NOPARACHUTE)
  F(sc_EventThread, SDL_INIT_EVENTTHREAD)
#undef F
  if(f & SDL_INIT_VIDEO) {
    vid.close(); // force lost surface
  }
  int rv = SDL_Init(f);
  SDL_EnableUNICODE(1);
  return rv;
}

int RSDLSystem::close(void) {
  vid.close();
  SDL_Quit();
  return 0;
}

bool RSDLSystem::wait(double t) {
  SDL_Delay(int(t*1000));
  return true;
}

double RSDLSystem::clock(void) {
  return SDL_GetTicks()/1000.0;
}

RSDLSystem::RSDLSystem(void)
  : vid(this) {
  rc_init();
}

RSDLSystem::~RSDLSystem(void) {
  close(); // always stop system
}
*/