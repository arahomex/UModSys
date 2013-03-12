#ifndef __NANOUTL_TYPES_MATH_3D_H
#define __NANOUTL_TYPES_MATH_3D_H 1
//*****************************************************
// nanoutl/types/math_3d.h: common 3d & 2d mathematics used 
//                          in renderers etc.
//*****************************************************

#include <nanoutl/types/math_c.h>

namespace NanoUTL {
namespace math3d {

using namespace mathc;

//***************************************
//***************************************

template<typename _Type> struct TVector4;
template<typename _Type> struct TVector3;
template<typename _Type> struct TVector2;

template<typename _Type> struct TQuaternion4;

template<typename _Type> struct TMatrix44;
template<typename _Type> struct TMatrix33;
template<typename _Type> struct TMatrix34;

template<typename _Type> struct TBoundBox3;
template<typename _Type> struct TBoundBox2;

template<typename _Type> struct TPlane3;

//***************************************

unsigned int one_bit_container(unsigned int value);

//***************************************
//***************************************

template<typename _Type>
inline _Type _scalar4s(const _Type* L, const _Type* R, int stride) {
  return L[0]*R[0] + L[1]*R[stride] + L[2]*R[stride<<1] + L[3]*R[(stride<<1)+stride];
}

template<typename _Type>
inline _Type _scalar3s(const _Type* L, const _Type* R, int stride) {
  return L[0]*R[0] + L[1]*R[stride] + L[2]*R[stride<<1];
}

template<typename _Type>
inline _Type _scalar4(const _Type* L, const _Type* R) {
  return L[0]*R[0] + L[1]*R[1] + L[2]*R[2] + L[3]*R[3];
}

template<typename _Type>
inline _Type _scalar3(const _Type* L, const _Type* R) {
  return L[0]*R[0] + L[1]*R[1] + L[2]*R[2];
}

template<typename _Type1, typename _Type2>
inline _Type1 _scalar3t(const _Type1* L, const _Type2* R) {
  return L[0]*R[0] + L[1]*R[1] + L[2]*R[2];
}

template<typename _Type>
inline _Type _norma3s(const _Type* L, int stride) {
  return L[0]*L[0] + L[stride]*L[stride] + L[stride<<1]*L[stride<<1];
}

//***************************************

template<typename _Type>
struct TVector4 {
  typedef _Type Type;
  //
  Type v[4];
  //
  inline TVector4(void) {}
  inline TVector4(Type a1, Type a2, Type a3, Type a4)
    { v[0] = a1; v[1] = a2; v[2] = a3; v[3] = a4; }
  inline TVector4(Type a)
    { v[0] = v[1] = v[2] = v[3] = a; }
  inline TVector4(const Type *a)
    { v[0] = a[0]; v[1] = a[1]; v[2] = a[2]; v[3] = a[3]; }
  //
  inline const _Type& operator()(int c) const { return v[c]; }
  inline const _Type& operator[](int c) const { return v[c]; }
  inline _Type& operator[](int c) { return v[c]; }
  inline void set1(int c, _Type sv) { v[c] = sv; }
  //
  inline void set(Type a1, Type a2, Type a3, Type a4)
    { v[0] = a1; v[1] = a2; v[2] = a3; v[3] = a4; }
  inline void set(Type a)
    { v[0] = a; v[1] = a; v[2] = a; v[3] = a; }
  inline void set4(Type a)
    { v[3] = a; }
  inline void setp(const Type* a)
    { v[0] = a[0]; v[1] = a[1]; v[2] = a[2]; v[3] = a[3]; }
  inline void setp3(const Type* a)
    { v[0] = a[0]; v[1] = a[1]; v[2] = a[2]; }
  //
  inline _Type scalar(const TVector4& R) const
    { return v[0]*R.v[0] + v[1]*R.v[1] + v[2]*R.v[2] + v[3]*R.v[3]; }
  inline _Type scalar(const _Type* R) const
    { return v[0]*R[0] + v[1]*R[1] + v[2]*R[2] + v[3]*R[3]; }
  inline _Type scalar(const _Type* R, int stride) const
    { return v[0]*R[0] + v[1]*R[1*stride] + v[2]*R[2*stride] + v[3]*R[3*stride]; }
  //
  inline TVector4 scale(const TVector4 &R) const { return TVector4(v[0]*R.v[0], v[1]*R.v[1], v[2]*R.v[2], v[3]*R.v[3]); }
  //
  inline TVector4 cross(const TVector4 &R) {
    return TVector4(_Type(0)); // !TODO!
  }
  //
  inline TVector4 normalized(_Type mult) const {
    Type k = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);
    if(k>mathc::small_value(&k)) {
      k = mult/k;
      return TVector4(v[0]*k, v[1]*k, v[2]*k, v[3]*k);
    }
    return TVector4(Type(0));
  }
  //
  inline _Type qdist(const TVector4& R) const { 
    _Type a1=v[0]-R.v[0], a2=v[1]-R.v[1], a3=v[2]-R.v[2], a4=v[3]-R.v[3]; 
    return a1*a1 + a2*a2 + a3*a3 + a4*a4;
  }
  inline _Type dist(const TVector4& R) const 
    { return sqrt(qdist(R)); }
  inline _Type norma(void) const
    { return v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]; }
  inline _Type len(void) const
    { return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]); }
  //
  inline TVector4 operator-(void) const // UNA
    { return TVector4(-v[0], -v[1], -v[2], -v[3]); }
  //
  inline TVector4 operator+(const TVector4& R) const
    { return TVector4(v[0]+R.v[0], v[1]+R.v[1], v[2]+R.v[2], v[3]+R.v[3]); }
  inline TVector4 operator-(const TVector4& R) const
    { return TVector4(v[0]-R.v[0], v[1]-R.v[1], v[2]-R.v[2], v[3]+R.v[3]); }
  inline TVector4 operator*(_Type R) const
    { return TVector4(v[0]*R, v[1]*R, v[2]*R, v[3]*R); }
  inline TVector4 operator/(_Type R) const
    { return TVector4(v[0]/R, v[1]/R, v[2]/R, v[3]/R); }
  //
  inline const TVector4& operator+=(const TVector4& R)
    { v[0] += R.v[0]; v[1] += R.v[1]; v[2] += R.v[2]; v[3] += R.v[3]; return *this; }
  inline const TVector4& operator-=(const TVector4& R)
    { v[0] -= R.v[0]; v[1] -= R.v[1]; v[2] -= R.v[2]; v[3] -= R.v[3]; return *this; }
  inline const TVector4& operator*=(_Type R)
    { v[0] *= R; v[1] *= R; v[2] *= R; v[3] *= R; return *this; }
  inline const TVector4& operator/=(_Type R)
    { v[0] /= R; v[1] /= R; v[2] /= R; v[3] /= R; return *this; }
};

