//------------------------------------
// RGeneralObject_Primitive
//------------------------------------

struct RGeneralObject_Primitive : public IGeneralObject {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RGeneralObject_Primitive, 2, IGeneralObject)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  struct VisualState;
  struct RenderState;
  struct LogicState;
  struct PhysicsState;
  //
  enum eObjectType {
    ot_Null,
    ot_Sphere,
    ot_Cube,
    ot_TexCube,
    ot_Cylinder,
    ot_Cone,
    ot_Pyramid,
  };
public: // IGeneralObject
  // state generation functions
  bool valid(void) const;
  PVisualObject visual_new(IRenderer* r, IRenderObject* ro); // generate visual dynamic data
  PRenderObject render_new(IRenderer* r); // generate visual static data
  PLogicObject logic_new(void); // generate logical (scene) data
  PPhysicsObject physics_new(void); // generate physics data
  IGeneralObject* get_root(void); // return root render object for states
public:
  RGeneralObject_Primitive(DOwner *pv, const SParameters& args);
  ~RGeneralObject_Primitive(void);
  inline bool validate_construction(void) { return true; }
};

//------------------------------------
// 
//------------------------------------
