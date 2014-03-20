#ifndef __UMODSYS_TL_MATH_MATHC_H
#define __UMODSYS_TL_MATH_MATHC_H 1
/*************************************************************/
// file: umodsys/tl/math/math_c.h
// info: common mathematics (randoms, calcs, etc)
/*************************************************************/

#ifndef M_PI
//#define M_PI 3.1415926535897932384626433832795
#endif

namespace UModSys {
namespace mathc {

//***************************************

const double pi = 3.1415926535897932384626433832795;
const double k_rad2deg = 180.0/pi;
const double k_deg2rad = pi/180.0;

//***************************************

inline double abs(double v) { return fabs(v); }
inline float abs(float v) { return float(fabs(v)); }
inline int abs(int v) { return labs(v); }

inline double sqrt(double v) { return ::sqrt(v); }
inline float sqrt(float v) { return ::sqrt(v); }
inline int sqrt(int v) { return (int)::sqrt(double(v)); }

/*
template<typename T> inline T small_value(void) { return 0; }
template<> inline double small_value<double>(void) { return 1e-4; }
template<> inline float small_value<float>(void) { return 1e-4f; }
*/

template<typename T> inline T small_value(const T* tmp=NULL) { return 0; }
template<> inline double small_value<double>(const double *tmp) { return 1e-8; }
template<> inline float small_value<float>(const float *tmp) { return 1e-8f; }

template<typename T> inline T small_value2(const T* tmp=NULL) { return 0; }
template<> inline double small_value2<double>(const double *tmp) { return 1e-16; }
template<> inline float small_value2<float>(const float *tmp) { return 1e-16f; }

//***************************************

inline double torad(double v) { return v*k_deg2rad; }
inline float torad(float v) { return float(v*k_deg2rad); }
inline float torad(int v) { return float(v*k_deg2rad); }

inline double todeg(double v) { return v*k_rad2deg; }
inline float todeg(float v) { return float(v*k_rad2deg); }
inline float todeg(int v) { return float(v*k_rad2deg); }

//***************************************

inline float circle1_x(double a) { return float(0.5*(cos(a)+1)); }
inline float circle1_y(double a) { return float(0.5*(sin(a)+1)); }

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

// all outlines defined in ums_math.cpp
const unsigned long RandomGenerator_MV = 134775813;
const double RandomGenerator_MMR = 1.0/4294967295.0;
const double RandomGenerator_MS = 2147483648.0;

struct SRandomGenerator {
  //
  enum {
    mode_And = 'a',
    mode_Or = 'o',
    mode_None = 0
  };
  //
  struct Seed { signed long s; };

  Seed seed;

  inline SRandomGenerator(void) { seed.s=0; }
  inline SRandomGenerator(Seed s) { seed=s; }

  inline double gen(void) {
    return
      ( (seed.s = seed.s * RandomGenerator_MV + 1)
        + RandomGenerator_MS
      )*RandomGenerator_MMR;
  }
  //
  inline long operator()(long R, long S=0) { return long(gen()*R)+S; }
  inline int operator()(int R, int S=0) { return int(gen()*R)+S; }
  inline double operator()(double R=1, double S=0) { return gen()*R+S; }
  inline float operator()(float R, float S=0) { return float(gen()*R+S); }
  //
  inline long genN(long R, long S=0) { return long(gen()*R)+S; }
  inline int genN(int R, int S=0) { return int(gen()*R)+S; }
  inline double genN(double R=1, double S=0) { return gen()*R+S; }
  inline float genN(float R, float S=0) { return float(gen()*R+S); }
  //
  inline long genAB(long A, long B) { return long(gen()*(B-A))+A; }
  inline int genAB(int A, int B) { return int(gen()*(B-A))+A; }
  inline double genAB(double A, double B) { return gen()*(B-A)+A; }
  inline float genAB(float A, float B) { return float(gen()*(B-A)+A); }
  //
  bool less_tries(double limit, int tries, int mode=mode_And);
  int less_tries2(double limit, int tries, int tries2, int mode2=mode_And);
  //
  void randomize(void);
  inline Seed save(void) { return seed; }
  inline void restore(Seed s) { seed=s; }
};

//***************************************

} // namespace mathc
} // namespace UModSys

#endif // __UMODSYS_TL_MATH_MATHC_H
