#ifndef __UMODSYS_CORE_STDMATH3D_H
#define __UMODSYS_CORE_STDMATH3D_H 1
/*************************************************************/
// file: umodsys/core/stdmath3d.h
// info: math3d instantiation
/*************************************************************/

#include <umodsys/stdcore.h>
#include <umodsys/tl/math/math_3d.h>
#include <umodsys/tl/util/type_constructor.h>
#include <umodsys/tl/util/object_id.h>

namespace UModSys {

#define UMODSYS_MULTI_TYPE(_templ) \
  UMODSYS_SCALAR_CONSTRUCTOR_T1(T, math3d::_templ<T>) \
  UMODSYS_TL_TDATAUNIQUEID_ATOM( math3d::_templ<core::uint8> ) \
  UMODSYS_TL_TDATAUNIQUEID_ATOM( math3d::_templ<core::sint8> ) \
  UMODSYS_TL_TDATAUNIQUEID_ATOM( math3d::_templ<core::uint16> ) \
  UMODSYS_TL_TDATAUNIQUEID_ATOM( math3d::_templ<core::sint16> ) \
  UMODSYS_TL_TDATAUNIQUEID_ATOM( math3d::_templ<core::uint32> ) \
  UMODSYS_TL_TDATAUNIQUEID_ATOM( math3d::_templ<core::sint32> ) \
  UMODSYS_TL_TDATAUNIQUEID_ATOM( math3d::_templ<core::float32> ) \
  UMODSYS_TL_TDATAUNIQUEID_ATOM( math3d::_templ<core::float64> )

UMODSYS_MULTI_TYPE(TVector4)
UMODSYS_MULTI_TYPE(TVector3)
UMODSYS_MULTI_TYPE(TVector2)

UMODSYS_MULTI_TYPE(TMatrix44)
UMODSYS_MULTI_TYPE(TMatrix33)
UMODSYS_MULTI_TYPE(TMatrix34)

UMODSYS_MULTI_TYPE(TQuaternion4)
UMODSYS_MULTI_TYPE(TPlane3)

UMODSYS_MULTI_TYPE(TBoundBox3)
UMODSYS_MULTI_TYPE(TBoundBox2)

#undef UMODSYS_MULTI_TYPE

} // namespace UModSys

#endif // __UMODSYS_CORE_STDMATH3D_H
