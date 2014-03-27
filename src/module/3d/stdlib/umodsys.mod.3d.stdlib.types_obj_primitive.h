//------------------------------------
// object_primitive
//------------------------------------

namespace object_primitive {

enum eObjectKind {
  ok_Null,
  ok_Sphere,
  ok_Cube,
  ok_Cylinder,
  ok_Cone,
  ok_Pyramid,
};

#pragma pack(push, 1)
struct VertexPC {
  float32 x, y, z;
  uint8 r,g,b,a;
  //
  void set_xyz(float32 xx, float32 yy, float32 zz) {
    x = xx; y = yy; z = zz;
  }
  void set_rgba(uint8 rr, uint8 gg, uint8 bb, uint8 aa=255) {
    r = rr; g = gg; b = bb; a = aa;
  }
  void set(const DPoint& p, const DColor& c) {
    x = p(0); y = p(1); z = p(2);
    r = c(0)*255; g = c(1)*255; b = c(2)*255; a = c(3)*255;
  }
  //
  static SVertexElemInfo s_inf[2];
};
#pragma pack(pop)

//------------------------------------
// RGeneral
//------------------------------------

struct RGeneral : public IGeneralObject {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::object_primitive::RGeneral, 2, IGeneralObject)
  UMODSYS_REFOBJECT_REFMODULE()
public:
public: // IGeneralObject
  // state generation functions
  bool valid(void) const;
  PVisualObject visual_new(IRenderer* r, IRenderObject* ro); // generate visual dynamic data
  PRenderObject render_new(IRenderer* r); // generate visual static data
  PLogicObject logic_new(void); // generate logical (scene) data
//  PPhysicsObject physics_new(void); // generate physics data
  IGeneralObject* get_root(void); // return root render object for states
public:
  eObjectKind kind;
public:
  RGeneral(DOwner *pv, const SParameters& args);
  ~RGeneral(void);
  inline bool validate_construction(void) { return true; }
};

//------------------------------------
// RRender
//------------------------------------

struct RRender : public IRenderObject_Brush {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::object_primitive::RRender, 2, IRenderObject_Brush)
  UMODSYS_REFOBJECT_REFOTHER(RGeneral)
public: // IRenderObject
  bool valid(void) const;
  bool render_update(void); // re-initalize links
  PGeneralObject object_get(void) const; // get object
  PVisualObject visual_new(void); // generate dynamic data
public:
  IVertexArray::P vamain, vatex;
  eRenderPrimitiveType ptype;
  int nvertices;
public:
  RRender(DOwner *pv, IRenderer *rr);
  ~RRender(void);
  //
  inline RGeneral* g(void) { return *refs; }
  //
  bool render(const SRenderState& state);
  bool build_cube(void);
};

//------------------------------------
// RVisual
//------------------------------------

struct RVisual : public IVisualObject_Brush {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::object_primitive::RVisual, 2, IVisualObject_Brush)
  UMODSYS_REFOBJECT_REFOTHER(RRender)
public: // IVisualObject
  bool valid(void) const;
  PGeneralObject object_get(void) const; // get object
  PRenderObject render_get(void) const; // get object
  bool visual_update(void); // re-initalize links
  bool logic_sync(void); // update data
  bool render_state(const SRenderState& state);
public: // IVisualObject_Brush
public:
public:
  RVisual(DOwner *pv);
  ~RVisual(void);
  //
  inline RRender* r(void) { return *refs; }
  inline RGeneral* g(void) { return *r()->refs; }
};

//------------------------------------
// /object_primitive
//------------------------------------

} // namespace object_primitive

//------------------------------------
// 
//------------------------------------
