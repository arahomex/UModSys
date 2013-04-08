#include "umodsys.base.rmodule.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RModule::
//***************************************

IModuleLibrary* RModule::get_library(void) const
{
  return owner;
}

const SModuleInfo& RModule::get_info(void) const
{
  return minfo;
}

//***************************************

bool RModule::is_open(void) const
{
  return ireg!=NULL && ireg->mr_isopen();
}

bool RModule::open(void)
{
  dbg_put(rsdl_Module, "RModule(%p)::open()\n", this);
  if(ireg!=NULL)
    return ireg->mr_open();
  if(!owner->lib_load())
    return false;
  if(ireg!=NULL && ireg->mr_open())
    return true;
  owner->lib_unload();
  return false;
}

bool RModule::close(void)
{
  dbg_put(rsdl_Module, "RModule(%p)::close()\n", this);
  if(ireg==NULL)
    return false;
  ireg->mr_close();
  if(!ireg->mr_isopen())
    owner->lib_unload();
  return true;
}

//***************************************

bool RModule::reg(IModObject* mo, bool doreg)
{
  if(!doreg) {
    return unreg_obj(mo);
  }
  ModuleObjInfo* mi = reg_obj(mo);
  if(mi==NULL)
    return false;
  return true;
}

bool RModule::reg(IGenerator* gen, bool doreg)
{
  if(!doreg) {
    return unreg_obj(gen);
  }
  ModuleObjInfo* mi = reg_obj(gen);
  if(mi==NULL)
    return false;
  if(mi->start_gen==array_index_none) { // do rescan
    return reg_generator(mi, gen);
  }
  return true;
}

//***************************************

size_t RModule::mod_findobjname(core::IRefObject::TypeId intr, core::IRefObject::TypeId found[], size_t nfound)
{
  size_t rv=0;
  for(size_t i=0; i<~mogts; i++) {
    if(mogts(i)!=intr)
      continue;
    if(found!=NULL) {
      if(rv>=nfound) 
        break;
      for(size_t j=0; j<~mogis; j++) {
        const GeneratedObjInfo& x = mogis(j);
        if(x.start_elem<=i && i<x.start_elem+x.count_elem) {
          found[rv++] = x.tid;
          break;
        }
      }
    } else {
      rv++;
    }
  }
  return rv;
}

bool RModule::mod_generate(core::IRefObject::P& obj, core::IRefObject::TypeId name, const core::SParameters& args)
{
  for(size_t j=0; j<~mogis; j++) {
    const GeneratedObjInfo& x = mogis(j);
    if(x.tid!=name)
      continue;
    for(size_t k=0; k<~mos; k++) {
      const ModuleObjInfo& mi = mos(k);
      if(mi.start_gen<=j && j<mi.start_gen+mi.count_gen) {
        bool fld = false;
        if(!is_open()) {
          if(!open())
            return false; // this module fails
          fld = true;
          dbg_put(rsdl_Module, "RModule(%p):: pre-loaded\n", this);
        }
        IGenerator* g = static_cast<IGenerator*>(mi.mo);
        if(g==NULL) {
          if(fld)
            close();
          return false; // internal error
        }
        dbg_put(rsdl_Module, "RModule(%p)::generate() { gen=%p }\n", this, g);
        if(g->generate(obj, name, args)) {
          if(fld)
            close();
          return true;
        }
        if(fld)
          close();
        break; // try next generator
      }
    }
  }
  return false;
}

//***************************************
//***************************************