//***************************************

template<typename _Type>
struct TVector3 {
  typedef _Type Type;
  //
  Type v[3];
  //
  inline TVector3(void) {}
  inline TVector3(Type a1, Type a2, Type a3)
    { v[0] = a1; v[1] = a2; v[2] = a3; }
  inline TVector3(Type a)
    { v[0] = v[1] = v[2] = a; }
  inline TVector3(const Type *a)
    { v[0] = a[0]; v[1] = a[1]; v[2] = a[2]; }
  //
  inline const _Type& operator()(int c) const { return v[c]; }
  inline const _Type& operator[](int c) const { return v[c]; }
  inline _Type& operator[](int c) { return v[c]; }
  inline void set1(int c, _Type sv) { v[c] = sv; }
  inline bool operator==(const TVector3 &R) const { return v[0]==R.v[0] && v[1]==R.v[1] && v[2]==R.v[2]; }
  inline bool operator!=(const TVector3 &R) const { return !(*this==R); }
  //
  inline void set(Type a1, Type a2, Type a3)
    { v[0] = a1; v[1] = a2; v[2] = a3; }
  inline void set(Type a)
    { v[0] = a; v[1] = a; v[2] = a; }
  inline void setp(const Type *a)
    { v[0] = a[0]; v[1] = a[1]; v[2] = a[2]; }
  inline void mult(Type k)
    { v[0] *= k; v[1] *= k; v[2] *= k; }
  //
  inline void set_scale(const TVector3& L, const TVector3& R) 
    { v[0] = L.v[0]*R.v[0]; v[1] = L.v[1]*R.v[1]; v[2] = L.v[2]*R.v[2]; }
  inline void set_scale(const TVector3& L, const TVector3& R, const TVector3& add) 
    { v[0] = L.v[0]*R.v[0]+add.v[0]; v[1] = L.v[1]*R.v[1]+add.v[1]; v[2] = L.v[2]*R.v[2]+add.v[2]; }
  //
  inline _Type scalar(const TVector3& R) const
    { return v[0]*R.v[0] + v[1]*R.v[1] + v[2]*R.v[2]; }
  inline _Type scalar(const _Type* R) const
    { return v[0]*R[0] + v[1]*R[1] + v[2]*R[2]; }
  inline _Type scalar(const _Type* R, int stride) const
    { return v[0]*R[0] + v[1]*R[1*stride] + v[2]*R[2*stride]; }
  //
  inline TVector3 scale(const TVector3 &R) const { return TVector3(v[0]*R.v[0], v[1]*R.v[1], v[2]*R.v[2]); }
  //
  inline TVector3 cross(const TVector3& R) const
    { return TVector3(v[1]*R.v[2] - v[2]*R.v[1], v[2]*R.v[0] - v[0]*R.v[2], v[0]*R.v[1] - v[1]*R.v[0]); }
  inline TVector3 cross(const _Type* R) const
    { return TVector3(v[1]*R[2] - v[2]*R[1], v[2]*R[0] - v[0]*R[2], v[0]*R[1] - v[1]*R[0]); }
  inline void cross(const TVector3& L, const TVector3& R)
    { v[0]=L.v[1]*R.v[2]-L.v[2]*R.v[1]; v[1]=L.v[2]*R.v[0]-L.v[0]*R.v[2]; v[2]=L.v[0]*R.v[1]-L.v[1]*R.v[0]; }
  inline void cross(const TVector3& L, const TVector3& R, bool revert) { 
    if(revert) {
      v[0]=L.v[2]*R.v[1]-L.v[1]*R.v[2]; v[1]=L.v[0]*R.v[2]-L.v[2]*R.v[0]; v[2]=L.v[1]*R.v[0]-L.v[0]*R.v[1]; 
    } else {
      v[0]=L.v[1]*R.v[2]-L.v[2]*R.v[1]; v[1]=L.v[2]*R.v[0]-L.v[0]*R.v[2]; v[2]=L.v[0]*R.v[1]-L.v[1]*R.v[0]; 
    }
  }
  //
  inline void set_min(TVector3& min) const {
    if(v[0]<min.v[0]) min.v[0] = v[0];
    if(v[1]<min.v[1]) min.v[1] = v[1];
    if(v[2]<min.v[2]) min.v[2] = v[2];
  }
  inline void set_max(TVector3& max) const {
    if(v[0]>max.v[0]) max.v[0] = v[0];
    if(v[1]>max.v[1]) max.v[1] = v[1];
    if(v[2]>max.v[2]) max.v[2] = v[2];
  }
  inline void set_minmax(TVector3& min, TVector3& max) const {
    set_min(min); set_max(max);
  }
  //
  TVector3 rotate_about(const TVector3& ort, _Type angle) const;
  //
  inline TVector3 operator-(void) const // UNA
    { return TVector3(-v[0], -v[1], -v[2]); }
  inline TVector3 operator+(const TVector3& R) const
    { return TVector3(v[0]+R.v[0], v[1]+R.v[1], v[2]+R.v[2]); }
  inline TVector3 operator-(const TVector3& R) const
    { return TVector3(v[0]-R.v[0], v[1]-R.v[1], v[2]-R.v[2]); }
  inline TVector3 operator*(_Type R) const
    { return TVector3(v[0]*R, v[1]*R, v[2]*R); }
  inline TVector3 operator/(_Type R) const
    { return TVector3(v[0]/R, v[1]/R, v[2]/R); }
  //
  inline const TVector3& operator+=(const TVector3& R)
    { v[0] += R.v[0]; v[1] += R.v[1]; v[2] += R.v[2]; return *this; }
  inline const TVector3& operator-=(const TVector3& R)
    { v[0] -= R.v[0]; v[1] -= R.v[1]; v[2] -= R.v[2]; return *this; }
  inline const TVector3& operator*=(_Type R)
    { v[0] *= R; v[1] *= R; v[2] *= R; return *this; }
  inline const TVector3& operator/=(_Type R)
    { v[0] /= R; v[1] /= R; v[2] /= R; return *this; }
  //
  inline _Type qdist(const TVector3& R) const { 
    _Type a1=v[0]-R.v[0], a2=v[1]-R.v[1], a3=v[2]-R.v[2]; 
    return a1*a1 + a2*a2 + a3*a3;
  }
  inline _Type dist(const TVector3& R) const 
    { return sqrt(qdist(R)); }
  inline _Type norma(void) const
    { return v[0]*v[0] + v[1]*v[1] + v[2]*v[2]; }
  inline _Type len(void) const
    { return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]); }
  inline TVector3 normalized(void) const {
    Type k = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if(k>mathc::small_value(&k)) {
      k = 1/k;
      return TVector3(v[0]*k, v[1]*k, v[2]*k);
    }
    return TVector3(Type(0));
  }
  inline TVector3 normalized(_Type mult) const {
    Type k = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if(k>mathc::small_value(&k)) {
      k = mult/k;
      return TVector3(v[0]*k, v[1]*k, v[2]*k);
    }
    return TVector3(Type(0));
  }
  //
  inline _Type normalize(void) {
    Type k = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if(k>mathc::small_value(&k)) {
      mult(1/k); return k;
    } else {
      set(0, 0, 0); return 0;
    }
  }
};

