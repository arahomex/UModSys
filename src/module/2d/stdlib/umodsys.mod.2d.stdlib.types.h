//------------------------------------
// RImage_Memory
//------------------------------------

struct RImage_Memory : public IImage {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RImage_Memory, 2, IImage)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  struct State {
    SMemShared bin;
    int nplanes;
    SImageInfo info;
    SImageInfo infos[4];
    size_t offsets[5];
    //
    inline State(void)
      : nplanes(0) 
    {
      offsets[0] = 0;
    }
  };
public:
  bool set_info(const DInfo& inf, BCStr hint);
  bool set_data(const DPatchInfo& cinf, const SCMem& cbin, int planeno);
  //
  const SImageInfo& get_info(void) const;
  bool get_data(const DPatchInfo& winf, const SMem& wbin, int planeno) const;
  //
  void* get_hwinfo(HUniquePointer hint);
  const void* get_hwcinfo(HUniquePointer hint) const;
public:
  State state;
public:
  RImage_Memory(DOwner *pv, const SParameters& args);
  ~RImage_Memory(void);
  inline bool validate_construction(void) { return true; }
};

//------------------------------------
// RImageFactory_Memory
//------------------------------------

struct RImageFactory_Memory : public IImageFactory {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RImageFactory_Memory, 1, IImageFactory)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  DPoint image_get_maxsize(void) const;
  IImage::P image_new(void);
  IImage::P image_new(const DInfo& inf, BCStr hint);
public:
  RImageFactory_Memory(DOwner *pv, const SParameters& args);
  ~RImageFactory_Memory(void);
  inline bool validate_construction(void) { return true; }
};

//------------------------------------
// RMultiImage
//------------------------------------

struct RMultiImage : public IMultiImage {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RMultiImage, 2, IMultiImage)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  typedef tl::TDynarrayDynamic<SImageCellInfo> Cells;
  typedef tl::TDynarrayDynamic<IImage::P> Images;
public:
  DPoint get_max_layer_size(void) const;
  DPoint get_max_cell_size(void) const;
  DPoint get_fixed_cell_size(void) const; // 0,0 for variable
  Buint16 get_layer_count(void) const;
  IImage* get_layer(Buint16 idx) const;
  Buint32 get_cell_count(void) const;
  bool get_cell(Buint32 idx, DPoint &size, DPoint* ofs, Buint16* lay) const;
  bool get_cell(Buint32 idx, SImageCellInfo& info) const;
  //
  bool set_layer_count(Buint16 num);
  bool set_hint(BCStr hint, BCStr value); // must suport "mode":{"fixed","variable"}
  //
  bool setup_fixed_cell(int nx, int ny);
  bool setup_variable_cell(const SImageCellInfo* cells, Buint32 num, Buint32 base);
public:
  bool is_fixed;
  DPoint fixed_size;
  Cells cells;
  Images layers;
  IImageFactory::P factory;
public:
  RMultiImage(DOwner *pv, const SParameters& args);
  ~RMultiImage(void);
  //
  inline bool validate_construction(void) { return factory.valid(); }
};

//------------------------------------
// 
//------------------------------------
