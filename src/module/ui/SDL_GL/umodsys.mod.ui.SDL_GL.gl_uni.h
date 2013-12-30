// 
// RSDLVideoSurfaceGL::
// 

void RSDLVideoSurfaceGL::viewport_set(int x, int y, int dx, int dy) 
{
  gl.glViewport(x, y, dx, dy);
}

void RSDLVideoSurfaceGL::viewport_depth(double znear, double zfar) 
{
  gl.glDepthRange(znear, zfar);
}

void RSDLVideoSurfaceGL::update(void) 
{
  gl.glFinish();
  gl.e();
  SDL_GL_SwapBuffers();
  gl.e();
}

void RSDLVideoSurfaceGL::clear(bool color, bool depth, bool accum, bool stencil) 
{
  int f = 0;
  if(color) f |= GL_COLOR_BUFFER_BIT;
  if(depth) f |= GL_DEPTH_BUFFER_BIT;
  if(accum) f |= GL_ACCUM_BUFFER_BIT;
  if(stencil) f |= GL_STENCIL_BUFFER_BIT;
  gl.glClear(f);
}

void RSDLVideoSurfaceGL::clear_color(const DColorAlpha& color) 
{
  gl.glClearColor(color.v[0], color.v[1], color.v[2], color.v[3]);
}

void RSDLVideoSurfaceGL::clear_accum(const DColorAlpha& color) 
{
  gl.glClearAccum(color.v[0], color.v[1], color.v[2], color.v[3]);
}

void RSDLVideoSurfaceGL::clear_depth(double depth) 
{
  gl.glClearDepth(depth);
}

void RSDLVideoSurfaceGL::clear_index(double ind) 
{
  gl.glClearIndex(ind);
}

void RSDLVideoSurfaceGL::clear_stencil(int stenc) 
{
  gl.glClearStencil(stenc);
}

//
// gl matrix operations

void RSDLVideoSurfaceGL::matrix_mode_model(void) 
{
  gl.glMatrixMode(GL_MODELVIEW);
}

void RSDLVideoSurfaceGL::matrix_mode_projection(void) 
{
  gl.glMatrixMode(GL_PROJECTION);
}

void RSDLVideoSurfaceGL::matrix_mode_texture(void) 
{
  gl.glMatrixMode(GL_TEXTURE);
}

void RSDLVideoSurfaceGL::matrix_push(void) 
{
  gl.glPushMatrix();
}

void RSDLVideoSurfaceGL::matrix_pop(void) 
{
  gl.glPopMatrix();
}

void RSDLVideoSurfaceGL::matrix_frustum(double left, double right, double bottom, double top, double znear, double zfar) 
{
  gl.glFrustum(left, right, bottom, top, znear, zfar);
}

void RSDLVideoSurfaceGL::matrix_ortho(double left, double right, double bottom, double top, double znear, double zfar) 
{
  gl.glOrtho(left, right, bottom, top, znear, zfar);
}

void RSDLVideoSurfaceGL::matrix_E(void) 
{
  gl.glLoadIdentity();
}

void RSDLVideoSurfaceGL::matrix_create(const double *mat) 
{
  gl.glLoadMatrixd(mat);
}

void RSDLVideoSurfaceGL::matrix_mult(const double *mat) 
{
  gl.glMultMatrixd(mat);
}

void RSDLVideoSurfaceGL::matrix_rot(double A, double x, double y, double z) 
{
  gl.glRotated(A, x, y, z);
}

void RSDLVideoSurfaceGL::matrix_move(double x, double y, double z) 
{
  gl.glTranslated(x, y, z);
}

void RSDLVideoSurfaceGL::matrix_scale(double x, double y, double z) 
{
  gl.glScaled(x, y, z);
}

//
// gl additional specify
void RSDLVideoSurfaceGL::set_color(const DColor& fcolor, const double& a) 
{
  gl.glColor4f(fcolor.v[0], fcolor.v[1], fcolor.v[2], a);
}
void RSDLVideoSurfaceGL::set_color(const DColor& fcolor) 
{
  gl.glColor3f(fcolor.v[0], fcolor.v[1], fcolor.v[2]);
}
void RSDLVideoSurfaceGL::set_color(const DColorAlpha& fcolora) 
{
  gl.glColor4f(fcolora.v[0], fcolora.v[1], fcolora.v[2], fcolora.v[3]);
}