//***************************************

template<typename _Type>
struct TVector2 {
  typedef _Type Type;
  //
  Type v[2];
  //
  inline TVector2(void) {}
  inline TVector2(Type a1, Type a2)
    { v[0] = a1; v[1] = a2; }
  inline TVector2(Type a)
    { v[0] = v[1] = a; }
  inline TVector2(const Type *a)
    { v[0] = a[0]; v[1] = a[1]; }
  //
  inline const _Type& operator()(int c) const { return v[c]; }
  inline const _Type& operator[](int c) const { return v[c]; }
  inline _Type& operator[](int c) { return v[c]; }
  inline void set1(int c, _Type sv) { v[c] = sv; }
  inline bool operator==(const TVector2 &R) const { return v[0]==R.v[0] && v[1]==R.v[1]; }
  inline bool operator!=(const TVector2 &R) const { return !(*this==R); }
  //
  inline void set(Type a1, Type a2)
    { v[0] = a1; v[1] = a2; }
  inline void set(Type a1)
    { v[0] = a1; v[1] = a1; }
  inline void setp(const Type* a)
    { v[0] = a[0]; v[1] = a[1]; }
  inline void mult(Type k)
    { v[0] *= k; v[1] *= k; }
  inline void mult(Type k1, Type k2)
    { v[0] *= k1; v[1] *= k2; }
  inline void mult(const TVector2& base, Type k1, Type k2)
    { v[0] = base.v[0]*k1; v[1] = base.v[1]*k2; }
  inline void mult(const TVector2& base, const TVector2& k)
    { v[0] = base.v[0]*k.v[0]; v[1] = base.v[1]*k.v[1]; }
  //
  inline void set_min(TVector2& min) const {
    if(v[0]<min.v[0]) min.v[0] = v[0];
    if(v[1]<min.v[1]) min.v[1] = v[1];
  }
  inline void set_max(TVector2& max) const {
    if(v[0]>max.v[0]) max.v[0] = v[0];
    if(v[1]>max.v[1]) max.v[1] = v[1];
  }
  inline void set_minmax(TVector2& min, TVector2& max) const {
    set_min(min); set_max(max);
  }
  //
  inline TVector2 operator-(void) const // UNA
    { return TVector2(-v[0], -v[1]); }
  inline TVector2 operator+(const TVector2& R) const
    { return TVector2(v[0]+R.v[0], v[1]+R.v[1]); }
  inline TVector2 operator-(const TVector2& R) const
    { return TVector2(v[0]-R.v[0], v[1]-R.v[1]); }
  inline TVector2 operator*(_Type R) const
    { return TVector2(v[0]*R, v[1]*R); }
  inline TVector2 operator/(_Type R) const
    { return TVector2(v[0]/R, v[1]/R); }
  //
  inline const TVector2& operator+=(const TVector2& R)
    { v[0] += R.v[0]; v[1] += R.v[1]; return *this; }
  inline const TVector2& operator-=(const TVector2& R)
    { v[0] -= R.v[0]; v[1] -= R.v[1]; return *this; }
  inline const TVector2& operator*=(_Type R)
    { v[0] *= R; v[1] *= R; return *this; }
  inline const TVector2& operator/=(_Type R)
    { v[0] /= R; v[1] /= R; return *this; }
  //
  inline TVector2 scale(const TVector2 &R) const { return TVector2(v[0]*R.v[0], v[1]*R.v[1]); }
  //
  inline _Type scalar(const TVector2& R) const
    { return v[0]*R.v[0] + v[1]*R.v[1]; }
  inline TVector2 cross(const TVector2& R) const
    { return TVector2(v[0]*R.v[1], v[1]*R.v[0]); }
  //
  inline _Type qdist(const TVector2& R) const { 
    _Type a1=v[0]-R.v[0], a2=v[1]-R.v[1]; 
    return a1*a1 + a2*a2;
  }
  inline _Type dist(const TVector2& R) const 
    { return sqrt(qdist(R)); }
  inline _Type norma(void) const
    { return v[0]*v[0] + v[1]*v[1]; }
  inline _Type len(void) const
    { return sqrt(v[0]*v[0] + v[1]*v[1]); }
  inline void normalize(void) {
    Type k = sqrt(v[0]*v[0] + v[1]*v[1]);
    if(k>small_value(&k)) {
      mult(1/k);
    } else {
      set(0, 0);
    }
  }
  inline TVector2 normalized(_Type mult) const {
    Type k = sqrt(v[0]*v[0] + v[1]*v[1]);
    if(k>mathc::small_value(&k)) {
      k = mult/k;
      return TVector2(v[0]*k, v[1]*k);
    }
    return TVector2(Type(0));
  }
  //
};

