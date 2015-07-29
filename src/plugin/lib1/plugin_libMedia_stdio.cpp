#include <umodsys/base/module_ref.h>

UMODSYS_MODULE_DEF(media,stdio);

void refer(void)
{
  UMODSYS_MODULE_USE(media,stdio);
}


