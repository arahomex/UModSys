
//***************************************
// ::
//***************************************

struct RMultiImage2D_SDL_ttf : public lib2d::IMultiImage {
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(ui,SDL_core)::RMultiImage2D_SDL_ttf, 2, lib2d::IMultiImage)
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
  RMultiImage2D_SDL_ttf(DOwner *own, const SParameters& args)
  : refs(own), font(NULL) {
    max_glyph = 0xffff;
    open(args);
  }
  ~RMultiImage2D_SDL_ttf(void) {
    close();
  }
  bool validate_construction(void) { return font!=NULL; }
  bool valid(void) const { return font!=NULL; }
  //
  void close(void) {
    if(font!=NULL) {
      TTF_CloseFont(font);
      font = NULL;
    }
  }
  bool open(const SParameters& args) {
    close();
    BCStr filename = NULL;
    int size = 16;
    if(&args!=NULL) {
      args.get("filename", filename);
      args.get("size", size);
      args.get("max_glyph", max_glyph);
//      args.get("x", x); args.get("y", y);
//      args.get("dx", dx); args.get("dy", dy);
    }
    if(filename==NULL)
      return false;
    font = TTF_OpenFont(filename, size);
    if(font==NULL || !generate_glyphs()) {
      close();
      return false;
    }
    return true;
  }
  bool generate_glyphs(void) {
    int x[3], y[2];
    SDL_Color bk = color(0,0,0,0);
    SDL_Color fg = color(255,255,255,255);
    for(Uint32 ch=1; ch<max_glyph; ch++) {
      if(TTF_GlyphMetrics(font, ch, x+0, x+1, y+0, y+1, x+2))
        continue;
      Glyph* value = glyphs(ch, void_null());
      if(value==NULL)
        return false;
      SDL_Surface *surf = TTF_RenderGlyph_Shaded(font, ch, fg, bk);
      if(surf==NULL)
        return false;
      value->w = surf->w;
      value->h = surf->h;
      value->x = 0;
      value->y = 0;
      value->spacing = x[2];
      value->tex = as_texture(surf);
      SDL_FreeSurface(surf);
      if(value->tex==NULL)
        return false;
    }
    return true;
  }
  TTF_Font* get_font(void) const { return font; }
  SDL_Texture* as_texture(SDL_Surface* s) const;  /* IN INLINES */
  inline const Glyph* get_glyph(int id) const { return glyphs(id); }
public:  
  IImage* get_layer(int idx) {
    return NULL;
  }
  bool set_layer_count(int count) {
    return false; 
  }
  bool set_fixed_cell(int nx, int ny) {
    return false; 
  }
  bool set_var_cell(const SImageCellInfo* cells, int count, int base) {
    return false; 
  }
  bool set_hint(BCStr hint, BCStr value) {
    return false; 
  }
  bool get_cell_size(int idx, DPoint &size, DPoint* ofs) {
    return false; 
  }
  bool get_max_cell_size(DPoint &size) { 
    return false; 
  }
public:
};

//***************************************
// END
//***************************************
