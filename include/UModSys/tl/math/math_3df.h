#ifndef __NANOUTL_TYPES_MATH_3DF_H
#define __NANOUTL_TYPES_MATH_3DF_H 1
//*****************************************************
// nanoutl/types/math_3df.h: common 3d & 2d mathematics used
//                           in renderers etc.
//                           functions implementation
//*****************************************************

#include <nanoutl/types/math_3d.h>

namespace NanoUTL {
namespace math3d {

//***************************************
// template<typename _Type> struct TVector3
//
template<typename _Type>
TVector3<_Type> TVector3<_Type>::rotate_about(const TVector3<_Type>& ort, _Type angle) const {
  _Type c = cos(angle), s = sin(angle);
  _Type a1 = ort.v[0]*ort.v[0], a2 = ort.v[1]*ort.v[1], a3 = ort.v[2]*ort.v[2];
  _Type a12 = ort.v[0]*ort.v[1], a23 = ort.v[1]*ort.v[2], a13 = ort.v[0]*ort.v[2], a123 = a1+a2+a3;
  _Type ort_N = a1+a2+a3, ort_L = sqrt(ort_N), K = 1/ort_N;
  //
  return TVector3<_Type>(
   K*( ort.v[0]*a123
       + c*(v[0]*(a1+a3)-ort.v[0]*(ort.v[1]*v[1]+ort.v[2]*v[2]))
       + ort_L*(-ort.v[2]*v[1]+ort.v[1]*v[2])*s ),
   K*( ort.v[1]*a123
       + c*(v[1]*(a2+a3)-ort.v[1]*(ort.v[0]*v[0]+ort.v[2]*v[2]))
       + s*ort_L*(ort.v[2]*v[0]-ort.v[0]*v[2]) ),
   K*( ort.v[2]*a123
       + c*(v[2]*(a1+a2)-ort.v[2]*(ort.v[0]*v[0]+ort.v[1]*v[1]))
       + s*ort_L*(-ort.v[1]*v[0]+ort.v[0]*v[1]) )
  );
}

//***************************************
// template<typename _Type> struct TQuaternion4
//

template<typename _Type>
void TQuaternion4<_Type>::set_rotate(const _Type* vv) {
  _Type c1 = cos(vv[2]), s1 = sin(vv[2]);
  _Type c2 = cos(vv[1]), s2 = sin(vv[1]);
  _Type c3 = cos(vv[0]), s3 = sin(vv[0]);
  _Type c1c2 = c1*c2, s1s2 = s1*s2;
  v[0]=c1c2*c3 + s1s2*s3; //w
  v[1]=c1c2*s3 - s1s2*c3; //x
  v[2]=c1*s2*c3 + s1*c2*s3; //y
  v[3]=s1*c2*c3 - c1*s2*s3; //z
}

template<typename _Type>
void TQuaternion4<_Type>::get_rotate(_Type* vv) const {
  _Type w=vv[0], x=vv[1];
  _Type y=vv[2], z=vv[3];
  _Type sqw = w*w, sqx = x*x;
  _Type sqy = y*y, sqz = z*z;
  vv[2] = atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw)); // heading
  vv[0] = atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw)); // bank
  vv[1] = asin(-2.0 * (x*z - y*w)); // attitude
}

template<typename _Type>
inline TQuaternion4<_Type> TQuaternion4<_Type>::operator*(const TQuaternion4<_Type>& R) const {
  Type A=(q1.v[0]+q1.v[1])*(q2.v[0]+q2.v[1])
  Type B=(q1.v[3]-q1.v[2])*(q2.v[2]-q2.v[3])
  Type C=(q1.v[1]-q1.v[0])*(q2.v[2]+q2.v[3])
  Type D=(q1.v[2]+q1.v[3])*(q2.v[1]-q2.v[0])
  Type E=(q1.v[1]+q1.v[3])*(q2.v[1]+q2.v[2])
  Type F=(q1.v[1]-q1.v[3])*(q2.v[1]-q2.v[2])
  Type G=(q1.v[0]+q1.v[2])*(q2.v[0]-q2.v[3])
  Type H=(q1.v[0]-q1.v[2])*(q2.v[0]+q2.v[3])
  return Self(
    B + (-E - F + G + H) * 0.5, 
    A - ( E + F + G + H) * 0.5, 
    -C + ( E - F + G - H) * 0.5,
    -D + ( E - F - G + H) * 0.5
  );
}

//***************************************
// template<typename _Type> struct TMatrix44
//

