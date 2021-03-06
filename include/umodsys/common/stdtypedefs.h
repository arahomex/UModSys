#ifndef __UMODSYS_COMMON_STDTYPEDEFS_H
#define __UMODSYS_COMMON_STDTYPEDEFS_H 1
/*************************************************************/
// file: umodsys/common/stdtypedefs.h
// info: define core types
/*************************************************************/

#include <umodsys/common/stddefs.h>

namespace UModSys {

namespace core {
  //***************************************
  // core types
  //***************************************

  struct Void;
  struct Empty;

  struct SSourceContext;         // file&line info for alloc & exceptions
  struct SVersion;               // version in form hi/lo

  struct SUniquePointerInfo;
  struct SUniquePointer;
  struct SUniquePtrList;
  typedef const SUniquePointerInfo* HUniquePointer;

  struct IRoot;
  struct IRefObject;

  struct EExceptionBase;
    struct EFatal;
    struct EError;
    struct EWarning;

  struct SParameters;

  struct IMemAlloc;
  struct SIMemAlloc;
  struct SMemAlloc_Malloc;

  //***************************************
  // Outlines
  //***************************************

  extern unsigned char lost(const void* data);

  // defined in 'stdinlines.h'
  template<typename T> int ptr_compare(T a, T b);
  template<typename T> int scalar_compare(T a, T b);
  template<typename T1, typename T2> int any_ptr_compare_null(const T1* a, const T2* b);
  template<typename T> T scalar_min(T a, T b);
  template<typename T> T scalar_max(T a, T b);
  template<typename T> void t_set_array1(T *a, T value, size_t size);
  template<typename T> void t_set_array(T* arr, size_t N, const T& v);

  const size_t array_index_none = size_t(-1);

  const void * const cnil = NULL;
  void* const nil = NULL;
  Void* const void_nil = NULL;

  template<typename T> inline T* tnil(void) { return NULL; };
  inline Void* void_null(void) { return NULL; }
  inline Void& void_obj(void) { return *void_null(); };


} // namespace core

namespace base {
  //***************************************
  // base types
  //***************************************

  struct IModuleReg;
  struct IModule;
  struct IModuleLibrary;
  struct IModuleLoader;
  //
  struct SModuleRegChain;
  struct SModuleInfo;
  struct SModuleContext;

  struct IKernel;

  struct IModObject;
  struct IGenerator;
  struct IShell;

} // namespace base


extern base::SModuleContext M;
extern const core::SIMemAlloc& local_memory(void);
extern const core::SIMemAlloc& shared_memory(void);

/*
namespace objects {
  struct IRoot;               // general static object

  struct ISysObject;          // static system object
  struct IRefObject;          // ref-counted object
  struct IModObject;          // module object. RC
  struct IGenerator;          // generate any objectby defined parameters

  struct SSourceContext;         // file&line info for alloc & exceptions

  struct EGeneral;
  struct EFatal;
  struct EFailure;

  struct IDebug;
  struct IConsole;
  struct IConsoleHInput;         // console input helper
  struct SConsoleColorInfo;      // color info
  struct IHistory;
}

namespace system {
  template<bool isPrecise, bool isShared> struct TModule_Allocator;
  //
  typedef TModule_Allocator<false, false> SModule_AllocatorFast;
  typedef TModule_Allocator<true,  false> SModule_AllocatorPrecise;
  typedef TModule_Allocator<false, true > SModule_AllocatorFast_Shared;
  typedef TModule_Allocator<true,  true > SModule_AllocatorPrecise_Shared;

  struct IKernel;
  struct IModuleDefinition;   // module definition, only for module declaration
  struct IModule;             // module loaded
  struct IShell;              // main script executor
  struct IMemCache;           // cache memory
  struct IMemPool;            // memory pool
  struct IMemHeap;            // ordinal memory heap
}

namespace media {
  struct IMediaLibrary;   // whole library
  struct IMediaFilter;    // this one process binary data to object
  struct IMediaGroup;     // hold different types of objects
  struct IMediaArchive;   // perform load/save of binary data
  struct IMediaReader;    // file reader
  struct IMediaWriter;    // file writer

  struct SMediaFilter;    // arguments for filtering

  struct SMediaFileInfo;  // information about archive entry

  struct CMediaArchiveFrame;        // archive frame
  struct IMediaArchiveFrameClient;  // archive frame functionality

  struct IParameters;         // utilize SParameter
  struct SParameters;         // hold parameter data

  // database externs (sdb.h)
  namespace simple_db {
    struct ISimpleDB;         // simple database interface
    struct CFileOperatorINI;  // write INI files
    struct CFileOperatorCSV;  // write CSV files
  }
}
} // namespace core
*/

} // namespace UModSys

#endif // __UMODSYS_COMMON_STDTYPEDEFS_H
