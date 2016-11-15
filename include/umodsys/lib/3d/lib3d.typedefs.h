#ifndef __UMODSYS_LIB3D_TYPEDEFS_H
#define __UMODSYS_LIB3D_TYPEDEFS_H 1
/*************************************************************/
// file: umodsys/lib/lib3d/lib3d.typedefs.h
// info: 3d typedefs
/*************************************************************/

#include <umodsys/lib/2d/lib2d.typedefs.h>
#include <umodsys/tl/math/math_3df.h>
#include <umodsys/tl/container/isockets.h>
#include <umodsys/common/base/obj_res.h>

namespace UModSys {
namespace lib3d {

using namespace core;

//***************************************
// FORWARD/TYPEDEF
//***************************************

// typedefs

typedef lib2d::DColorElemf                  DColorElem;
typedef lib2d::DColorElemi                  DColorElemi;
typedef lib2d::DColorElemb                  DColorElemb;
typedef lib2d::DColorAlphaf                 DColorAlpha;
typedef lib2d::DColorAlpha                  DColorAlphai;
typedef lib2d::DColorAlphab                 DColorAlphab;
typedef lib2d::DPoint                       DPoint2i;
typedef lib2d::DPointf                      DPoint2f;
typedef lib2d::SBlendColorf                 DBlendColor;
typedef lib2d::DColor                       DColori;
typedef lib2d::DColorb                      DColorb;

typedef Bfloat64                            DScalar;
typedef Bfloat32                            DScalarNormal, DScalarM, DTexScalar;
typedef Bsint32                             DInt;
typedef Buint8                              DByte;
typedef math3d::TVector2<DTexScalar>        DTexPoint;
typedef math3d::TVector3<DInt>              DPointInt;
typedef math3d::TVector3<DColorElem>        DColor;
typedef math3d::TVector3<DScalar>           DPoint, DPoint3;
typedef math3d::TVector3<DScalarNormal>     DNormal; 
typedef math3d::TVector3<DScalarM>          DPointM; 
typedef math3d::TVector4<DScalar>           DPoint4;
typedef math3d::TMatrix44<DScalar>          DMatrix4;
typedef math3d::TMatrix33<DScalar>          DMatrix3;
typedef math3d::TQuaternion4<DScalar>       DQuat;
typedef math3d::TPlane3<DScalar>            DPlane;
typedef math3d::TBoundBox3<DScalar>         DBoundBox;

// render queue
struct ISharedLibrary;                      // shared lib3d data
struct IRenderConsole;                      // console renderer
struct IRenderer;                           // renderer utility
struct IRenderDriver;                       // driver for renderer
struct ITexture;                            // single render texture
struct ITextureCells;                       // a couple of render textures (may hold in one Tex)
                                           
struct SRenderState;                        // state for rendering
struct SRenderDriverTexBox;                 // part of custom texture
struct SPicture;                            // picture info

// logic: scene                                   
struct IScene;                              // prepared scene
struct ISceneController;                    // control objects inside a scene
                                           
struct ISceneNode;                          // node inside a scene
                                           
// general: object

struct IGeneralObject;                      // general 3d object data source

struct IVisualObject;                       // object representation: visual object to display, only draw (dynamic geom, dynamic data)
  struct IVisualObject_Mesh;                // mesh object, can be animated
  struct IVisualObject_Picture;             // picture and billboard object
  struct IVisualObject_Particles;           // particle system object
  struct IVisualObject_Light;               // light
  struct IVisualObject_Brush;               // any visible object
  struct IVisualObject_Camera;              // camera object

struct IRenderObject;                       // object representation: renderer statics (textures, static geom, static data)

struct ILogicObject;                        // object representation in scene node 
  struct ILogicObject_Light;                // light
  struct ILogicObject_Brush;                // any visible object
  struct ILogicObject_Camera;               // camera object

struct IPhysicsObject;                      // object representation: physics

// visualize
struct IVisualizer;                         // visualizer general
struct IVisualizerScene;                    // visualizer for scene nodes
                                          
// physics                                
struct IPhysics;                          
struct IPhysicsNode;                      
struct IPhysicsState;                     
                                          
struct IPhysicsLogicNode;                 
struct IPhysicsLogicState;
struct IPhysicsLogics;

// geomerty
struct SObjectTriMeshData;                  // tri-mesh data united
struct SObjectITriangle;                    // indexed triange
struct SObjectParticle;                     // particle information

// material
struct SMaterial;                           // material information (general)
struct SMaterialDT;                         // dynamic texturing material

template<int size> struct TMaterialST;      // static texturing material

// lights
struct SLight;                              // common & ambient
struct SLightSphere;                        // point light
struct SLightCylinder;                      // directional
struct SLightCone;                          // spot
struct SUnionLight;                         // union of all lights

// references
typedef const ISceneNode*                   HSceneNode;
typedef tl::TRefObject<const ISceneNode>    PSceneNode;
typedef tl::TRefObject<const IPhysicsNode>  PPhysicsNode;

typedef tl::TRefObject<IGeneralObject>      PGeneralObject;
typedef tl::TRefObject<ILogicObject>        PLogicObject;
typedef tl::TRefObject<IPhysicsObject>      PPhysicsObject;
typedef tl::TRefObject<IVisualObject>       PVisualObject;
typedef tl::TRefObject<IRenderObject>       PRenderObject;

typedef tl::TRefObject<IPhysics>            PPhysics;
typedef tl::TRefObject<IScene>              PScene;
typedef tl::TRefObject<IScene, 
                       tl::TRefObjectWeakFunc<IScene> 
                      >                     PWScene;
typedef tl::TRefObject<IRenderer>           PRenderer;
typedef tl::TRefObject<ISharedLibrary>      PSharedLibrary;
typedef tl::TRefObject<IRenderDriver>       PRendererDriver;

typedef tl::TRefObject<ISceneController>    PSceneController;
typedef tl::TRefObject<IRenderConsole>      PRenderConsole;

typedef tl::TRefObject<ITexture>            PTexture;
typedef tl::TRefObject<ITextureCells>       PTextureCells;

// resources
typedef TIResource<ITexture>                IResTexture;
typedef TIResource<ITextureCells>           IResTextureCells;

//***************************************
// ENUMS
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


enum {
  max_MaterialTextures = 256
};

enum eRenderMapFlagsFilter {
  rmff_Disabled,
  rmff_Nearest,
  rmff_Linear,
  rmff_MipNearest,
  rmff_MipLinear,
  rmff_LinearMipNearest,
  rmff_LinearMipLinear,
  rmff_Unknown
};

enum eRenderMapFlagsCombine {
  rmfc_Disabled,
  rmfc_Normal,
  rmfc_Replace,
  rmfc_Modulate,
  rmfc_Blend,
  rmfc_Decal,
  rmfc_Add,
  rmfc_Enlight,
  rmfc_Complex,
  rmfc_Ghost,
  rmfc_Lightmap,
  rmfc_Unknown
};

enum eRenderMapFlagsWrap {
  rmfw_Disabled,
  rmfw_NoWrap,
  rmfw_UWrap,
  rmfw_VWrap,
  rmfw_UVWrap,
  rmfw_Edge,
  rmfw_Unknown
};

enum eObjectBrushOutline {
  obo_Null,
  obo_Point,
  obo_Line,
  obo_Plane,
  obo_Sphere,
  obo_Box,
  obo_Cylinder,
  obo_Cone,
  obo_ConvexMesh,
  obo_ConvexMultiMesh,
  obo_ConvexCloud,
  obo_ConcaveMesh,
  obo_Unknown,
  obo_User // above is user-defined outlines
};

//***************************************

enum eRenderStateComponents {
  rsc_TCamera         = 0x00000001,
  rsc_TLocal          = 0x00000002,
  rsc_TWorldCamera    = 0x00000010,
  rsc_TWorldLocal     = 0x00000020,
  rsc_TLocalCamera    = 0x00000040,
  rsc_TCameraLocal    = 0x00000080,
  rsc_OrtX            = 0x00001000,
  rsc_OrtY            = 0x00002000,
  rsc_OrtZ            = 0x00004000,
  rsc_Origin0         = 0x00008000,
  rsc_CameraPlanes    = 0x00010000,
  rsc_CameraPlanesW   = 0x00020000,
  rsc_CameraPlanesL   = 0x00040000,
  rsc_Camera          = 0x01000000,
  //
  rsc_AllLocal        = rsc_TLocal | rsc_TWorldLocal | rsc_TCameraLocal | rsc_TLocalCamera 
                      | rsc_CameraPlanesL,
  rsc_AllCamera       = rsc_TCamera | rsc_TWorldCamera | rsc_TCameraLocal | rsc_TLocalCamera 
                      | rsc_CameraPlanesL | rsc_CameraPlanesW | rsc_CameraPlanes
                      | rsc_OrtX | rsc_OrtY | rsc_OrtZ | rsc_Origin0
                      | rsc_Camera,
  //
  rsc_None            = 0x00000000
};

enum eRenderPrimitiveComponents {
  rpc_Point           = 0x00000001,
  rpc_Normal          = 0x00000002,
  rpc_Color           = 0x00000004,
  rpc_ColorAlpha      = 0x00000008,
  rpc_Texture1        = 0x00000010,
  rpc_Texture2        = 0x00000020,
  rpc_Texture3        = 0x00000040,
  rpc_Texture         = 0x00000070,
  rpc_None            = 0x00000000
};

enum eRenderPrimitiveType {
  rp_Null         = 0,
  rp_Point        = 1,
  rp_1 = rp_Point,
  rp_Line         = 2,
  rp_2 = rp_Line,
  rp_Tri          = 3,
  rp_Triangle     = rp_Tri,
  rp_3 = rp_Triangle,
  rp_Quad         = 4,
  rp_4 = rp_Quad,
  rp_Poly         = 10,
  rp_Strip        = 11, // triangle strip
  rp_Fan          = 12, // triangle fan
  rp_PolyLine     = 20, // polygon closed
  rp_MultiLine    = 21, // polygon non-closed
  rp_Unknown      = -1
};

enum eVertexClass {
  vc_None,
  vc_Coord,
  vc_Normal,
  vc_Color,
  vc_TexCoord,
  vc_Edge,
  vc_Index,
  vc__Count
};

enum eVertexAType {
  vaet_None,
  vaet_Int8,
  vaet_UInt8,
  vaet_Int16,
  vaet_UInt16,
  vaet_Int32,
  vaet_UInt32,
  vaet_Float32,
  vaet_Float64,
  vaet__Count
};

//***************************************
// INLINES
//***************************************

inline DNormal p2p(const DPoint &p) 
{ 
  return DNormal(p.v[0], p.v[1], p.v[2]); 
}

inline DPoint p2p(const DNormal &p) 
{ 
  return DPoint(p.v[0], p.v[1], p.v[2]); 
}

inline DColorAlpha operator*(const DColorAlpha& L, const DColor& R) 
{ 
    return DColorAlpha(L.v[0]*R.v[0], L.v[1]*R.v[1], L.v[2]*R.v[2], L.v[3]); 
}

inline DColorAlpha operator*(const DColorAlpha& L, const DColorAlpha& R) 
{ 
  return DColorAlpha(L.v[0]*R.v[0], L.v[1]*R.v[1], L.v[2]*R.v[2], L.v[3]*R.v[3]); 
}

//***************************************

inline bool plane_cross(DPoint& X, const DPlane& a, const DPlane& b, const DPlane& c) 
{
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
// END
//***************************************

} // namespace lib3d
} // namespace UModSys

#endif // __UMODSYS_LIB3D_TYPEDEFS_H
