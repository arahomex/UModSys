
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
//ITexture::P RRenderDriver3D::register_tex(libmedia::ILibrary* mg, const DCString& texname, const SRenderMapFlags& deff)
//ITextureCells::P RRenderDriver3D::register_texcells(libmedia::ILibrary * mg, const DCString& texname, const SRenderMapFlags& deff)
//ITexture::P RRenderDriver3D::register_tex(const DPoint2i& size, const SRenderMapFlags& deff, lib2d::eImageType type)
// this one create a new box used for supply textures

// -- create new

ITexture::P RRenderDriver3D::create_tex(void)
{
  ITexture::P rv;
  if(!ValidateConstruction(rv, new(local_memory()) RTextureGL(this)))
    return NULL;
  return rv;
}

ITextureCells::P RRenderDriver3D::create_texcells(void)
{
  return NULL;
}


IVertexArray::P RRenderDriver3D::create_array(int lcount, const SVertexElemInfo layers[], int vcount)
{
  RVertexArray::SelfP arr = new(local_memory()) RVertexArray(this, false);
  if(!arr->Alloc(layers, lcount, vcount, true))
    return false;
  if(!arr->is_valid())
    return false;
  return arr();
}

IVertexArray::P RRenderDriver3D::create_array(int lcount, const SVertexElemInfo layers[], int vcount, const void* rawdata, size_t rawsize)
{
  RVertexArray::SelfP arr = new(local_memory()) RVertexArray(this, true);
  if(!arr->Alloc(layers, lcount, vcount, true))
    return false;
  if(!arr->Upload(rawdata, rawsize))
    return false;
  if(!arr->is_valid())
    return false;
  return arr();
}

//***************************************
// -- conversion

bool RRenderDriver3D::map_2d_to_3d(DTexPoint &p3d, const DPoint2i &p2d)
{
  lib2d::DPoint pp = p2d - screen2d_voffset, pp2;
  DTexPoint rv = screen2d_relsize;
  rv.mult(DScalar(pp(0))/screen2d_vsize(0), DScalar(pp(1))/screen2d_vsize(1));
  rv += screen2d_reloffset;
  p3d = rv*2 + DTexPoint(-1);
  return true;
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
