
//***************************************
// RRenderDriver3D::
//***************************************

libui::ITerminal* RRenderDriver3D::get_terminal(void) const 
{
  return *refs;
}

IRefObject* RRenderDriver3D::get_controller(TypeId ctrl) const 
{
  return NULL;
}

lib2d::IMultiImage::P RRenderDriver3D::new_font(const SParameters& params, const DCString &tagname)
{
  lib2d::IMultiImage::P rv;
  BCStr fn;
  if(params.get("filename", fn) && libmedia::is_extension_case(fn, ".ttf")) {
    if(!ValidateConstruction(rv, new(local_memory()) RMultiImage2D_SDL_ttf(this, params)))
      return NULL;
    return rv;
  }
  return NULL;
}

lib2d::IImage::P RRenderDriver3D::new_picture(const SParameters& params, const DCString &tagname)
{
  return NULL;
}

// -- information

const SParameters* RRenderDriver3D::get_max_values(void) const 
{
  return &max_values;
}

const SParameters* RRenderDriver3D::get_frame_values(void) const 
{
  return &frame_values;
}

bool RRenderDriver3D::set_parameters(BCStr mode, const SParameters& P) 
{
  return false;
}

//***************************************
// END
//***************************************
