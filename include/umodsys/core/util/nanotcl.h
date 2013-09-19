#ifndef __UMODSYS_UTIL_NANOTCL_H
#define __UMODSYS_UTIL_NANOTCL_H 1

// based on Tcl in ~ 500 lines of code by Salvatore antirez Sanfilippo. BSD licensed

#include <umodsys/core/stdcore.h>
#include <umodsys/tl/composite/array.h>

namespace UModSys {
namespace core {
namespace util {
namespace NanoTCL {

struct SInterpretator;
struct SVariable;
struct SCommand;
struct SContext;

enum eStatus { 
  sOK, sERR, sRETURN, sBREAK, sCONTINUE 
};

enum eInState {    
  isESC, isSTR, isCMD, isVAR, isSEP, isEOL, isEOF
};

typedef DStringShared TCL_Value;

struct SInterpretator {

};

struct SCommand {
};

struct SVariable {
};

struct SContext {
    tl::TArray< SVariable, tl::TAllocator<SIMemAlloc> > vars;
    struct picolCallFrame *parent; /* parent is NULL at top level */
};

} // namespace NanoTCL
} // namespace util
} // namespace core
} // namespace UModSys


#endif