template<typename _Type>
inline void TMatrix44<_Type>::setZ(void) {
  v[ 0]=0; v[ 1]=0; v[ 2]=0; v[ 3]=0;
  v[ 4]=0; v[ 5]=0; v[ 6]=0; v[ 7]=0;
  v[ 8]=0; v[ 9]=0; v[10]=0; v[11]=0;
  v[12]=0; v[13]=0; v[14]=0; v[15]=0;
}

template<typename _Type>
inline void TMatrix44<_Type>::setE(_Type M) {
  v[ 0]=M; v[ 1]=0; v[ 2]=0; v[ 3]=0;
  v[ 4]=0; v[ 5]=M; v[ 6]=0; v[ 7]=0;
  v[ 8]=0; v[ 9]=0; v[10]=M; v[11]=0;
  v[12]=0; v[13]=0; v[14]=0; v[15]=M;
}

template<typename _Type>
inline void TMatrix44<_Type>::setE3(_Type M) {
  v[ 0]=M; v[ 1]=0; v[ 2]=0;
  v[ 4]=0; v[ 5]=M; v[ 6]=0;
  v[ 8]=0; v[ 9]=0; v[10]=M;
}

template<typename _Type>
void TMatrix44<_Type>::mult(const Self& L, const Self& R) {
  v[0*4+0] = _scalar4s(L.v+0*4, R.v+0, 4);
  v[0*4+1] = _scalar4s(L.v+0*4, R.v+1, 4);
  v[0*4+2] = _scalar4s(L.v+0*4, R.v+2, 4);
  v[0*4+3] = _scalar4s(L.v+0*4, R.v+3, 4);
  v[1*4+0] = _scalar4s(L.v+1*4, R.v+0, 4);
  v[1*4+1] = _scalar4s(L.v+1*4, R.v+1, 4);
  v[1*4+2] = _scalar4s(L.v+1*4, R.v+2, 4);
  v[1*4+3] = _scalar4s(L.v+1*4, R.v+3, 4);
  v[2*4+0] = _scalar4s(L.v+2*4, R.v+0, 4);
  v[2*4+1] = _scalar4s(L.v+2*4, R.v+1, 4);
  v[2*4+2] = _scalar4s(L.v+2*4, R.v+2, 4);
  v[2*4+3] = _scalar4s(L.v+2*4, R.v+3, 4);
  v[3*4+0] = _scalar4s(L.v+3*4, R.v+0, 4);
  v[3*4+1] = _scalar4s(L.v+3*4, R.v+1, 4);
  v[3*4+2] = _scalar4s(L.v+3*4, R.v+2, 4);
  v[3*4+3] = _scalar4s(L.v+3*4, R.v+3, 4);
}

template<typename _Type>
void TMatrix44<_Type>::mult(const Self& L, Type Rk) {
  v[0*4+0] = L.v[0*4+0]*Rk;
  v[0*4+1] = L.v[0*4+1]*Rk;
  v[0*4+2] = L.v[0*4+2]*Rk;
  v[0*4+3] = L.v[0*4+3]*Rk;
  v[1*4+0] = L.v[1*4+0]*Rk;
  v[1*4+1] = L.v[1*4+1]*Rk;
  v[1*4+2] = L.v[1*4+2]*Rk;
  v[1*4+3] = L.v[1*4+3]*Rk;
  v[2*4+0] = L.v[2*4+0]*Rk;
  v[2*4+1] = L.v[2*4+1]*Rk;
  v[2*4+2] = L.v[2*4+2]*Rk;
  v[2*4+3] = L.v[2*4+3]*Rk;
  v[3*4+0] = L.v[3*4+0]*Rk;
  v[3*4+1] = L.v[3*4+1]*Rk;
  v[3*4+2] = L.v[3*4+2]*Rk;
  v[3*4+3] = L.v[3*4+3]*Rk;
}

