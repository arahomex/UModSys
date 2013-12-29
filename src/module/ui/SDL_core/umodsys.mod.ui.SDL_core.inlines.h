
//***************************************
// RRenderDriver2D::
//***************************************

libui::ITerminal* RRenderDriver2D::get_terminal(void) const 
{
  return *refs;
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

//***************************************
// RMultiImage2D_SDL_ttf::
//***************************************

SDL_Texture* RMultiImage2D_SDL_ttf::as_texture(SDL_Surface* s) const
{
  if(!refs->valid())
    return NULL;
  return SDL_CreateTextureFromSurface(refs->get_rend(), s);
}

//***************************************
// END
//***************************************
