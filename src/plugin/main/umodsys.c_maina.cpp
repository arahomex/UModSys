#include "umodsys.basesys.rsystem.h"
#include "umodsys.basesys.rconsole_std.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

static void refer(void);

int main(int argc, char **argv)
{
  syshlp::con_setup();
  RConsole_std::s_console.reflect("umodsys.log");
  RSystem& S = RSystem::s_sys;
  //
  S.set_console(&RConsole_std::s_console);
  M.Link(&S, S.get_sharemem());
  //
  S.init();
  S.get_modloader()->moduledb_scan("", false);
  S.get_modloader()->moduledb_scan("*", true);
  S.get_modloader()->moduledb_save("moduledb.conf-hdb");
  //
  S.exec_args(argc, argv);
  //
  S.exec_tests();
  S.exec_main();
  //
  S.deinit();
  S.set_console(NULL);
  RConsole_std::s_console.reflect(NULL);
  //
  syshlp::con_restore();
  lost_func(refer);
  return 0;
}

UMODSYS_MODULE_DEF(media,stdio);
UMODSYS_MODULE_DEF(media,stdlib);
UMODSYS_MODULE_DEF(media,serialize_json);
UMODSYS_MODULE_DEF(media,serialize_yaml);
UMODSYS_MODULE_DEF(media,zlib);
UMODSYS_MODULE_DEF(lib2d,stdlib);
UMODSYS_MODULE_DEF(lib3d,stdlib);
UMODSYS_MODULE_DEF(ui,frames);
UMODSYS_MODULE_DEF(media,images_std);
UMODSYS_MODULE_DEF(ui,SDL_core);
UMODSYS_MODULE_DEF(ui,SDL_GL);

static void refer(void)
{
  UMODSYS_MODULE_USE(media,stdio);
  UMODSYS_MODULE_USE(media,stdlib);
  UMODSYS_MODULE_USE(media,serialize_json);
  UMODSYS_MODULE_USE(media,serialize_yaml);
  UMODSYS_MODULE_USE(media,zlib);
  UMODSYS_MODULE_USE(lib2d,stdlib);
  UMODSYS_MODULE_USE(lib3d,stdlib);
  UMODSYS_MODULE_USE(ui,frames);
  UMODSYS_MODULE_USE(media,images_std);
  UMODSYS_MODULE_USE(ui,SDL_core);
  UMODSYS_MODULE_USE(ui,SDL_GL);
}
