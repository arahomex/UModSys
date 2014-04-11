
//***************************************
// RTextureGL::
//***************************************

//***************************************
// IImage

bool RTextureGL::set_info(const DInfo& inf, BCStr hint)
{
  if(!tex.set_reallocate(inf.type, inf.size(0), inf.size(1), hint))
    return false;
  imginfo = inf;
  return true;
}

bool RTextureGL::set_data(const DPatchInfo& inf, const SCMem& bin, int planeno)
{
  if((inf.type & lib2d::it_Mask_Class)==lib2d::it_Class_Linear) {
    if(planeno==0 || planeno==-1)
      return tex.set_linear(inf, bin);
    return false;
  }
/*
  if((inf.type & lib2d::it_Mask_Class)==lib2d::it_Class_Palette) {
    return tex.set_indexed(info, bin);
  }
*/
  return false;
}

const lib2d::SImageInfo& RTextureGL::get_info(void) const
{
  return imginfo;
}

bool RTextureGL::get_data(const DPatchInfo& inf, const SMem& bin, int planeno) const
{
  return false;
}

void* RTextureGL::get_hwinfo(HUniquePointer hint)
{
  if(hint==_root_get_interface_type())
    return this;
  return NULL;
}

const void* RTextureGL::get_hwcinfo(HUniquePointer hint) const
{
  return NULL;
}

//***************************************
// ITexture

IRenderDriver* RTextureGL::get_driver(void)
{
  return *refs;
}

bool RTextureGL::set_flags(const SRenderMapFlags& flags)
{
  return false;
}

DTexPoint RTextureGL::get_fsize(void)
{
  return DTexPoint(tex.twidth, tex.theight);
}

//***************************************
//***************************************

RTextureGL::RTextureGL(DOwner *own)
: refs(own), tex(&own->gl)
{
  tex.gen();
}

RTextureGL::~RTextureGL(void)
{
  tex.clear();
}

bool RTextureGL::bind_tex(void)
{
  tex.bind();
  return true;
}

//***************************************
// END
//***************************************
