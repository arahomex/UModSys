
struct RTerminalX;
struct RRenderDriver3D;
struct RVertexArray;

enum ePhaseMode {
  phm_None,
  phm_Error,
  phm_Begin,
  phm_End,
  phm_3D,
  phm_2D,
  phm_Draw,
  phm_Unknown,
  //
  phms_None      = 1<<phm_None,
  phms_Error     = 1<<phm_Error,
  phms_Begin     = 1<<phm_Begin,
  phms_End       = 1<<phm_End,
  phms_3D        = 1<<phm_3D,
  phms_2D        = 1<<phm_2D,
  phms_Draw      = 1<<phm_Draw,
  phms_Unknown   = 1<<phm_Unknown
};

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
  int cur_tm, video_init_counter;
  TParametersA<1024> max_values;
  TParametersA<1024> frame_values;
  SDL_GLContext glctx;
  ePhaseMode phm;
  Bsint8 mode2d;
  lib3d::DPoint2i screen2d_voffset, screen2d_vsize;
  lib3d::DTexPoint screen2d_reloffset, screen2d_relsize;
  bool debug_matrix;
  //
  tl::TRefObjects<RMultiImage2D_SDL_ttf>::Weak cur_font_ttf;
public:
  static BStr s_phm_names[phm_Unknown+1];
  static int s_phm_goods[phm_Unknown];
  static int s_phm_bads[phm_Unknown];
  //
  SGLFuncsLegacy gl;
public:
  RRenderDriver3D(DOwner *own);
  ~RRenderDriver3D(void);
  //
  bool validate_construction(void) { return wnd.wnd!=NULL; }
  //
  bool Valid(void) const { return wnd.wnd!=NULL; }
  //
  void Close(void);
  bool Open(const SParameters& args);
  bool Mode_2d_begin(void);
  bool Mode_2d_end(void);
  //
  void SetColor(void);
  void Update(void);
  void SetT(const DMatrix4& T, bool isGet);
  bool SetFrustum(const DPoint &center, DScalar dx, DScalar dy, DScalar znear, DScalar zfar, bool isGet);
  //
  bool NextPhm(ePhaseMode phm2);
  bool IsPhm(ePhaseMode phm2);
  //
  void DisableTextures(void) { gl.set_stage_num(0); }
  //
  inline bool NextPhmV(ePhaseMode phm2) { return Valid() && NextPhm(phm2); }
  inline bool IsPhmV(ePhaseMode phm2) { return Valid() && IsPhm(phm2); }
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
  void setup_alpha(lib2d::DColorElemi alpha, int transmode);
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
  bool camera_reset(void); // reset all coordinates check to [-1..1]
  bool camera_ortho(const DPoint &center, const DPoint &size);
  bool camera_frustum(const DPoint &center, const DPoint &size);
  bool camera_frustum(const DPoint &center, const DTexPoint &fov, DScalar min, DScalar max);
  // -- allocate vector-cache
  // all data will be reset after end of frame
/*
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
*/
  //
  bool driver_is_reset(void);
  //
  void phase_start(int phaseid, int alpha_mode); // begin desired phase
  void phase_2d(bool nearmode); // begin 2d phase (after or before all other)
  void render_start(void); // caches is filled, begin render elements
  void render_stop(void); // caches is not required anymore
  // -- create new
  ITexture::P create_tex(void);
  ITextureCells::P create_texcells(void);
  IVertexArray::P create_array(int lcount, const SVertexElemInfo layers[], int vcount);
  IVertexArray::P create_array(int lcount, const SVertexElemInfo layers[], int vcount, const void* rawdata, size_t rawsize);
  // -- register
  // return ptr, try to adapt data to required ptr
//  virtual IConsoleHFCSDisplay::P new_console_HFCSD(const DPoint2i& pos, const DPoint2i& size, const SParameters* params);
//*  ITexture::P register_tex(libmedia::ILibrary* mg, const DCString& texname, const SRenderMapFlags& deff);
//*  ITextureCells::P register_texcells(libmedia::ILibrary * mg, const DCString& texname, const SRenderMapFlags& deff);
  // this one create a new box used for supply textures
//*  ITexture::P register_tex(const DPoint2i& size, const SRenderMapFlags& deff, lib2d::eImageType type);
  // -- setup dynamic lights, light indices: <=0 =error, 0x10000+ = HW, 0x20000+ = SW, 0x30000+ = omni
  bool setup_clearlights(eLightType type, bool emulated, bool hardware);
  int  setup_addlight(eLightType type, const SLight& light, const DMatrix4& T);
  bool setup_setlight(int idx, eLightType type, const SLight& light, const DMatrix4& T);
  bool setup_setlight(int idx, bool enabled);
  void setup_T(const DMatrix4& T);
  bool setup_material(const SMaterial *mat); // NULL to disable materials
  bool setup_array(IVertexArray *va, int targets, int layers);
  bool setup_array(IVertexArray *va, eVertexClass target, int layer);
  bool setup_array_none(void);
  //
  // -- render primitives by lists
  // enable lists, up to 4, first list always have rpc_Point, other never have it
