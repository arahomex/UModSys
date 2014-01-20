
//***************************************
// RMultiImage2D_SDL_ttf::
//***************************************

struct RMultiImage2D_SDL_ttf : public lib2d::IMultiImage {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RMultiImage2D_SDL_ttf, 2, lib2d::IMultiImage)
  UMODSYS_REFOBJECT_REFOTHER(RRenderDriver)
public:
  typedef SMultiImage2D_SDL_ttf_tex Tex;
public:
  struct Glyph {
    Tex tex;
    int x, y, w, h, spacing;
    //
    Glyph(void) : tex(NULL), spacing(0), w(0), h(0), x(0), y(0) {}
    ~Glyph(void) { kill_texture(tex); } 
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
  inline const Glyph* get_glyph(int id) const { return glyphs(id); }
  //
  static bool new_texture(Tex& tex, SDL_Surface* s, RRenderDriver* drv);
  static void kill_texture(Tex& tex);
  static void init_texture(Tex& tex);
public:  
  lib2d::DPoint get_max_layer_size(void) const { return lib2d::DPoint(0,0); }
  lib2d::DPoint get_max_cell_size(void) const { return lib2d::DPoint(0,0); }
  lib2d::DPoint get_fixed_cell_size(void) const { return lib2d::DPoint(0,0); }
  uint16 get_layer_count(void) const { return 0; }
  lib2d::IImage* get_layer(Buint16 idx) const { return NULL; }
  uint32 get_cell_count(void) const { return 0; }
  bool get_cell(uint32 idx, lib2d::DPoint &size, lib2d::DPoint* ofs, uint16* lay) const { return false; }
  bool get_cell(uint32 idx, lib2d::SImageCellInfo& info) const { return false; }
  //
  bool set_layer_count(uint16 num) { return false; }
  bool set_hint(BCStr hint, BCStr value) { return false; }
  //
  bool setup_fixed_cell(int nx, int ny) { return false; }
  bool setup_variable_cell(const lib2d::SImageCellInfo* cells, Buint32 num, Buint32 base) { return false; }
public:
};

//***************************************
// END
//***************************************
