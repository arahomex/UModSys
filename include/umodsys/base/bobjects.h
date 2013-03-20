#ifndef __UMODSYS_BASE_OBJECTS_H
#define __UMODSYS_BASE_OBJECTS_H 1
/*************************************************************/
// file: umodsys/base/bobjects.h
// info: base objects
/*************************************************************/

#include <umodsys/core/objects.h>
#include <umodsys/base/module.h>
#include <umodsys/tl/util/type_constructor.h>

namespace UModSys {
namespace base {

using core::SParameters;
using core::DCString;
using core::BCStr;
using core::IRefObject;

//***************************************
// DEFINES
//***************************************

#define UMODSYS_SHELL(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_IMPLEMENT1(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_UNIIMPLEMENT() \
  inline void rc_init(void) {} \
  inline virtual void suicide(void) {}

//***************************************
// IModObject::  - per-module class base
//***************************************

struct IModObject : public IRefObject {
  virtual IModule* get_module(void) const =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(base::IModObject, 2, IRefObject);
};

//***************************************
// IGenerator::  - generator class base
//***************************************

struct IGenerator : public IModObject {
  virtual int get_generated_names(DPtrList& list) const =0; // only names
  virtual int get_generated_types(DPtrList& list, const DCString& name) const =0; // may have different types per name
  virtual bool generate(IRefObject::P& obj, const DCString& type, const SParameters& args) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(base::IGenerator, 2, IModObject);
};

//***************************************
// IShell::  - main module functionality
//***************************************

struct IShell : public IModObject {
  virtual bool connect(BCStr command) =0;
  virtual bool disconnect(BCStr command) =0;
  virtual bool process_command(BCStr command, BCStr args) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(base::IShell, 2, IModObject);
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_OBJECTS_H
