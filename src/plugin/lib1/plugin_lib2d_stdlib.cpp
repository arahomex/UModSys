#include <umodsys/base/module_ref.h>

UMODSYS_MODULE_DEF(lib2d,stdlib);

void refer(void)
{
  UMODSYS_MODULE_USE(lib2d,stdlib);
}

