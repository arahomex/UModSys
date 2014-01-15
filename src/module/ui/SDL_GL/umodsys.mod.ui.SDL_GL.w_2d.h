
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
  return NULL;
}

lib2d::IImage::P RRenderDriver3D::new_picture(const SParameters& params, const DCString &tagname)
{
  return NULL;
}

//
// -- main ones

void RRenderDriver3D::begin(void) 
{
  if(!valid())
    return;
}

void RRenderDriver3D::end(void) 
{
  if(!valid())
    return;
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

//
// -- setup next primitives

bool RRenderDriver3D::setup_font(lib2d::IMultiImage* font, const lib2d::DPointf* scale) 
{
  if(!valid())
    return false;
  return false;
}

void RRenderDriver3D::setup_color(const lib2d::DColorf& c) 
{
  if(!valid())
    return;
  cur_color.setp3(c.v);
  set_color();
}

void RRenderDriver3D::setup_color(const lib2d::DColor& c) 
{
  if(!valid())
    return;
  cur_color.set3(c(0)/255.0f, c(1)/255.0f, c(2)/255.0f);
  set_color();
}

void RRenderDriver3D::setup_coloralpha(const lib2d::DColorAlphaf& c, int transmode) 
{
  if(!valid())
    return;
  cur_color = c;
  cur_tm = transmode;
  set_color();
}

void RRenderDriver3D::setup_coloralpha(const lib2d::DColorAlpha& c, int transmode) 
{
  if(!valid())
    return;
  cur_color.set(c(0)/255.0f, c(1)/255.0f, c(2)/255.0f, c(3)/255.0f);
  cur_tm = transmode;
  set_color();
}

void RRenderDriver3D::setup_blendcolor(const lib2d::SBlendColor& c) 
{
  if(!valid())
    return;
  cur_color.set(c.color(0)/255.0f, c.color(1)/255.0f, c.color(2)/255.0f, c.color(3)/255.0f);
  cur_tm = c.transmode;
  set_color();
}

void RRenderDriver3D::setup_blendcolor(const lib2d::SBlendColorf& c) 
{
  if(!valid())
    return;
  cur_color = c.color(0);
  cur_tm = c.transmode;
  set_color();
}

void RRenderDriver3D::setup_alpha(lib2d::DColorElem alpha, int transmode) 
{
  if(!valid())
    return;
  cur_color.set4(alpha/255.0f);
  cur_tm = transmode;
  set_color();
}

void RRenderDriver3D::setup_alpha(lib2d::DColorElemf alpha, int transmode) 
{
  if(!valid())
    return;
  cur_color.set4(alpha);
  cur_tm = transmode;
  set_color();
}

// -- render text
//

bool RRenderDriver3D::render_text_advanced(TextInfo &info, BCStrL piclist, int count) 
{
  return false;
}

//
// -- render quad/box/tri

void RRenderDriver3D::render_box(const lib2d::DPoint& a, const lib2d::DPoint& b) 
{
  if(!valid())
    return;
}

void RRenderDriver3D::render_tri(const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPoint& c) 
{
  if(!valid())
    return;
}

void RRenderDriver3D::render_quad(const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPoint& c, const lib2d::DPoint& d) 
{
  if(!valid())
    return;
}

// -- render quad/box/tri texture

void RRenderDriver3D::render_box(lib2d::IImage* image, const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPointf& ta, const lib2d::DPointf& tb) 
{
  if(!valid())
    return;
}

// -- render line/rect

void RRenderDriver3D::render_line(const lib2d::DPoint& a, const lib2d::DPoint& b) 
{
  if(!valid())
    return;
}

void RRenderDriver3D::render_rect(const lib2d::DPoint& a, const lib2d::DPoint& b) 
{
  if(!valid())
    return;
}

//***************************************
// END
//***************************************