template<typename _Type>
void TMatrix44<_Type>::mult(const Self& L, const Self& R, Type Rk) {
  v[0*4+0] = _scalar4sk(L.v+0*4, R.v+0, 4)*Rk;
  v[0*4+1] = _scalar4sk(L.v+0*4, R.v+1, 4)*Rk;
  v[0*4+2] = _scalar4sk(L.v+0*4, R.v+2, 4)*Rk;
  v[0*4+3] = _scalar4sk(L.v+0*4, R.v+3, 4)*Rk;
  v[1*4+0] = _scalar4sk(L.v+1*4, R.v+0, 4)*Rk;
  v[1*4+1] = _scalar4sk(L.v+1*4, R.v+1, 4)*Rk;
  v[1*4+2] = _scalar4sk(L.v+1*4, R.v+2, 4)*Rk;
  v[1*4+3] = _scalar4sk(L.v+1*4, R.v+3, 4)*Rk;
  v[2*4+0] = _scalar4sk(L.v+2*4, R.v+0, 4)*Rk;
  v[2*4+1] = _scalar4sk(L.v+2*4, R.v+1, 4)*Rk;
  v[2*4+2] = _scalar4sk(L.v+2*4, R.v+2, 4)*Rk;
  v[2*4+3] = _scalar4sk(L.v+2*4, R.v+3, 4)*Rk;
  v[3*4+0] = _scalar4sk(L.v+3*4, R.v+0, 4)*Rk;
  v[3*4+1] = _scalar4sk(L.v+3*4, R.v+1, 4)*Rk;
  v[3*4+2] = _scalar4sk(L.v+3*4, R.v+2, 4)*Rk;
  v[3*4+3] = _scalar4sk(L.v+3*4, R.v+3, 4)*Rk;
}

template<typename _Type>
void TMatrix44<_Type>::mult3(const Self& L, const Self& R) {
  v[0*4+0] = _scalar3s(L.v+0*4, R.v+0, 4);
  v[0*4+1] = _scalar3s(L.v+0*4, R.v+1, 4);
  v[0*4+2] = _scalar3s(L.v+0*4, R.v+2, 4);
  v[1*4+0] = _scalar3s(L.v+1*4, R.v+0, 4);
  v[1*4+1] = _scalar3s(L.v+1*4, R.v+1, 4);
  v[1*4+2] = _scalar3s(L.v+1*4, R.v+2, 4);
  v[2*4+0] = _scalar3s(L.v+2*4, R.v+0, 4);
  v[2*4+1] = _scalar3s(L.v+2*4, R.v+1, 4);
  v[2*4+2] = _scalar3s(L.v+2*4, R.v+2, 4);
}

template<typename _Type>
void TMatrix44<_Type>::set_rotate(const TVector3<_Type>& R) {
    Type cr = Type(cos(R.v[0])), sr = Type(sin(R.v[0])),
         cp = Type(cos(R.v[1])), sp = Type(sin(R.v[1])),
         cy = Type(cos(R.v[2])), sy = Type(sin(R.v[2]));
    Type srsp = sr*sp, crsp = cr*sp;

    v[0*4+0] = cp*cy;
    v[0*4+1] = cp*sy;
    v[0*4+2] = -sp;
    v[0*4+3] = 0;


    v[1*4+0] = srsp*cy-cr*sy;
    v[1*4+1] = srsp*sy+cr*cy;
    v[1*4+2] = sr*cp;
    v[1*4+3] = 0;

    v[2*4+0] = crsp*cy+sr*sy;
    v[2*4+1] = crsp*sy-sr*cy;
    v[2*4+2] = cr*cp;
    v[2*4+3] = 0;
}

template<typename _Type>
void TMatrix44<_Type>::set_rotate(_Type a1, _Type a2, _Type a3) {
    Type cr = Type(cos(a1)), sr = Type(sin(a1)),
         cp = Type(cos(a2)), sp = Type(sin(a2)),
         cy = Type(cos(a3)), sy = Type(sin(a3));
    Type srsp = sr*sp, crsp = cr*sp;

    v[0*4+0] = cp*cy;
    v[0*4+1] = cp*sy;
    v[0*4+2] = -sp;
    v[0*4+3] = 0;


    v[1*4+0] = srsp*cy-cr*sy;
    v[1*4+1] = srsp*sy+cr*cy;
    v[1*4+2] = sr*cp;
    v[1*4+3] = 0;

    v[2*4+0] = crsp*cy+sr*sy;
    v[2*4+1] = crsp*sy-sr*cy;
    v[2*4+2] = cr*cp;
    v[2*4+3] = 0;
}

template<typename _Type>
void TMatrix44<_Type>::set_rotateT(const TVector3<_Type>& R) {
    Type cr = Type(cos(R.v[0])), sr = Type(sin(R.v[0])),
         cp = Type(cos(R.v[1])), sp = Type(sin(R.v[1])),
         cy = Type(cos(R.v[2])), sy = Type(sin(R.v[2]));
    Type srsp = sr*sp, crsp = cr*sp;

    v[0*4+0] = cp*cy;
    v[1*4+0] = cp*sy;
    v[2*4+0] = -sp;
    v[3*4+0] = 0;

    v[0*4+1] = srsp*cy-cr*sy;
    v[1*4+1] = srsp*sy+cr*cy;
    v[2*4+1] = sr*cp;
    v[3*4+1] = 0;

    v[0*4+2] = crsp*cy+sr*sy;
    v[1*4+2] = crsp*sy-sr*cy;
    v[2*4+2] = cr*cp;
    v[3*4+2] = 0;
}

