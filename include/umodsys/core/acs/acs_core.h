#ifndef __UMODSYS_CORE_ACS_CORE_H
#define __UMODSYS_CORE_ACS_CORE_H 1
/*************************************************************/
// file: umodsys/base/acs/acs_core.h
// info: ACS core objects
/*************************************************************/

#include <umodsys/core/stdstrings.h>

namespace UModSys {
namespace core {
namespace ACS {

typedef int BInt;
typedef double BFloat;
typedef DCString BStr;

struct IValue;
 struct IType;
 struct IInteger;
 struct IString;
 struct ISymbol;
 struct IClosure;
 struct IList;
 struct IHash;

typedef tl::TRefObject<IValue>     PValue;
typedef tl::TRefObject<IType>      PType;
typedef tl::TRefObject<IInteger>   PInteger;
typedef tl::TRefObject<IString>    PString;
typedef tl::TRefObject<ISymbol>    PSymbol;
typedef tl::TRefObject<IList>      PList;
typedef tl::TRefObject<IHash>      PHash;
typedef tl::TRefObject<IClosure>   PClosure;


enum eDefaultType {
  dt_Null,
  dt_Symbol,
  dt_Type,
  dt_Integer,
  dt_Float,
  dt_String,
  dt_Closure,
  dt_Class,
  dt_Object,
};

struct IVM;

struct IValue {
  virtual void ref_add(void) = 0;
  virtual void ref_remove(void) = 0;
  virtual PType get_type(IVM* vm) = 0;
  virtual int get_typeid(IVM* vm) = 0; // or eDefaultType
  virtual PString to_string(IVM* vm) = 0;
  virtual PInteger to_integer(IVM* vm) = 0;
  virtual PFloat to_float(IVM* vm) = 0;
  virtual void call(IVM* vm, IList* args, IList* result) = 0;
};

struct IType : IValue {
  
};

struct IList : IValue {
  virtual void clear(void) = 0;
  virtual void resize(IVM* vm, int size) = 0;
  virtual int get_size(IVM* vm) = 0;
  virtual PValue get_idx(IVM* vm, int idx) = 0;
  virtual void set_idx(IVM* vm, int idx, IValue* value) = 0;
};

struct IHash : IValue {
  virtual void clear(void) = 0;
  virtual PValue get_idx(IVM* vm, IValue* idx) = 0;
  virtual void set_idx(IVM* vm, IValue* idx, IValue* value) = 0;
};

struct IInteger : IValue {
  virtual BInt get_value(IVM* vm) = 0;
  virtual void set_value(IVM* vm, BInt value) = 0;
};

struct IFloat : IValue {
  virtual BInt get_value(IVM* vm) = 0;
  virtual void set_value(IVM* vm, BInt value) = 0;
};

struct IString : IValue {
  virtual BStr get_value(IVM* vm) = 0;
  virtual void set_value(IVM* vm, BStr value) = 0;
};

struct ISymbol : IValue {
  virtual BStr get_text(IVM* vm) = 0;
  virtual BInt get_uid(IVM* vm) = 0;
};

struct IClosure : IValue {
};


struct IVM {
  virtual PSymbol  get_symbol(BStr text) =0;
  virtual PString  new_string(BStr value) =0;
  virtual PInteger new_int(BInt value) =0;
  virtual PFloat   new_float(BFloat value) =0;
  virtual PList    new_list(int size=0) =0;
  virtual PHash    new_hash(void) =0;
};

} // namespace ACS
} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_LANG_ACS_CORE_H