//***************************************

template<typename _Type>
struct TQuaternion4 {
  typedef _Type Type;
  typedef TQuaternion4<_Type> Self;
  //
  Type v[4];
  //
  inline TQuaternion4(void) {}
  inline TQuaternion4(Type a1, Type a2, Type a3, Type a4)
    { v[0] = a1; v[1] = a2; v[2] = a3; v[3] = a4; }
  inline TQuaternion4(Type a)
    { v[0] = v[1] = v[2] = a[3] = a; }
  inline TQuaternion4(const Type *a)
    { v[0] = a[0]; v[1] = a[1]; v[2] = a[2]; v[3] = a[3]; }
  //
  inline void set1(int c, _Type sv) { v[c] = sv; }
  //
  inline void set(Type a1, Type a2, Type a3, Type a4)
    { v[0] = a1; v[1] = a2; v[2] = a3; v[3] = a4; }
  //
  inline Self operator+(const Self& R) const { return Self(v[0]+R.v[0], v[1]+R.v[1], v[2]+R.v[2], v[3]+R.v[3]); }
  inline Self operator-(const Self& R) const { return Self(v[0]-R.v[0], v[1]-R.v[1], v[2]-R.v[2], v[3]-R.v[3]); }
  inline Self operator*(const Type& R) const { return Self(v[0]*R, v[1]*R, v[2]*R, v[3]*R); }
  inline Self operator/(const Type& R) const { return Self(v[0]/R, v[1]/R, v[2]/R, v[3]/R); }
  //
  inline const Self& operator+=(const Self& R) { v[0]+=R.v[0]; v[1]+=R.v[1]; v[2]+=R.v[2]; v[3]+=R.v[3]; return *this; }
  inline const Self& operator-=(const Self& R) { v[0]-=R.v[0]; v[1]-=R.v[1]; v[2]-=R.v[2]; v[3]-=R.v[3]; return *this; }
  inline const Self& operator*=(const Type& R) { v[0]*=R; v[1]*=R; v[2]*=R; v[3]*=R; return *this; }
  inline const Self& operator/=(const Type& R) { v[0]/=R; v[1]/=R; v[2]/=R; v[3]/=R; return *this; }
  //
  inline void set_mult(const Self& A, const Self& B) {
    v[0] = A.v[0]*B.v[0] - A.v[1]*B.v[1] - A.v[2]*B.v[2] - A.v[3]*B.v[3];
    v[1] = A.v[1]*B.v[0] + A.v[0]*B.v[1] - A.v[3]*B.v[2] + A.v[2]*B.v[3];
    v[2] = A.v[2]*B.v[0] + A.v[3]*B.v[1] + A.v[0]*B.v[2] - A.v[1]*B.v[3];
    v[3] = A.v[3]*B.v[0] - A.v[2]*B.v[1] - A.v[1]*B.v[2] + A.v[0]*B.v[3];
  }
  //
  Self operator*(const Self& R) const;
  //
  inline const Type& operator()(int idx) const { return v[idx]; }
  inline const Type& operator[](int idx) const { return v[idx]; }
  inline Type& operator[](int idx) { return v[idx]; }
  //
  inline _Type qnorma(void) const { return v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]; }
  inline _Type norma(void) const { return sqrt(qnorma()); }
  //
  inline void normalize(void) {
    _Type n = norma();
    if(n>mathc::small_value(&n)) {
      v[0] /= n; v[1] /= n; v[2] /= n; v[3] /= n;
    }
  }
  inline Self normalized(void) {
    _Type n = 1/norma(); if(n<mathc::small_value(&n)) return Self(1,0,0,0);
    return Self(v[0]*n, v[1]*n, v[2]*n, v[3]*n);
  }
  //
  inline void invert(void) {
    _Type n = v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3];
    v[0] /= n;
    v[1] /= -n;
    v[2] /= -n;
    v[3] /= -n;
  }
  inline void conjugate(void) {
    v[1] *= -1;
    v[2] *= -1;
    v[3] *= -1;
  }
  //
  inline void set_rotate(const TVector3<_Type>& v) { set_rotate(v.v); }
  void set_rotate(const _Type* vv);
  inline void get_rotate(const TVector3<_Type>& v) const { set_rotate(v.v); }
  void get_rotate(_Type* vv) const;
};

