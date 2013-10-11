#include "umodsys.basesys.rsystem.h"
#include "umodsys.basesys.rconsole_std.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

UMODSYS_MODULE_DEF(media,stdio);

static void refer(void)
{
  UMODSYS_MODULE_USE(media,stdio);
}

int main(int argc, char **argv)
{
  syshlp::setup_console();
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
  syshlp::restore_console();
  lost_func(refer);
  return 0;
}

