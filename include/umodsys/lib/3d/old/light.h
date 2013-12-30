#ifndef __UMS_LIB3D_LIGHT_H
#define __UMS_LIB3D_LIGHT_H 1
//*****************************************************
// lib3d/light.h: common 3d library interfaces, light objects
//*****************************************************

#include <ums/lib3d/objects.h>

namespace UMS {
namespace lib3d {

//***************************************

struct SLight {
  DColor color;
};

struct SLightFade {
  BScalarM base, power;

  SLightFade(const BScalarM &b=1, const BScalarM &p=2) : base(b), power(p) {}
  void set(const BScalarM &b=1, const BScalarM &p=2) { base = b; power = p; }

  BScalarM fade(const BScalarM& dist) const { return ::pow(dist/base, power); }
};

//***************************************

struct SLightSphere : public SLight {
  SLightFade sphere;
};


//***************************************

struct SLightCylinder : public SLight {
  SLightFade cylinder;
};

//***************************************

struct SLightCone : public SLight {
  SLightFade sphere;
  SLightFade cylinder;
};

//***************************************

struct SUnionLight {
  eLightType type;
  byte light[sizeof(SLightCone)];
  //
  SLight& omni(void) { return *(SLight*)&light; }
  SLightSphere& sphere(void) { return *(SLightSphere*)&light; }
  SLightCylinder& cylinder(void) { return *(SLightCylinder*)&light; }
  SLightCone& cone(void) { return *(SLightCone*)&light; }
  //
  const SLight& omni(void) const { return *(SLight*)&light; }
  const SLightSphere& sphere(void) const { return *(SLightSphere*)&light; }
  const SLightCylinder& cylinder(void) const { return *(SLightCylinder*)&light; }
  const SLightCone& cone(void) const { return *(SLightCone*)&light; }
  //
  void set(const SLight &R) { type = lt_Omni; omni() = R; }
  void set(const SLightSphere &R) { type = lt_Sphere; sphere() = R; }
  void set(const SLightCylinder &R) { type = lt_Cylinder; cylinder() = R; }
  void set(const SLightCone &R) { type = lt_Cone; cone() = R; }
};


//***************************************

} // namespace lib3d
} // namespace UMS

#endif
