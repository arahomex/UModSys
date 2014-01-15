
//***************************************
// RRenderDriver3D::
//***************************************

RRenderDriver3D::RRenderDriver3D(DOwner *own)
: refs(own), 
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
  wnd.close();
}

bool RRenderDriver3D::open(const SParameters& args) 
{
  close();
  if(!wnd.open(args)) {
    close();
    return false;
  }
  max_values.add("xoffset", wnd.x); max_values.add("yoffset", wnd.y);
  max_values.add("width", wnd.dx); max_values.add("height", wnd.dy);
  //
  return true;
}

void RRenderDriver3D::set_color(void) 
{
}

//***************************************
// END
//***************************************
