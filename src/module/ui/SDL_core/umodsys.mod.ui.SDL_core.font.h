
//***************************************
// RMultiImage2D_SDL_ttf::
//***************************************

RMultiImage2D_SDL_ttf::RMultiImage2D_SDL_ttf(DOwner *own, const SParameters& args)
: refs(own), font(NULL) 
{
  max_glyph = 0xffff;
  open(args);
}

RMultiImage2D_SDL_ttf::~RMultiImage2D_SDL_ttf(void) 
{
  close();
}

void RMultiImage2D_SDL_ttf::close(void) 
{
  if(font!=NULL) {
    TTF_CloseFont(font);
    font = NULL;
  }
}

bool RMultiImage2D_SDL_ttf::open(const SParameters& args) 
{
  close();
  BCStr filename = NULL;
  int size = 16;
  if(&args!=NULL) {
    args.get("filename", filename);
    args.get("size", size);
    args.get("max_glyph", max_glyph);
//      args.get("x", x); args.get("y", y);
//      args.get("dx", dx); args.get("dy", dy);
  }
  if(filename==NULL)
    return false;
  font = TTF_OpenFont(filename, size);
  if(font==NULL || !generate_glyphs()) {
    close();
    return false;
  }
  return true;
}

SDL_Texture* RMultiImage2D_SDL_ttf::as_texture(SDL_Surface* s) const
{
  if(!refs->valid())
    return NULL;
  return SDL_CreateTextureFromSurface(refs->get_rend(), s);
}


bool RMultiImage2D_SDL_ttf::generate_glyphs(void) 
{
  int x[3], y[2];
  SDL_Color bk = color(0,0,0,0);
  SDL_Color fg = color(255,255,255,255);
  for(Uint32 ch=1; ch<max_glyph; ch++) {
    if(TTF_GlyphMetrics(font, ch, x+0, x+1, y+0, y+1, x+2))
      continue;
    Glyph* value = glyphs(ch, void_null());
    if(value==NULL)
      return false;
    SDL_Surface *surf = TTF_RenderGlyph_Shaded(font, ch, fg, bk);
    if(surf==NULL)
      return false;
    value->w = surf->w;
    value->h = surf->h;
    value->x = 0;
    value->y = 0;
    value->spacing = x[2];
    value->tex = as_texture(surf);
    SDL_FreeSurface(surf);
    if(value->tex==NULL)
      return false;
  }
  return true;
}

//***************************************
// END
//***************************************
