#include <umodsys/base/module_ref.h>

UMODSYS_MODULE_DEF(ui,SDL_core);

void refer(void)
{
  UMODSYS_MODULE_USE(ui,SDL_core);
}


