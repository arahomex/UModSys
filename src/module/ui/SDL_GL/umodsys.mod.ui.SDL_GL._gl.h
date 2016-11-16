
struct SSDLLightGL {
  int index;
  bool enabled;
  SUnionLight light;
  //
  inline SSDLLightGL(void) {}
};

//
// SSDLVideoTextureGL class
//
struct SSDLVideoTextureGL {
  const SGLFuncs& gl;
  SGLFuncs::TextureInfo tinfo;
  //
  SMemShared mem, memtable;
  GLenum int_format, ext_format, ext_formatsize;
  lib2d::eImageType etype;
  int elemsize;
  int width, height, palsize;
  int rwidth, rheight;
  float twidth, theight, itwidth, itheight;
  bool updated, resetsize; 
  byte mipmap;
  SRenderMapFlags inflags;
  int saved_vic;
  //
  void set_mipmap(void);
  void set_size(int w, int h);
  void set_palsize(int e);
  void set_args(GLenum i_f, GLenum e_f, GLenum e_fs, int es) {
    int_format=i_f; ext_format=e_f; ext_formatsize=e_fs; elemsize=es;
  }
  bool alloc(void);
  void zinit(int v=0);
  bool upload(void);
  bool reupload(void);
  void dorebind(bool fast);
  //
  bool set_reallocate(lib2d::eImageType type, int dx, int dy, BCStr hint);
  bool set_linear(const lib2d::SImagePatchInfo& info, const SCMem& linear);
  bool set_indexed(const lib2d::SImagePatchInfo& info, const SCMem& index, const SCMem& xtable);
  //
  inline void crebind(bool fast=true) { if(saved_vic!=*gl.vic) dorebind(fast); if(updated) { updated=false; reupload(); } }
  inline void bind(void) { crebind(false); gl.set_bound_ARB(tinfo.name); }
  void set_flags(const SRenderMapFlags& flags);
  //
  SSDLVideoTextureGL(const SGLFuncs& gl);
  ~SSDLVideoTextureGL(void);
  //
  inline static int hint_format(const DHString &h, int d) { return SGLFuncs::image_hint_format(h, d); }
  inline static int hint_format(lib2d::eImageType h, int &fmt, int &size) { return SGLFuncs::image_hint_format(h, fmt, size); }
};

inline void SSDLVideoTextureGL::set_flags(const SRenderMapFlags& flags) 
{ 
  if(flags.filter!=rmff_Unknown) {
    bool mips = (flags.filter>=rmff_MipNearest && flags.filter<=rmff_LinearMipLinear);
    if((mipmap!=0)!=mips) { if(mips) mipmap=0; else set_mipmap(); }
  }
  inflags.merge(flags); gl.set_flags(&tinfo, inflags); 
}

//
// RSDLVideoTextureGL class
//
struct RSDLVideoTextureGL : public ITexture {
  UMS_REFOBJECT_RIMPLEMENT_DEF(RSDLVideoTextureGL, ITexture, RSDLVideoSurfaceGL)
  //
  SSDLVideoTextureGL tex;
//  int width, height;
  //
  int get_gl_id(void) { return tex.tinfo.name; }
  //
  // -------------------- OVERRIDES
  bool set_reallocate(lib2d::eImageType type, int dx, int dy, BCStr hint);
  bool set_begin(void);
  bool set_linear(const lib2d::SImagePatchInfo& info, const SCMem& linear);
  bool set_indexed(const lib2d::SImagePatchInfo& info, const SCMem& index, const SCMem& xtable);
  bool set_end(void);
  bool set_flags(const SRenderMapFlags& flags);
  //
  IRenderDriver* get_driver(void);
  DTexPoint get_fsize(void);
  lib2d::DPoint get_size(void);
  void* get_hwinfo(const void* hint);
  // -------------------- /OVERRIDES
  //
  RSDLVideoTextureGL(RSDLVideoSurfaceGL* pv);
  ~RSDLVideoTextureGL(void);
};

