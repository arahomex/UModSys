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
  IResTexture* texture_load(const DCString& name, const DCString& objname, int flags=tf_Normal);
  IResTexture* texture_alloc(SRenderDriverTexBox* box, const DPoint2i& size, lib2d::eImageType type);
  IResTextureCells* font_load(const DCString& name, const DCString& objname, int flags=tf_Normal);
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
  bool render_reset(const SRenderState& state);
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
// RVisualizerScene
//------------------------------------

struct RVisualizerScene : public IVisualizerScene {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RVisualizerScene, 2, IVisualizerScene)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  typedef tl::TDynarrayDynamic<Scene> Scenes;
  typedef tl::TDynarrayDynamic<IVisualObject*> ObjectQueue;
  typedef tl::TRStackSocket<IVisualObject*, ObjectQueue> ObjectQueueSocket;
public: // IVisualizer
  // links
  IRenderer* renderer_get(void) const;
  bool renderer_set(IRenderer* r);
  // visualize
  bool render_3d(int phaseid, bool isalpha, IRenderDriver* drv);
  bool render_2d(IRenderDriver* drv);
public: // IVisualizerScene
  // scene
  size_t scene_count(void) const;
  Scene scene_get(size_t idx) const;
  size_t scene_index(const Scene &s) const;
  bool scene_add(const Scene &s);
  bool scene_remove(const Scene &s);
/*
public: // ISceneProcessor
  bool on_scene_node_add(IScene* sc, HSceneNode node);
  bool on_scene_node_remove(IScene* sc, HSceneNode node);
  bool on_scene_node_change(IScene* sc, HSceneNode node);
*/

public:
  IRenderer::P renderer;
  ISharedLibrary::P lib;
  Scenes scenes;
  ObjectQueue queue;
public:
  RVisualizerScene(DOwner *pv, const SParameters& args);
  ~RVisualizerScene(void);
  inline bool validate_construction(void) { return true; }
  //
  void clear_cache(void);
};

//------------------------------------
// RVisualSceneMapper_Direct
//------------------------------------

struct RVisualSceneMapper_Direct : public IVisualSceneMapper {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RVisualSceneMapper_Direct, 2, IVisualSceneMapper)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  typedef tl::TScatterArray<IVisualObject::P, HSceneNode> Cache;
public: // IVisualSceneMapper
  bool renderer_set(IRenderer* r);
  bool node_update_all(IScene* scene);
  bool queue_fill(Queue& q, int phaseid, bool isalpha);
public: // ISceneProcessor
  bool on_scene_node_add(IScene* sc, HSceneNode node);
  bool on_scene_node_remove(IScene* sc, HSceneNode node);
  bool on_scene_node_change(IScene* sc, HSceneNode node);
public:
  Cache cache;
public:
  RVisualSceneMapper_Direct(DOwner *pv, const SParameters& args);
  ~RVisualSceneMapper_Direct(void);
  inline bool validate_construction(void) { return true; }
};


//------------------------------------
// 
//------------------------------------
