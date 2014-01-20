
struct RTerminalX;
struct RRenderDriver2D;

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
