#include <umodsys/base/module_ref.h>

UMODSYS_MODULE_DEF(lib2d,stdlib);
UMODSYS_MODULE_DEF(lib3d,stdlib);
UMODSYS_MODULE_DEF(ui,frames);
UMODSYS_MODULE_DEF(media,images_std);

void refer(void)
{
  UMODSYS_MODULE_USE(lib2d,stdlib);
  UMODSYS_MODULE_USE(lib3d,stdlib);
  UMODSYS_MODULE_USE(ui,frames);
  UMODSYS_MODULE_USE(media,images_std);
}