bool RModule::reg_generator(ModuleObjInfo* mi, IGenerator* gen)
{
  HUniquePointer alist[max_generated_names];
  SUniquePtrList lst(alist, max_generated_names);
  size_t bn = ~mogis;
  mi->start_gen = bn;
  mi->count_gen = 0;
  if(gen->get_generated_names(lst)==0)
    return true; // empty generator
  size_t nn = ~lst;
  if(!mogis.pushn(nn)) {
    err_put(
      rsdl_Module, 
      "RModule(%p)::reg_generator(%s %p) -- allocate error: info\n", 
      this, gen->get_interface_info().name, gen
    );
    return false;
  }
  //
  for(size_t i=0; i<nn; i++) {
    GeneratedObjInfo& gi = mogis[i+bn];
    gi.tid = lst[i];
  }
  mi->count_gen = nn;
  //
  for(size_t i=0; i<nn; i++) {
    GeneratedObjInfo& gi = mogis[i+bn];
    lst.clear();
    if(gen->get_generated_types(lst, gi.tid)==0)
      continue;
    size_t btn = ~mogts;
    size_t nt = ~lst;
    gi.start_elem = btn;
    if(!mogts.pushn(nt)) {
      err_put(
        rsdl_Module, 
        "RModule(%p)::reg_generator(%s %p) -- allocate error: type\n", 
        this, gen->get_interface_info().name, gen
      );
      return false;
    }
    for(size_t k=0; k<nt; k++) {
      mogts[btn+k] = lst[k];
    }
    gi.count_elem = nt;
  }
  return true;
}

RModule::ModuleObjInfo* RModule::reg_obj(IModObject* mo)
{
  if(mo==NULL)
    return NULL;
  TypeId uid = mo->get_uid();
  TypeId tid = mo->get_interface_type();
  dbg_put(
    rsdl_Module, 
    "RModule(%p)::reg_obj(%s %p) -- { %p:%s(%d) %p:%s(%d) }\n", 
    this, mo->get_interface_info().name, mo,
    tid, tid->name, tid->verno,
    uid, uid->name, uid->verno
  );
  if(uid==NULL) {
    err_put(
      rsdl_Module, 
      "RModule(%p)::reg_obj(%s %p) -- integrity error: no uid\n", 
      this, mo->get_interface_info().name, mo
    );
    return NULL;
  }
  //
  for(size_t i=0; i<~mos; i++) {
    ModuleObjInfo& mi = mos[i];
    if(mi.uid==uid) {
      if(mi.mo!=NULL && mi.mo!=mo) {
        err_put(
          rsdl_Module, 
          "RModule(%p)::reg_obj(%s %p) -- integrity error: invalid mo/uid xref mo{%p uid=%p} reg{%p uid=%p}\n", 
          this, mo->get_interface_info().name, mo,
          mo, uid, mi.mo, mi.uid
        );
      }
      mi.mo = mo;
      mi.reg_count++;
      return &mi; // already registered
    }
  }
  //
  if(!mos.push()) {
    err_put(
      rsdl_Module, 
      "RModule(%p)::reg_obj(%s %p) -- alloc error\n", 
      this, mo->get_interface_info().name, mo
    );
    return NULL;
  }
  //
  ModuleObjInfo* rv = &mos.last();
  rv->mo = mo;
  rv->tid = tid;
  rv->uid = uid;
  rv->reg_count++;
  return rv;
}

bool RModule::unreg_obj(IModObject* mo)
{
  if(mo==NULL)
    return false;
  TypeId uid = mo->get_uid();
  if(uid==NULL) {
    err_put(
      rsdl_Module, 
      "RModule(%p)::unreg_obj(%s %p) -- integrity error: no uid\n", 
      this, mo->get_interface_info().name, mo
    );
    return false;
  }
  for(size_t i=0; i<~mos; i++) {
    ModuleObjInfo& mi = mos[i];
    if(mi.uid==uid || mi.mo==mo) {
      if(mi.uid!=uid || mi.mo!=mo) {
        err_put(
          rsdl_Module, 
          "RModule(%p)::unreg_obj(%s %p) -- integrity error: invalid mo/uid xref mo{%p uid=%p} reg{%p uid=%p}\n", 
          this, mo->get_interface_info().name, mo,
          mo, uid, mi.mo, mi.uid
        );
        return false; // integrity error
      }
      if(--mi.reg_count<=0) {
        mi.mo = NULL; // clear pointer
      }
      return true; // ok
    }
  }
  return false;
}

