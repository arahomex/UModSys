#ifndef __UMODSYS_CORE_STDDEFS_H
#define __UMODSYS_CORE_STDDEFS_H 1
/*************************************************************/
// file: umodsys/core/stddefs.h
// info: include all shared required h files
//       also define basic compiler and os defines
/*************************************************************/

//***************************************
//***************************************
//***************************************

// compiler checks

#if defined(_MSC_VER)
  #pragma warning(disable:4355)
  #pragma warning(disable:4267) // conversion from 'a' to 'b', possible loss of data
  #pragma warning(disable:4244) // warning C4244: 'argument' : conversion from '' to '', possible loss of data
  #pragma warning(disable:4018) // warning C4018: 'op' : signed/unsigned mismatch

  #pragma warning(disable:4996) // warning C4996: 'func' was declared deprecated
  #pragma warning(disable:4290) // warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)

  #define _CRT_SECURE_NO_DEPRECATE // exclude deprecated ones
//  #pragma message ("Visual studio warnings disabled")

  #if !defined(_WIN32)
    #define _WIN32
  #endif
  #if !defined(WIN32)
    #define WIN32
  #endif

  // includes
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdarg.h>

  #include <string.h>
  #include <ctype.h>
  #include <time.h>
  #include <limits.h>

  #include <math.h>

  #include <new>
  // /includes

  #define UMODSYS_THROW(_x) throw _x
  #define UMODSYS_NOTHROW() throw()

  #define snprintf _snprintf
#elif defined(__GNUC__)

  #ifndef _GNU_SOURCE
    #define _GNU_SOURCE         /* See feature_test_macros(7) */
  #endif

  // includes
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdarg.h>

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

#elif defined(UMODSYS_COMPILER_DEFINES)

  #include "umodsys_compiler_defines.h"

#else

  #error Unknown (unsupported) compiler

#endif

//***************************************
//***************************************
//***************************************


//#ifndef M_PI
//#define M_PI 3.1415926535897932384626433832795
//#endif

//***************************************
//***************************************
//***************************************

#ifndef UMODSYS_SOURCEINFO
  #define _UMODSYS_SOURCEINFO (&::UModSys::core::SSourceContext(__FILE__, __LINE__, __FUNCTION__))
  #define _UMODSYS_SOURCEINFON (&::UModSys::core::SSourceContext(__FILE__, __LINE__, NULL))
  #define _UMODSYS_SOURCEINFOZ ((::UModSys::core::SSourceContext*)NULL)
  #if defined(_DEBUG) || defined(DEBUG)
    #define UMODSYS_SOURCEINFO _UMODSYS_SOURCEINFO
    #define UMODSYS_SOURCEINFON _UMODSYS_SOURCEINFON
    #define UMODSYS_SOURCEINFOZ _UMODSYS_SOURCEINFOZ
  #else
    #define UMODSYS_SOURCEINFO _UMODSYS_SOURCEINFOZ
    #define UMODSYS_SOURCEINFON _UMODSYS_SOURCEINFOZ
    #define UMODSYS_SOURCEINFOZ _UMODSYS_SOURCEINFOZ
  #endif
#endif

//***************************************
//***************************************
//***************************************

#if defined(_WIN32) || defined(WIN32)
  #ifndef NANOUTL_CPU_INTELTYPES
//    #define UMS_CPU_INTELTYPES
  #endif
  #ifndef NANOUTL_CPU_WCHAR
    #define NANOUTL_CPU_WCHAR
  #endif
  #ifndef NANOUTL_CPU_LE
    #define NANOUTL_CPU_LE
  #endif
  #ifndef NANOUTL_CPU_INT32
    #define NANOUTL_CPU_INT32
  #endif
  #ifndef NANOUTL_CPU_BYTE8
    #define NANOUTL_CPU_BYTE8
  #endif
  #ifndef NANOUTL_INT64_ENABLED
    #define NANOUTL_INT64_ENABLED 1
  #endif
  #ifndef NANOUTL_WCHAR_SEPARATED_TYPE
    #define NANOUTL_WCHAR_SEPARATED_TYPE 1
  #endif
  #ifndef NANOUTL_WCHAR_UTF16
    #define NANOUTL_WCHAR_UTF16 1
  #endif
  #ifdef NANOUTL_WCHAR_UTF32
    #undef NANOUTL_WCHAR_UTF32
  #endif
#endif

//***************************************
//***************************************
//***************************************

#endif // __UMODSYS_CORE_STDDEFS_H
