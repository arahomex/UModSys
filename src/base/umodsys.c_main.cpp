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
  S.set_console(&RConsole_std::s_console);
  //
  S.init();
  S.moduledb_scan("", false);
  S.moduledb_scan("*", true);
  S.moduledb_save("moduledb.conf-hdb");
  //
  S.exec_args(argc, argv);
  S.exec_main();
  //
  S.deinit();
  S.set_console(NULL);
  return 0;
}

