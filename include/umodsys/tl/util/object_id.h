#ifndef __UMODSYS_TL_UTIL_OBJECTID_H
#define __UMODSYS_TL_UTIL_OBJECTID_H 1
/*************************************************************/
// file: umodsys/tl/util/object_id.h
// info: x-map of object UniPtr
/*************************************************************/

#include <umodsys/core/source.h>
#include <umodsys/core/uniqueptr.h>

namespace UModSys {
namespace tl {

/*************************************************************/

template<typename Object>
struct TObjectUniqueID;

/*************************************************************/
/*************************************************************/

template<typename Object>
struct TObjectUniqueID {
  static core::SUniquePointer s_id;
  //
  inline static const core::HUniquePointer get_id(void) { return s_id; }
  inline static const core::SUniquePointerInfo& get_info(void) { return s_id.info; }
};

template<typename Object>
core::SUniquePointer TObjectUniqueID<Object>::s_id(
  "typeinfo", 
  Object::_get_interface_infoname(), 
  Object::_get_interface_infover() 
);

/*************************************************************/
/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_UTIL_OBJECTID_H
