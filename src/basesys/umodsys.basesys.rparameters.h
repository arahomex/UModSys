#ifndef __UMODSYS_BASE_RPARAMETERS_H
#define __UMODSYS_BASE_RPARAMETERS_H 1
/*************************************************************/
// file: src/base/console/umodsys.base.rparameters.h
// info: implementation of parameter worker
/*************************************************************/

#include "umodsys.basesys.rcommon.h"

namespace UModSys {
namespace base {
namespace rsystem {

using namespace core;

//***************************************
// RParameters
//***************************************

struct RParameters : public IParameters {
  //
  typedef tl::TRefObjectFunc<IRefObject> DRefObjFunc;
  typedef tl::TRefObjectFunc<IParameters> DParametersFunc;
  //
  // SParametersData
  void p_deinit(SParametersData *p);
  void p_init(SParametersData *p, int defalloc);
  void p_init(SParametersData *p, const SParametersData& r);
  void p_init_s(SParametersData *p, void* buffer, int nalloc); // static
  void p_init_s(SParametersData *p, const SParametersData& r, void *buffer, int nalloc); // static
  void p_copy(SParametersData *p, const SParametersData& r);
  //
  bool add(SParametersData *p, BCStr name, BCStr value);
  bool add(SParametersData *p, BCStr name, int value);
  bool add(SParametersData *p, BCStr name, const double &value);
  bool add(SParametersData *p, BCStr name, IRefObject *value);
  //
  bool get(const SParametersData *p, BCStr name, BCStr &value);
  bool get(const SParametersData *p, BCStr name, int &value);
  bool get(const SParametersData *p, BCStr name, double &value);
  bool get(const SParametersData *p, BCStr name, IRefObject* &value);
  //
  bool next(const SParametersData *p, BCStr &name);
  bool next(const SParametersData *p, DCString& name);
public:
  enum ElemType {
    et_VArray = 0x8000,
    //
    et_Null = 0,
    et_Int,
    et_Double,
    et_String,
    et_RefObject,
    et_Last
  };
  //
  static int vsizes[];
  //
  struct ElemSValue {
    int len; // rounded up to (intsize) bytes quant
    BChar value[1];
  };
  //
  struct Elem { // for normal
    int type; 
    int full_len; // rounded up to (intsize) bytes quant total length
    int name_hash;
    int name_len; // rounded up to (intsize) bytes quant
    BChar name[1];
    //
    int &ival(void) { return *reinterpret_cast<int*>(name+name_len); }
    double &dval(void) { return *reinterpret_cast<double*>(name+name_len); }
    ElemSValue& sval(void) { return *reinterpret_cast<ElemSValue*>(name+name_len); }
    IRefObject* &oval(void) { return *reinterpret_cast<IRefObject**>(name+name_len); }
    //
    int ival(void) const { return *reinterpret_cast<const int*>(name+name_len); }
    const double &dval(void) const { return *reinterpret_cast<const double*>(name+name_len); }
    const ElemSValue& sval(void) const { return *reinterpret_cast<const ElemSValue*>(name+name_len); }
    IRefObject* oval(void) const { return *reinterpret_cast<IRefObject*const*>(name+name_len); }
    //
    Elem* next(void) { return reinterpret_cast<Elem*>(reinterpret_cast<char*>(this)+full_len); }
    const Elem* next(void) const { return reinterpret_cast<const Elem*>(reinterpret_cast<const char*>(this)+full_len); }
    int end_size(const void* start) const { return reinterpret_cast<const char*>(next())-reinterpret_cast<const char*>(start); }
  };
  //
protected:
  ISystem* sys;
  bool strict;
  //
  const Elem* find_elem(const void *data, int max, int type, int namehash, BCStr name); // if NULL - not found desired type
  Elem* get_last(void *data, int current, int max, int estimated_size); // if NULL - no more space
  int estimate_size(Elem& tmp, int namehash, BCStr name, int type, int typesizeext=0); // typesizeext is valid only for string values
  void free_elems(void *data, int max);
  void alloc_elems(void *data, int max);
  bool add_last(SParametersData *p, BCStr name, const void *value, int type);
public:
  RParameters(ISystem* s);
  ~RParameters(void);
public:
  UMODSYS_REFOBJECT_IMPLEMENT1(UModSys::base::rsystem::RParameters, 2, IParameters);
  UMODSYS_REFOBJECT_SINGLE()
};


//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RPARAMETERS_H
