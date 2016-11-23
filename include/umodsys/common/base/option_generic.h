#ifndef __UMODSYS_COMMON_OPTION_GENERIC_H
#define __UMODSYS_COMMON_OPTION_GENERIC_H 1
/*************************************************************/
// file: umodsys/common/base/option_generic.h
// info: genetic option object (duck typed)
/*************************************************************/

#include <umodsys/common/stdtypedefs.h>
#include <umodsys/common/stdstrings.h>
#include <umodsys/common/base/obj_core.h>
#include <umodsys/common/base/mem_fixed.h>

namespace UModSys {
namespace core {

enum eOptionType;

struct IOptionNode;                         // any option
struct TIOptionNode<eOptionType type>;      // typed option

//***************************************
// eOptionType
//***************************************

enum eOptionType {
  opt_Null,
  opt_Undefined,
  opt_Bool,
  opt_Int32,
  opt_Int64,
  opt_Float32,
  opt_Float64,
  opt_String,
  opt_SString,
  opt_RefObject,
  opt_Struct,
  opt_Tuple,
  opt_Array,
  opt_Map,
  opt_Unknown,
};

struct EOptionTypeCast {
  eOptionType from, to;
  //
  EOptionTypeCast(eOptionType f, eOptionType t) : from(f), to(t) {}
};

//***************************************
// IOptionNode::
//***************************************

struct IOptionNode {
  virtual eOptionType get_type(void) const =0;
  //
  template<eOptionType type>
  const TIOptionNode<type>* t_as(void) const {
    if(type!=get_type()) {
      throw EOptionTypeCast(get_type(), type);
    }
    return static_cast< TIOptionNode<type>* >(this);
  }
  //
  template<eOptionType type>
  TIOptionNode<type>* t_as(void) {
    if(type!=get_type()) {
      throw EOptionTypeCast(get_type(), type);
    }
    return static_cast< TIOptionNode<type>* >(this);
  }
};

//***************************************
// TIOptionNode<?>::
//***************************************


struct TIOptionNode<opt_Null> : IOptionNode {
  enum { opt_type = opt_Null };
  eOptionType get_type(void) const { return opt_type; }
};

struct TIOptionNode<opt_Undefined> : IOptionNode {
  enum { opt_type = opt_Undefined };
  eOptionType get_type(void) const { return opt_type; }
};

struct TIOptionNode<opt_Bool> : IOptionNode {
  enum { opt_type = opt_Undefined };
  eOptionType get_type(void) const { return opt_type; }
  //
  virtual bool get_data(void) const =0;
  virtual void set_data(bool v) =0;
};

struct TIOptionNode<opt_Int32> : IOptionNode {
  enum { opt_type = opt_Int32 };
  eOptionType get_type(void) const { return opt_type; }
  //
  virtual Bsint32 get_data(void) const =0;
  virtual void    set_data(BSint32 v) =0;
};


//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_COMMON_OPTION_GENERIC_H
