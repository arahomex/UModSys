#ifndef __UMODSYS_CORE_OBJECTS_RESOURCE_H
#define __UMODSYS_CORE_OBJECTS_RESOURCE_H 1
/*************************************************************/
// file: umodsys/common/objects_res.h
// info: resource objects
/*************************************************************/

#include <umodsys/common/objects.h>

namespace UModSys {
namespace core {

//***************************************
// TIResource
//***************************************

template<typename BType>
struct TIResource : public IRefObject {
public:
  typedef BType ResourceType;
  typedef TIResource<BType> ThisResource;
public:
  virtual ResourceType* resource_get(void) const =0;
  virtual bool resource_isloaded(void) const =0;
  virtual size_t resource_getsize(core::HUniquePointer location) const =0;
  //
  virtual bool resource_load(void) =0;
  virtual bool resource_save(void) =0;
  virtual bool resource_clear(void) =0;
  virtual bool resource_sethint(core::HUniquePointer hint, int hintvalue) =0;
protected:
  UMODSYS_ROOT_INTIMPLEMENT_NONAME(ThisResource, 1, IRefObject);
public:
  typedef tl::TRefObject<ThisResource> P, DInterfaceP;
};

//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_SOURCE_H