/*
  bool setup_lists(int render_primitive_components, const int* list, int listid);
  bool setup_lists2(int count, int render_primitive_components, const int* list, ...);
  bool render_primitive(eRenderPrimitiveType primitive, int count, int starts, ...);
  bool render_primitive(eRenderPrimitiveType primitive, int count, const int* starts);
*/
  bool render_primitive(eRenderPrimitiveType primitive, int count, int start);
  //
  bool setup_state(const SRenderState& S); // discarded on phase end or render start/stop
  bool setup_2d(const DPoint2i* vsize, const DPoint2i* voffset, const DTexPoint* relsize, const DTexPoint *reloffset); // window coordinates
  // -- render pictures
  void render_picture(const SPicture& A);
  void render_picture(const SPicture& A, const SPicture& B, DColorElem trans);
  void render_particles(const SObjectParticle* op);
  // -- render text
  void render_text_3d(BStrL piclist, int count);
  //
  // -- conversion
  bool map_2d_to_3d(DTexPoint &p3d, const DPoint2i &p2d);
  bool map_2d_to_3d(DPoint &p3d, const DPoint2i &p2d);
  bool map_3d_to_2d(DPoint2i &p2d, const DPoint &p3d);
  bool map_3d_to_2d(DPoint2i &p2d, const DTexPoint &p3d);
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
// RVertexArray::
//***************************************

struct RVertexArray : public lib3d::IVertexArray {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RVertexArray, 2, lib3d::IVertexArray)
  UMODSYS_REFOBJECT_REFOTHER(RRenderDriver3D)
  //
  struct SArrayFill {
    //
    bool texcoords[gl_multitex_levels];
    bool coord, color, normal, edge, index;
    //
    inline SArrayFill(void) { clear(); }
    //
    void clear(void);
    bool check(SVertexElemInfo& x, bool iss);
    bool check_single(const SVertexElemInfo& x);
  };
  //
  struct SLayInfo {
    SVertexElemInfo vei;
    size_t start;
    GLint stride, type, num;
  };
  typedef tl::TDynarrayDynamic<SLayInfo, tl::DAllocatorTight> Layers;
public:
  static BByte vat_sizes[vaet__Count];
  static eVertexAType type_map[vc__Count][vaet__Count];
  static BByte count_class[vc__Count][2];
  //
  static GLenum gl_type(eVertexAType type);
public:
  GLuint vbo, count;
  Layers layers;
  SArrayFill fill;
  bool created, uploaded, isdynamic, isstatic;
  size_t elem_size, cache_size;
  BByte *cache;
  SGLFuncsLegacy *gl;
protected:
public:
  RVertexArray(RRenderDriver3D *pv, bool iss, bool isd=false);
  ~RVertexArray(void);
  //
  bool Free(void);
  bool Alloc(const SVertexElemInfo lays[], int lcount, int ecount, bool single=true);
  bool Alloc(void); // cache create
  bool Upload(void);
  bool Upload(const void *raw, size_t rawsize);
  bool Use(unsigned laymask=~0);
  //
  static bool StrideConvert(
    eVertexAType t1, Buint8 c1, void* p1, int s1,
    eVertexAType t2, Buint8 c2, const void* p2, int s2,
    int num
  );
public:
  bool is_valid(void) const;
  int get_array_count(void) const;
  int get_layer_count(void) const;
  SVertexElemInfo get_layer_info(int layid) const;
  //
  bool get_layer_vdata(eVertexAType type, void* buf, int start, int ecount, int lay) const;
  bool set_layer_vdata(eVertexAType type, const void* buf, int start, int ecount, int lay);
};

//***************************************
// RTextureGL::
//***************************************

struct RTextureGL : public lib3d::ITexture {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RTextureGL, 2, lib3d::ITexture)
  UMODSYS_REFOBJECT_REFOTHER(RRenderDriver3D)
  //
public: // IImage
  bool set_info(const DInfo& inf, BCStr hint);
  bool set_data(const DPatchInfo& inf, const SCMem& bin, int planeno);
  //
  const lib2d::SImageInfo& get_info(void) const;
  bool get_data(const DPatchInfo& inf, const SMem& bin, int planeno) const;
  //
  void* get_hwinfo(HUniquePointer hint);
  const void* get_hwcinfo(HUniquePointer hint) const;
public: // ITexture
  IRenderDriver* get_driver(void);
  bool set_flags(const SRenderMapFlags& flags);
  DTexPoint get_fsize(void);
public:
  RTextureGL(DOwner *own);
  ~RTextureGL(void);
  //
  inline bool validate_construction(void) { return tex.valid(); }
  //
  bool bind_tex(void);
public:
  lib2d::SImageInfo imginfo;
  STextureGL tex;
};

//***************************************
// END
//***************************************
