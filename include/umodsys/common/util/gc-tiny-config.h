//Tiny GC

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <new>

#ifdef __SIZEOF_POINTER__
 #define TGC_POINTER_SIZE (__SIZEOF_POINTER__)
#else
 #if PTRDIFF_MAX<0xFF
  #define TGC_POINTER_SIZE 1
 #elif PTRDIFF_MAX<0xFFFF
  #define TGC_POINTER_SIZE 2
 #elif PTRDIFF_MAX<0xFFFFFF
  #define TGC_POINTER_SIZE 3
 #elif PTRDIFF_MAX<0xFFFFFFFF
  #define TGC_POINTER_SIZE 4
 #elif PTRDIFF_MAX<0xFFFFFFFFFF
  #define TGC_POINTER_SIZE 5
 #elif PTRDIFF_MAX<0xFFFFFFFFFFFF
  #define TGC_POINTER_SIZE 6
 #elif PTRDIFF_MAX<0xFFFFFFFFFFFFFF
  #define TGC_POINTER_SIZE 7
 #elif PTRDIFF_MAX<0xFFFFFFFFFFFFFFFF
  #define TGC_POINTER_SIZE 8
 #else
  #define TGC_POINTER_SIZE 0
 #endif
#endif