//***************************************

template<typename _Type>
struct TMatrix44 {
  typedef _Type Type;
  typedef TMatrix44<_Type> Self;
  typedef TVector4<_Type> Vector4;
  typedef TVector3<_Type> Vector3;
  //
  Type v[4*4];
  //
  inline const _Type& operator()(int r, int c) const { return v[r*4+c]; }
  inline void set1(int r, int c, _Type sv) { v[r*4+c] = sv; }
  //
  inline TMatrix44(void) {}
  inline TMatrix44(Type a)
    { set_array(v, 4*4, a); }
  inline TMatrix44(const Type *a)
    { memcpy(v, a, sizeof(_Type)*4*4); }
  inline TMatrix44(Type ar, Type at)
    { set_ERT(ar, at); }
  inline TMatrix44(bool R, bool T) 
    {  if(R) set_ER(1); if(T) set_ET(1); }
  //
  void setZ(void);
  void setE(_Type M=1);
  void setE3(_Type M=1);
  //
  inline void set_row(int r, const _Type *vv) {
    _Type* x = v+r*4; x[0] = vv[0]; x[1] = vv[1]; x[2] = vv[2]; x[3] = vv[3];
  }
  inline void set_row(int r, _Type a1, _Type a2, _Type a3, _Type a4=0) { 
    _Type* x = v+r*4; x[0] = a1; x[1] = a2; x[2] = a3; x[3] = a4;
  }
  inline TVector4<_Type> get_row(int r) const { return TVector4<_Type>(v+r*4); }
  //
  //
  inline void mult(TVector4<_Type>& V, const TVector4<_Type>& R) const { 
    V.v[0] = R.scalar(v+0, 4);
    V.v[1] = R.scalar(v+1, 4);
    V.v[2] = R.scalar(v+2, 4);
    V.v[3] = R.scalar(v+3, 4);
  }
  inline void mult(TVector3<_Type>& V, const TVector3<_Type>& R) const { 
    V.v[0] = R.scalar(v+0, 4) + R[4*3+0];
    V.v[1] = R.scalar(v+1, 4) + R[4*3+1];
    V.v[2] = R.scalar(v+2, 4) + R[4*3+2];
  }
  //
  void mult(const Self& L, const Self& R);
  void mult(const Self& L, const Self& R, Type Rk);
  void mult3(const Self& L, const Self& R); // only 3x3 up matrix
  void mult(const Self& L, Type Rk);
  //
  Self operator*(const Self& R) const { Self rv; rv.mult(*this, R); return rv; }
  Self operator*(const _Type& R) const { Self rv; rv.mult(*this, R); return rv; }
  inline void operator*=(TMatrix44 &R) {
    TMatrix44 M = *this; mult(M, R);
  }
  //
  void set_rotate(const TVector3<_Type>& R);
  void set_rotate(_Type a1, _Type a2, _Type a3);
  void set_rotateT(const TVector3<_Type>& R);
  void set_rotate_about(const TVector3<_Type>& ort, _Type angle);
  void set_rotate_aboutT(const TVector3<_Type>& ort, _Type angle);
  void set_rotate(const TQuaternion4<_Type>& q);
  void set_rotateT(const TQuaternion4<_Type>& q);
  bool set_rotate_orts(const TVector3<_Type> &ort1, int nort1, const TVector3<_Type> &ort2, int nort2, const TVector3<_Type> &ort3, int nort3); 
  bool set_rotate_ortsT(const TVector3<_Type> &ort1, int nort1, const TVector3<_Type> &ort2, int nort2, const TVector3<_Type> &ort3, int nort3);
  //
  void add_rotate(const TVector3<_Type>& R);
  void add_rotate(_Type a1, _Type a2, _Type a3);
  void add_rotateT(const TVector3<_Type>& R);
  void add_rotate_about(const TVector3<_Type>& ort, _Type angle);
  void add_rotate_aboutT(const TVector3<_Type>& ort, _Type angle);
  void add_rotate(const TQuaternion4<_Type>& q);
  void add_rotateT(const TQuaternion4<_Type>& q);
  //
  TVector3<_Type> get_rotate_dir(int ort, _Type mult=1) { return mult3_ort(1, ort).normalized(mult); }
  TQuaternion4<_Type> get_quaternion(void) const;
  TQuaternion4<_Type> get_quaternionT(void) const;
  //
  void set_ERT(_Type e, _Type t=1);
  void set_ET(_Type t=1);
  void set_ER(_Type e);
  void set_scale(const TVector3<_Type>& R);
  void add_scale(const TVector3<_Type>& R);
  void add_scale(_Type R);
  //
  bool is_equal(const Self &R, Type eps=0) const {
#define MM(x) mathc::abs(v[x] - R.v[x])
  return (MM( 0) + MM( 1) + MM( 2) + MM( 3)
        + MM( 4) + MM( 5) + MM( 6) + MM( 7)
        + MM( 8) + MM( 9) + MM(10) + MM(11)
        + MM(12) + MM(13) + MM(14) + MM(15))<=eps;
#undef MM
  }
  bool is_equal(const Self &R, Type eps3, Type eps4) const {
#define MM(x) mathc::abs(v[x] - R.v[x])
  return (MM( 0) + MM( 1) + MM( 2)
        + MM( 4) + MM( 5) + MM( 6)
        + MM( 8) + MM( 9) + MM(10))<=eps3
      && (MM( 3) + MM( 7) + MM(11)
        + MM(12) + MM(13) + MM(14) + MM(15))<=eps4;
#undef MM
  }
  //
  void set_ort3(const _Type* A, const _Type* B, const _Type* C);
  void set_ort3T(const _Type* A, const _Type* B, const _Type* C);
  //
  inline void set_ort3(const _Type* ort, int ortid) 
    { ortid *= 4; v[0+ortid] = ort[0]; v[1+ortid] = ort[1]; v[2+ortid] = ort[2]; }
  inline void set_ort3T(const _Type* ort, int ortid) 
    { v[0+ortid] = ort[0]; v[4+ortid] = ort[1]; v[8+ortid] = ort[2]; }
  //
  inline void set_ort3(const TVector3<_Type>& A, const TVector3<_Type>& B, const TVector3<_Type>& C)
    { set_ort3(A.v, B.v, C.v); }
  inline void set_ort3T(const TVector3<_Type>& A, const TVector3<_Type>& B, const TVector3<_Type>& C)
    { set_ort3T(A.v, B.v, C.v); }
  //
  inline void set_translate(const TVector3<_Type>& R) {
      v[3*4+0] = R.v[0];
      v[3*4+1] = R.v[1];
      v[3*4+2] = R.v[2];
      v[3*4+3] = 1;
  }
  inline void set_translate(_Type a1, _Type a2, _Type a3) {
      v[3*4+0] = a1;
      v[3*4+1] = a2;
      v[3*4+2] = a3;
      v[3*4+3] = 1;
  }
  inline void add_translate(const TVector3<_Type>& R) {
      v[3*4+0] += R.v[0];
      v[3*4+1] += R.v[1];
      v[3*4+2] += R.v[2];
  }
  inline void add_translate(_Type a1, _Type a2, _Type a3) {
      v[3*4+0] += a1;
      v[3*4+1] += a2;
      v[3*4+2] += a3;
  }
  //
  inline TVector3<_Type> get_translate(void) const {
      return TVector3<_Type>(v[3*4+0], v[3*4+1], v[3*4+2]);
  }
  inline _Type get_sscale(void) const {
      return sqrt((_norma3s(v+0, 4)+_norma3s(v+1, 4)+_norma3s(v+2, 4))/3);
  }
  inline TVector3<_Type> get_scale(void) const {
      return TVector3<_Type>( sqrt(_norma3s(v+0, 4)), 
                              sqrt(_norma3s(v+1, 4)), 
                              sqrt(_norma3s(v+2, 4)) );
  }
  //
  //
  _Type get_determinant(void) const;
  bool set_inversed(const Self& R);
  //
  inline TVector4<_Type> mult(const TVector4<_Type>& R) const { 
    return TVector4<_Type>(
      R.scalar(v+0, 4),
      R.scalar(v+1, 4),
      R.scalar(v+2, 4),
      R.scalar(v+3, 4)
    );
  }
  inline TVector3<_Type> mult(const TVector3<_Type>& R) const { 
    return TVector3<_Type>(
      R.scalar(v+0, 4) + v[4*3+0],
      R.scalar(v+1, 4) + v[4*3+1],
      R.scalar(v+2, 4) + v[4*3+2]
    );
  }
  //
  inline TVector3<_Type> mult3(const TVector3<_Type>& R) const { 
    return TVector3<_Type>(R.scalar(v+0, 4), R.scalar(v+1, 4), R.scalar(v+2, 4));
  }
  inline TVector3<_Type> mult3_ort(_Type a1, int ortid) const {
    ortid *= 4;
    return TVector3<_Type>( v[0+ortid]*a1, v[1+ortid]*a1, v[2+ortid]*a1 );
  }
  inline TVector3<_Type> mult3_ortv(const TVector3<_Type>& R, int ortid) const {
    return mult3_ort(R.v[ortid], ortid);
  }
  inline TVector3<_Type> mult3_ortv(const TVector3<_Type>& R, int ortid, _Type K) const {
    return mult3_ort(R.v[ortid]*K, ortid);
  }
};

