#include "rsystem/umodsys.base.rsystem.h"
#include "rsystem/umodsys.base.rconsole_std.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

int main(int argc, char **argv)
{
  RConsole_std::s_console.reflect("umodsys.log");
  RSystem& S = RSystem::s_sys;
  //
  M.Link(&S, S.get_sharemem());
  S.set_console(&RConsole_std::s_console);
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
  return 0;
}

