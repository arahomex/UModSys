#ifndef __UMODSYS_COMMON_OBJECTS_H
#define __UMODSYS_COMMON_OBJECTS_H 1
/*************************************************************/
// file: umodsys/common/base/obj_core.h
// info: objects and interface bases
/*************************************************************/

#include <umodsys/common/stdtypedefs.h>
#include <umodsys/common/base/source.h>
#include <umodsys/common/base/uniq.h>

#include <umodsys/tl/util/ref_pointer.h>
#include <umodsys/tl/util/object_id.h>

//#include <umodsys/common/gc/gc-tiny.h>

namespace UModSys {
namespace core {

#include <umodsys/common/base/obj_core_def.h>
#include <umodsys/common/base/obj_core_root.h>
#include <umodsys/common/base/obj_core_refobject.h>
#include <umodsys/common/base/obj_core_gcobject.h>

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_COMMON_OBJECTS_H
