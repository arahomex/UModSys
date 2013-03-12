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
  static core::SUniquePointerInfo* get_id(void) { return s_id; }
};

template<typename Object>
core::SUniquePointer TObjectUniqueID<Object>::s_id(
  "typeinfo", 
  Object::_get_interface_cname(), 
  Object::_get_interface_verno() 
);

/*************************************************************/
/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_UTIL_OBJECTID_H
