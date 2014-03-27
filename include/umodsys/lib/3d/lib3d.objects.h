#ifndef __UMODSYS_LIB3D_OBJECTS_H
#define __UMODSYS_LIB3D_OBJECTS_H 1
/*************************************************************/
// file: umodsys/lib/lib3d/lib3d.objects.h
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
// IRenderObject_Brush::

struct IRenderObject_Brush : public IRenderObject {
  enum { ObjectKind = snk_Brush };
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IRenderObject_Brush, 2, IRenderObject);
};

//***************************************
// IVisualObject_Brush::

struct IVisualObject_Brush : public IVisualObject {
  enum { ObjectKind = snk_Brush };
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IVisualObject_Brush, 2, IVisualObject);
};

//---------------------------------------
// ILogicObject_Brush::

struct ILogicObject_Brush : public ILogicObject {
  enum { ObjectKind = snk_Brush };
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::ILogicObject_Brush, 2, ILogicObject);
};

//***************************************
// IVisualObject_Mesh::

struct IVisualObject_Mesh : public IVisualObject_Brush {
  enum { ObjectKind = snk_Mesh };
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IVisualObject_Mesh, 2, IVisualObject_Brush);
};

//***************************************
// IVisualObject_Picture::

struct IVisualObject_Picture : public IVisualObject_Brush {
  enum { ObjectKind = snk_Picture };
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IVisualObject_Picture, 2, IVisualObject_Brush);
};

//***************************************
// IVisualObject_Particles::

struct IVisualObject_Particles : public IVisualObject_Brush {
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
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IVisualObject_Particles, 2, IVisualObject_Brush);
};

//***************************************
// IVisualObject_Camera::

struct IVisualObject_Camera : public IVisualObject {
  enum { ObjectKind = snk_Camera };
  //
  virtual DTexPoint camera_get_fov(void) const =0;
  virtual bool camera_get_T(DMatrix4 &T) const =0;
  virtual bool camera_get_xyz(DPoint* ox, DPoint* oy, DPoint* oz) const =0;
  virtual int camera_get_planes(DPlane* planes) const =0;
  // cast ray from [-1,1],[-1,1] to world coordinates
  virtual bool camera_cast_ray(const DTexPoint &screen, DPoint &start, DPoint &dir) const =0;
  virtual bool camera_setup(IRenderDriver *driver) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IVisualObject_Camera, 2, IVisualObject);
};

//***************************************
// IVisualObject_Light::

struct IVisualObject_Light : public IVisualObject {
  enum { ObjectKind = snk_Light };
  //
  virtual bool light_supported(eLightType type) =0;
  virtual bool light_setinfo(eLightType type, const SLight& info) =0;
  virtual bool light_getinfo(eLightType type, SLight& info) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IVisualObject_Light, 2, IVisualObject);
};

//***************************************
// END
//***************************************

} // namespace lib3d
} // namespace UModSys

#endif // __UMODSYS_LIB3D_OBJECTS_H
