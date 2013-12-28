#ifndef _GNU_SOURCE
  #define _GNU_SOURCE         /* See feature_test_macros(7) */
#endif

// includes
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <string.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>
#include <wchar.h>

#include <math.h>

#include <stddef.h> // offsetof macro

#include <new>
// /includes

#define UMODSYS_THROW(_x) throw _x
#define UMODSYS_NOTHROW(_x) throw()

//  #pragma GCC visibility push(hidden)

// ---------------------- arch defines
#ifdef __BYTE_ORDER__
  #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #ifndef UMODSYS_ARCH_LE
      #define UMODSYS_ARCH_LE 1
    #endif
  #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #ifndef UMODSYS_ARCH_BE
      #define UMODSYS_ARCH_BE 1
    #endif
  #endif
#else
  #include <endian.h>
  #if __BYTE_ORDER == __LITTLE_ENDIAN
    #ifndef UMODSYS_ARCH_LE
      #define UMODSYS_ARCH_LE 1
    #endif
  #elif __BYTE_ORDER == __END_ENDIAN
    #ifndef UMODSYS_ARCH_BE
      #define UMODSYS_ARCH_BE 1
    #endif
  #endif
#endif

#if defined(__i386__) || defined(__x86_64__) || defined(__amd64__)
  #ifndef UMODSYS_ARCH_NONALIGNED
    #define UMODSYS_ARCH_NONALIGNED 1
  #endif
#endif
// ---------------------- /arch defines
