
//***************************************
// RRenderDriver3D::
//***************************************

RRenderDriver3D::RRenderDriver3D(DOwner *own)
: refs(own), 
  glctx(NULL),
  clear_color(0,0,0,0),
  cur_color(0,0,0,0), cur_tm(lib2d::tm_Opaque) 
{
}

RRenderDriver3D::~RRenderDriver3D(void) 
{
  close();
}

void RRenderDriver3D::close(void) 
{
  if(glctx!=NULL) {
    SDL_GL_DeleteContext(glctx);
    glctx = NULL;
  }
  wnd.close();
}

bool RRenderDriver3D::open(const SParameters& args) 
{
  close();
  //
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  //
  wnd.flags |= ::SDL_WINDOW_OPENGL;
  if(!wnd.open(args)) {
    close();
    return false;
  }
  max_values.add("xoffset", wnd.x); max_values.add("yoffset", wnd.y);
  max_values.add("width", wnd.dx); max_values.add("height", wnd.dy);
  //
  glctx = SDL_GL_CreateContext(wnd);
  SDL_GL_MakeCurrent(wnd, glctx);
  if(!gl.load())
    return false;
  //
  return true;
}

void RRenderDriver3D::set_color(void) 
{
}

//***************************************
// END
//***************************************
