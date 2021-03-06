#include "umodsys.basesys.rsystem.h"
#include "umodsys.basesys.rconsole_std.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

static void refer(void);

int main(int argc, char **argv)
{
  syshlp::validate_env();
  syshlp::validate_args(argc, argv);
  //
  syshlp::con_setup(syshlp::opt_enable_colors);
  if(syshlp::opt_stdlog_file!=NULL)
    RConsole_std::s_console.reflect(syshlp::opt_stdlog_file);
  //
  RSystemKernel& S = RSystemKernel::s_sys;
  //
  S.set_console(&RConsole_std::s_console);
  M.Link(&S, S.get_sharemem());
  //
  S.init();
  //
  S.exec_args(argc, argv);
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

static void refer(void)
{
}

