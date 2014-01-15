
struct RTerminalX;
struct RRenderDriver2D;
struct RMultiImage2D_SDL_ttf;

//***************************************
// RMultiImage2D_SDL_ttf::
//***************************************

struct RMultiImage2D_SDL_ttf : public lib2d::IMultiImage {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RMultiImage2D_SDL_ttf, 2, lib2d::IMultiImage)
  UMODSYS_REFOBJECT_REFOTHER(RRenderDriver2D)
public:
  struct Glyph {
    SDL_Texture* tex;
    int x, y, w, h, spacing;
    //
    Glyph(void) : tex(NULL), spacing(0), w(0), h(0), x(0), y(0) {}
    ~Glyph(void) { if(tex) { SDL_DestroyTexture(tex); tex = NULL; } }
  };
protected:
  TTF_Font* font;
  tl::TScatterArray<Glyph> glyphs;
  int max_glyph;
public:
  RMultiImage2D_SDL_ttf(DOwner *own, const SParameters& args);
  ~RMultiImage2D_SDL_ttf(void);
  bool validate_construction(void) { return font!=NULL; }
  bool valid(void) const { return font!=NULL; }
  //
  void close(void);
  bool open(const SParameters& args);
  bool generate_glyphs(void);
  TTF_Font* get_font(void) const { return font; }
  SDL_Texture* as_texture(SDL_Surface* s) const;
  inline const Glyph* get_glyph(int id) const { return glyphs(id); }
public:  
  DPoint get_max_layer_size(void) const { return DPoint(0,0); }
  DPoint get_max_cell_size(void) const { return DPoint(0,0); }
  DPoint get_fixed_cell_size(void) const { return DPoint(0,0); }
  uint16 get_layer_count(void) const { return 0; }
  IImage* get_layer(Buint16 idx) const { return NULL; }
  uint32 get_cell_count(void) const { return 0; }
  bool get_cell(uint32 idx, DPoint &size, DPoint* ofs, uint16* lay) const { return false; }
  bool get_cell(uint32 idx, SImageCellInfo& info) const { return false; }
  //
  bool set_layer_count(uint16 num) { return false; }
  bool set_hint(BCStr hint, BCStr value) { return false; }
  //
  bool setup_fixed_cell(int nx, int ny) { return false; }
  bool setup_variable_cell(const SImageCellInfo* cells, Buint32 num, Buint32 base) { return false; }
public:
};

//***************************************
// RRenderDriver2D::
//***************************************

struct RRenderDriver2D : public lib2d::IRenderDriver {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RRenderDriver2D, 2, lib2d::IRenderDriver)
  UMODSYS_REFOBJECT_COMPOSITE(RTerminalX)
protected:
  SWindow wnd;
  SDL_Renderer* rend;
  DColorAlphaf cur_color;
  DColorAlphaf clear_color;
  int cur_tm;
  TParametersA<256> max_values;
  TParametersA<256> frame_values;
  //
  tl::TRefObjects<RMultiImage2D_SDL_ttf>::Weak cur_font_ttf;
public:
  RRenderDriver2D(DOwner *own);
  ~RRenderDriver2D(void);
  bool validate_construction(void) { return wnd && rend; }
  bool valid(void) const { return rend!=NULL; }
  //
  void close(void);
  bool open(const SParameters& args);
  void set_color(void);
  inline SDL_Renderer* get_rend(void) const { return rend; }
public:  
  // -- UI
  libui::ITerminal* get_terminal(void) const;
  IRefObject* get_controller(TypeId ctrl) const;
  IMultiImage::P new_font(const SParameters& params, const DCString &tagname);
  IImage::P new_picture(const SParameters& params, const DCString &tagname);
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
  bool setup_font(IMultiImage* font, const DPointf* scale);
  void setup_color(const DColorf& c);
  void setup_color(const DColor& c);
  void setup_coloralpha(const DColorAlphaf& c, int transmode);
  void setup_coloralpha(const DColorAlpha& c, int transmode);
  void setup_blendcolor(const SBlendColor& c);
  void setup_blendcolor(const SBlendColorf& c);
  void setup_alpha(DColorElem alpha, int transmode);
  void setup_alpha(DColorElemf alpha, int transmode);
  // -- render text
  //
  bool render_text_advanced(TextInfo &info, BCStrL piclist, int count);
  //
  // -- render quad/box/tri
  void render_box(const DPoint& a, const DPoint& b);
  void render_tri(const DPoint& a, const DPoint& b, const DPoint& c);
  void render_quad(const DPoint& a, const DPoint& b, const DPoint& c, const DPoint& d);
  // -- render quad/box/tri texture
  void render_box(IImage* image, const DPoint& a, const DPoint& b, const DPointf& ta, const DPointf& tb);
  // -- render line/rect
  void render_line(const DPoint& a, const DPoint& b);
  void render_rect(const DPoint& a, const DPoint& b);
public:
};

//***************************************
// RTerminalX::
//***************************************

struct RTerminalX : public RTerminal {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RTerminalX, 1, RTerminal)
public:  
  mutable RRenderDriver2D d2d;
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