/*
//
// gl source pointers
void RSDLVideoSurfaceGL::source_point(const DColor* arr, int n) {
  gl.glVertexPointer(3, GL_DOUBLE, sizeof(DColor), arr->v);
}

void RSDLVideoSurfaceGL::source_map(const SVector2d* arr, int n) {
  //gl.glTexCoordPointer(2, GL_DOUBLE, sizeof(SVector2d), arr->v);
  tex_array = arr;
}

//
// gl primitives
void RSDLVideoSurfaceGL::sdraw_triangle(int p1, int p2, int p3) {
  GLuint elems[]={p1, p2, p3};
  gl.glEnableClientState(GL_VERTEX_ARRAY);
  gl.glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, elems);
  gl.glDisableClientState(GL_VERTEX_ARRAY);
}

void RSDLVideoSurfaceGL::sdraw_triangle_m(int p1, int p2, int p3, int m1, int m2, int m3) {
  gl.glEnableClientState(GL_VERTEX_ARRAY);
  gl.glBegin(GL_TRIANGLES);
    gl.glTexCoord2dv(tex_array[m1].v);
   gl.glArrayElement(p1);
    gl.glTexCoord2dv(tex_array[m2].v);
   gl.glArrayElement(p2);
    gl.glTexCoord2dv(tex_array[m3].v);
   gl.glArrayElement(p3);
  gl.glEnd();
  gl.glDisableClientState(GL_VERTEX_ARRAY);
}

void RSDLVideoSurfaceGL::sdraw_triangle_m2(int p1, int p2, int p3, int m1, int m2, int m3, int mm1, int mm2, int mm3) {
}

void RSDLVideoSurfaceGL::sdraw_quad(int p1, int p2, int p3, int p4) {
  GLuint elems[]={p1, p2, p3, p4};
  gl.glEnableClientState(GL_VERTEX_ARRAY);
  gl.glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, elems);
  gl.glDisableClientState(GL_VERTEX_ARRAY);
}

void RSDLVideoSurfaceGL::sdraw_quad_m(int p1, int p2, int p3, int p4, int m1, int m2, int m3, int m4) {
  gl.glEnableClientState(GL_VERTEX_ARRAY);
  gl.glBegin(GL_QUADS);
    gl.glTexCoord2dv(tex_array[m1].v);
   gl.glArrayElement(p1);
    gl.glTexCoord2dv(tex_array[m2].v);
   gl.glArrayElement(p2);
    gl.glTexCoord2dv(tex_array[m3].v);
   gl.glArrayElement(p3);
    gl.glTexCoord2dv(tex_array[m4].v);
   gl.glArrayElement(p4);
  gl.glEnd();
  gl.glDisableClientState(GL_VERTEX_ARRAY);
}

void RSDLVideoSurfaceGL::sdraw_quad_m2(int p1, int p2, int p3, int p4, int m1, int m2, int m3, int m4, int mm1, int mm2, int mm3, int mm4) {
}
*/

// **************** USER

bool RSDLVideoSurfaceGL::init(void) 
{
  bool rv = gl.load();
  if(!rv) 
    return false;
  // - read extensions string
  const char* ext = reinterpret_cast<const char*>(gl.glGetString(GL_EXTENSIONS));
  const char* vendor = reinterpret_cast<const char*>(gl.glGetString(GL_VENDOR));
  const char* renderer = reinterpret_cast<const char*>(gl.glGetString(GL_RENDERER));
  extensions_mem.alloc(ext);
  extensions.split(extensions_mem.getw(), ' ');
//    gl.glEnable(GL_COLOR_TABLE);
//    bool is_paletted_texture = strstr(ext, "GL_EXT_paletted_texture")!=NULL;
  bool is_paletted_texture = false;
  for(int i=0; i<extensions.count; i++) {
    if(strcmp(extensions.list[i], "GL_EXT_paletted_texture")==0) {
      is_paletted_texture = true;
    }
  }
  //
/*
  if(!gl.is_GL_VERSION_1_3)
    return false; // need this extension
  if(!gl.is_GL_VERSION_1_2)
    return false; // need this extension
*/
  if(!gl.is_GL_EXT_vertex_array)
    return false; // need this extension
  if(!gl.is_GL_ARB_multitexture)
    return false; // need this extension
  //
  gl.max_tex_level = gl.max_GL_ARB_multitexture;
  if(gl.max_tex_level<1)
    return false;
  //
  gl.glGetIntegerv(GL_MAX_LIGHTS, &max_lights);
  gl.glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texturesize);
  //
  if(!c_texpoints.resize(gl.max_tex_level))
    return false;
  if(!tex_to_texlevel.resize(max_MaterialTextures) || !texlevel_to_tex.resize(gl.max_tex_level))
    return false;
  {
    for(int i=0; i<gl.max_tex_level; i++)
      texlevel_to_tex[i] = 0;
  }
  {
    for(int i=0; i<max_MaterialTextures; i++)
      tex_to_texlevel[i] = 0;
  }
