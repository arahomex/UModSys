
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

BStr RRenderDriver3D::s_phm_names[phm_Unknown+1] = {
  "None",     //  phm_None,
  "Error",    //  phm_Error,
  "Begin",    //  phm_Begin,
  "End",      //  phm_End,
  "3D",       //  phm_3D,
  "2D",       //  phm_2D,
  "Draw",     //  phm_Draw,
  "Unknown",  //  phm_Unknown,
};


int RRenderDriver3D::s_phm_goods[phm_Unknown] = {
  phms_Error|phms_Begin,                    //  phm_None,
  phms_End,                                 //  phm_Error,
  phms_2D|phms_3D|phms_End|phms_Error,      //  phm_Begin,
  phms_Begin|phms_Error,                    //  phm_End,
  phms_Draw|phms_2D|phms_End|phms_Error,    //  phm_3D,
  phms_3D|phms_End|phms_Error,              //  phm_2D,
  phms_3D|phms_End|phms_Error,              //  phm_Draw,
};

int RRenderDriver3D::s_phm_bads[phm_Unknown] = {
  ~(phms_Error|phms_Begin),                    //  phm_None,
  0,                                           //  phm_Error,
  ~(phms_2D|phms_3D|phms_End|phms_Error),      //  phm_Begin,
  ~(phms_Begin|phms_Error),                    //  phm_End,
  ~(phms_Draw|phms_2D|phms_End|phms_Error),    //  phm_3D,
  ~(phms_3D|phms_End|phms_Error),              //  phm_2D,
  ~(phms_3D|phms_End|phms_Error),              //  phm_Draw,
};

//***************************************

RRenderDriver3D::RRenderDriver3D(DOwner *own)
: refs(own), 
  glctx(NULL),
  clear_color(0,0,0.1f,0),
  cur_color(0,0,0,0), 
  cur_tm(lib2d::tm_Opaque), phm(phm_None)
{
  debug_matrix = true;
  mode2d = 0;
  video_init_counter = 1;
  gl.vic = &video_init_counter;
}

RRenderDriver3D::~RRenderDriver3D(void) 
{
  Close();
}

//***************************************

void RRenderDriver3D::Close(void) 
{
  if(glctx!=NULL) {
    SDL_GL_DeleteContext(glctx);
    glctx = NULL;
  }
  wnd.close();
}

bool RRenderDriver3D::Open(const SParameters& args) 
{
  Close();
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
    Close();
    return false;
  }
  max_values.add("xoffset", wnd.x); max_values.add("yoffset", wnd.y);
  max_values.add("width", wnd.dx); max_values.add("height", wnd.dy);
  screen2d_voffset.set(0, 0);
  screen2d_vsize.set(wnd.dx, wnd.dy);
  //
  glctx = SDL_GL_CreateContext(wnd);
  SDL_GL_MakeCurrent(wnd, glctx);
  if(!gl.load(false))
    return false;
  //
  return true;
}

bool RRenderDriver3D::NextPhm(ePhaseMode phm2)
{
  if(s_phm_goods[phm] & (1<<phm2)) {
    phm = phm2;
    return true;
  }
  if(s_phm_bads[phm] & (1<<phm2)) {
    phm = phm_Error;
    s_dbg.put(0, cl_Error, "R3D:: can't go phase from [%s] to [%s]\n", s_phm_names[phm], s_phm_names[phm2]);
    return false;
  }
  return false;
}

bool RRenderDriver3D::IsPhm(ePhaseMode phm2)
{
  if(phm==phm2)
    return true;
  s_dbg.put(0, cl_Error, "R3D:: can't use phase [%s], require [%s]\n", s_phm_names[phm], s_phm_names[phm2]);
  return false;
}

//***************************************
// END
//***************************************
