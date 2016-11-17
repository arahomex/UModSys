#include <umodsys/core/module_ref.h>

UMODSYS_MODULE_DEF(media,images_std);

void refer(void)
{
  UMODSYS_MODULE_USE(media,images_std);
}