/*
  if(!gl.is_GL_EXT_paletted_texture)
    return false; // need this extension
*/
  //
  screensize.set(surface->w, surface->h);
  screen2d_vsize = screensize;
  screen2d_voffset.set(0,0);
  screen2d_relsize.set(1,1);
  screen2d_reloffset.set(0,0);
  //
  max_swlights = 256;
  max_omnilights = 256;
  light_enabled = false;
  lightsys.configure(this, max_omnilights, max_lights, max_swlights);
  c_picture.set_texture(NULL, SRenderMapFlags(rmff_Linear, rmfc_Modulate, rmfw_Disabled));
  c_picture.components |= mc_NoLight;
  omni_color.set(1);
  //
  max_values.add("mapsize", max_texturesize);
  max_values.add("mapsize", max_texturesize);
  max_values.add("maplevels", gl.max_tex_level);
  max_values.add("hwlights", max_lights);
  max_values.add("swlights", max_swlights);
  max_values.add("omnilights", max_omnilights);
  //
  frame_values.add("frame.polygons", 0);
  frame_values.add("frame.objects", 0);
  frame_values.add("frame.time", 0.0);
  frame_values.add("average.polygons", 0.0);
  frame_values.add("average.objects", 0.0);
  frame_values.add("average.fps", 0.0);
  //
  param_update_2d();
  //
  return true;
}

void RSDLVideoSurfaceGL::stop(void) 
{
  if(surface!=NULL) {
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    surface=NULL;
  }
}

void RSDLVideoSurfaceGL::param_update_2d(void)
{
  max_values.add("maxwidth", screensize(0));
  max_values.add("maxheight", screensize(1));
  //
  max_values.add("width", screen2d_vsize(0));
  max_values.add("height", screen2d_vsize(1));
  max_values.add("xoffset", screen2d_voffset(0));
  max_values.add("yoffset", screen2d_voffset(1));
  //
  max_values.add("relwidth", screen2d_relsize(0));
  max_values.add("relheight", screen2d_relsize(1));
  max_values.add("relxoffset", screen2d_reloffset(0));
  max_values.add("relyoffset", screen2d_reloffset(1));
}


RSDLVideoSurfaceGL::RSDLVideoSurfaceGL(SDL_Surface* surf, RSDLTerminal* pv) 
: gl(&pv->video_init_counter), 
  surface(NULL), sfont(NULL), 
  mode2d(0), deep_mode(dm_None),
  max_values(M), frame_values(M),
  c_points(M), c_colors(M), c_normals(M), c_coloralphas(M),
  disabled_primitives(false), disabled_setup(false), disabled_render(false) {
  //
  gl.max_tex_level = 0;
  gl.tex_levels = 0;
  rc_init(pv);
  list_count = 0;
  list_updated = true;
  //
  fv_N_length = 3;
  //
  saved_color.set(1,1,1);
  saved_alpha = 1;
  es_color.set(1,1,1);
  es_alpha = 1;
  es_transmode = -1;
  saved_transmode = -1;
  //
  surface = surf;
}

RSDLVideoSurfaceGL::~RSDLVideoSurfaceGL(void) {
  parent->close_gl();
  //
  extensions_mem.free();
  gl.unload();
//    UMS_REFOBJECT_FREE(font_bit);
}

