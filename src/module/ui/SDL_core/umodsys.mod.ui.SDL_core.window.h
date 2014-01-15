
//***************************************
// RRenderDriver2D::
//***************************************

RRenderDriver2D::RRenderDriver2D(DOwner *own)
: refs(own), 
  rend(NULL),
  clear_color(0,0,0,0),
  cur_color(0,0,0,0), cur_tm(tm_Opaque) 
{
}

RRenderDriver2D::~RRenderDriver2D(void) 
{
  close();
}

void RRenderDriver2D::close(void) 
{
  if(rend!=NULL) {
    SDL_DestroyRenderer(rend);
    rend = NULL;
  }
  wnd.close();
}

bool RRenderDriver2D::open(const SParameters& args) 
{
  close();
  if(!wnd.open(args)) {
    close();
    return false;
  }
  max_values.add("xoffset", wnd.x); max_values.add("yoffset", wnd.y);
  max_values.add("width", wnd.dx); max_values.add("height", wnd.dy);
  //
  rend = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
  if(rend==NULL) {
    close();
    return false;
  }
  SDL_RenderClear(rend);
  return true;
}

void RRenderDriver2D::set_color(void) 
{
  SDL_SetRenderDrawColor(rend, cur_color(0)*255, cur_color(1)*255, cur_color(2)*255, cur_color(3)*255);
  int bm = SDL_BLENDMODE_NONE;
  switch(cur_tm) {
    case tm_Glass:
    case tm_DarkGlass:
    case tm_Lighten:
    case tm_AlphaLighten:
    case tm_ColorLighten:
    case tm_Glow:
    case tm_SemiGlow:
    case tm_SemiGlass:
      bm = SDL_BLENDMODE_ADD;
      break;
  }
  SDL_SetRenderDrawBlendMode(rend, SDL_BlendMode(bm));
}

libui::ITerminal* RRenderDriver2D::get_terminal(void) const 
{
  return *refs;
}

IRefObject* RRenderDriver2D::get_controller(TypeId ctrl) const 
{
  return NULL;
}

lib2d::IMultiImage::P RRenderDriver2D::new_font(const SParameters& params, const DCString &tagname)
{
  lib2d::IMultiImage::P rv;
  if(!ValidateConstruction(rv, new(local_memory()) RMultiImage2D_SDL_ttf(this, params)))
    return NULL;
  return rv;
}

lib2d::IImage::P RRenderDriver2D::new_picture(const SParameters& params, const DCString &tagname)
{
  return NULL;
}

//
// -- main ones

void RRenderDriver2D::begin(void) 
{
  if(!valid())
    return;
  SDL_SetRenderDrawColor(rend, clear_color(0)*255, clear_color(1)*255, clear_color(2)*255, clear_color(3)*255);
  SDL_RenderClear(rend);
}

void RRenderDriver2D::end(void) 
{
  if(!valid())
    return;
  SDL_RenderPresent(rend);
}

// -- information

const SParameters* RRenderDriver2D::get_max_values(void) const 
{
  return &max_values;
}

const SParameters* RRenderDriver2D::get_frame_values(void) const 
{
  return &frame_values;
}

bool RRenderDriver2D::set_parameters(BCStr mode, const SParameters& P) 
{
  return false;
}

//
// -- setup next primitives

bool RRenderDriver2D::setup_font(IMultiImage* font, const DPointf* scale) 
{
  if(!valid())
    return false;
  cur_font_ttf.clear();
  if(font==NULL)
    return true;
  if(font->t_ref_get_other_interface(cur_font_ttf))
    return true;
  return false;
}

void RRenderDriver2D::setup_color(const DColorf& c) 
{
  if(!valid())
    return;
  cur_color.setp3(c.v);
  set_color();
}

void RRenderDriver2D::setup_color(const DColor& c) 
{
  if(!valid())
    return;
  cur_color.set3(c(0)/255.0f, c(1)/255.0f, c(2)/255.0f);
  set_color();
}

void RRenderDriver2D::setup_coloralpha(const DColorAlphaf& c, int transmode) 
{
  if(!valid())
    return;
  cur_color = c;
  cur_tm = transmode;
  set_color();
}

