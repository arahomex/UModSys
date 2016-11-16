// 
// RSDLTerminal::
// 
UMS_REFOBJECT_CIMPLEMENT_BODY(RSDLTerminal)

void RSDLTerminal::stop_screen(void) 
{
  if(screen_gl) {
    screen_gl->stop();
/*
    screen_gl->ref_remove();
    screen_gl = NULL;
*/
  }
}

RSDLTerminal::~RSDLTerminal(void) 
{
}

RSDLTerminal::RSDLTerminal(RGenerator* s) 
: tc_key(this), tc_mouse(this), tc_joy(this), tc_3d(this), tc_2d(this) 
{
  def_cpc = NULL;
  screen_gl = NULL;
  video_lost = false;
  video_init_counter = 1;
  //
  rc_init(s);
  //
  init_keymap();
  mouse1 = false;
  memset(keydown, 0, sizeof(keydown));
  key_repeat_1 = key_repeat_2 = 0; // no repeat
  //
  def_cpc = M->char_codepage("UTF8");
  //
  event.type = SDL_NOEVENT;
}

void RSDLTerminal::set_title(BCStr title) 
{
  SDL_WM_SetCaption(title, NULL);
}

int RSDLTerminal::open(BCStr driver_name, int flags) 
{
  BChar name2[256];

  if(driver_name) {
    if(SDL_VideoDriverName(name2, sizeof name2)) {
      if(strcmp(driver_name, name2)==0)
        return 1; // already open this
    }
  } else {
#if defined(SDLV13)
/*
    if(SDL_GetVideoDevice()) {
      return 1; // already open this
    }
*/
    SDL_Surface *ss = SDL_GetVideoSurface();
    if(ss && ss->w>1) {
      return 1; // already open this
    }
#elif defined(SDLV12)
    if(SDL_GetVideoSurface()) {
      return 1; // already open this
    }
#endif
  }
  int rv = SDL_VideoInit(driver_name, flags);
  SDL_EnableUNICODE(1);
  stop_screen();
  return rv;
}

int RSDLTerminal::close(void) 
{
  stop_screen();
  SDL_VideoQuit();
  return 0;
}

RSDLVideoSurfaceGL* RSDLTerminal::get_screen_gl(void) 
{
  return screen_gl;
}

RSDLVideoSurfaceGL* RSDLTerminal::open_screen_gl(int width, int height, int bpp, int flags) 
{
  int f = flags;
  f |= SDL_OPENGL;
  //
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, (f & SDL_DOUBLEBUF)!=0);
//  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
//  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 7);
//  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  f &= ~SDL_DOUBLEBUF;
  //
  SDL_Surface* new_s = SDL_SetVideoMode(width, height, bpp, f);
  //
  if(SDL_GL_GetProcAddress("glEnd")==NULL) {
    // need to load GL library
    SDL_GL_LoadLibrary(NULL);
  }
  //
  if(new_s==NULL) {
    stop_screen();
    return NULL; // fail
  }
  //
  saved_width = width;
  saved_height = height;
  saved_bpp = bpp;
  saved_flags = flags;
  video_init_counter++;
  //
  if(saved_flags & SDL_FULLSCREEN) {
    SDL_WM_GrabInput(SDL_GRAB_ON);
  } else {
    SDL_WM_GrabInput(SDL_GRAB_OFF);
  }
  //
  if(screen_gl) {
    if(screen_gl->surface==new_s) {
      screen_gl->gl.clear();
      screen_gl->init();
      return screen_gl; // not allocate
    }
    //
    screen_gl->stop();
    //
    if(screen_gl->surface==NULL) {
      screen_gl->surface = new_s;
      screen_gl->gl.clear();
      screen_gl->init();
      return screen_gl; // link back
    }
  }
  //
  if(screen_gl)
    screen_gl->ref_remove();
  //
  screen_gl = new RSDLVideoSurfaceGL(new_s, this);
  if(!screen_gl->init()) { // invalid opengl driver (unsupport some functions)
    screen_gl->ref_remove();
    screen_gl = NULL;
    return NULL; 
  }
  //
  termstate = UI::ts_Active; // reset after terminated
  //
  return screen_gl;
}

bool RSDLTerminal::resize_screen(int width, int height) 
{
//  M.debug_put(dps_Info, "Resize to %d %d\n", width, height);
  video_lost = true;
  if(screen_gl) {
    return open_screen_gl(width, height, saved_bpp, saved_flags)!=NULL;
  } else
    return NULL;
}


lib3d::IRenderDriver* RSDLTerminal::connection_create_handler(lib3d::IRenderDriver* hint, BCStr name, const SParameters* params)
{
  if(open(NULL, 0)<0)
    return NULL;                                        
  if(screen_gl!=NULL)
    return screen_gl;
  int width=640, height=480, bpp=16, flags=SDL_OPENGL | SDL_DOUBLEBUF;
  TPrintBuffer<256> SVA;
  if(params) {
    params->get("width", width);
    params->get("height", height);
    params->get("bpp", bpp);
    params->get("flags", flags);
    bool flag;
    if(params->get("fullscreen", flag)) if(flag) flags |= SDL_FULLSCREEN; else flags &= ~SDL_FULLSCREEN;
    if(params->get("resize", flag)) if(flag) flags |= SDL_RESIZABLE; else flags &= ~SDL_RESIZABLE;
    //
    if(params->get("centered", flag)) {
      SDL_putenv(SVA("SDL_VIDEO_CENTERED=%s", flag ? "center" : ""));
    }
    int xoffset=0, yoffset=0;
    flag = params->get("x", xoffset) || params->get("xoffset", xoffset) || params->get("startx", xoffset);
    flag = params->get("y", yoffset) || params->get("yoffset", yoffset) || params->get("starty", yoffset) || flag;
    if(flag) {
      SDL_putenv(SVA("SDL_VIDEO_WINDOW_POS=%d,%d", xoffset, yoffset));
    }
  }
  return open_screen_gl(width, height, bpp, flags);
}

