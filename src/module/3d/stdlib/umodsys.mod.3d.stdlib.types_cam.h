//------------------------------------
// RGeneralObject_Camera
//------------------------------------

struct RGeneralObject_Camera : public IGeneralObject {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RGeneralObject_Camera, 2, IGeneralObject)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  struct VisualState;
  struct LogicState;
public:
public: // IGeneralObject
  // state generation functions
  bool valid(void) const;
  PVisualObject visual_new(IRenderer* r, IRenderObject* ro); // generate visual dynamic data
  PRenderObject render_new(IRenderer* r); // generate visual static data
  PLogicObject logic_new(void); // generate logical (scene) data
  PPhysicsObject physics_new(void); // generate physics data
  IGeneralObject* get_root(void); // return root render object for states
public:
  DTexPoint def_fov;
  DScalar screen, fade;
public:
  RGeneralObject_Camera(DOwner *pv, const SParameters& args);
  ~RGeneralObject_Camera(void);
  inline bool validate_construction(void) { return true; }
};

//------------------------------------
// RGeneralObject_Camera::VisualState
//------------------------------------

struct RGeneralObject_Camera::VisualState : public IVisualObject_Camera {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RGeneralObject_Camera::VisualState, 2, IVisualObject_Camera)
  UMODSYS_REFOBJECT_REFOTHER(RGeneralObject_Camera)
public: // IVisualObject
  bool valid(void) const;
  PGeneralObject object_get(void) const; // get object
  PRenderObject render_get(void) const; // get object
  bool visual_update(void); // re-initalize links
  bool logic_sync(void); // update data
  bool render_state(SRenderState& state);
public: // IVisualObject_Camera
  DTexPoint camera_get_fov(void) const;
  bool camera_get_T(DMatrix4 &T) const;
  bool camera_get_xyz(DPoint* ox, DPoint* oy, DPoint* oz) const;
  int camera_get_planes(DPlane* planes) const;
  // cast ray from [-1,1],[-1,1] to world coordinates
  bool camera_cast_ray(const DTexPoint &screen, DPoint &start, DPoint &dir) const;
  bool camera_setup(IRenderDriver *driver);
public:
  DTexPoint fov;
  DPoint target, right, up;
  DScalar screen, fade;
public:
  VisualState(DOwner *pv, IRenderer *rr);
  ~VisualState(void);
};

//------------------------------------
// 
//------------------------------------