void RRenderDriver2D::setup_coloralpha(const DColorAlpha& c, int transmode) 
{
  if(!valid())
    return;
  cur_color.set(c(0)/255.0f, c(1)/255.0f, c(2)/255.0f, c(3)/255.0f);
  cur_tm = transmode;
  set_color();
}

void RRenderDriver2D::setup_blendcolor(const SBlendColor& c) 
{
  if(!valid())
    return;
  cur_color.set(c.color(0)/255.0f, c.color(1)/255.0f, c.color(2)/255.0f, c.color(3)/255.0f);
  cur_tm = c.transmode;
  set_color();
}

void RRenderDriver2D::setup_blendcolor(const SBlendColorf& c) 
{
  if(!valid())
    return;
  cur_color = c.color(0);
  cur_tm = c.transmode;
  set_color();
}

void RRenderDriver2D::setup_alpha(DColorElem alpha, int transmode) 
{
  if(!valid())
    return;
  cur_color.set4(alpha/255.0f);
  cur_tm = transmode;
  set_color();
}

void RRenderDriver2D::setup_alpha(DColorElemf alpha, int transmode) 
{
  if(!valid())
    return;
  cur_color.set4(alpha);
  cur_tm = transmode;
  set_color();
}

// -- render text
//

bool RRenderDriver2D::render_text_advanced(TextInfo &info, BCStrL piclist, int count) 
{
  if(!valid())
    return false;
  if(piclist==NULL || *piclist==0)
    return true;
  if(cur_font_ttf.valid()) {
    if(count==-1)
      count = tl::su::slen(piclist);
    bool is_caretsize = (info.options & to_CalcCaretSize)!=0;
    int x=info.a->v[0], y=info.a->v[1];
    for(int i=0; i<count; i++) {
      int pic = piclist[i];
      const RMultiImage2D_SDL_ttf::Glyph *g = cur_font_ttf->get_glyph(pic);
      if(g==NULL || g->tex==NULL)
        continue; // skip dummy glyph
      if(is_caretsize && i>=info.caret_pos) {
        info.calc_caret.set(x+g->x, y+g->y, g->w, g->h);
        is_caretsize = false;
      }
      SDL_SetTextureColorMod(g->tex, cur_color(0)*255, cur_color(1)*255, cur_color(2)*255);
      SDL_RenderCopy(rend, g->tex, NULL, sdlRect(x+g->x, y+g->y, g->w, g->h));
      x += g->spacing;
    }
    return true;
  }
  return false;
}

//
// -- render quad/box/tri

void RRenderDriver2D::render_box(const DPoint& a, const DPoint& b) 
{
  if(!valid())
    return;
  SDL_RenderFillRect(rend, sdlRect(a, b));
}

void RRenderDriver2D::render_tri(const DPoint& a, const DPoint& b, const DPoint& c) 
{
  if(!valid())
    return;
  SDL_Point abc[4] = { 
    point(a), point(b), point(c), point(a)
  };
  SDL_RenderDrawLines(rend, abc, 4);
}

void RRenderDriver2D::render_quad(const DPoint& a, const DPoint& b, const DPoint& c, const DPoint& d) 
{
  if(!valid())
    return;
  SDL_Point abcd[5] = { 
    point(a), point(b), point(c), point(d), point(a)
  };
  SDL_RenderDrawLines(rend, abcd, 5);
}

// -- render quad/box/tri texture

void RRenderDriver2D::render_box(IImage* image, const DPoint& a, const DPoint& b, const DPointf& ta, const DPointf& tb) 
{
  if(!valid())
    return;
  //image->
//    SDL_RenderCopy(rend, g->tex, NULL, &rect(x+g->x, y+g->y, g->w, g->h));
//    SDL_RenderFillRect(rend, &rect(a, b));
}

// -- render line/rect

void RRenderDriver2D::render_line(const DPoint& a, const DPoint& b) 
{
  if(!valid())
    return;
  SDL_RenderDrawLine(rend, a(0), a(1), b(0), b(1));
}

void RRenderDriver2D::render_rect(const DPoint& a, const DPoint& b) 
{
  if(!valid())
    return;
  SDL_RenderDrawRect(rend, sdlRect(a, b));
}

//***************************************
// END
//***************************************
