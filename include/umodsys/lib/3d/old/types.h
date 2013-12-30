#ifndef __UMS_LIB3D_TYPES_H
#define __UMS_LIB3D_TYPES_H 1
//*****************************************************
// lib3d/lib3d_def.h: common 3d library structures & classes
//*****************************************************

#include <ums/lib2d/lib2d.h>
#include <ums/core/objects/console_help.h>

namespace UMS {
namespace lib3d {

using namespace core;
using namespace core::media;

//***************************************

typedef double                BScalar;
typedef float                 BScalarNormal, BScalarM, BTexScalar;
typedef math3d::TVector3f     DColor;
typedef math3d::TVector3d     DPoint;
typedef math3d::TVector3f     DNormal, DPointM;
typedef math3d::TVector4d     DPoint4;
typedef math3d::TMatrix44d    DMatrix4;
typedef math3d::TMatrix33d    DMatrix3;
typedef math3d::TQuaternion4d DQuat;
typedef math3d::TPlane3d      DPlane;
typedef math3d::TBoundBox3d   DBoundBox;

typedef lib2d::BColorElemf    BColorElem;
typedef lib2d::BColorElem     BColorElemi;
typedef lib2d::DColorAlphaf   DColorAlpha;
typedef lib2d::DPoint         DPointInt, SIntPoint;
typedef lib2d::DPointf        DTexPoint;
typedef lib2d::SBlendColorf   DBlendColor;
typedef lib2d::DColor         DColori;

//***************************************


struct IObject;                     // general 3d object, invisible
  struct IObject_Brush;             // any visible object
    struct IObject_Mesh;            // mesh object, can be animated
    struct IObject_Picture;         // picture and billboard object
    struct IObject_Particles;       // particle system object
  struct IObject_Light;             // light object
    struct IObject_Light_Ambient;   // ambient light
    struct IObject_Light_Sphere;    // point light
    struct IObject_Light_Parallel;  // cylindrical (directional) light
    struct IObject_Light_Cone;      // cone (spot) light
  struct IObject_Camera;            // camera object

struct IObjectCollider;             // collider object for each brush
struct IObjectComplex;              // composite multi-object holder
struct IObjectClass;                // object creation & per-class properties

struct IObjectVisualState;          // state for object node (visual aspects)
struct IObjectRenderState;          // state for renderer driver (textures)
struct IObjectShapeState;           // state for physics & visual describing math of object

// scene
struct IScene;                  // prepared scene
struct ISceneController;        // control objects inside a scene

// physics
struct IPhysics;
struct IPhysicsNode;
struct IPhysicsState;

struct IPhysicsLogicNode;
struct IPhysicsLogicState;
struct IPhysicsLogics;

// visual
struct SVisualizerNode;
struct IVisualizerState;
struct IVisualizer;

// library
struct ISharedLibrary;          // shared lib3d data

// render driver
struct IRenderConsole;          // console renderer
struct IRenderer;               // renderer utility
struct IRenderDriver;           // driver for renderer
struct ITexture;                // single render texture
struct ITextureCells;           // a couple of render textures (may hold in one Tex)

struct SRenderState;            // state for rendering
struct SRenderDriverTexBox;     // part of custom texture
struct SPicture;                // picture info
//struct SSceneMoveInfo;        // physics info how to move
//struct SSceneMoved;           // physics moved information

// scene nodes
struct ISceneNode;              // node inside a scene

// mesh data
struct SObjectTriMeshData;    // tri-mesh data united
struct SObjectITriangle;      // indexed triange
struct SObjectParticle;       // particle information

// material
struct SMaterial;             // material information (general)
struct SMaterialDT;           // dynamic texturing material

template<int size>
struct TMaterialST;           // static texturing material

// lights
struct SLight;                // common & ambient
struct SLightSphere;          // point light
struct SLightCylinder;        // directional
struct SLightCone;            // spot
struct SUnionLight;           // union of all lights

//***************************************

typedef const void*                       HTagType;

typedef TRefObject<const ISceneNode>      PSceneNode;
typedef TRefObject<const IPhysicsNode>    PPhysicsNode;

typedef const ISceneNode*                 HSceneNode;
typedef const IPhysicsNode*               HPhysicsNode;

typedef TRefObject<IObject>               PObject;
typedef TRefObject<IObject>               PObject;
typedef TRefObject<IObjectVisualState>    PObjectVisualState;
typedef TRefObject<IObjectRenderState>    PObjectRenderState;
typedef TRefObject<IObjectShapeState>     PObjectShapeState;

typedef TRefObject<IPhysics>              PPhysics;
typedef TRefObject<IScene>                PScene;
typedef TRefObject<IRenderer>             PRenderer;
typedef TRefObject<ISharedLibrary>        PSharedLibrary;

typedef TRefObject<ISceneController>      PSceneController;
typedef TRefObject<IRenderConsole>        PRenderConsole;

//***************************************

enum eTextureFlags {
  tf_Normal       = 0x00000000,
  tf_MipMap       = 0x00000001,
  tf_NoMipMap     = 0x00000002,
  tf_Linear       = 0x00000010,
  tf_Nearest      = 0x00000020,
  tf_Unknown      = 0x00000100
};

//***************************************

enum eSceneNodePhase {
  snp_PhaseMin    = 0x00000001,
  snp_PreRender   = 0x00000010,
  snp_RenderMain  = 0x00000020,
  snp_RenderShell = 0x00000040,
  snp_RenderLines = 0x00000080,
  snp_RenderTran  = 0x00000100, // first transparrent phase
  snp_RenderAft   = 0x01000000,
  snp_RenderEff1  = 0x04000000,
  snp_RenderEff2  = 0x08000000,
  snp_RenderEff3  = 0x10000000,
  snp_RenderEff4  = 0x20000000,
  snp_RenderEff5  = 0x40000000,
  snp_PhaseMax    = 0x80000000,
  snp_RenderNone  = 0x00000000 // no render phase
};

//***************************************

enum eLightType {
  lt_Omni,        // ambient light
  lt_Sphere,      // spherical, point
  lt_Cylinder,    // parallel, directional
  lt_Cone,        // spot, cone, projector
  lt_All,         // all lights
  lt_Unknown
};

//***************************************

enum eSceneNodeKind {
  snk_Null        = 0x0000, // empty node
  snk_Camera      = 0x0100, // camera node
  snk_Light       = 0x0200, // light node
  snk_Brush       = 0x0300, // brush node
  snk_SceneObject = 0x0301, // special scene-only object (reserved)
  snk_Particles   = 0x0302, // particle node (reserved)
  snk_Mesh        = 0x0303, // mesh node (reserved)
  snk_Picture     = 0x0304, // picture node (reserved)
  snk_Object      = 0x03ff, // unknown object node (reserved)
  snk_Portal      = 0x0400, // portal node
  snk_SubScene    = 0x500,  // sub-scene
  snk_Sky         = 0x0600, // sky box
  snk_Unknown     = 0xff00  // bad node
};


//***************************************
//***************************************

inline DNormal p2p(const DPoint &p) { return DNormal(p.v[0], p.v[1], p.v[2]); }
inline DPoint p2p(const DNormal &p) { return DPoint(p.v[0], p.v[1], p.v[2]); }

inline DColorAlpha operator*(const DColorAlpha& L, const DColor& R) 
  { return DColorAlpha(L.v[0]*R.v[0], L.v[1]*R.v[1], L.v[2]*R.v[2], L.v[3]); }
inline DColorAlpha operator*(const DColorAlpha& L, const DColorAlpha& R) 
  { return DColorAlpha(L.v[0]*R.v[0], L.v[1]*R.v[1], L.v[2]*R.v[2], L.v[3]*R.v[3]); }

//***************************************

inline bool plane_cross(DPoint& X, const DPlane& a, const DPlane& b, const DPlane& c) {
  DMatrix3 A, A1;
  A.set_col(0, a.normal.v);
  A.set_col(1, b.normal.v);
  A.set_col(2, c.normal.v);
  DPoint D(a.range0, b.range0, c.range0);
  if(!A1.set_inversed(A))
    return false;
  X = A1.mult(D);
  return true;
}

//***************************************
// utilities, defined in "common/lib3d_u.cpp"

struct SShapeTriangle {
  int a, b, c;
  const DPlane* plane;
};

extern bool _util_convexcloud(const DPlane* planes, int nplanes, DPoint *vertices, int &nvertices);
extern bool _util_convexcloud(const DPlane* const* planes, int nplanes, DPoint *vertices, int &nvertices);

extern bool _util_convexshape(const DPlane* planes, int nplanes, DPoint *vertices, int &nvertices, SShapeTriangle *triangles, int &ntriangles);
extern bool _util_convexshape(const DPlane* const* planes, int nplanes, DPoint *vertices, int &nvertices, SShapeTriangle *triangles, int &ntriangles);

// end utilities, defined in "common/lib3d_u.cpp"
//***************************************

} // namespace lib3d
} // namespace UMS

#endif
