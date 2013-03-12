#ifndef __NANOUTL_TYPES_MATH_U3D_H
#define __NANOUTL_TYPES_MATH_U3D_H 1
//*****************************************************
// nanoutl/types/math_3du.h: utils
//*****************************************************

#include <nanoutl/types/math_3d.h>

namespace NanoUTL {
namespace math3d {
namespace utils3d {

//***************************************
// Matrix camera tranformations
//

#if 0
template<typename C, typename F>
inline bool set_frustum(TMatrix44<C> &M, const C center[3], const F fov[2], C maxz) 
{
  if(fov[0]<0.01 || fov[0]>M_PI-0.01 || fov[1]<0.01 || fov[1]>M_PI-0.01) {
    return false; // bad FOV
  }
  C x = (C)tan(fov[0]/2)*center[2]/2, y = (C)tan(fov[1]/2)*center[2]/2;
  return set_frustum(M, center[0]-x, center[0]+x, center[1]-y, center[1]+y, center[2], maxz);
}

template<typename C, typename F> 
inline bool set_frustum(TMatrix44<C> &M, const TVector3<C>& center, const TVector2<F> fov, C maxz)
{
  if(fov(0)<0.01 || fov(0)>M_PI-0.01 || fov(1)<0.01 || fov(1)>M_PI-0.01) {
    return false; // bad FOV
  }
  C x = (C)tan(fov(0)/2)*center(2)/2, y = (C)tan(fov(1)/2)*center(2)/2;
  return set_frustum(M, center(0)-x, center(0)+x, center(1)-y, center(1)+y, center(2), maxz);
}

template<typename C>
inline bool set_frustum(TMatrix44<C> &M, C xmin, C xmax, C ymin, C ymax, C zmin, C zmax) 
{
  if(zmin<1e-3 || zmax/zmin>1e8)
    return false; // too much distorsion
/*
  M.set_row(0, 2*zmin/(xmax-xmin), 0, (xmax+xmin)/(xmax-xmin), 0);
  M.set_row(1, 0, 2*zmin/(ymax-ymin), (ymax+ymin)/(ymax-ymin), 0);
  M.set_row(2, 0, 0, -(zmax+zmin)/(zmax-zmin), -2*zmax*zmin/(zmax-zmin));
  M.set_row(3, 0, 0, -1, 0);
*/
  M.set_row(0, 2*zmin/(xmax-xmin), 0, 0, 0);
  M.set_row(1, 0, 2*zmin/(ymax-ymin), 0, 0);
  M.set_row(2, (xmax+xmin)/(xmax-xmin), (ymax+ymin)/(ymax-ymin), -(zmax+zmin)/(zmax-zmin), -1);
  M.set_row(3, 0, 0, -2*zmax*zmin/(zmax-zmin), 0);
  return true;
}
#endif

template<typename C, typename F> 
bool set_frustum(TMatrix44<C> &M, const TVector2<F> fov, C minz, C maxz)
{
  if(fov(0)<0.01 || fov(0)>M_PI-0.01 || fov(1)<0.01 || fov(1)>M_PI-0.01) {
    return false; // bad FOV
  }
  C x = (C)tan(fov(0)/2)*minz/2, y = (C)tan(fov(1)/2)*minz/2;
  return set_frustum(M, x, y, minz, maxz);
}

template<typename C> 
bool set_frustum(TMatrix44<C> &M, C dx, C dy, C dz, C zmax)
{
  if(dz<1e-3 || zmax/dz>1e8)
    return false; // too much distorsion
  M.set_row(0, dz/dx, 0, 0, 0);
  M.set_row(1, 0, dz/dx, 0, 0);
  M.set_row(2, 0, 0, -(zmax+dz)/(zmax-dz), -1);
  M.set_row(3, 0, 0, -2*zmax*dz/(zmax-dz), 0);
  return true;
}

//***************************************
// Matrix translate
//

template<typename C> 
inline const TMatrix44<C>& set_translate(TMatrix44<C> &M, const TVector3<C>& t)
{
  M.setE(1); M.set_translate(t);
  return M;
}

template<typename C> 
inline const TMatrix44<C>& add_translate_L(TMatrix44<C> &M, const TVector3<C>& t)
{
  TMatrix44<C> tmp; 
  tmp.setE(1); tmp.set_translate(t);
  M = tmp*M;
  return M;
}

template<typename C> 
inline const TMatrix44<C>& add_translate(TMatrix44<C> &M, const TVector3<C>& t)
{
  TMatrix44<C> tmp; 
  tmp.setE(1); tmp.set_translate(t);
  M = M*tmp;
  return M;
}

//***************************************
// Matrix scale
//

template<typename C> 
inline const TMatrix44<C>& set_scale(TMatrix44<C> &M, const TVector3<C>& s)
{
  tmp.set_scale(s); tmp.setT(1);
  return M;
}

template<typename C> 
inline const TMatrix44<C>& add_scale_L(TMatrix44<C> &M, const TVector3<C>& s)
{
  TMatrix44<C> tmp; 
  tmp.set_scale(s); tmp.setT(1);
  M = tmp*M;
  return M;
}

template<typename C> 
inline const TMatrix44<C>& add_scale(TMatrix44<C> &M, const TVector3<C>& s)
{
  TMatrix44<C> tmp; 
  tmp.set_scale(s); tmp.setT(1);
  M = M*tmp;
  return M;
}

//***************************************
} // namespace utils3d
} // namespace math3d
} // namespace NanoUTL

#endif // __NANOUTL_TYPES_MATH_U3D_H
