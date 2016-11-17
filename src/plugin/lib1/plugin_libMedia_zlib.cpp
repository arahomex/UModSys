#include <umodsys/core/module_ref.h>

UMODSYS_MODULE_DEF(media,zlib);

void refer(void)
{
  UMODSYS_MODULE_USE(media,zlib);
}