//***************************************

template<typename _Type>
struct TMatrix33 {
  typedef _Type Type;
  typedef TMatrix33<_Type> Self;
  //
  Type v[3*3];
  //
  inline const _Type& operator()(int r, int c) const { return v[r*3+c]; }
  inline void set1(int r, int c, _Type sv) { v[r*3+c] = sv; }
  //
  inline TMatrix33(void) {}
  inline TMatrix33(Type a)
    { set_array(v, 3*3, a); }
  inline TMatrix33(const Type *a)
    { memcpy(v, a, sizeof(_Type)*3*3); }
  //
  void setZ(void);
  void setE(_Type M=1);
  //
  inline void set_row(int r, const _Type *vv) 
    { _Type* x = v+r*3; x[0] = vv[0]; x[1] = vv[1]; x[2] = vv[2];}
  inline void set_row(int r, _Type a1, _Type a2, _Type a3) 
    { _Type* x = v+r*3; x[0] = a1; x[1] = a2; x[2] = a3; }
  inline TVector3<_Type> get_row(int r) const { return TVector3<_Type>(v+r*3); }
  //
  inline void set_col(int c, const _Type *vv) 
    { _Type* x = v+c; x[0*3] = vv[0]; x[1*3] = vv[1]; x[2*3] = vv[2];}
  inline void set_col(int c, _Type a1, _Type a2, _Type a3) 
    { _Type* x = v+c; x[0*3] = a1; x[1*3] = a2; x[2*3] = a3;}
  //
  inline void mult(TVector3<_Type>& V, const TVector3<_Type>& R) const { 
    V.v[0] = R.scalar(v+0, 3);
    V.v[1] = R.scalar(v+1, 3);
    V.v[2] = R.scalar(v+2, 3);
  }
  inline TVector3<_Type> mult(const TVector3<_Type>& R) const 
    { return TVector3<_Type>( R.scalar(v+0, 3), R.scalar(v+1, 3), R.scalar(v+2, 3) ); }
  //
  void mult(const Self& L, const Self& R);
  //
  _Type get_determinant(void) const;
  bool set_inversed(const Self& R);
};

