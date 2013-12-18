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
  #include "platform/def_msvc.h"
#elif defined(__GNUC__)
  #include "platform/def_gcc.h"
#elif defined(UMODSYS_COMPILER_DEFINES)
  #include "umodsys_compiler_defines.h"
#else
  #error Unknown (unsupported) compiler
#endif

//***************************************
//***************************************
//***************************************

#define UMODSYS_DIVUP(_x, _y) ((_x)/(_y)+((_x)%(_y)!=0))

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

#ifndef UMODSYS_MALLOC
  #define UMODSYS_MALLOC(s, f, l)      ::malloc(s)
#endif
#ifndef UMODSYS_FREE
  #define UMODSYS_FREE(p, f, l)        ::free(p)
#endif
#ifndef UMODSYS_REALLOC
  #define UMODSYS_REALLOC(p, s, f, l)  ::realloc(p, s)
#endif

//***************************************
//***************************************
//***************************************

#if defined(_WIN32) || defined(WIN32)
  #include "platform/def_win32.h"
#endif

//***************************************
//***************************************
//***************************************

#endif // __UMODSYS_CORE_STDDEFS_H
