#ifndef __UMODSYS_LIB3D_OBJECTS_H
#define __UMODSYS_LIB3D_OBJECTS_H 1
/*************************************************************/
// file: umodsys/lib/lib2d/lib3d.objects.h
// info: objects 3d
/*************************************************************/

#include <umodsys/lib/3d/lib3d.common.h>

namespace UModSys {
namespace lib3d {

//***************************************
// TYPES
//***************************************

//***************************************
// SObjectParticle::

struct SObjectParticle {
  const SPicture* pic;
  DColorAlpha color;
  //
  DPointM pos;
  DScalarM cur_time, end_time;
};

//***************************************
// SObjectParticleStatic::

struct SObjectParticleStatic : public SObjectParticle {
  DColorAlpha c_speed;
  DPointM speed, accel;
};

//***************************************
// INTERFACES
//***************************************

//***************************************
// INodeObject_Brush::

struct INodeObject_Brush : public INodeObject {
  enum { ObjectKind = snk_Brush };
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::INodeObject_Brush, 2, INodeObject);
};

//***************************************
// INodeObject_Mesh::

struct INodeObject_Mesh : public INodeObject_Brush {
  enum { ObjectKind = snk_Mesh };
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::INodeObject_Mesh, 2, INodeObject_Brush);
};

//***************************************
// INodeObject_Picture::

struct INodeObject_Picture : public INodeObject_Brush {
  enum { ObjectKind = snk_Picture };
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::INodeObject_Picture, 2, INodeObject_Brush);
};

//***************************************
// INodeObject_Particles::

struct INodeObject_Particles : public INodeObject_Brush {
  enum { ObjectKind = snk_Particles };
  //
  virtual bool particle_setup(int size, int count, int npics=1) =0;
  virtual int particle_count(void) =0; // size solid array of particles
  virtual int particle_countused(void) =0;
  virtual int particle_countfreed(void) =0;
  virtual SObjectParticle* particle_array(void) =0; // solid array of particles
  virtual SObjectParticle* particle_chain(void) =0; // first used, use next
  virtual SObjectParticle* particle_alloc(void) =0;
  virtual bool particle_free(void) =0; // free all unused particles
  //
  virtual const SPicture* pic_set(int pid,
                                  const DTexPoint &p1, const DTexPoint &p2,
                                  const DTexPoint &t1, const DTexPoint &t2,
                                  ITexture *tex) =0;
  virtual const SPicture* pic_set(int pid, ITexture *tex, DScalar s=1.0) =0;
  virtual const SPicture* pic_get(int pid) =0;
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::INodeObject_Particles, 2, INodeObject_Brush);
};

//***************************************
// INodeObject_Camera::

struct INodeObject_Camera : public INodeObject {
  enum { ObjectKind = snk_Camera };
  //
  virtual DTexPoint camera_get_fov(void) =0;
  virtual bool camera_get_T(DMatrix4 &T) =0;
  virtual bool camera_get_xyz(DPoint* ox, DPoint* oy, DPoint* oz) =0;
  virtual int camera_get_planes(DPlane* planes) =0;
  // cast ray from [-1,1],[-1,1] to world coordinates
  virtual bool camera_cast_ray(const DTexPoint &screen, DPoint &start, DPoint &dir) =0;
  virtual bool camera_setup(IRenderDriver *driver) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::INodeObject_Camera, 2, INodeObject);
};

//***************************************
// INodeObject_Light::

struct INodeObject_Light : public INodeObject {
  enum { ObjectKind = snk_Light };
  //
  virtual bool light_supported(eLightType type) =0;
  virtual bool light_setinfo(eLightType type, const SLight& info) =0;
  virtual bool light_getinfo(eLightType type, SLight& info) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::INodeObject_Light, 2, INodeObject);
};

//***************************************
// END
//***************************************

} // namespace lib3d
} // namespace UModSys

#endif // __UMODSYS_LIB3D_OBJECTS_H
