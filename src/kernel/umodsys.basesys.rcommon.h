#ifndef __UMODSYS_BASE_RDEBUG_H
#define __UMODSYS_BASE_RDEBUG_H 1
/*************************************************************/
// file: src/base/system/umodsys.base.rcommon.h
// info: common constants and variables
/*************************************************************/

#include <umodsys/stdcore.h>

#include <umodsys/common/stdsyshlp.h>
#include <umodsys/common/stdstrings.h>

#include <umodsys/tl/container/dynarray.h>
#include <umodsys/tl/container/scatter_array.h>
#include <umodsys/tl/container/quant_array.h>
#include <umodsys/tl/codec/json_emit.h>


namespace UModSys {
namespace base {
namespace rsystem {

using namespace core;

struct IModuleLibraryUni;

struct RModuleLibrarySO;
struct RModuleLibraryThis;

struct RModule;
struct RConsole_std;

struct RWriterFile;
struct RReaderFile;


typedef tl::TDynarrayDynamic<
  tl::TRefObject<RModuleLibrarySO>,
  tl::DAllocatorMallocFast
> RModuleLibrarySOArray;

typedef tl::TStaticStringPool<
  core::BChar,
  tl::su::TComparerBinaryHash<core::BChar>,
  core::SMemAlloc_Malloc
> DSystemStaticPool;

typedef tl::TJSON_Emit<RWriterFile, tl::TDynarrayStatic<BByte, 1024> > DJsonEmitterFile;

//***************************************
// DEBUG CONST
//***************************************

enum eRSystemDebugLevels {
  rsdl_MemoryError,
  rsdl_System,
  rsdl_Module,
  rsdl_ModuleLibrary,
  rsdl_SoLoad,
  rsdl_Uid,
  rsdl_SysTests,
  rsdl_TCL,
};

extern SDebug rsys_dbg;

void dbg_put(eRSystemDebugLevels dl, const char *fmt, ...);
void err_put(eRSystemDebugLevels dl, const char *fmt, ...);

#define FMT_STR(_x_) int(~(_x_)), *(_x_)
#define FMT_SS "%.*s"

struct RWriterFile {
  FILE *f;
  //
  RWriterFile(FILE *ff) : f(ff) {}
  ~RWriterFile(void) { if(f!=NULL) fclose(f); }
  //
  bool write_char(char ch) { return f!=NULL && fputc(ch, f)!=EOF; }
  bool write_chars(const char *chs, size_t len) { return f!=NULL && fwrite(chs, 1, len, f)==len; }
};

inline void keyvalue(DJsonEmitterFile::DObject& obj, const char *key, int value) { obj->key(key); obj->nums(value); }
inline void keyvalue(DJsonEmitterFile::DObject& obj, const char *key, const char* value) { obj->key(key); obj->str(value); }

inline void keyvalue(DJsonEmitterFile::DObject& obj, const char *key, HUniquePointer value) 
{
  obj->key(key);
  DJsonEmitterFile::DObject sub(obj->obj());
  keyvalue(sub, "name", value->name);
  keyvalue(sub, "verno", value->verno);
}
inline void keyvalue(DJsonEmitterFile::DObject& obj, const char *key, const SVersion& value) 
{
  obj->key(key);
  DJsonEmitterFile::DObject sub(obj->obj());
  keyvalue(sub, "major", value.vmajor);
  keyvalue(sub, "minor", value.vminor);
}
inline void value(DJsonEmitterFile::DArray& arr, HUniquePointer value)
{
  DJsonEmitterFile::DObject sub(arr->obj());
  keyvalue(sub, "name", value->name);
  keyvalue(sub, "verno", value->verno);
}


//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RMODULE_H
