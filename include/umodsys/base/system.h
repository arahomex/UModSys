#ifndef __UMODSYS_BASE_SYSTEM_H
#define __UMODSYS_BASE_SYSTEM_H 1
/*************************************************************/
// file: umodsys/base/system.h
// info: base system object
/*************************************************************/

#include <umodsys/stdcore.h>

#include <umodsys/base/console.h>
#include <umodsys/base/module.h>

namespace UModSys {
namespace base {

//***************************************
// ISystem
//***************************************

struct ISystem : public core::IRoot
{
public:
  ~ISystem(void);
public:
  virtual core::IUniquePointerResolver* get_upr(void) =0;
  virtual core::ISourceContextAdapter* get_sca(void) =0;
  //
  virtual IModuleLoader* get_modloader(void) =0;
  virtual IModuleLibrary* get_syslib(void) = 0;
  //
  virtual core::IMemAlloc* get_sysmem(void) =0;
  virtual core::IMemAlloc* get_sharemem(void) =0;
  virtual IConsole* get_console(void) =0;
  virtual core::IParameters* get_params(void) =0;
public:
  UMODSYS_ROOT_IMPLEMENT1(UModSys::base::ISystem, 3, IRoot);
};

//***************************************
// SModuleContext
//***************************************

struct SModuleContext {
public:
  typedef core::IRefObject::TypeId TypeId;
  typedef core::BCStr BCStr;
protected:
  ISystem* isys;
  IConsole* icon;
  core::SIMemAlloc smem;
  core::SIMemAlloc smem_shared;
  IModuleLoader* imodloader;
public:
  ~SModuleContext(void);
  SModuleContext(void);
  void Link(ISystem* is, core::IMemAlloc* privmem);
  //
  inline operator ISystem*(void) const { return isys; }
  inline operator IConsole*(void) const { return icon; }
  inline operator core::IMemAlloc*(void) const { return smem.imem; }
  inline ISystem* operator->(void) const { return isys; }
  inline const core::SIMemAlloc& operator()(void) const { return smem; }
  inline ISystem* operator*(void) const { return isys; }
  //
  inline ISystem& sys(void) const { return *isys; }
  inline IConsole& con(void) const { return *icon; }
  inline core::IMemAlloc& imem(void) const { return *smem.imem; }
  inline const core::SIMemAlloc& mem(void) const { return smem; }
  inline core::IMemAlloc& imem_shared(void) const { return *smem_shared.imem; }
  inline const core::SIMemAlloc& mem_shared(void) const { return smem_shared; }
  //
public:
  void dbg_put(const char *fmt, ...);
  //
  inline size_t findobjname(TypeId intr, TypeId found[], size_t nfound, BCStr mask=NULL) {
    if(imodloader==NULL)
      return 0;
    return imodloader->moduledb_findobjname(intr, found, nfound, mask);
  }
  //
  template<typename Intr, typename Array>
  inline bool t_findobjname(Array& found, BCStr mask=NULL) {
    TypeId intr = Intr::_root_get_interface_type();
    found.Resize(0);
    if(imodloader==NULL)
      return false;
    size_t rv = imodloader->moduledb_findobjname(intr, found.All(), found.MaxLen(), mask);
    if(!found.Resize(rv) || rv==0)
      return false;
    return true;
  }
  //
  template<typename Intr>
  inline TypeId t_firstobjname(BCStr mask=NULL) {
    TypeId intr = Intr::_root_get_interface_type(), tid;
    if(imodloader==NULL)
      return NULL;
    if(imodloader->moduledb_findobjname(intr, &tid, 1, mask)==0)
      return NULL;
    return tid;
  }
  //
  template<typename Intr>
  inline bool t_firstobjname(TypeId &tid, core::BCStr mask=NULL) {
    tid = NULL;
    TypeId intr = Intr::_root_get_interface_type();
    if(imodloader==NULL)
      return false;
    if(imodloader->moduledb_findobjname(intr, &tid, 1, mask)==0)
      return false;
    return true;
  }
  //
  template<typename RData>
  inline bool t_generate(tl::TRefObject<RData> &rv, TypeId name, const core::SParameters& args) {
    if(imodloader==NULL)
      return false;
    core::IRefObject::P obj;
    if(!imodloader->moduledb_generate(obj, name, args) || !obj.valid())
      return false;
    if(!obj->t_ref_get_other_interface(rv))
      return false;
    return true;
  }
  template<typename RData>
  inline bool t_generate_first(tl::TRefObject<RData> &rv, const core::SParameters& args, core::BCStr mask=NULL, TypeId *inf=NULL) {
    TypeId found;
    if(!t_firstobjname<RData>(found, mask))
      return false;
    if(inf!=NULL)
      *inf = found;
    return t_generate(rv, found, args);
  }
};

//***************************************
// END
//***************************************

} // namespace base

//***************************************
// 
//***************************************

extern base::SModuleContext M;

//***************************************
// core::SIMemAlloc::
//***************************************

inline core::SIMemAlloc::SIMemAlloc(void) 
: imem(M) {
}

//***************************************
// END
//***************************************

} // namespace UModSys

#endif // __UMODSYS_BASE_SYSTEM_H