template<typename _Type>
void TMatrix44<_Type>::set_rotate_about(const TVector3<_Type>& ort, _Type angle) {
  _Type a1 = ort.v[0]*ort.v[0], a2 = ort.v[1]*ort.v[1], a3 = ort.v[2]*ort.v[2];
  _Type a12 = ort.v[0]*ort.v[1], a23 = ort.v[1]*ort.v[2], a13 = ort.v[0]*ort.v[2];
  _Type ort_N = a1+a2+a3, ort_L = sqrt(ort_N), K = 1/ort_N;
  _Type c = cos(angle), s = sin(angle), c1=1-c;

  v[0*4+0] = K*(c*(a2+a3)+a1);
  v[0*4+1] = K*(a12*c1 + ort_L*ort.v[2]*s);
  v[0*4+2] = K*(a13*c1 - ort_L*ort.v[1]*s);
  v[0*4+3] = 0;

  v[1*4+0] = K*(a12*c1 - ort_L*ort.v[2]*s);
  v[1*4+1] = K*(c*(a1+a3)+a2);
  v[1*4+2] = K*(a23*c1 + ort_L*ort.v[0]*s);
  v[1*4+3] = 0;

  v[2*4+0] = K*(a13*c1 + ort_L*ort.v[1]*s);
  v[2*4+1] = K*(a23*c1 - ort_L*ort.v[0]*s);
  v[2*4+2] = K*(c*(a1+a2)+a3);
  v[2*4+3] = 0;
}

template<typename _Type>
void TMatrix44<_Type>::set_rotate_aboutT(const TVector3<_Type>& ort, _Type angle) {
  _Type a1 = ort.v[0]*ort.v[0], a2 = ort.v[1]*ort.v[1], a3 = ort.v[2]*ort.v[2];
  _Type a12 = ort.v[0]*ort.v[1], a23 = ort.v[1]*ort.v[2], a13 = ort.v[0]*ort.v[2];
  _Type ort_N = a1+a2+a3, ort_L = sqrt(ort_N), K = 1/ort_N;
  _Type c = cos(angle), s = sin(angle), c1=1-c;

  v[0*4+0] = K*(c*(a2+a3)+a1);
  v[1*4+0] = K*(a12*c1 + ort_L*ort.v[2]*s);
  v[2*4+0] = K*(a13*c1 - ort_L*ort.v[1]*s);
  v[3*4+0] = 0;

  v[0*4+1] = K*(a12*c1 - ort_L*ort.v[2]*s);
  v[1*4+1] = K*(c*(a1+a3)+a2);
  v[2*4+1] = K*(a23*c1 + ort_L*ort.v[0]*s);
  v[3*4+1] = 0;

  v[0*4+2] = K*(a13*c1 + ort_L*ort.v[1]*s);
  v[1*4+2] = K*(a23*c1 - ort_L*ort.v[0]*s);
  v[2*4+2] = K*(c*(a1+a2)+a3);
  v[3*4+2] = 0;
}

template<typename _Type>
void TMatrix44<_Type>::set_rotate(const TQuaternion4<_Type>& q) {
    _Type w=q.v[0], x=q.v[1];
    _Type y=q.v[2], z=q.v[3];
    _Type ww = w*w, xx = x*x;
    _Type yy = y*y, zz = z*z;
    //
    v[0*4+0] = ww + xx - yy - zz;
    v[1*4+1] = ww -xx + yy - zz;
    v[2*4+2] = ww -xx - yy + zz;
    _Type tmp1 = x*y;
    _Type tmp2 = z*w;
    v[0*4+1] = 2 * (tmp1 + tmp2);
    v[1*4+0] = 2 * (tmp1 - tmp2);
    tmp1 = x*z;
    tmp2 = y*w;
    v[0*4+2] = 2 * (tmp1 - tmp2);
    v[2*4+0] = 2 * (tmp1 + tmp2);
    tmp1 = y*z;
    tmp2 = x*w;
    v[1*4+2] = 2 * (tmp1 + tmp2);
    v[2*4+1] = 2 * (tmp1 - tmp2);
}

