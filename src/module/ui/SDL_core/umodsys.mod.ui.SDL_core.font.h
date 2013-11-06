
//***************************************
// ::
//***************************************

struct RMultiImage2D_SDL_ttf : public lib2d::IMultiImage {
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(ui,SDL_core)::RMultiImage2D_SDL_ttf, 2, lib2d::IMultiImage)
  UMODSYS_REFOBJECT_REFMODULE()
protected:
  TTF_Font* font;
public:
  RMultiImage2D_SDL_ttf(DOwner *own, const SParameters& args)
  : refs(own), font(NULL) {
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
//      args.get("x", x); args.get("y", y);
//      args.get("dx", dx); args.get("dy", dy);
    }
    if(filename==NULL)
      return false;
    font = TTF_OpenFont(filename, size);
    if(font==NULL) {
      close();
      return false;
    }
    return true;
  }
  TTF_Font* get_font(void) const { return font; }
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
