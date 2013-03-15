#ifndef __UMODSYS_BASE_MODULE_IMPLEMENTATION_H
#define __UMODSYS_BASE_MODULE_IMPLEMENTATION_H 1
/*************************************************************/
// file: umodsys/base/module_impl.h
// info: base module object implementation
/*************************************************************/

#include <umodsys/core/stdcore.h>
#include <umodsys/base/system.h>

namespace UModSys {
namespace base {

//***************************************
// IModuleInfo
//***************************************

struct IModuleReg {
public:
  SModuleInfo minfo;
  int load_count;
public:
  virtual bool open(ISystem* isys) =0;
  virtual bool close(void) =0;
};

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
  static size_t s_gather(IModuleReg* rlist[], size_t nlist);
  static IModuleReg* s_get(size_t id);
};

//***************************************
// SModuleContext
//***************************************

struct SModuleContext {
protected:
  ISystem* isys;
  IConsole* icon;
  core::IMemAlloc* imem;
public:
  ~SModuleContext(void);
  SModuleContext(void);
  void Link(ISystem* is, core::IMemAlloc* privmem);
  //
  inline operator ISystem*(void) const { return isys; }
  inline operator IConsole*(void) const { return icon; }
  inline operator core::IMemAlloc*(void) const { return imem; }
  inline ISystem* operator->(void) const { return isys; }
  inline core::IMemAlloc& operator()(void) const { return *imem; }
  //
  inline IConsole& con(void) const { return *icon; }
  inline core::IMemAlloc& mem(void) const { return *imem; }
  //
public:
  void dbg_put(const char *fmt, ...);
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

extern base::SModuleContext M;

} // namespace UModSys

#endif // __UMODSYS_BASE_MODULE_IMPLEMENTATION_H