//
// RSDLVideoTextureGL_Same class
//
struct RSDLVideoTextureCellsGL : public ITextureCells {
  UMS_REFOBJECT_RIMPLEMENT_DEF(RSDLVideoTextureCellsGL, ITextureCells, RSDLVideoSurfaceGL)
  //
  struct CellBox {
    int layer;
    DPointInt start, size;
    DTexPoint tstart, tsize;
  };
  struct CellEmpty {
    DPointInt size;
  };
  //
  bool fixed;
  int width, height, base, count;
  // - fixed
  lib2d::TCellsDef2<BTexScalar> cd;
  // - dynamic
  DTexPoint maxsize, maxsizetex, spacing;
  types::TArray1<CellBox> cboxes;
  types::TArray1<CellEmpty> cemptys;
  types::TArray1<int> cindices; // high 8 bit is type, low 24 bit is index
  // - layers
  types::TArray1<RSDLVideoTextureGL::SelfP> layers;
  //
  // -------------------- OVERRIDES IImageList
  lib2d::IImage* get_layer(int idx);
  bool set_layer_count(int count);
  bool set_fixed_cell(int nx, int ny);
  bool set_var_cell(const lib2d::SImageCellInfo* cells, int count, int base);
  bool set_hint(BCStr hint, BCStr value);
  bool get_cell_size(int idx, lib2d::DPoint &size, lib2d::DPoint* ofs);
  bool get_max_cell_size(lib2d::DPoint &size);
  //
  IRenderDriver* get_driver(void);
  bool get_cell_info(int idx, SPicture& pic, DTexPoint* start);
  // -------------------- /OVERRIDES
  //
  bool x_draw1(int x, int y, int ch);
  //
  // for emulate draw
  SSDLVideoTextureGL* get_tex(int cid) const;
  lib2d::eSubImageMode get_raw_cell(int cid, DTexPoint* size, DTexPoint *ofs) const;
  //
  struct DrawState {
    int x, y;
    BTexScalar scale[2];
    // fixed only
    BTexScalar dsize[2], dspace[2];
    SSDLVideoTextureGL* tex;
  };
  bool x_draw_init(DrawState& ds, int x, int y);
  bool x_draw_elem(DrawState& ds, int ch);
  //
  RSDLVideoTextureCellsGL(RSDLVideoSurfaceGL* pv);
  ~RSDLVideoTextureCellsGL(void);
};