template<typename _Type>
void TMatrix44<_Type>::set_rotateT(const TQuaternion4<_Type>& q) {
    _Type w=q.v[0], x=q.v[1];
    _Type y=q.v[2], z=q.v[3];
    _Type ww = w*w, xx = x*x;
    _Type yy = y*y, zz = z*z;
    //
    v[0*4+0] = ww + xx - yy - zz;
    v[1*4+1] = ww -xx + yy - zz;
    v[2*4+2] = ww -xx - yy + zz;
    _Type tmp1 = x*y;
    _Type tmp2 = z*w;
    v[1*4+0] = 2 * (tmp1 + tmp2);
    v[0*4+1] = 2 * (tmp1 - tmp2);
    tmp1 = x*z;
    tmp2 = y*w;
    v[2*4+0] = 2 * (tmp1 - tmp2);
    v[0*4+2] = 2 * (tmp1 + tmp2);
    tmp1 = y*z;
    tmp2 = x*w;
    v[2*4+1] = 2 * (tmp1 + tmp2);
    v[1*4+2] = 2 * (tmp1 - tmp2);
}

template<typename _Type>
bool TMatrix44<_Type>::set_rotate_orts(const TVector3<_Type> &ort1, int nort1, const TVector3<_Type> &ort2, int nort2, const TVector3<_Type> &ort3, int nort3) {
  if(nort1==nort2 || nort2==nort3)
    return false;
  Vector3 a, b, c;
  a = ort1.normalized();
  b.cross(a, ort2, nort2>nort1);
  Type k = b.norma();
  if(k<small_value(&k)) {
    c.cross(a, ort3, nort1>nort3);
    b.cross(c, ort3, nort2>nort3);
  } else {
    c.cross(a, b, nort1>nort3);
  }
  b.normalize();
  c.normalize();
  set_ort3(a.v, nort1);
  set_ort3(b.v, nort3);
  set_ort3(c.v, nort2);
  return true;
}

template<typename _Type>
bool TMatrix44<_Type>::set_rotate_ortsT(const TVector3<_Type> &ort1, int nort1, const TVector3<_Type> &ort2, int nort2, const TVector3<_Type> &ort3, int nort3) {
  return false;
}

template<typename _Type>
TQuaternion4<_Type> TMatrix44<_Type>::get_quaternion(void) const {
  _Type w = sqrt(1 + v[0*4+0] + v[1*4+1] + v[2*4+2]) / 2, k = 1/(4*w);
  return TQuaternion4<_Type>(w,
                             (v[1*4+2] - v[2*4+1]) * k,
                             (v[2*4+0] - v[0*4+2]) * k,
                             (v[0*4+1] - v[1*4+0]) * k ).normalized();
}

template<typename _Type>
TQuaternion4<_Type> TMatrix44<_Type>::get_quaternionT(void) const {
  _Type w = sqrt(1 + v[0*4+0] + v[1*4+1] + v[2*4+2]) / 2, k = 1/(4*w);
  return TQuaternion4<_Type>(w,
                             (v[2*4+1] - v[1*4+2]) * k,
                             (v[0*4+2] - v[2*4+0]) * k,
                             (v[1*4+0] - v[0*4+1]) * k ).normalized();
}

template<typename _Type>
inline void TMatrix44<_Type>::set_ER(_Type e) {
    v[0*4+0] = e;
    v[0*4+1] = 0;
    v[0*4+2] = 0;
    v[0*4+3] = 0;

    v[1*4+0] = 0;
    v[1*4+1] = e;
    v[1*4+2] = 0;
    v[1*4+3] = 0;

    v[2*4+0] = 0;
    v[2*4+1] = 0;
    v[2*4+2] = e;
    v[2*4+3] = 0;

}

template<typename _Type>
inline void TMatrix44<_Type>::set_ET(_Type t) {
    v[3*4+0] = 0;
    v[3*4+1] = 0;
    v[3*4+2] = 0;
    v[3*4+3] = t;
}

template<typename _Type>
inline void TMatrix44<_Type>::set_ERT(_Type e, _Type t) {
    v[0*4+0] = e;
    v[0*4+1] = 0;
    v[0*4+2] = 0;
    v[0*4+3] = 0;

    v[1*4+0] = 0;
    v[1*4+1] = e;
    v[1*4+2] = 0;
    v[1*4+3] = 0;

    v[2*4+0] = 0;
    v[2*4+1] = 0;
    v[2*4+2] = e;
    v[2*4+3] = 0;

    v[3*4+0] = 0;
    v[3*4+1] = 0;
    v[3*4+2] = 0;
    v[3*4+3] = t;
}

template<typename _Type>
inline void TMatrix44<_Type>::set_ort3(const _Type* A, const _Type* B, const _Type* C) {
    v[0*4+0] = A[0];
    v[0*4+1] = A[1];
    v[0*4+2] = A[2];
    v[0*4+3] = 0;

    v[1*4+0] = B[0];
    v[1*4+1] = B[1];
    v[1*4+2] = B[2];
    v[1*4+3] = 0;

    v[2*4+0] = C[0];
    v[2*4+1] = C[1];
    v[2*4+2] = C[2];
    v[2*4+3] = 0;
}

