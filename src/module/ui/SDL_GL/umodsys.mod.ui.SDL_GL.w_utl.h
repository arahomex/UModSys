
//***************************************
// RMultiImage2D_SDL_ttf::
//***************************************

bool RMultiImage2D_SDL_ttf::new_texture(Tex& tex, SDL_Surface* s, RRenderDriver* drv) 
{
  if(drv==NULL)
    return NULL;
  tex.gl = &drv->gl;
  if(!tex.gen())
    return false;
  if(!tex.set_reallocate(lib2d::it_I8_R8G8B8A8, s->w, s->h, NULL))
    return false;
  //
  // paletted surface
  const SDL_Color* pal = s->format->palette->colors;
  const BByte *src = static_cast<const BByte *>(s->pixels);
  BByte *dest = tex.mem.get_tdata<BByte>();
  size_t num = s->w * s->h;
  for(; num; num--, dest+=4) {
    const SDL_Color* p = pal + *src++;
    dest[0] = p->r;
    dest[1] = p->g;
    dest[2] = p->b;
    dest[3] = p->a;
  }
  return tex.upload();
}

void RMultiImage2D_SDL_ttf::kill_texture(Tex& tex)
{
  if(tex.valid()) {  
    tex.clear();
  } 
}

void RMultiImage2D_SDL_ttf::init_texture(Tex& tex)
{
}

//***************************************
// RRenderDriver3D::
//***************************************

RRenderDriver3D::RRenderDriver3D(DOwner *own)
: refs(own), 
  glctx(NULL),
  clear_color(0,0,0,0),
  cur_color(0,0,0,0), cur_tm(lib2d::tm_Opaque) 
{
  video_init_counter = 1;
  gl.vic = &video_init_counter;
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
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
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
  if(!gl.load(false))
    return false;
  //
  return true;
}

void RRenderDriver3D::set_color(void) 
{
}

void RRenderDriver3D::Update(void) 
{
  gl.glFinish();
  gl.e();
  SDL_GL_SwapWindow(wnd);
  gl.e();
}

//***************************************
// END
//***************************************
