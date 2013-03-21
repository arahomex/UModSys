#include "rsystem/umodsys.base.rsystem.h"
#include "rsystem/umodsys.base.rconsole_std.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

int main(int argc, char **argv)
{
  RSystem::s_sys.set_console(&RConsole_std::s_console);
  //
  RSystem::s_sys.init();
  RSystem::s_sys.moduledb_scan("");
  RSystem::s_sys.moduledb_scan("*");
  RSystem::s_sys.exec_args(argc, argv);
  RSystem::s_sys.exec_main();
  //
  RSystem::s_sys.deinit();
  RSystem::s_sys.set_console(NULL);
  return 0;
}

