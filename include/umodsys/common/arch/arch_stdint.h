
namespace arch_types {
typedef int64_t        sint64;
typedef uint64_t       uint64;
typedef int32_t        sint32;
typedef uint32_t       uint32;
typedef int16_t        sint16;
typedef uint16_t       uint16;
typedef int8_t         sint8;
typedef uint8_t        uint8;

typedef float                 float32;
typedef double                float64;
typedef long double           float80;

typedef uint64 OSTime;
typedef uint64 OSTimeMsec;
} // namespace arch_types

#define UMODSYS_HAS_INT64

#include "arch_defines.h"

#if !defined(UMODSYS_PTRSIZE)
#elif UINTPTR_MAX==0xffff
  #define UMODSYS_PTRSIZE 2
#elif UINTPTR_MAX==0xffffffff
  #define UMODSYS_PTRSIZE 4
#elif UINTPTR_MAX==0xffffffffffffffff
  #define UMODSYS_PTRSIZE 8
#else
  #error "Unsupported pointer size"
#endif

