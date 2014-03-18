
//***************************************
// RRenderDriver3D::
//***************************************

// -- allocate vector-cache
// all data will be reset after end of frame

/*
DPoint* RRenderDriver3D::alloc_points(int count)
{
  return NULL;
}

void RRenderDriver3D::alloc_points(const DPoint* x, int count)
{
}

DColor* RRenderDriver3D::alloc_colors(int count)
{
  return NULL;
}

void RRenderDriver3D::alloc_colors(const DColor* x, int count)
{
}

DColorAlpha* RRenderDriver3D::alloc_coloralphas(int count)
{
  return NULL;
}

void RRenderDriver3D::alloc_coloralphas(const DColorAlpha* x, int count)
{
}

DTexPoint* RRenderDriver3D::alloc_texpoints(int count, int maplevel)
{
  return NULL;
}

void RRenderDriver3D::alloc_texpoints(const DTexPoint* x, int count, int maplevel)
{
}

DNormal* RRenderDriver3D::alloc_normals(int count)
{
  return NULL;
}

void RRenderDriver3D::alloc_normals(const DNormal* x, int count)
{
}

int* RRenderDriver3D::alloc_index(int count, int level)
{
  return NULL;
}
*/

//***************************************

bool RRenderDriver3D::driver_is_reset(void)
{
  return false;
}

//***************************************
// -- register
// return ptr, try to adapt data to required ptr
//  virtual IConsoleHFCSDisplay::P new_console_HFCSD(const DPoint2i& pos, const DPoint2i& size, const SParameters* params)

ITexture::P RRenderDriver3D::register_tex(libmedia::ILibrary* mg, const DCString& texname, const SRenderMapFlags& deff)
{
  return NULL;
}

ITextureCells::P RRenderDriver3D::register_texcells(libmedia::ILibrary * mg, const DCString& texname, const SRenderMapFlags& deff)
{
  return NULL;
}

// this one create a new box used for supply textures

ITexture::P RRenderDriver3D::register_tex(const DPoint2i& size, const SRenderMapFlags& deff, lib2d::eImageType type)
{
  return NULL;
}


IVertexArray::P RRenderDriver3D::create_array(const SVertexElemInfo layers[], int count)
{
  return NULL;
}

//***************************************
// -- conversion

bool RRenderDriver3D::map_2d_to_3d(DTexPoint &p3d, const DPoint2i &p2d)
{
  return false;
}

bool RRenderDriver3D::map_2d_to_3d(DPoint &p3d, const DPoint2i &p2d)
{
  return false;
}

bool RRenderDriver3D::map_3d_to_2d(DPoint2i &p2d, const DPoint &p3d)
{
  return false;
}

bool RRenderDriver3D::map_3d_to_2d(DPoint2i &p2d, const DTexPoint &p3d)
{
  return false;
}

//***************************************
// END
//***************************************
