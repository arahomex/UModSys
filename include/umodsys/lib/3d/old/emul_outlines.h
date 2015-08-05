#ifndef __UMS_LIB3D_BRUSH_OUTLINES_EMULATION_H
#define __UMS_LIB3D_BRUSH_OUTLINES_EMULATION_H 1
//*****************************************************
// lib3d/emul_outlines.h: common 3d library interfaces, brush outlines emulation
//*****************************************************

#include <ums/lib3d/objects.h>
#include <ums/lib3d/outlines.h>

namespace UMS {
namespace lib3d {
namespace emulate_outlines {

//***************************************
//***************************************

struct CNull : public IObjectBrushOutlineNull {
  eObjectBrushOutline get_type(void) { return eObjectBrushOutline(type); }

  inline CNull(void) {}
};

//***************************************

#define START_OUTLINE(_type, _base) \
  template<typename Root> struct _type : public _base { \
    Root* root; \
    eObjectBrushOutline get_type(void) { return eObjectBrushOutline(type); } \
    inline _type(Root *r) : root(r) {} \
    inline ~_type(void) {} \

#define END_OUTLINE() \
  };


//***************************************

START_OUTLINE(TPoint, IObjectBrushOutlinePoint)
  bool get_point(DPoint &M) { return root->get_outline_point(M); }
END_OUTLINE()

//***************************************

START_OUTLINE(TLine, IObjectBrushOutlineLine)
  bool get_line(DPoint &M, DPoint &Dir) { return root->get_outline_line(M, Dir); }
END_OUTLINE()

//***************************************

START_OUTLINE(TPlane, IObjectBrushOutlinePlane)
  bool get_plane(DPlane &plane) { return root->get_outline_plane(plane); }
END_OUTLINE()

//***************************************

START_OUTLINE(TSphere, IObjectBrushOutlineSphere)
  bool get_sphere(DPoint &C, BScalar& R) { return root->get_outline_sphere(C, R); }
END_OUTLINE()

//***************************************

START_OUTLINE(TBox, IObjectBrushOutlineBox)
  bool get_box(DPoint &min, DPoint &max) { return root->get_outline_box(min, max); }
END_OUTLINE()

//***************************************

START_OUTLINE(TCylinder, IObjectBrushOutlineCylinder)
  bool get_cylinder(DPoint &C, DPoint &Dir, BScalar& R) { return root->get_outline_cylinder(C, Dir, R); }
END_OUTLINE()

//***************************************

START_OUTLINE(TConvexCloud, IObjectBrushOutlineConvexCloud)
  bool get_cloud(SObjectCloudData& cloud) { return root->get_outline_cylinder(cloud); }
END_OUTLINE()

//***************************************

START_OUTLINE(TConvexMesh, IObjectBrushOutlineConvexMesh)
  bool get_mesh(SObjectMeshData& mesh) { return root->get_outline_mesh(mesh); }
END_OUTLINE()

//***************************************

START_OUTLINE(TConvexMultiMesh, IObjectBrushOutlineConvexMultiMesh)
  bool get_mesh(SObjectMultiMeshData& mesh) { return root->get_outline_mmesh(mesh); }
END_OUTLINE()

//***************************************

START_OUTLINE(TConcaveMesh, IObjectBrushOutlineConcaveMesh)
  bool get_mesh(SObjectMeshData& mesh) { return root->get_outline_mesh(mesh); }
END_OUTLINE()

//***************************************
//***************************************


} // namespace emulate_outlines
} // namespace lib3d
} // namespace UMS

#endif
