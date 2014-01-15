
//***************************************
// RRenderDriver3D::
//***************************************

// -- camera setup

bool RRenderDriver3D::camera_ortho(const DPoint &center, const DPoint &size)
{
  return false;
}

bool RRenderDriver3D::camera_frustum(const DPoint &center, const DPoint &size)
{
  return false;
}

bool RRenderDriver3D::camera_frustum(const DPoint &center, const DTexPoint &fov, DScalar min, DScalar max)
{
  return false;
}

// -- allocate vector-cache
// all data will be reset after end of frame

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

//

bool RRenderDriver3D::driver_is_reset(void)
{
  return false;
}

//

void RRenderDriver3D::phase_start(int phaseid, int alpha_mode) // begin desired phase
{
}

void RRenderDriver3D::phase_2d(bool nearmode=true) // begin 2d phase (after or before all other)
{
}

void RRenderDriver3D::render_start(void) // caches is filled, begin render elements
{
}

void RRenderDriver3D::render_stop(void) // caches is not required anymore
{
}

// -- register
// return ptr, try to adapt data to required ptr
//  virtual IConsoleHFCSDisplay::P new_console_HFCSD(const SIntPoint& pos, const SIntPoint& size, const SParameters* params)

ITexture::P RRenderDriver3D::register_tex(libmedia::ILibrary* mg, const DCString& texname, const SRenderMapFlags& deff)
{
  return NULL;
}

ITextureCells::P RRenderDriver3D::register_texcells(libmedia::ILibrary * mg, const DCString& texname, const SRenderMapFlags& deff)
{
  return NULL;
}

// this one create a new box used for supply textures

ITexture::P RRenderDriver3D::register_tex(const SIntPoint& size, const SRenderMapFlags& deff, lib2d::eImageType type)
{
  return NULL;
}

// -- setup dynamic lights, light indices: <=0 =error, 0x10000+ = HW, 0x20000+ = SW, 0x30000+ = omni

bool RRenderDriver3D::setup_clearlights(eLightType type=lt_All, bool emulated=true, bool hardware=true)
{
  return false;
}

int  RRenderDriver3D::setup_addlight(eLightType type, const SLight& light, const DMatrix4& T)
{
  return -1;
}

bool RRenderDriver3D::setup_setlight(int idx, eLightType type, const SLight& light, const DMatrix4& T)
{
  return false;
}

bool RRenderDriver3D::setup_setlight(int idx, bool enabled)
{
  return false;
}

void RRenderDriver3D::setup_T(const DMatrix4& T)
{
}

bool RRenderDriver3D::setup_material(const SMaterial *mat) // NULL to disable materials
{
  return false;
}

//
// -- render primitives by lists
// enable lists, up to 4, first list always have rpc_Point, other never have it
bool RRenderDriver3D::setup_lists(int render_primitive_components, const int* list, int listid=0)
{
  return false;
}

bool RRenderDriver3D::setup_lists2(int count, int render_primitive_components, const int* list, ...)
{
  return false;
}

bool RRenderDriver3D::render_primitive(eRenderPrimitiveType primitive, int count, int starts=0, ...)
{
  return false;
}

bool RRenderDriver3D::render_primitive(eRenderPrimitiveType primitive, int count, const int* starts)
{
  return false;
}

//

bool RRenderDriver3D::setup_state(const SRenderState& S) // discarded on phase end or render start/stop
{
  return false;
}

bool RRenderDriver3D::setup_2d(const SIntPoint* vsize, const SIntPoint* voffset, const DTexPoint* relsize, const DTexPoint *reloffset) // window coordinates
{
  return false;
}

// -- render pictures

void RRenderDriver3D::render_picture(const SPicture& A)
{
}

void RRenderDriver3D::render_picture(const SPicture& A, const SPicture& B, DColorElem trans)
{
}

void RRenderDriver3D::render_particles(const SObjectParticle* op)
{
}

// -- render text

void RRenderDriver3D::render_text_3d(BStrL piclist, int count)
{
}


//
// -- conversion

bool RRenderDriver3D::map_2d_to_3d(DTexPoint &p3d, const SIntPoint &p2d)
{
  return false;
}

bool RRenderDriver3D::map_2d_to_3d(DPoint &p3d, const SIntPoint &p2d)
{
  return false;
}

bool RRenderDriver3D::map_3d_to_2d(SIntPoint &p2d, const DPoint &p3d)
{
  return false;
}

bool RRenderDriver3D::map_3d_to_2d(SIntPoint &p2d, const DTexPoint &p3d)
{
  return false;
}

//***************************************
// END
//***************************************
