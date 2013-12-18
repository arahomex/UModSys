#pragma warning(disable:4355)
#pragma warning(disable:4267) // conversion from 'a' to 'b', possible loss of data
#pragma warning(disable:4244) // warning C4244: 'argument' : conversion from '' to '', possible loss of data
#pragma warning(disable:4018) // warning C4018: 'op' : signed/unsigned mismatch

#pragma warning(disable:4996) // warning C4996: 'func' was declared deprecated
#pragma warning(disable:4290) // warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)

#define _CRT_SECURE_NO_DEPRECATE // exclude deprecated ones
//#pragma message ("Visual studio warnings disabled")

#if !defined(_WIN32)
  #define _WIN32
#endif
#if !defined(WIN32)
  #define WIN32
#endif

// includes
#ifdef _DEBUG
//    #define _CRTDBG_MAP_ALLOC
  #include <crtdbg.h>
  //
//    #define UMODSYS_MALLOC(s, _f, _l)      ::_malloc_dbg(s, _NORMAL_BLOCK, _f, _l)
//    #define UMODSYS_FREE(p, _f, _l)        ::_free_dbg(p, _NORMAL_BLOCK)
//    #define UMODSYS_REALLOC(p, s, _f, _l)  ::_realloc_dbg(p, s, _NORMAL_BLOCK, _f, _l)
  #define UMODSYS_MALLOC(s, _f, _l)      ::_malloc_dbg(s, _CLIENT_BLOCK, _f, _l)
  #define UMODSYS_FREE(p, _f, _l)        ::_free_dbg(p, _CLIENT_BLOCK)
  #define UMODSYS_REALLOC(p, s, _f, _l)  ::_realloc_dbg(p, s, _CLIENT_BLOCK, _f, _l)

#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <io.h>

#include <string.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>

#include <math.h>

#include <new>
// /includes

#ifdef free
  #undef free
  namespace UModSys { inline void free(void* p) { _free_dbg(p, _NORMAL_BLOCK); } }
#endif

#define UMODSYS_THROW(_x) throw _x
#define UMODSYS_NOTHROW() throw()

#define snprintf _snprintf

// ---------------------- arch defines
#ifndef UMODSYS_ARCH_BYTE8
  #define UMODSYS_ARCH_BYTE8 8
#endif
#ifndef UMODSYS_ARCH_LE
  #define UMODSYS_ARCH_LE 1
#endif
#ifndef UMODSYS_ARCH_NONALIGNED
  #define UMODSYS_ARCH_NONALIGNED 1
#endif
// ---------------------- /arch defines