//***************************************

template<typename _Type> 
struct TBoundBox2 {
  typedef TVector2<_Type> Vec;
  Vec A, B;
  //
  inline TBoundBox2()
    {}
  inline TBoundBox2(const Vec &a, const Vec &b)
    : A(a), B(b) {}
  inline TBoundBox2(_Type x, _Type y, _Type dx, _Type dy)
    : A(x, y), B(x+dx, y+dy) {}
  inline TBoundBox2(const TBoundBox2& outer, const TBoundBox2& relative)
    : A(outer.A(0)+relative.A(0), outer.A(1)+relative.A(1)), 
      B(outer.A(0)+relative.B(0), outer.A(1)+relative.B(1)) {
    if(B(0)>outer.B(0)) B[0] = outer.B(0);
    if(B(1)>outer.B(1)) B[1] = outer.B(1);
  }
  //
  inline void set(bool tmp) { A.set(0,0); B.set(0,0); }
  inline void set(const Vec &a, const Vec &b) { A=a; B=b; }
  inline void set(_Type x, _Type y, _Type dx, _Type dy) { A.set(x,y); B.set(x+dx, y+dy); }
  inline void set(const TBoundBox2& outer, const TBoundBox2& relative) { 
    A.set(outer.A(0)+relative.A(0), outer.A(1)+relative.A(1)); 
    B.set(outer.A(0)+relative.B(0), outer.A(1)+relative.B(1));
    if(B(0)>outer.B(0)) B[0] = outer.B(0);
    if(B(1)>outer.B(1)) B[1] = outer.B(1);
  }
  //
  inline TBoundBox2 relative(void) const { return TBoundBox2(Vec(0,0), B-A); }
  inline TBoundBox2 relative(const Vec &v) const { return TBoundBox2(v, v+B-A); }
  inline Vec size(void) const { return B-A; }
  inline _Type sizex(void) const { return B.v[0]-A.v[0]; }
  inline _Type sizey(void) const { return B.v[1]-A.v[1]; }
  inline _Type originx(void) const { return A.v[0]; }
  inline _Type originy(void) const { return A.v[1]; }
  //
  inline Vec get_point(bool x, bool y) { return Vec( x ? B.v[0] : A.v[0], y ? B.v[1] : A.v[1]); }
  //
  inline bool inside(const Vec &p) { 
    return p(0)>=A(0) && p(1)>=A(1) && p(0)<=B(0) && p(1)<=B(1); 
  }
};

