
//***************************************
// RRenderDriver3D::
//***************************************

//
// -- setup next primitives

bool RRenderDriver3D::setup_font(lib2d::IMultiImage* font, const lib2d::DPointf* scale) 
{
  if(!Valid())
    return false;
  cur_font_ttf.clear();
  if(font==NULL)
    return true;
  if(font->t_ref_get_other_interface(cur_font_ttf))
    return true;
  return false;
}

void RRenderDriver3D::setup_color(const lib2d::DColorf& c) 
{
  if(!Valid())
    return;
  cur_color.setp3(c.v);
  SetColor();
}

void RRenderDriver3D::setup_color(const lib2d::DColor& c) 
{
  if(!Valid())
    return;
  cur_color.set3(c(0)/255.0f, c(1)/255.0f, c(2)/255.0f);
  SetColor();
}

void RRenderDriver3D::setup_coloralpha(const lib2d::DColorAlphaf& c, int transmode) 
{
  if(!Valid())
    return;
  cur_color = c;
  cur_tm = transmode;
  SetColor();
}

void RRenderDriver3D::setup_coloralpha(const lib2d::DColorAlpha& c, int transmode) 
{
  if(!Valid())
    return;
  cur_color.set(c(0)/255.0f, c(1)/255.0f, c(2)/255.0f, c(3)/255.0f);
  cur_tm = transmode;
  SetColor();
}

void RRenderDriver3D::setup_blendcolor(const lib2d::SBlendColor& c) 
{
  if(!Valid())
    return;
  cur_color.set(c.color(0)/255.0f, c.color(1)/255.0f, c.color(2)/255.0f, c.color(3)/255.0f);
  cur_tm = c.transmode;
  SetColor();
}

void RRenderDriver3D::setup_blendcolor(const lib2d::SBlendColorf& c) 
{
  if(!Valid())
    return;
  cur_color = c.color(0);
  cur_tm = c.transmode;
  SetColor();
}

void RRenderDriver3D::setup_alpha(lib2d::DColorElemi alpha, int transmode) 
{
  if(!Valid())
    return;
  cur_color.set4(alpha/255.0f);
  cur_tm = transmode;
  SetColor();
}

void RRenderDriver3D::setup_alpha(lib2d::DColorElemf alpha, int transmode) 
{
  if(!Valid())
    return;
  cur_color.set4(alpha);
  cur_tm = transmode;
  SetColor();
}

//***************************************
// END
//***************************************
