#include <umodsys/base/module_ref.h>

UMODSYS_MODULE_DEF(lib3d,stdlib);

void refer(void)
{
  UMODSYS_MODULE_USE(lib3d,stdlib);
}

