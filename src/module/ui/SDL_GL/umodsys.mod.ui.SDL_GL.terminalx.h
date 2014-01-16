
//***************************************
// RTerminalX::
//***************************************

RTerminalX::RTerminalX(DOwner *own, const SParameters& args)
: RTerminal(own),
  d3d(this)
{
  s_dbg.menable();
}

RTerminalX::~RTerminalX(void) 
{
}

lib2d::IRenderDriver* RTerminalX::get_drv2d(void) const
{
  return &d3d;
}

lib3d::IRenderDriver* RTerminalX::get_drv3d(void) const
{
  return &d3d;
}

lib2d::IRenderDriver* RTerminalX::open_drv2d(const SParameters* params)
{
  return d3d.open(*params) ? &d3d : NULL;
}

lib3d::IRenderDriver* RTerminalX::open_drv3d(const SParameters* params)
{
  return d3d.open(*params) ? &d3d : NULL;
}

bool RTerminalX::close_drvxd(void)
{
  d3d.close();
  return true;
}

//***************************************
// END
//***************************************
