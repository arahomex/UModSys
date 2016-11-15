#ifndef __UMODSYS_COMMON_ARCH_H
#define __UMODSYS_COMMON_ARCH_H 1
//*****************************************************
// umodsys/common/arch.h: defined arhitect-related types
//*****************************************************

#include <umodsys/common/stddefs.h>
#include <umodsys/common/stdtypedefs.h>
#include <umodsys/common/stdinlines.h>

namespace UModSys {
namespace core {

//***************************************
//***************************************
//***************************************

#ifdef UMODSYS_ARCH_INCLUDE
  #include UMODSYS_ARCH_INCLUDE
#else
  #error "Undefined ARCH type"
#endif

/*
#if UCHAR_MAX==0xff
  #ifndef UMODSYS_ARCH_BYTE8
    #define UMODSYS_ARCH_BYTE8
  #endif
#endif

#ifdef UMODSYS_ARCH_BYTE8
  #define UMODSYS_ARCH_BYTE_BITS 8
  // all chars is unsigned
  enum {
    MIN_Char = -0x80, MAX_Char = 0x7F,
    MIN_CharW = 0, MAX_CharW = 0xFFFF
  };
  //
  #if UINT_MAX==0xffff
    #include "arch/arch_int16.h"
  #elif UINT_MAX==0xffffffff
    #include "arch/arch_int32.h"
  #elif UINT_MAX==0xffffffffffffffff
    #include "arch/arch_int64.h"
  #else
    #error "Unsupported integer size"
  #endif
#else // UMODSYS_ARCH_BYTE8
  #error "Undefined ARCH byte type"
#endif // UMODSYS_ARCH_BYTE8
*/

//***************************************

typedef time_t BTime;

extern BTime null_time;

#define UMODSYS_ALIGNED_SIZE_BY(_x, _ybit) (((_x)+~(~0<<_ybit)) & (1<<_ybit))
#define UMODSYS_MEMQ(_count, _mem_quant) ((((_count) + ((_mem_quant)-1)) / (_mem_quant)) * (_mem_quant))
#define UMODSYS_BITMAP_SIZE(_count) UMODSYS_MEMQ(_count, UMODSYS_ARCH_BYTE_BITS)

//***************************************
// Version
//***************************************

struct SVersion {
  Buint16 vmajor, vminor;
  //
  inline SVersion(Buint16 ma, Buint16 mi) : vmajor(ma), vminor(mi) {}
  inline SVersion(void) : vmajor(0), vminor(0) {}
  //
  inline bool operator==(const SVersion& R) const 
    { return vmajor==R.vmajor && vminor==R.vminor; }
  inline bool operator!=(const SVersion& R) const 
    { return vmajor!=R.vmajor && vminor!=R.vminor; }
  inline bool operator>(const SVersion& R) const 
    { return vmajor>R.vmajor || (vmajor==R.vmajor && vminor>R.vminor); }
  inline bool operator<(const SVersion& R) const 
    { return vmajor>R.vmajor || (vmajor==R.vmajor && vminor<R.vminor); }
  inline bool operator>=(const SVersion& R) const 
    { return vmajor>R.vmajor || (vmajor==R.vmajor && vminor>=R.vminor); }
  inline bool operator<=(const SVersion& R) const 
    { return vmajor>R.vmajor || (vmajor==R.vmajor && vminor<=R.vminor); }
  inline int cmp(const SVersion& R) const 
    { return vmajor==R.vmajor ? core::scalar_compare(vminor, R.vminor) : core::scalar_compare(vmajor, R.vmajor); }
};

//***************************************
// INLINES
//***************************************

inline size_t mem_q(size_t count) 
{
  return ((count + (mem_quant-1)) / mem_quant) * mem_quant;
}

inline int scalar_compare(const SVersion &L, const SVersion& R) 
{ 
  return L.cmp(R); 
}


//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_COMMON_ARCH_H
