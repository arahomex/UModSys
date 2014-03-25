//------------------------------------
// RRenderer::
//------------------------------------

//------------------------------------
//------------------------------------
// IPropNamedAdapter

bool RRenderer::set_value(const DCString& name, const SPropValueR& value)
{
  return false;
}

bool RRenderer::get_value(const DCString& name, const SPropValueW& value) const
{
  return false;
}

//------------------------------------
//------------------------------------
// IRenderer

//------------------------------------
// -- support functions, library

ISharedLibrary* RRenderer::library_get(void) const
{
  return lib;
}

//------------------------------------
// -- support functions, driver

IRenderDriver* RRenderer::driver_get(void) const
{
  return driver;
}

bool RRenderer::driver_set(IRenderDriver* drv, libmedia::ILibrary *mg)
{
  driver = drv;
  return true;
}

//------------------------------------
// -- support functions, texture id

ITexture* RRenderer::texture_load(const DCString& name, const DCString& objname, int flags)
{
  return NULL;
}

ITexture* RRenderer::texture_alloc(SRenderDriverTexBox* box, const DPoint2i& size, lib2d::eImageType type)
{
  return NULL;
}

ITextureCells* RRenderer::font_load(const DCString& name, const DCString& objname, int flags)
{
  return NULL;
}

//------------------------------------
// -- support functions, convert

bool RRenderer::cast_ray(IScene* scene, const DTexPoint &screen, DPoint &start, DPoint &dir) const // in relative roord
{
  return false;
}

bool RRenderer::cast_ray(IScene* scene, const DPoint2i &screen, DPoint &start, DPoint &dir) const // in driver coord
{
  return false;
}

bool RRenderer::cast_ray_center(const DPoint2i &screen, DPoint &centered) const // in screen coord
{
  return false;
}

//------------------------------------
// -- support functions, opid functionals and hints

int RRenderer::get_opid(void) const
{
  return 0;
}

IPropNamedAdapter& RRenderer::hints(void)
{
  return *this;
}

const IPropNamedAdapter& RRenderer::hints(void) const
{
  return *this;
}

//------------------------------------
// -- top-level functions

bool RRenderer::render_begin(void)
{
  if(!driver.valid())
    return false;
  driver->begin();
  return true;
}

bool RRenderer::render_3d(IVisualizer* visi, int phase, bool isalpha)
{
  if(!driver.valid())
    return false;
  driver->phase_start(phase, isalpha);
  return visi!=NULL ? visi->render_3d(phase, isalpha, driver) : false;
}

bool RRenderer::render_2d(IVisualizer* visi)
{
  if(!driver.valid())
    return false;
  driver->phase_2d();
  return visi!=NULL ? visi->render_2d(driver) : false;
}

bool RRenderer::render_extra(const SRenderState& state, int comps)
{
  if(!driver.valid())
    return false;
  return false;
}

bool RRenderer::render_end(void)
{
  if(!driver.valid())
    return false;
  driver->end();
  return true;
}

//------------------------------------
//------------------------------------

RRenderer::RRenderer(DOwner *pv, const SParameters& args)
: refs(pv) 
{
}

RRenderer::~RRenderer(void)
{
}

//------------------------------------
// 
//------------------------------------
