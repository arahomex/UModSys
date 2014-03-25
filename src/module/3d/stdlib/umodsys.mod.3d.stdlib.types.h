//------------------------------------
// RRenderer
//------------------------------------

struct RRenderer : public IRenderer, public IPropNamedAdapter {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RRenderer, 2, IRenderer)
  UMODSYS_REFOBJECT_REFMODULE()
public: // IPropNamedAdapter
  bool set_value(const DCString& name, const SPropValueR& value);
  bool get_value(const DCString& name, const SPropValueW& value) const;
public: // IRenderer
  // -- support functions, library
  ISharedLibrary* library_get(void) const;
  // -- support functions, driver
  IRenderDriver* driver_get(void) const;
  bool driver_set(IRenderDriver* driver, libmedia::ILibrary *mg);
  // -- support functions, texture id
  ITexture* texture_load(const DCString& name, const DCString& objname, int flags=tf_Normal);
  ITexture* texture_alloc(SRenderDriverTexBox* box, const DPoint2i& size, lib2d::eImageType type);
  ITextureCells* font_load(const DCString& name, const DCString& objname, int flags=tf_Normal);
  // -- support functions, convert
  bool cast_ray(IScene* scene, const DTexPoint &screen, DPoint &start, DPoint &dir) const; // in relative roord
  bool cast_ray(IScene* scene, const DPoint2i &screen, DPoint &start, DPoint &dir) const; // in driver coord
  bool cast_ray_center(const DPoint2i &screen, DPoint &centered) const; // in screen coord
  // -- support functions, opid functionals and hints
  int get_opid(void) const;
  IPropNamedAdapter& hints(void);
  const IPropNamedAdapter& hints(void) const;
  //
  // -- top-level functions
  bool render_begin(void);
  bool render_3d(IVisualizer* visi, int phase, bool isalpha);
  bool render_2d(IVisualizer* visi);
  bool render_extra(const SRenderState& state, int comps);
  bool render_end(void);
public:
  IRenderDriver::P driver;
  ISharedLibrary::P lib;
public:
  RRenderer(DOwner *pv, const SParameters& args);
  ~RRenderer(void);
  inline bool validate_construction(void) { return true; }
};

//------------------------------------
// 
//------------------------------------

//------------------------------------
// 
//------------------------------------
