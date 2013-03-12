#ifndef __UMODSYS_CORE_STDTYPEDEFS_H
#define __UMODSYS_CORE_STDTYPEDEFS_H 1
/*************************************************************/
// file: umodsys/core/stdtypedefs.h
// info: define core types
/*************************************************************/

#include <umodsys/core/stddefs.h>

namespace UModSys {

namespace core {
  //***************************************
  // Types
  //***************************************

  struct Void;
  struct Empty;

  struct SSourceContext;         // file&line info for alloc & exceptions
  struct SUniquePointerInfo;
  struct SUniquePointer;

  struct IRoot;
  struct IRefObject;
  struct IModObject;
  struct IGenerator;

  struct IModule;

  struct EExceptionBase;
    struct EFatal;
    struct EError;
    struct EWarning;

  struct SParameters;

  //***************************************
  // Outlines
  //***************************************

  unsigned char lost(const void* data);

} // namespace core

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

  struct ISystem;
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

#endif // __UMODSYS_CORE_STDTYPEDEFS_H
