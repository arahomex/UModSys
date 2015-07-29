#include <umodsys/base/module_ref.h>

UMODSYS_MODULE_DEF(media,stdio);
UMODSYS_MODULE_DEF(media,stdlib);
UMODSYS_MODULE_DEF(media,serialize_json);
UMODSYS_MODULE_DEF(media,serialize_yaml);
UMODSYS_MODULE_DEF(media,zlib);

void refer(void)
{
  UMODSYS_MODULE_USE(media,stdio);
  UMODSYS_MODULE_USE(media,stdlib);
  UMODSYS_MODULE_USE(media,serialize_json);
  UMODSYS_MODULE_USE(media,serialize_yaml);
  UMODSYS_MODULE_USE(media,zlib);
}


