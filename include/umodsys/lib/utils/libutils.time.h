#ifndef __UMODSYS_LIBUTILS_TIME_H
#define __UMODSYS_LIBUTILS_TIME_H 1
/*************************************************************/
// file: umodsys/lib/libutils/libutils.time.h
// info: time measure
/*************************************************************/

#include <umodsys/stdcore.h>
#include <umodsys/core/syshlp.h>

namespace UModSys {
namespace libutils {

using namespace core;

//***************************************
// FORWARD/TYPEDEF
//***************************************

struct STimeMeasure;
struct STimeMeasureNum;

//***************************************
// ENUM/CONST
//***************************************


//***************************************
// TYPES
//***************************************

//***************************************
// STimeMeasure

struct STimeMeasure {
  unsigned long t1, t2;

  inline STimeMeasure(void) { t1=t2=0; }
  inline void start(void) { t1 = syshlp::t_msec(); }
  inline void stop(void) { t2 = syshlp::t_msec(); }
  inline void next(STimeMeasure& nv) { stop(); nv.t1 = t2; }
  inline void operator >>(STimeMeasure& nv) { stop(); nv.t1 = t2; }
  inline unsigned long msec(void) const { return t2 - t1; }
  inline double sec(void) const { return msec()/1000.0; }
};

//***************************************
// STimeMeasureNum

struct STimeMeasureNum : public STimeMeasure {
  unsigned long num;

  inline STimeMeasureNum(unsigned long n=1) { num = n; }
  inline void startn(unsigned long n) { num = n; start(); }
  inline void stopn(unsigned long n) { num = n; stop(); }
  inline double nps(void) const { return num/sec(); }
  inline double spn(void) const { return sec()/num; }
};


//***************************************
// INLINES/OUTLINES
//***************************************

//***************************************
// END
//***************************************

} // namespace libutils
} // namespace UModSys

#endif // __UMODSYS_LIBUTILS_TIME_H