template<typename _Type>
inline void TMatrix44<_Type>::set_ort3T(const _Type* A, const _Type* B, const _Type* C) {
    v[0*4+0] = A[0];
    v[0*4+1] = B[0];
    v[0*4+2] = C[0];
    v[0*4+3] = 0;

    v[1*4+0] = A[1];
    v[1*4+1] = B[1];
    v[1*4+2] = C[1];
    v[1*4+3] = 0;

    v[2*4+0] = A[2];
    v[2*4+1] = B[2];
    v[2*4+2] = C[2];
    v[2*4+3] = 0;
}

template<typename _Type>
inline void TMatrix44<_Type>::set_scale(const TVector3<_Type>& R) {
    v[0*4+0] = R.v[0];
    v[0*4+1] = 0;
    v[0*4+2] = 0;
    v[0*4+3] = 0;

    v[1*4+0] = 0;
    v[1*4+1] = R.v[1];
    v[1*4+2] = 0;
    v[1*4+3] = 0;

    v[2*4+0] = 0;
    v[2*4+1] = 0;
    v[2*4+2] = R.v[2];
    v[2*4+3] = 0;
}

template<typename _Type>
inline void TMatrix44<_Type>::add_scale(const TVector3<_Type>& R) {
    v[0*4+0] *= R.v[0];
    v[0*4+1] *= R.v[0];
    v[0*4+2] *= R.v[0];

    v[1*4+0] *= R.v[1];
    v[1*4+1] *= R.v[1];
    v[1*4+2] *= R.v[1];

    v[2*4+0] *= R.v[2];
    v[2*4+1] *= R.v[2];
    v[2*4+2] *= R.v[2];
}

template<typename _Type>
inline void TMatrix44<_Type>::add_scale(_Type R) {
    v[0*4+0] *= R;
    v[0*4+1] *= R;
    v[0*4+2] *= R;

    v[1*4+0] *= R;
    v[1*4+1] *= R;
    v[1*4+2] *= R;

    v[2*4+0] *= R;
    v[2*4+1] *= R;
    v[2*4+2] *= R;
}

template<typename _Type>
_Type TMatrix44<_Type>::get_determinant(void) const {
#define RM(x, y) v[x+y*4]
  Type det = (RM(0, 0) * RM(1, 1) - RM(1, 0) * RM(0, 1)) * (RM(2, 2) * RM(3, 3) - RM(3, 2) * RM(2, 3))
           - (RM(0, 0) * RM(2, 1) - RM(2, 0) * RM(0, 1)) * (RM(1, 2) * RM(3, 3) - RM(3, 2) * RM(1, 3))
           + (RM(0, 0) * RM(3, 1) - RM(3, 0) * RM(0, 1)) * (RM(1, 2) * RM(2, 3) - RM(2, 2) * RM(1, 3))
           + (RM(1, 0) * RM(2, 1) - RM(2, 0) * RM(1, 1)) * (RM(0, 2) * RM(3, 3) - RM(3, 2) * RM(0, 3))
           - (RM(1, 0) * RM(3, 1) - RM(3, 0) * RM(1, 1)) * (RM(0, 2) * RM(2, 3) - RM(2, 2) * RM(0, 3))
           + (RM(2, 0) * RM(3, 1) - RM(3, 0) * RM(2, 1)) * (RM(0, 2) * RM(1, 3) - RM(1, 2) * RM(0, 3));
#undef RM
  return det;
}