template<typename _Type> 
struct TBoundBox3 {
  typedef TVector3<_Type> Vec;
  Vec A, B;
  //
  inline TBoundBox3()
    {}
  inline TBoundBox3(const Vec &a, const Vec &b)
    : A(a), B(b) {}
  inline TBoundBox3(_Type x, _Type y, _Type z, _Type dx, _Type dy, _Type dz)
    : A(x, y, z), B(x+dx, y+dy, z+dz) {}
  inline TBoundBox3(const TBoundBox3& outer, const TBoundBox3& relative)
    : A(outer.A(0)+relative.A(0), outer.A(1)+relative.A(1), outer.A(2)+relative.A(2)), 
      B(outer.A(0)+relative.B(0), outer.A(1)+relative.B(1), outer.A(2)+relative.B(2)) {
    if(B(0)>outer.B(0)) B[0] = outer.B(0);
    if(B(1)>outer.B(1)) B[1] = outer.B(1);
    if(B(2)>outer.B(2)) B[2] = outer.B(2);
  }
  //
  inline void set(bool tmp) { A.set(0,0,0); B.set(0,0,0); }
  inline void set(const Vec &a, const Vec &b) { A=a; B=b; }
  inline void set(_Type x, _Type y, _Type dx, _Type dy) { A.set(x,y); B.set(x+dx, y+dy); }
  inline void set(const TBoundBox3& outer, const TBoundBox3& relative) { 
    A.set(outer.A(0)+relative.A(0), outer.A(1)+relative.A(1), outer.A(2)+relative.A(2)); 
    B.set(outer.A(0)+relative.B(0), outer.A(1)+relative.B(1), outer.A(2)+relative.B(2));
    if(B(0)>outer.B(0)) B[0] = outer.B(0);
    if(B(1)>outer.B(1)) B[1] = outer.B(1);
    if(B(2)>outer.B(2)) B[2] = outer.B(2);
  }
  //
  inline TBoundBox3 relative(void) const { return TBoundBox3(Vec(0,0,0), B-A); }
  inline TBoundBox3 relative(const Vec &v) const { return TBoundBox3(v, v+B-A); }
  inline Vec size(void) const { return B-A; }
  inline _Type sizex(void) const { return B.v[0]-A.v[0]; }
  inline _Type sizey(void) const { return B.v[1]-A.v[1]; }
  inline _Type sizez(void) const { return B.v[2]-A.v[2]; }
  inline _Type originx(void) const { return A.v[0]; }
  inline _Type originy(void) const { return A.v[1]; }
  inline _Type originz(void) const { return A.v[2]; }
  //
  inline Vec get_point(bool x, bool y, bool z) { return Vec( x ? B.v[0] : A.v[0], y ? B.v[1] : A.v[1], z ? B.v[2] : A.v[2]); }
  //
  inline bool inside(const Vec &p) { 
    return p(0)>=A(0) && p(1)>=A(1) && p(2)>=A(2) && p(0)<=B(0) && p(1)<=B(1) && p(2)<=B(2); 
  }
};

//***************************************

template<typename _Type>
struct TPlane3 {
  typedef TVector3<_Type> Vector;
  typedef TMatrix44<_Type> Matrix4;
  typedef TMatrix33<_Type> Matrix3;
  typedef _Type BScalar;
  //
  Vector normal;
  BScalar range0;
  //
  inline TPlane3(const Vector &n, BScalar r) : normal(n), range0(r) {}
  inline TPlane3(BScalar x, BScalar y, BScalar z, BScalar r) : normal(x,y,z), range0(r) {}
  inline TPlane3(void) {}
  //
  inline void from_3p(const Vector &A, const Vector &B, const Vector &C) {
    normal.cross(B-A, C-B);
    normal.normalize();
    range0 = normal.scalar(A);
  }
  inline void from_3pr(const Vector &A, const Vector &B, const Vector &C) {
    normal.cross(A-B, B-C);
    normal.normalize();
    range0 = normal.scalar(A);
  }
  //
  inline void transform(const TPlane3& R, const Matrix4& T) {
    if(R.range0) {
      Vector pp = T.mult(R.normal*R.range0);
      normal = T.mult3(R.normal);
      normal.normalize();
      range0 = pp.scalar(normal);
    } else {
      normal = T.mult3(R.normal);
      normal.normalize();
      range0 = T.get_translate().scalar(normal);
    }
  }
  //
  inline BScalar pointside(const Vector &M) const { return normal.scalar(M)-range0; }
  //
  inline bool cross(Vector& rv, const Vector& start, const Vector& dir) {
    BScalar tz = dir.scalar(normal);
    if(mathc::abs(tz)<mathc::small_value(&tz)) {
      return false;
    }
    tz = (range0 - start.scalar(normal)) / tz;
    rv = start + dir * tz;
    return true;
  }
};

//***************************************

typedef TMatrix44<double> TMatrix44d;
typedef TMatrix44<float>  TMatrix44f;

typedef TMatrix33<double> TMatrix33d;
typedef TMatrix33<float>  TMatrix33f;

typedef TQuaternion4<double> TQuaternion4d;

typedef TVector4<int>     TVector4i;
typedef TVector4<float>   TVector4f;
typedef TVector4<double>  TVector4d;

typedef TVector3<int>     TVector3i;
typedef TVector3<float>   TVector3f;
typedef TVector3<double>  TVector3d;

typedef TVector2<int>     TVector2i;
typedef TVector2<float>   TVector2f;
typedef TVector2<double>  TVector2d;

typedef TBoundBox2<int>   TBoundBox2i;
typedef TBoundBox2<float> TBoundBox2f;
typedef TBoundBox2<double>TBoundBox2d;

typedef TBoundBox3<int>   TBoundBox3i;
typedef TBoundBox3<float> TBoundBox3f;
typedef TBoundBox3<double>TBoundBox3d;

typedef TPlane3<int>      TPlane3i;
typedef TPlane3<float>    TPlane3f;
typedef TPlane3<double>   TPlane3d;

//***************************************

inline unsigned int one_bit_container(unsigned int value) {
  if(value<=1) return 1;
  else {
    unsigned int rv, v = value+value-1;
    do {
      rv = v;
      v &= (v-1);
    } while(v);
    return rv;
  }
}

//***************************************

} // namespace math3d
} // namespace NanoUTL

#endif // __NANOUTL_TYPES_MATH_3D_H
