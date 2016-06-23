#ifndef __UMODSYS_BASE_MODULE_IMPLEMENTATION_H
#define __UMODSYS_BASE_MODULE_IMPLEMENTATION_H 1
/*************************************************************/
// file: umodsys/base/module_impl.h
// info: base module object implementation
/*************************************************************/

#include <umodsys/stdcore.h>
#include <umodsys/base/basetypes.h>
#include <umodsys/base/module.h>
#include <umodsys/base/system.h>
//#include <umodsys/base/system.h>
//#include <umodsys/base/console.h>

namespace UModSys {
namespace base {

//***************************************
// IModuleReg
//***************************************

struct IModuleReg {
public:
  SModuleInfo minfo;
  int load_count;
  IModule *module;
  SDebug* sdbg;
public:
  IModuleReg(const SModuleInfo &mi, SDebug* dbg);
  IModuleReg(const char* n, core::Buint16 vh, core::Buint16 vl, const char *i, core::Buint32 bno, core::BTime u, SDebug* dbg);
  virtual ~IModuleReg(void);
  virtual SDebug* mr_getdebug(void) const;
  virtual bool mr_isopen(void) const;
  virtual bool mr_open(void);
  virtual bool mr_close(void);
  //
  virtual bool do_open(void) =0;
  virtual bool do_close(void) =0;
public:
  inline bool reg(IModObject* mo) { return module->reg(mo, true); }
  inline bool reg(IGenerator* g) { return module->reg(g, true); }
  inline bool unreg(IModObject* mo) { return module->reg(mo, false); }
  inline bool unreg(IGenerator* g) { return module->reg(g, false); }
};

#define UMODSYS_MODULEREG(_name, _prefix, _info) \
  base::IModuleReg(_name, _prefix##_MAJOR, _prefix##_MINOR, _info, _prefix##_BUILD, _prefix##_TIME, &s_dbg)

//***************************************
// IModuleLibraryReg
//***************************************

struct IModuleLibraryReg {
public:
public:
  IModuleLibraryReg(void);
  virtual ~IModuleLibraryReg(void);
  virtual size_t mlr_count(void) const =0;
  virtual IModuleReg* mlr_get(size_t id) const =0;
  virtual bool mlr_isopen(void) const =0;
  virtual bool mlr_open(ISystem* isys, core::IMemAlloc* privmem) =0;
  virtual bool mlr_close(void) =0;
};

//***************************************
// SModuleRegChain
//***************************************

struct SModuleRegChain : public tl::TList2Node<SModuleRegChain> {
public:
  IModuleReg* imr;
  //
  static SModuleRegChain root;
  //
  SModuleRegChain(IModuleReg* r);
  SModuleRegChain(IModuleReg& r);
  explicit SModuleRegChain(int v);
  ~SModuleRegChain(void);
  //
/*  static size_t s_gather(IModuleReg* rlist[], size_t nlist);*/
  static IModuleReg* s_get(size_t id);
};

//***************************************
// RModuleLibraryReg
//***************************************

struct RModuleLibraryReg_Chain : public IModuleLibraryReg {
public:
  RModuleLibraryReg_Chain(void);
  ~RModuleLibraryReg_Chain(void);
public:
  size_t mlr_count(void) const;
  IModuleReg* mlr_get(size_t id) const;
  bool mlr_isopen(void) const;
  bool mlr_open(ISystem* isys, core::IMemAlloc* privmem);
  bool mlr_close(void);
public:
  static RModuleLibraryReg_Chain s_library;
};

//***************************************
// module implementation
//***************************************

extern int so_entry_enabled_flag;

inline void so_enable(void) { core::lost(&so_entry_enabled_flag); }

//***************************************
// END
//***************************************

} // namespace base

} // namespace UModSys

#endif // __UMODSYS_BASE_MODULE_IMPLEMENTATION_H
