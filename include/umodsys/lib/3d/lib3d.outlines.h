#ifndef __UMODSYS_LIB3D_OUTLINES_H
#define __UMODSYS_LIB3D_OUTLINES_H 1
/*************************************************************/
// file: umodsys/lib/lib3d/lib3d.outlines.h
// info: outlines of object
/*************************************************************/

#include <umodsys/lib/3d/lib3d.common.h>

namespace UModSys {
namespace lib3d {

//***************************************
// TYPES
//***************************************

struct SObjectITriangle {
  int v1, v2, v3, n;
};

struct SObjectCloudData {
  const DPoint* verts;
  int nverts;
  //
  inline SObjectCloudData(void) {
    verts = NULL;
    nverts = 0;
  }
};

struct SObjectMeshData : public SObjectCloudData {
  const DPoint* normals;
  const SObjectITriangle* tris;
  int nnormals, ntris;
  //
  inline SObjectMeshData(void) {
    normals = NULL;
    tris = NULL;
    nnormals = ntris = 0;
  }
  bool calc_aabb(DPoint& pmin, DPoint& pmax) const;
};

struct SObjectMultiMeshData : public SObjectMeshData {
  const int* meshes; // count of triangles of each one
  int nmeshes;
  //
  inline SObjectMultiMeshData(void) {
    meshes = NULL;
    nmeshes = 0;
  }
  //
  SObjectMeshData get(int idx) const;
};

//***************************************
//***************************************

struct IObjectBrushOutlineNull : public IObjectBrushOutline {
  enum { type = obo_Null };
};

struct IObjectBrushOutlinePoint : public IObjectBrushOutline {
  enum { type = obo_Point };
  //
  virtual bool get_point(DPoint &M) =0;
};

struct IObjectBrushOutlineLine : public IObjectBrushOutline {
  enum { type = obo_Line };
  //
  virtual bool get_line(DPoint &M, DPoint &Dir) =0;
};

struct IObjectBrushOutlinePlane : public IObjectBrushOutline {
  enum { type = obo_Plane };
  //
  virtual bool get_plane(DPlane &plane) =0;
};

struct IObjectBrushOutlineSphere : public IObjectBrushOutline {
  enum { type = obo_Sphere };
  //
  virtual bool get_sphere(DPoint &C, DScalar& R) =0;
};

struct IObjectBrushOutlineBox : public IObjectBrushOutline {
  enum { type = obo_Box };
  //
  virtual bool get_box(DPoint &min, DPoint &max) =0;
};

struct IObjectBrushOutlineCylinder : public IObjectBrushOutline {
  enum { type = obo_Cylinder };
  //
  virtual bool get_cylinder(DPoint &C, DPoint &Dir, DScalar& R) =0;
};

struct IObjectBrushOutlineConvexCloud : public IObjectBrushOutline {
  enum { type = obo_ConvexCloud };
  //
  virtual bool get_cloud(SObjectCloudData& cloud) =0;
};

struct IObjectBrushOutlineConvexMesh : public IObjectBrushOutline {
  enum { type = obo_ConvexMesh };
  //
  virtual bool get_mesh(SObjectMeshData& mesh) =0;
};

struct IObjectBrushOutlineConvexMultiMesh : public IObjectBrushOutline {
  enum { type = obo_ConvexMultiMesh };
  //
  virtual bool get_mesh(SObjectMultiMeshData& mesh) =0;
};

struct IObjectBrushOutlineConcaveMesh : public IObjectBrushOutline {
  enum { type = obo_ConcaveMesh };
  //
  virtual bool get_mesh(SObjectMeshData& mesh) =0;
};

//***************************************
//***************************************

//***************************************
// ** INLINES **
//***************************************

inline SObjectMeshData SObjectMultiMeshData::get(int idx) const 
{
  SObjectMeshData rv = *this;
  for(int i=0; i<idx; i++) {
    rv.tris += meshes[i];
  }
  rv.ntris = meshes[idx];
  return rv;
}

inline bool SObjectMeshData::calc_aabb(DPoint& pmin, DPoint& pmax) const
{
  pmin.set(1e300); pmax.set(-1e300);
  const SObjectITriangle *t = tris;
  for(int i=0; i<ntris; i++, t++) {
    verts[t->v1].set_minmax(pmin, pmax);
    verts[t->v2].set_minmax(pmin, pmax);
    verts[t->v3].set_minmax(pmin, pmax);
  }
  return true;
}

//***************************************
// END
//***************************************

} // namespace lib3d
} // namespace UModSys

#endif // __UMODSYS_LIB3D_OUTLINES_H
