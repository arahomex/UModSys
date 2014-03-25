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

template<typename Object> struct TObjectUniqueID;

template<typename Object> struct TDataUniqueID;

template<typename Base, size_t Value> struct TConstUniqueID;

struct SDataUniqueID;

/*************************************************************/
/*************************************************************/

struct SDataUniqueID {
  core::HUniquePointer id;
  //
  SDataUniqueID(void) : id(NULL) {}
  SDataUniqueID(core::HUniquePointer i) : id(i) {}
  //
  inline bool valid(void) const { return id!=NULL; }
};

/*************************************************************/
/*************************************************************/

template<typename Object>
struct TObjectUniqueID {
  static core::SUniquePointer s_id;
  //
  inline static SDataUniqueID get_uid(void) { return SDataUniqueID(s_id); }
  inline static core::HUniquePointer get_id(void) { return s_id; }
  inline static const core::SUniquePointerInfo& get_info(void) { return s_id.info; }
};

template<typename Object>
core::SUniquePointer TObjectUniqueID<Object>::s_id(
  "typeinfo", 
  Object::_root_get_interface_infoname(), 
  Object::_root_get_interface_infover() 
);

/*************************************************************/

template<typename Object>
struct TDataUniqueID {
  static core::SUniquePointer s_id;
  //
  inline static SDataUniqueID get_uid(void) { return SDataUniqueID(s_id); }
  inline static core::HUniquePointer get_id(void) { return s_id; }
  inline static const core::SUniquePointerInfo& get_info(void) { return s_id.info; }
  //
  static const char* data_name(void);
  static int data_version(void);
};

template<typename Object>
inline const char* TDataUniqueID<Object>::data_name(void) 
{ 
  return Object::_data_infoname(); 
}
template<typename Object>
inline int TDataUniqueID<Object>::data_version(void) 
{ 
  return Object::_data_infover(); 
}

template<typename Object>
core::SUniquePointer TDataUniqueID<Object>::s_id(
  "datainfo", 
  TDataUniqueID<Object>::data_name(), 
  TDataUniqueID<Object>::data_version() 
);


/*************************************************************/

template<typename Base, size_t Value>
struct TConstUniqueID {
  static core::SUniquePointer s_id;
  //
  inline static core::HUniquePointer get_id(void) { return s_id; }
  inline static const core::SUniquePointerInfo& get_info(void) { return s_id.info; }
  //
  static const char* const_name(void);
  static int const_version(void);
};

/*
template<typename Base, size_t Value> 
const char* TConstUniqueID<Base, Value>::const_name(void) 
{ 
  return NULL; 
}
*/
template<typename Base, size_t Value> 
int TConstUniqueID<Base, Value>::const_version(void) 
{ 
  return 0; 
}

template<typename Base, size_t Value>
core::SUniquePointer TConstUniqueID<Base, Value>::s_id(
  "const", 
  TConstUniqueID<Base, Value>::const_name(), 
  0
);

template<typename Base, size_t Value>
inline core::HUniquePointer const_unique_id(void)
{
  return TConstUniqueID<Base, Value>::get_id();
}


/*************************************************************/
// TDataUniqueID for ordinal types
/*************************************************************/

#define UMODSYS_TL_TDATAUNIQUEID(_type, _name, _ver) \
  template<> inline const char* ::UModSys::tl::TDataUniqueID<_type>::data_name(void) { return _name; } \
  template<> inline int ::UModSys::tl::TDataUniqueID<_type>::data_version(void) { return _ver; }
#define UMODSYS_TL_TDATAUNIQUEID_ATOM(_type) UMODSYS_TL_TDATAUNIQUEID(_type, #_type, 0)

#define UMODSYS_TL_TCONSTUNIQUEID(_type, _val, _name, _ver) \
  template<> inline const char* ::UModSys::tl::TConstUniqueID<_type, _val>::const_name(void) { return _name; } \
  template<> inline int ::UModSys::tl::TConstUniqueID<_type, _val>::const_version(void) { return _ver; }
#define UMODSYS_TL_TCONSTUNIQUEID_ATOM(_type, _val) UMODSYS_TL_TCONSTUNIQUEID(_type, _val, #_val, 0)

/*************************************************************/
/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_UTIL_OBJECTID_H
