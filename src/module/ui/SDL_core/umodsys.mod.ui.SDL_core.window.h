
//***************************************
// ::
//***************************************

struct RRenderDriver2D_SDL_core : public lib2d::IRenderDriver {
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(ui,SDL_core)::RRenderDriver2D_SDL_core, 2, lib2d::IRenderDriver)
  UMODSYS_REFOBJECT_REFOTHER(RTerminal_SDL_core)
protected:
  SDL_Window *wnd;
  SDL_Renderer* rend;
  DColorAlphaf cur_color;
  DColorAlphaf clear_color;
  int cur_tm;
public:
  RRenderDriver2D_SDL_core(DOwner *own, const SParameters& args)
  : refs(own), 
    wnd(NULL), rend(NULL),
    clear_color(0,0,0,0),
    cur_color(0,0,0,0), cur_tm(tm_Opaque) {
    open(args);
  }
  ~RRenderDriver2D_SDL_core(void) {
    close();
  }
  bool validate_construction(void) { return wnd && rend; }
  bool valid(void) const { return rend!=NULL; }
  //
  void close(void) {
    if(rend!=NULL) {
      SDL_DestroyRenderer(rend);
      rend = NULL;
    }
    if(wnd!=NULL) {
      SDL_DestroyWindow(wnd);
      wnd = NULL;
    }
  }
  bool open(const SParameters& args) {
    close();
    BCStr caption = "SDL2";
    int x = 20, y = 20, dx = 640, dy = 480;
//    int flags = SDL_WINDOW_OPENGL;
    int flags = 0;
    if(&args!=NULL) {
      args.get("caption", caption);
      args.get("x", x); args.get("y", y);
      args.get("dx", dx); args.get("dy", dy);
    }
    wnd = SDL_CreateWindow(caption, x, y, dx, dy, flags);
    if(wnd==NULL) {
      close();
      return false;
    }
    rend = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
    if(rend==NULL) {
      close();
      return false;
    }
    SDL_RenderClear(rend);
    return true;
  }
  void set_color(void) {
    SDL_SetRenderDrawColor(rend, cur_color(0)*255, cur_color(1)*255, cur_color(2)*255, cur_color(3)*255);
    int bm = SDL_BLENDMODE_NONE;
    switch(cur_tm) {
      case tm_Glass:
      case tm_DarkGlass:
      case tm_Lighten:
      case tm_AlphaLighten:
      case tm_ColorLighten:
      case tm_Glow:
      case tm_SemiGlow:
      case tm_SemiGlass:
        bm = SDL_BLENDMODE_ADD;
        break;
    }
    SDL_SetRenderDrawBlendMode(rend, SDL_BlendMode(bm));
  }
public:  
  // -- UI
  libui::ITerminal* get_terminal(void) const; /* IN INLINES */
  IRefObject* get_controller(TypeId ctrl) const {
    return NULL;
  }
  //
  // -- main ones
  void begin(void) {
    if(!valid())
      return;
    SDL_SetRenderDrawColor(rend, clear_color(0)*255, clear_color(1)*255, clear_color(2)*255, clear_color(3)*255);
    SDL_RenderClear(rend);
  }
  void end(void) {
    if(!valid())
      return;
    SDL_RenderPresent(rend);
  }
  // -- information
  const SParameters* get_max_values(void) const {
    return NULL;
  }
  const SParameters* get_frame_values(void) const {
    return NULL;
  }
  bool set_parameters(BCStr mode, const SParameters& P) {
    return false;
  }
  //
  // -- setup next primitives
  bool setup_font(IMultiImage* font, const DPointf* scale) {
    if(!valid())
      return false;
    return false;
  }
  void setup_color(const DColorf& c) {
    if(!valid())
      return;
    cur_color.setp3(c.v);
    set_color();
  }
  void setup_color(const DColor& c) {
    if(!valid())
      return;
    cur_color.set3(c(0)/255.0f, c(1)/255.0f, c(2)/255.0f);
    set_color();
  }
  void setup_coloralpha(const DColorAlphaf& c, int transmode) {
    if(!valid())
      return;
    cur_color = c;
    cur_tm = transmode;
    set_color();
  }
  void setup_coloralpha(const DColorAlpha& c, int transmode) {
    if(!valid())
      return;
    cur_color.set(c(0)/255.0f, c(1)/255.0f, c(2)/255.0f, c(3)/255.0f);
    cur_tm = transmode;
    set_color();
  }
  void setup_blendcolor(const SBlendColor& c) {
    if(!valid())
      return;
    cur_color.set(c.color(0)/255.0f, c.color(1)/255.0f, c.color(2)/255.0f, c.color(3)/255.0f);
    cur_tm = c.transmode;
    set_color();
  }
  void setup_blendcolor(const SBlendColorf& c) {
    if(!valid())
      return;
    cur_color = c.color(0);
    cur_tm = c.transmode;
    set_color();
  }
  void setup_alpha(BColorElem alpha, int transmode) {
    if(!valid())
      return;
    cur_color.set4(alpha/255.0f);
    cur_tm = transmode;
    set_color();
  }
  void setup_alpha(BColorElemf alpha, int transmode) {
    if(!valid())
      return;
    cur_color.set4(alpha);
    cur_tm = transmode;
    set_color();
  }
  // -- render text
  //
  bool render_text_advanced(TextInfo &info, BCStrL piclist, int count) {
    if(!valid())
      return false;
    return false;
  }
  //
  // -- render quad/box/tri
  void render_box(const DPoint& a, const DPoint& b) {
    if(!valid())
      return;
    SDL_RenderFillRect(rend, &rect(a, b));
  }
  void render_tri(const DPoint& a, const DPoint& b, const DPoint& c) {
    if(!valid())
      return;
  }
  void render_quad(const DPoint& a, const DPoint& b, const DPoint& c, const DPoint& d) {
    if(!valid())
      return;
  }
  // -- render quad/box/tri texture
  void render_box(IImage* image, const DPoint& a, const DPoint& b, const DPointf& ta, const DPointf& tb) {
    if(!valid())
      return;
//    SDL_RenderFillRect(rend, &rect(a, b));
  }
  // -- render line/rect
  void render_line(const DPoint& a, const DPoint& b) {
    if(!valid())
      return;
    SDL_RenderDrawLine(rend, a(0), a(1), b(0), b(1));
  }
  void render_rect(const DPoint& a, const DPoint& b) {
    if(!valid())
      return;
    SDL_RenderDrawRect(rend, &rect(a, b));
  }
public:
};

//***************************************
// END
//***************************************
