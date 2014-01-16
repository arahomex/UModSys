
struct RTerminalX;
struct RRenderDriver3D;

//***************************************
// RRenderDriver3D::
//***************************************

struct RRenderDriver3D : public lib3d::IRenderDriver {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RRenderDriver3D, 2, lib3d::IRenderDriver)
  UMODSYS_REFOBJECT_COMPOSITE(RTerminalX)
protected:
  SWindow wnd;
  lib2d::DColorAlphaf cur_color;
  lib2d::DColorAlphaf clear_color;
  int cur_tm;
  TParametersA<1024> max_values;
  TParametersA<1024> frame_values;
  SDL_GLContext glctx;
  SGLFunc gl;
public:
  RRenderDriver3D(DOwner *own);
  ~RRenderDriver3D(void);
  //
  bool validate_construction(void) { return wnd.wnd!=NULL; }
  bool valid(void) const { return wnd.wnd!=NULL; }
  //
  void close(void);
  bool open(const SParameters& args);
  void set_color(void);
public: // lib2d::IRenderDriver
  // -- UI
  libui::ITerminal* get_terminal(void) const;
  IRefObject* get_controller(TypeId ctrl) const;
  lib2d::IMultiImage::P new_font(const SParameters& params, const DCString &tagname);
  lib2d::IImage::P new_picture(const SParameters& params, const DCString &tagname);
  //
  // -- main ones
  void begin(void);
  void end(void);
  // -- information
  const SParameters* get_max_values(void) const;
  const SParameters* get_frame_values(void) const;
  bool set_parameters(BCStr mode, const SParameters& P);
  //
  // -- setup next primitives
  bool setup_font(lib2d::IMultiImage* font, const lib2d::DPointf* scale);
  void setup_color(const lib2d::DColorf& c);
  void setup_color(const lib2d::DColor& c);
  void setup_coloralpha(const lib2d::DColorAlphaf& c, int transmode);
  void setup_coloralpha(const lib2d::DColorAlpha& c, int transmode);
  void setup_blendcolor(const lib2d::SBlendColor& c);
  void setup_blendcolor(const lib2d::SBlendColorf& c);
  void setup_alpha(lib2d::DColorElem alpha, int transmode);
  void setup_alpha(lib2d::DColorElemf alpha, int transmode);
  // -- render text
  //
  bool render_text_advanced(TextInfo &info, BCStrL piclist, int count);
  //
  // -- render quad/box/tri
  void render_box(const lib2d::DPoint& a, const lib2d::DPoint& b);
  void render_tri(const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPoint& c);
  void render_quad(const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPoint& c, const lib2d::DPoint& d);
  // -- render quad/box/tri texture
  void render_box(lib2d::IImage* image, const lib2d::DPoint& a, const lib2d::DPoint& b, const lib2d::DPointf& ta, const lib2d::DPointf& tb);
  // -- render line/rect
  void render_line(const lib2d::DPoint& a, const lib2d::DPoint& b);
  void render_rect(const lib2d::DPoint& a, const lib2d::DPoint& b);
public: // lib3d::IRenderDriver
  // -- camera setup
  bool camera_ortho(const DPoint &center, const DPoint &size);
  bool camera_frustum(const DPoint &center, const DPoint &size);
  bool camera_frustum(const DPoint &center, const DTexPoint &fov, DScalar min, DScalar max);
  // -- allocate vector-cache
  // all data will be reset after end of frame
  DPoint* alloc_points(int count);
  void alloc_points(const DPoint* x, int count);
  DColor* alloc_colors(int count);
  void alloc_colors(const DColor* x, int count);
  DColorAlpha* alloc_coloralphas(int count);
  void alloc_coloralphas(const DColorAlpha* x, int count);
  DTexPoint* alloc_texpoints(int count, int maplevel=0);
  void alloc_texpoints(const DTexPoint* x, int count, int maplevel=0);
  DNormal* alloc_normals(int count);
  void alloc_normals(const DNormal* x, int count);
  int* alloc_index(int count, int level=0);
  //
  bool driver_is_reset(void);
  //
  void phase_start(int phaseid, int alpha_mode); // begin desired phase
  void phase_2d(bool nearmode); // begin 2d phase (after or before all other)
  void render_start(void); // caches is filled, begin render elements
  void render_stop(void); // caches is not required anymore
  // -- register
  // return ptr, try to adapt data to required ptr
//  virtual IConsoleHFCSDisplay::P new_console_HFCSD(const SIntPoint& pos, const SIntPoint& size, const SParameters* params);
  ITexture::P register_tex(libmedia::ILibrary* mg, const DCString& texname, const SRenderMapFlags& deff);
  ITextureCells::P register_texcells(libmedia::ILibrary * mg, const DCString& texname, const SRenderMapFlags& deff);
  // this one create a new box used for supply textures
  ITexture::P register_tex(const SIntPoint& size, const SRenderMapFlags& deff, lib2d::eImageType type);
  // -- setup dynamic lights, light indices: <=0 =error, 0x10000+ = HW, 0x20000+ = SW, 0x30000+ = omni
  bool setup_clearlights(eLightType type, bool emulated, bool hardware);
  int  setup_addlight(eLightType type, const SLight& light, const DMatrix4& T);
  bool setup_setlight(int idx, eLightType type, const SLight& light, const DMatrix4& T);
  bool setup_setlight(int idx, bool enabled);
  void setup_T(const DMatrix4& T);
  bool setup_material(const SMaterial *mat); // NULL to disable materials
  //
  // -- render primitives by lists
  // enable lists, up to 4, first list always have rpc_Point, other never have it
  bool setup_lists(int render_primitive_components, const int* list, int listid);
  bool setup_lists2(int count, int render_primitive_components, const int* list, ...);
  bool render_primitive(eRenderPrimitiveType primitive, int count, int starts, ...);
  bool render_primitive(eRenderPrimitiveType primitive, int count, const int* starts);
  //
  bool setup_state(const SRenderState& S); // discarded on phase end or render start/stop
  bool setup_2d(const SIntPoint* vsize, const SIntPoint* voffset, const DTexPoint* relsize, const DTexPoint *reloffset); // window coordinates
  // -- render pictures
  void render_picture(const SPicture& A);
  void render_picture(const SPicture& A, const SPicture& B, DColorElem trans);
  void render_particles(const SObjectParticle* op);
  // -- render text
  void render_text_3d(BStrL piclist, int count);
  //
  // -- conversion
  bool map_2d_to_3d(DTexPoint &p3d, const SIntPoint &p2d);
  bool map_2d_to_3d(DPoint &p3d, const SIntPoint &p2d);
  bool map_3d_to_2d(SIntPoint &p2d, const DPoint &p3d);
  bool map_3d_to_2d(SIntPoint &p2d, const DTexPoint &p3d);
public:
};

//***************************************
// RTerminalX::
//***************************************

struct RTerminalX : public RTerminal {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RTerminalX, 1, RTerminal)
public:  
  mutable RRenderDriver3D d3d;
  //
  int states;
public:  
  RTerminalX(DOwner *own, const SParameters& args);
  ~RTerminalX(void);
  //
  lib2d::IRenderDriver* get_drv2d(void) const;
  lib3d::IRenderDriver* get_drv3d(void) const;
  lib2d::IRenderDriver* open_drv2d(const SParameters* params);
  lib3d::IRenderDriver* open_drv3d(const SParameters* params);
  bool close_drvxd(void);
  //
  inline bool validate_construction(void) { return true; }
public:  
};

//***************************************
// END
//***************************************
