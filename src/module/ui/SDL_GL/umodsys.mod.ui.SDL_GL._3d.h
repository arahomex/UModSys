// 
// RSDLVideoSurfaceGL::
// 

// ----------------------------------------------------------------------
// -- support variables & functions
// ----------------------------------------------------------------------

static GLfloat f_color_default[4]={1,1,1,1};
static GLfloat f_color_zero[4]={0,0,0,1};
static GLfloat color_position[4]={0,0,0,1};
static GLfloat color_direction[4]={0,0,-1,0};

static DColorAlpha color_zero(f_color_zero);
static DColorAlpha color_default(f_color_default);


void RSDLVideoSurfaceGL::set_transmode(int mode) {
  if(mode<=0) {
    gl.set_blend(false);
  } else if(mode<lib2d::tm_Unknown) {
    gl.set_blend(true);
    gl.set_blend(gl.mapTransparrentMode_SRC[mode], gl.mapTransparrentMode_DST[mode]);
  }
}

void RSDLVideoSurfaceGL::set_T(const DMatrix4& T, bool getit) {
  gl.glMatrixMode(GL_MODELVIEW);
  //
  if(getit) {
    double im[16];
    gl.glGetDoublev(GL_MODELVIEW_MATRIX, im);
    double tt[16] = {
      T.v[0*4+0], T.v[0*4+1], T.v[0*4+2], 0,
      T.v[1*4+0], T.v[1*4+1], T.v[1*4+2], 0,
      T.v[2*4+0], T.v[2*4+1], T.v[2*4+2], 0,
      T.v[3*4+0], T.v[3*4+1], T.v[3*4+2], 1
    };
    gl.glMultMatrixd(tt);
    gl.glGetDoublev(GL_MODELVIEW_MATRIX, im);
    gl.e();
  } else {
    double tt[16] = {
      T.v[0*4+0], T.v[0*4+1], T.v[0*4+2], 0,
      T.v[1*4+0], T.v[1*4+1], T.v[1*4+2], 0,
      T.v[2*4+0], T.v[2*4+1], T.v[2*4+2], 0,
      T.v[3*4+0], T.v[3*4+1], T.v[3*4+2], 1
    };
    gl.glMultMatrixd(tt);
  }
}

// -- camera setup
bool RSDLVideoSurfaceGL::camera_ortho(const DPoint &center, const DPoint &size) {
  gl.glMatrixMode(GL_PROJECTION);
  gl.glLoadIdentity();
  gl.glOrtho(center.v[0]-size.v[0]/2, center.v[0]+size.v[0]/2, 
             center.v[1]-size.v[1]/2, center.v[1]+size.v[1]/2, 
             center.v[2]-size.v[2]/2, center.v[2]+size.v[2]/2);
  return gl.glGetError()!=GL_NO_ERROR;
}

bool RSDLVideoSurfaceGL::camera_frustum(const DPoint &center, const DPoint &size) {
  gl.glMatrixMode(GL_PROJECTION);
  gl.glLoadIdentity();
  gl.glFrustum(center.v[0]-size.v[0]/2, center.v[0]+size.v[0]/2, 
               center.v[1]-size.v[1]/2, center.v[1]+size.v[1]/2, 
               center.v[2], center.v[2]+size.v[2]);
//  gl.glTranslated(0, 0, -center.v[2]);
  return gl.glGetError()!=GL_NO_ERROR;
}

bool RSDLVideoSurfaceGL::camera_frustum(const DPoint &center, const DTexPoint &fov, 
                                        BScalar min, BScalar max) {
  gl.glMatrixMode(GL_PROJECTION);
  gl.glLoadIdentity();
  BScalar x, y;
  if(fov.v[0]<0.01 || fov.v[0]>math3d::pi-0.01
    || fov.v[1]<0.01 || fov.v[1]>math3d::pi-0.01) {
    return false; // bad FOV
  }
  x = tan(fov.v[0]/2)*min/2, y = tan(fov.v[1]/2)*min/2;
  gl.glFrustum(center.v[0]-x, center.v[0]+x, 
               center.v[1]-y, center.v[1]+y, 
               min, max);
//  gl.glTranslated(0, 0, -min);
  return gl.glGetError()!=GL_NO_ERROR;
}