template<typename _Type>
bool TMatrix44<_Type>::set_inversed(const Self& R) {
#define RM(x, y) R.v[x+y*4]
#define MM(x, y) v[x+y*4]
  Type det = R.get_determinant();
  if (mathc::abs(det)<mathc::small_value2(&det))
          return false; // too small

  det = 1/det;

  MM(0, 0) = det * (RM(1, 1) * (RM(2, 2) * RM(3, 3) - RM(3, 2) * RM(2, 3)) + RM(2, 1) * (RM(3, 2) * RM(1, 3) - RM(1, 2) * RM(3, 3)) + RM(3, 1) * (RM(1, 2) * RM(2, 3) - RM(2, 2) * RM(1, 3)));
  MM(1, 0) = det * (RM(1, 2) * (RM(2, 0) * RM(3, 3) - RM(3, 0) * RM(2, 3)) + RM(2, 2) * (RM(3, 0) * RM(1, 3) - RM(1, 0) * RM(3, 3)) + RM(3, 2) * (RM(1, 0) * RM(2, 3) - RM(2, 0) * RM(1, 3)));
  MM(2, 0) = det * (RM(1, 3) * (RM(2, 0) * RM(3, 1) - RM(3, 0) * RM(2, 1)) + RM(2, 3) * (RM(3, 0) * RM(1, 1) - RM(1, 0) * RM(3, 1)) + RM(3, 3) * (RM(1, 0) * RM(2, 1) - RM(2, 0) * RM(1, 1)));
  MM(3, 0) = det * (RM(1, 0) * (RM(3, 1) * RM(2, 2) - RM(2, 1) * RM(3, 2)) + RM(2, 0) * (RM(1, 1) * RM(3, 2) - RM(3, 1) * RM(1, 2)) + RM(3, 0) * (RM(2, 1) * RM(1, 2) - RM(1, 1) * RM(2, 2)));
  MM(0, 1) = det * (RM(2, 1) * (RM(0, 2) * RM(3, 3) - RM(3, 2) * RM(0, 3)) + RM(3, 1) * (RM(2, 2) * RM(0, 3) - RM(0, 2) * RM(2, 3)) + RM(0, 1) * (RM(3, 2) * RM(2, 3) - RM(2, 2) * RM(3, 3)));
  MM(1, 1) = det * (RM(2, 2) * (RM(0, 0) * RM(3, 3) - RM(3, 0) * RM(0, 3)) + RM(3, 2) * (RM(2, 0) * RM(0, 3) - RM(0, 0) * RM(2, 3)) + RM(0, 2) * (RM(3, 0) * RM(2, 3) - RM(2, 0) * RM(3, 3)));
  MM(2, 1) = det * (RM(2, 3) * (RM(0, 0) * RM(3, 1) - RM(3, 0) * RM(0, 1)) + RM(3, 3) * (RM(2, 0) * RM(0, 1) - RM(0, 0) * RM(2, 1)) + RM(0, 3) * (RM(3, 0) * RM(2, 1) - RM(2, 0) * RM(3, 1)));
  MM(3, 1) = det * (RM(2, 0) * (RM(3, 1) * RM(0, 2) - RM(0, 1) * RM(3, 2)) + RM(3, 0) * (RM(0, 1) * RM(2, 2) - RM(2, 1) * RM(0, 2)) + RM(0, 0) * (RM(2, 1) * RM(3, 2) - RM(3, 1) * RM(2, 2)));
  MM(0, 2) = det * (RM(3, 1) * (RM(0, 2) * RM(1, 3) - RM(1, 2) * RM(0, 3)) + RM(0, 1) * (RM(1, 2) * RM(3, 3) - RM(3, 2) * RM(1, 3)) + RM(1, 1) * (RM(3, 2) * RM(0, 3) - RM(0, 2) * RM(3, 3)));
  MM(1, 2) = det * (RM(3, 2) * (RM(0, 0) * RM(1, 3) - RM(1, 0) * RM(0, 3)) + RM(0, 2) * (RM(1, 0) * RM(3, 3) - RM(3, 0) * RM(1, 3)) + RM(1, 2) * (RM(3, 0) * RM(0, 3) - RM(0, 0) * RM(3, 3)));
  MM(2, 2) = det * (RM(3, 3) * (RM(0, 0) * RM(1, 1) - RM(1, 0) * RM(0, 1)) + RM(0, 3) * (RM(1, 0) * RM(3, 1) - RM(3, 0) * RM(1, 1)) + RM(1, 3) * (RM(3, 0) * RM(0, 1) - RM(0, 0) * RM(3, 1)));
  MM(3, 2) = det * (RM(3, 0) * (RM(1, 1) * RM(0, 2) - RM(0, 1) * RM(1, 2)) + RM(0, 0) * (RM(3, 1) * RM(1, 2) - RM(1, 1) * RM(3, 2)) + RM(1, 0) * (RM(0, 1) * RM(3, 2) - RM(3, 1) * RM(0, 2)));
  MM(0, 3) = det * (RM(0, 1) * (RM(2, 2) * RM(1, 3) - RM(1, 2) * RM(2, 3)) + RM(1, 1) * (RM(0, 2) * RM(2, 3) - RM(2, 2) * RM(0, 3)) + RM(2, 1) * (RM(1, 2) * RM(0, 3) - RM(0, 2) * RM(1, 3)));
  MM(1, 3) = det * (RM(0, 2) * (RM(2, 0) * RM(1, 3) - RM(1, 0) * RM(2, 3)) + RM(1, 2) * (RM(0, 0) * RM(2, 3) - RM(2, 0) * RM(0, 3)) + RM(2, 2) * (RM(1, 0) * RM(0, 3) - RM(0, 0) * RM(1, 3)));
  MM(2, 3) = det * (RM(0, 3) * (RM(2, 0) * RM(1, 1) - RM(1, 0) * RM(2, 1)) + RM(1, 3) * (RM(0, 0) * RM(2, 1) - RM(2, 0) * RM(0, 1)) + RM(2, 3) * (RM(1, 0) * RM(0, 1) - RM(0, 0) * RM(1, 1)));
  MM(3, 3) = det * (RM(0, 0) * (RM(1, 1) * RM(2, 2) - RM(2, 1) * RM(1, 2)) + RM(1, 0) * (RM(2, 1) * RM(0, 2) - RM(0, 1) * RM(2, 2)) + RM(2, 0) * (RM(0, 1) * RM(1, 2) - RM(1, 1) * RM(0, 2)));
#undef RM
#undef MM
  return true;
}

