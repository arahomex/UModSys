// 
// RSDLVideoTextureGL::
// 
UMS_REFOBJECT_RIMPLEMENT_BODY(RSDLVideoTextureGL)

RSDLVideoTextureGL::RSDLVideoTextureGL(RSDLVideoSurfaceGL* pv) 
  : tex(pv->gl) {
  //
  rc_init(pv);
}

RSDLVideoTextureGL::~RSDLVideoTextureGL(void) {
}

IRenderDriver* RSDLVideoTextureGL::get_driver(void) { return parent; }

bool RSDLVideoTextureGL::set_reallocate(lib2d::eImageType type, int dx, int dy, BCStr hint)
{
  return tex.set_reallocate(type, dx, dy, hint);
}

bool RSDLVideoTextureGL::set_linear(const lib2d::SImagePatchInfo& info, const SCMem& linear)
{
  return tex.set_linear(info, linear);
}

bool RSDLVideoTextureGL::set_indexed(const lib2d::SImagePatchInfo& info, const SCMem& index, const SCMem& xtable)
{
  return tex.set_indexed(info, index, xtable);
}

DTexPoint RSDLVideoTextureGL::get_fsize(void)
{
  return DTexPoint(tex.twidth, tex.theight);
}

lib2d::DPoint RSDLVideoTextureGL::get_size(void)
{
  return lib2d::DPoint(tex.width, tex.height);
}

void* RSDLVideoTextureGL::get_hwinfo(const void* hint)
{
  if(hint==&parent->gl)
    return &tex;
  return NULL;
}

bool RSDLVideoTextureGL::set_flags(const SRenderMapFlags& flags)
{
  tex.set_flags(flags);
  return true;
}