bool RModule::alloc_minfo(const SModuleInfo &mi2)
{
  dbg_put(
    rsdl_Module, "RModule::alloc_minfo({\"%s\", \"%s\", {%d,%d} })\n", 
    mi2.name(), mi2.info(), mi2.verno.vmajor, mi2.verno.vminor
  );
  minfo.name = owner->sys->get_modloader()->moduledb_get_string(mi2.name);
  minfo.info = owner->sys->get_modloader()->moduledb_get_string(mi2.info);
  minfo.verno = mi2.verno;
  return true;
}

bool RModule::scan(void)
{
  if(ireg==NULL)
    return false;
  //
//  dbg_put(rsdl_Module, "RModule(%p)::open()\n", this);
  if(!open()) // also scan if possible
    return false;
  close();
  return true;
}

bool RModule::save_db(FILE *f)
{
  fprintf(f, "  BEGIN MODULE %s(%d,%d)\n", minfo.name(), minfo.verno.vmajor, minfo.verno.vminor);
  for(size_t i=0; i<~mos; i++) {
    const ModuleObjInfo& mi = mos(i);
    dbg_put(
      rsdl_Module, 
      "RModule(%p)::save_db(...) -- { %p %p }\n", 
      this,
      mi.tid, 
      mi.uid
    );
/*
    dbg_put(
      rsdl_Module, 
      "RModule(%p)::save_db(...) -- { %p:%s(%d) %p:%s(%d) }\n", 
      this,
      mi.tid, mi.tid->name, mi.tid->verno,
      mi.uid, mi.uid->name, mi.uid->verno
    );
*/
    if(mi.start_gen==array_index_none) { // modobjects
      fprintf(
        f, "    MODOBJECT %s(%d) %s(%d)\n", 
        mi.tid->name, mi.tid->verno,
        mi.uid->name, mi.uid->verno
      );
    } else {
      fprintf(
        f, "    BEGIN GENERATOR %s(%d) %s(%d)\n", 
        mi.tid->name, mi.tid->verno,
        mi.uid->name, mi.uid->verno
      );
      size_t bp = mi.start_gen;
      for(size_t k=0; k<mi.count_gen; k++) {
        const GeneratedObjInfo& gi = mogis(bp+k);
        fprintf(
          f, "      BEGIN REFOBJECT %s(%d)\n", 
          gi.tid->name, gi.tid->verno
        );
        for(size_t kk=0; kk<gi.count_elem; kk++) {
          TypeId tid = mogts(gi.start_elem+kk);
          fprintf(
            f, "        TYPE %s(%d)\n", 
            tid->name, tid->verno
          );
        }
        fprintf(
          f, "      END REFOBJECT %s(%d)\n", 
          gi.tid->name, gi.tid->verno
        );
      }
      fprintf(
        f, "    END GENERATOR %s(%d) %s(%d)\n", 
        mi.tid->name, mi.tid->verno,
        mi.uid->name, mi.uid->verno
      );
    }
  }
  fprintf(f, "  END MODULE %s(%d,%d)\n", minfo.name(), minfo.verno.vmajor, minfo.verno.vminor);
  return true;
}

//***************************************
//***************************************

RModule::RModule(RModuleLibrary *pv, IModuleReg *imr)
: owner(NULL), ireg(imr)
{
  rc_init(pv);
  if(ireg!=NULL) {
    alloc_minfo(ireg->minfo);
    ireg->module = this;
  }
}

RModule::~RModule(void)
{
  if(ireg!=NULL) {
    ireg->module = NULL;
    ireg = NULL;
  }
}

UMODSYS_REFOBJECT_UNIIMPLEMENT_BODY(RModule)

//***************************************
// ::
//***************************************