//***************************************
// template<typename _Type> struct TMatrix33
//

template<typename _Type>
void TMatrix33<_Type>::setZ(void) {
  v[ 0]=0; v[ 1]=0; v[ 2]=0;
  v[ 3]=0; v[ 4]=0; v[ 5]=0;
  v[ 6]=0; v[ 7]=0; v[ 8]=0;
}

template<typename _Type>
void TMatrix33<_Type>::setE(_Type M) {
  v[ 0]=M; v[ 1]=0; v[ 2]=0;
  v[ 3]=0; v[ 4]=M; v[ 5]=0;
  v[ 6]=0; v[ 7]=0; v[ 8]=M;
}

template<typename _Type>
void TMatrix33<_Type>::mult(const Self& L, const Self& R) {
  v[0*3+0] = _scalar3s(L.v+0*3, R.v+0, 3);
  v[0*3+1] = _scalar3s(L.v+0*3, R.v+1, 3);
  v[0*3+2] = _scalar3s(L.v+0*3, R.v+2, 3);
  v[1*3+0] = _scalar3s(L.v+1*3, R.v+0, 3);
  v[1*3+1] = _scalar3s(L.v+1*3, R.v+1, 3);
  v[1*3+2] = _scalar3s(L.v+1*3, R.v+2, 3);
  v[2*3+0] = _scalar3s(L.v+2*3, R.v+0, 3);
  v[2*3+1] = _scalar3s(L.v+2*3, R.v+1, 3);
  v[2*3+2] = _scalar3s(L.v+2*3, R.v+2, 3);
}


template<typename _Type>
_Type TMatrix33<_Type>::get_determinant(void) const {
#define RM(y, x) v[(x-1)+(y-1)*3]
  Type det = RM(1,1)*(RM(2,2)*RM(3,3)-RM(2,3)*RM(3,2))
           + RM(2,1)*(RM(3,2)*RM(1,3)-RM(1,2)*RM(3,3))
           + RM(3,1)*(RM(1,2)*RM(2,3)-RM(2,2)*RM(1,3));
#undef RM
  return det;
}

template<typename _Type>
bool TMatrix33<_Type>::set_inversed(const Self& R) {
#define RM(y, x) R.v[(x-1)+(y-1)*3]
#define MM(x, y) v[x+y*3]
  Type det = R.get_determinant();
  if (mathc::abs(det)<mathc::small_value(&det))
          return false; // too small

  det = 1/det;

  MM(0, 0) = det * (RM(2,2)*RM(3,3)-RM(2,3)*RM(3,2));
  MM(1, 0) = det * (RM(1,3)*RM(3,2)-RM(1,2)*RM(3,3));
  MM(2, 0) = det * (RM(1,2)*RM(2,3)-RM(1,3)*RM(2,2));
  MM(0, 1) = det * (RM(2,3)*RM(3,1)-RM(2,1)*RM(3,3));
  MM(1, 1) = det * (RM(1,1)*RM(3,3)-RM(1,3)*RM(3,1));
  MM(2, 1) = det * (RM(1,3)*RM(2,1)-RM(1,1)*RM(2,3));
  MM(0, 2) = det * (RM(2,1)*RM(3,2)-RM(2,2)*RM(3,1));
  MM(1, 2) = det * (RM(1,2)*RM(3,1)-RM(1,1)*RM(3,2));
  MM(2, 2) = det * (RM(1,1)*RM(2,2)-RM(1,2)*RM(2,1));
#undef RM
#undef MM
  return true;
}

//***************************************
} // namespace math3d
} // namespace NanoUTL

#endif // __NANOUTL_TYPES_MATH_3DF_H
