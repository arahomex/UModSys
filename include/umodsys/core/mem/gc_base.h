#ifndef __UMODSYS_CORE_MEM_GC_BASE_H
#define __UMODSYS_CORE_MEM_GC_BASE_H 1
/*************************************************************/
// file: umodsys/core/mem/gc_base.h
// info: gc base
/*************************************************************/

#include <umodsys/core/source.h>
#include <umodsys/core/arch.h>

namespace UModSys {
namespace core {
namespace gc {

using namespace core;

//****************************************************************
// typedefs
//****************************************************************


#if UMODSYS_PTRSIZE == 4
//#error "Pointer size 4")
typedef Buint8 DGeneration;
typedef Buint8 DFlags;
typedef Buint16 DLocks;
#elif UMODSYS_PTRSIZE == 8
//#error "Pointer size 8"
typedef Buint16 DGeneration;
typedef Buint16 DFlags;
typedef Buint32 DLocks;
#else
//#error "Pointer size ??"
typedef Buint8 DGeneration;
typedef Buint8 DFlags;
typedef Buint16 DLocks;
#endif

//****************************************************************

struct SObjectHeader; // header of object

struct IPool; // pool allocator
struct IObject; // gc allocated object

template<typename Node> struct SInternalRef;
template<typename Node> struct SInternalStrongRef;
template<typename Node> struct SExternalRef;
template<typename Node> struct SExternalWeakRef;
template<typename Node> struct SExternalWeakPtr;

} // namespace gc
} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_MEM_GC_BASE_H
