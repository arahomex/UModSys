
//***************************************
// RTerminalX::
//***************************************

RTerminalX::RTerminalX(DOwner *own, const SParameters& args)
: RTerminal(own),
  d2d(this)
{
}

RTerminalX::~RTerminalX(void) 
{
}

lib2d::IRenderDriver* RTerminalX::get_drv2d(void) const
{
  return &d2d;
}

lib3d::IRenderDriver* RTerminalX::get_drv3d(void) const
{
  return NULL;
}

lib2d::IRenderDriver* RTerminalX::open_drv2d(const SParameters* params)
{
  return d2d.open(*params) ? &d2d : NULL;
}

lib3d::IRenderDriver* RTerminalX::open_drv3d(const SParameters* params)
{
  return NULL;
}

bool RTerminalX::close_drvxd(void)
{
  d2d.close();
  return true;
}

//***************************************
// END
//***************************************