// ----------------------------------------------------------------------
// -- allocate vector-cache
// ----------------------------------------------------------------------

DPoint* RSDLVideoSurfaceGL::alloc_points(int count) {
  return c_points.alloc(count);
}

void RSDLVideoSurfaceGL::alloc_points(const DPoint* x, int count) 
{
  c_points.alloc(x, count);
}

DTexPoint* RSDLVideoSurfaceGL::alloc_texpoints(int count, int maplevel) 
{
  if(maplevel<0 || maplevel>=gl.max_tex_level)
    return NULL;
  return c_texpoints[maplevel].alloc(count);
}

void RSDLVideoSurfaceGL::alloc_texpoints(const DTexPoint* x, int count, int maplevel) 
{
  if(maplevel<0 || maplevel>=gl.max_tex_level)
    return;
  //
  c_texpoints[maplevel].alloc(x, count);
}

DNormal* RSDLVideoSurfaceGL::alloc_normals(int count) 
{
  return c_normals.alloc(count);
}

void RSDLVideoSurfaceGL::alloc_normals(const DNormal* x, int count) 
{
  c_normals.alloc(x, count);
}

int* RSDLVideoSurfaceGL::alloc_index(int count, int level) 
{
  if(level<0)
    return NULL;
  if(level<~c_indices) {
    c_indices.resize(level+1);
  }
  return c_indices[level].alloc(count);
}

DColor* RSDLVideoSurfaceGL::alloc_colors(int count) 
{
  return c_colors.alloc(count);
}

void RSDLVideoSurfaceGL::alloc_colors(const DColor* x, int count) 
{
  c_colors.alloc(x, count);
}

DColorAlpha* RSDLVideoSurfaceGL::alloc_coloralphas(int count)
{
  return c_coloralphas.alloc(count);
}

void RSDLVideoSurfaceGL::alloc_coloralphas(const DColorAlpha* x, int count)
{
  c_coloralphas.alloc(x, count);
}


// ----------------------------------------------------------------------
// -- register
// ----------------------------------------------------------------------

ITextureCells::P RSDLVideoSurfaceGL::register_texcells(IMediaGroup* mg, const DHString& texname, const SRenderMapFlags& deff) 
{
  {
    // 1. create object
    RSDLVideoTextureCellsGL::SelfP rv = new RSDLVideoTextureCellsGL(this);
    //
    // 2. load data to it
//    rv->tex.tinfo.set_flags(deff);
    if(mg->load_object(rv(), NULL, texname, NULL, mf_relpath::Yes | mf_relname::Yes, "lib3d::ITextureCells"))
      return rv();
    if(mg->load_object(rv(), NULL, texname, NULL, mf_relpath::Yes | mf_relname::Yes, "lib2d::IMultiImage"))
      return rv();
  }
  return NULL;
}

ITexture::P RSDLVideoSurfaceGL::register_tex(IMediaGroup* mg, const DHString& name, const SRenderMapFlags& deff) 
{
  // 1. create object
  RSDLVideoTextureGL::SelfP rv = new RSDLVideoTextureGL(this);
  // 2. load data to it
  //
  rv->set_flags(deff);
  if(mg->load_object(rv(), NULL, name, NULL, mf_relpath::Yes | mf_relname::Yes, "lib3d::ITexture"))
    return rv();
  if(mg->load_object(rv(), NULL, name, NULL, mf_relpath::Yes | mf_relname::Yes, "lib2d::IImage"))
    return rv();
  //
  return NULL;
}

ITexture::P RSDLVideoSurfaceGL::register_tex(const lib2d::DPoint& size, const SRenderMapFlags& deff, lib2d::eImageType type) 
{
  RSDLVideoTextureGL::SelfP rv = new RSDLVideoTextureGL(this);
  //
  if(rv->set_reallocate(type, size(0), size(1), NULL) && rv->set_flags(deff)) 
    return rv();
  //
  return NULL;
}

// ----------------------------------------------------------------------
//
// ----------------------------------------------------------------------