#define MAXLISTS 4
//
// RSDLVideoSurfaceGL class
//
struct RSDLVideoSurfaceGL : public IRenderDriver, public emulate_light::ISystemHardware {
  UMS_REFOBJECT_RIMPLEMENT(RSDLVideoSurfaceGL, IRenderDriver, RSDLTerminal)
  //
  enum eDeepMode {
    dm_None,
    dm_Error,
    dm_Frame,
    dm_Draw,
    dm_End,
    dm_Unknown
  };
  eDeepMode deep_mode;
  //
  struct ListInfo {
    int rm;
    const int *list;
  };
  //
  struct FrameValues {
    double time;
    int npolys, nobjects, nframes;
    //
    inline FrameValues(void) { begin(); }
    //
    inline void begin(double T=-1) {
      time = T;
      npolys = nobjects = nframes = 0;
    }
    inline double len(double T) { return T-time; }
    inline void end(double T) { time = T - time; }
  };
  FrameValues fv_1, fv_N;
  TParametersA<2048> max_values, frame_values;
  double fv_N_length;
  //
  lib2d::IConvertorDriver *icd;
  SGLFuncs gl;
  SDL_Surface* surface;
  const DTexPoint* tex_array;
  DAString extensions_mem;
  TArrayStr<BChar, 1024> extensions;
  GLint max_lights, max_swlights, max_omnilights, max_textures, max_texturesize;
  emulate_light::SSystem lightsys;
  DColor omni_color;
  bool light_enabled;
  //
  // cache values
  TMaterialST<1> c_picture;
  TArrayMemCache<DPoint> c_points;
  types::TArray< TArrayMemCache<DTexPoint> > c_texpoints;
  types::TArray< TArrayMemCache<int> > c_indices;
  TArrayMemCache<DColor> c_colors;
  TArrayMemCache<DColorAlpha> c_coloralphas;
  TArrayMemCache<DNormal> c_normals;
  types::TArray< int > tex_to_texlevel, texlevel_to_tex;
//  int render_mode, render_mode_2, render_components;
  ListInfo lists[MAXLISTS];
  int list_count;
  bool list_updated;
  DColor saved_color, es_color;
  float saved_alpha, es_alpha;
  int es_transmode, saved_transmode;
  int mode2d; // 0, -1(near), 1(far)
  bool disabled_primitives, disabled_setup, disabled_render;
  const SRenderState *svstate;
  DTexPoint screen2d_reloffset, screen2d_relsize;
  lib2d::DPoint screen2d_voffset, screen2d_vsize;
  lib2d::DPoint screensize;
  DTexPoint kfont;
  RSDLVideoTextureCellsGL::SelfP sfont;
  //
  bool font_init(DFontContext& CC);
  bool font_draw(DFontContext& CC);
  //
  // parameters
  //
  //------------------------ implement emulate_light::ISystemHardware
  bool els_clear(void);
  bool els_init_info(emulate_light::SHardwareInfo* info, int id);
  bool els_set_omni(const DColor& color);
  bool els_set_omni(emulate_light::SHardwareInfo* info, const SLight& L);
  bool els_set_cone(emulate_light::SHardwareInfo* info, const SLightCone& L, const DMatrix4& T);
  bool els_set_cylinder(emulate_light::SHardwareInfo* info, const SLightCylinder& L, const DMatrix4& T);
  bool els_set_sphere(emulate_light::SHardwareInfo* info, const SLightSphere& L, const DMatrix4& T);
  //------------------------ implement IRenderDriver3D
  // -- UI
  UI::ITerminal* get_terminal(void);
  UI::IKeyboardController* get_controller_keyboard(void);
  UI::IMouseController* get_controller_mouse(void);
  // -- main ones
  void begin(void);
  void end(void);
  // -- camera setup
  bool camera_ortho(const DPoint &center, const DPoint &size);
  bool camera_frustum(const DPoint &center, const DPoint &size);
  bool camera_frustum(const DPoint &center, const DTexPoint &fov, BScalar min, BScalar max);
  // -- allocate vector-cache
  DPoint* alloc_points(int count);
  DTexPoint* alloc_texpoints(int count, int maplevel=0);
  DColor* alloc_colors(int count);
  DNormal* alloc_normals(int count);
  DColorAlpha* alloc_coloralphas(int count);
  void alloc_points(const DPoint* x, int count);
  void alloc_texpoints(const DTexPoint* x, int count, int maplevel);
  void alloc_colors(const DColor* x, int count);
  void alloc_normals(const DNormal* x, int count);
  int* alloc_index(int count, int level);
  void alloc_coloralphas(const DColorAlpha* x, int count);
  //
  bool driver_is_reset(void) { return false; }
  IConsoleHFCSDisplay::P new_console_HFCSD(const lib2d::DPoint& pos, const lib2d::DPoint& size, const SParameters* par);
  // -- render bounds
  void phase_start(int phaseid, int alpha_mode); // begin desired phase
  void phase_2d(bool nearmode); // begin 2d phase (after or before all other)
  void render_start(void); // caches is filled, begin render elements
  void render_stop(void); // caches is not required anymore
  //
  void set_transmode(int mode);
  // -- register
  ITexture::P register_tex(IMediaGroup* mg, const DHString& texname, const SRenderMapFlags& deff);
  ITextureCells::P register_texcells(IMediaGroup* mg, const DHString& texname, const SRenderMapFlags& deff);
  ITexture::P register_tex(const lib2d::DPoint& size, const SRenderMapFlags& deff, lib2d::eImageType type);
  //
  // -- information
  const SParameters* get_max_values(void) { return &max_values; }
  const SParameters* get_frame_values(void) { return &frame_values; }
  bool set_parameters(BCStr mode, const SParameters& P);
  // -- setup dynamic lights, light indices: <=0 =error, 0x10000+ = HW, 0x20000+ = SW, 0x30000+ = omni
  bool setup_clearlights(eLightType type, bool emulated, bool hardware);
  int  setup_addlight(eLightType type, const SLight& light, const DMatrix4& T);
  bool setup_setlight(int idx, eLightType type, const SLight& light, const DMatrix4& T);
  bool setup_setlight(int idx, bool enabled);
  // -- setup next primitives
  void setup_T(const DMatrix4& T);
  bool setup_material(const SMaterial *mat);
  //
  // -- render primitives by lists
  bool setup_lists(int render_primitive_components, const int* list, int listid);
  bool setup_lists2(int count, int render_primitive_components, const int* list, ...);
  bool render_primitive(eRenderPrimitiveType primitive, int count, int starts=0, ...);
  bool render_primitive(eRenderPrimitiveType primitive, int count, const int* starts);
  //
  bool setup_state(const SRenderState& S) { svstate = &S; return true; }
  bool setup_2d(const lib2d::DPoint* vsize, const lib2d::DPoint* voffset, const DTexPoint* relsize, const DTexPoint *reloffset);
  bool map_2d_to_3d(DTexPoint &p3d, const lib2d::DPoint &p2d);
  bool map_2d_to_3d(DPoint &p3d, const lib2d::DPoint &p2d);
  bool map_3d_to_2d(lib2d::DPoint &p2d, const DPoint &p3d);
  bool map_3d_to_2d(lib2d::DPoint &p2d, const DTexPoint &p3d);
  // -- render pictures
  void render_picture(const SPicture& A);
  void render_picture(const SPicture& A, const SPicture& B, float trans);
  void render_particles(const SObjectParticle* op);
  // -- render text
  void render_text_3d(BStrU piclist, int count);
  // -------------------- 2D
  // -- setup next primitives
  bool setup_font(lib2d::IMultiImage* font, const DTexPoint* scale);
  void setup_color(const DColor& c);
  void setup_coloralpha(const DColorAlpha& ca, int transmode);
  void setup_blendcolor(const DBlendColor& c) { setup_coloralpha(c, c); }
  void setup_alpha(float alpha, int transmode);
  void setup_color(const lib2d::DColor& c) { setup_color(lib2d::c2c(c)); }
  void setup_coloralpha(const lib2d::DColorAlpha& ca, int transmode) { setup_coloralpha(lib2d::c2c(ca), transmode); }
  void setup_alpha(int alpha, int transmode) { setup_alpha(lib2d::c2c(alpha), transmode); }
  void setup_blendcolor(const lib2d::SBlendColor& c) { setup_coloralpha(c, c); }
  // -- render text
  bool render_text_advanced(TextInfo &info, BStrU piclist, int count);
  // -- render quad/box/tri
  void render_box(const lib2d::DPoint& a, const lib2d::DPoint& b);
  void render_tri(const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPoint& c);
  void render_quad(const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPoint& c, const lib2d::DPoint& d);
  // -- render quad/box/tri textured
  void render_box(lib2d::IImage* image, const lib2d::DPoint& a, const lib2d::DPoint& b, const DTexPoint& ta, const DTexPoint& tb);
  // -- render line/rect
  void render_line(const lib2d::DPoint& a, const lib2d::DPoint& b);
  void render_rect(const lib2d::DPoint& a, const lib2d::DPoint& b);
  //------------------------ /implement IRenderDriver3D
  //
  void viewport_set(int x, int y, int dx, int dy);
  void viewport_depth(double znear, double zfar);
  void update(void);
  //
  void clear(bool color, bool depth, bool accum, bool stencil);
  void clear_color(const DColorAlpha& color);
  void clear_accum(const DColorAlpha& color);
  void clear_depth(double depth);
  void clear_index(double ind);
  void clear_stencil(int stenc);
  //
  // gl matrix operations
  void matrix_mode_model(void);
  void matrix_mode_projection(void);
  void matrix_mode_texture(void);
  void matrix_push(void);
  void matrix_pop(void);
  void matrix_frustum(double left, double right, double bottom, double top, double znear, double zfar);
  void matrix_ortho(double left, double right, double bottom, double top, double znear, double zfar);
  void matrix_E(void);
  void matrix_create(const double *mat);
  void matrix_mult(const double *mat);
  void matrix_rot(double A, double x, double y, double z);
  void matrix_move(double x, double y, double z);
  void matrix_scale(double x, double y, double z);
  //
  // gl additional specify
  void set_color(const DColor& fcolor, const double& a);
  void set_color(const DColor& fcolor);
  void set_color(const DColorAlpha& fcolora);
  //
  void _set_color(void) { saved_alpha=1; saved_color.set(1); gl.glColor4f(1,1,1,1); }
  //
  // gl 2d primitives
  void mode_2d_begin(void);
  void mode_2d_end(void);
  void draw2d_rect(const DTexPoint& a, const DTexPoint& b);
  void draw2d_rect_m(const DTexPoint& a, const DTexPoint& b,
                     const DTexPoint& ma, const DTexPoint& mb);
  //
  bool x_drawimage(SSDLVideoTextureGL* tex, float x, float y, float dx, float dy, float u, float v, float du, float dv);
  //
  inline void _glArrayElement(int idx) {
#if 0
    gl.glArrayElement(idx);
#else
    emul_glArrayElement(idx, lists[0].rm);
#endif
  }
  inline void _glDrawElements(int primitives, int count, int type, const int *pindex) {
#if 1
    gl.glDrawElements(primitives, count, type, pindex);
#else
    emul_glDrawElements(primitives, count, pindex, render_mode);
#endif
    _set_color();
    gl.glNormal3f(0,0,1);
  }
  //
  bool init(void);
  void stop(void) ;
  //
  void render_update(void);
  inline void ru(void) { if(list_updated) render_update(); }
  //
  inline void emul_glDrawElements(int primitives, int count, const int *pindex, int render_mode)
  {
    gl.glBegin(primitives);
    for(int i=0, m=count; i<m; i++) {
      emul_glArrayElement(pindex[i], render_mode);
    }
    gl.glEnd();
  }
  //
  inline void emul_glArrayElement(int idx, int render_mode)
  {
    if(render_mode & rpc_Texture) {
      for(int i=0; i<gl.tex_levels; i++) {
        if(gl.get_tex2d_ARB(i)) {
          gl.set_stage_ARB_t(i);
          gl.glTexCoord2fv(c_texpoints[ texlevel_to_tex[i] ].cvalues[idx].v);
        }
      }
    }
    if(render_mode & rpc_Color) {
      gl.glColor3fv(c_colors.cvalues[idx].v);
    }
    if(render_mode & rpc_Normal) {
      gl.glNormal3fv(c_normals.cvalues[idx].v);
    }
    if(render_mode & rpc_Point) {
      gl.glVertex3dv(c_points.cvalues[idx].v);
    }
  }
  //
  inline void set_tex(int idx, const lib3d::SMaterialTexture* mt, eRenderMapFlagsCombine mode=rmfc_Unknown) {
    SSDLVideoTextureGL* ti = mt->texture->tget_hwinfo<SSDLVideoTextureGL>(&gl);
    gl.set_stage_ARB(idx);
    gl.set_tex2d_ARB(true);
    ti->tinfo.holder->bind();
//    gl.set_bound_ARB(ti ? ti->name : gl_no_texture);
    if(mode==rmfc_Unknown) {
      gl.set_flags(&ti->tinfo, mt->flags);
    } else {
      gl.set_flags(&ti->tinfo, mt->flags, mode);
    }
    gl.tex_levels = idx+1;
  }
  //
  void set_T(const DMatrix4& T, bool getit);
  //
  void emul_mmap(int idx);
  void emul_coloralpha(const DColor *color, const float *alpha, const int *tm=NULL);
  void emul_coloralpha(const DColor *color, const float *alpha, int tm) { emul_coloralpha(color, alpha, &tm); }
  void emul_coloralpha_reset(void) { emul_coloralpha(&saved_color, &saved_alpha, &saved_transmode); }
  inline void emul_material(int side, int mode, const DColorAlpha &ca) {
    float v[4] = {
      ca.v[0] * saved_color.v[0],
      ca.v[1] * saved_color.v[1],
      ca.v[2] * saved_color.v[2],
      ca.v[3] * saved_alpha
    };
    gl.glMaterialfv(side, mode, v);
    if(mode==GL_AMBIENT) {
      if(v[3]!=1) {
        emul_coloralpha(NULL, &v[3], saved_transmode);
      } else {
        emul_coloralpha_reset();
      }
    }
  }
  void disable_lights(void);
  //
  void param_update_2d(void);
  //
  RSDLVideoSurfaceGL(SDL_Surface* surf, RSDLTerminal* pv);
  ~RSDLVideoSurfaceGL(void);
};

