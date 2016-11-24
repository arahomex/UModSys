#include "umodsys.basesys.rsystem.h"
#include "umodsys.basesys.rmodule.h"

#if defined(_DEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RSystemKernel::
//***************************************

#include "umodsys.basesys.rsystem_tests_shell.h"
#include "umodsys.basesys.rsystem_tests_tcl.h"
#include "umodsys.basesys.rsystem_tests_json.h"
//#include "umodsys.basesys.rsystem_tests_.h"

//***************************************
//***************************************

bool RSystemKernel::exec_tests(BStr testid)
{
//  dump_upis();
  dbg_put(rsdl_SysTests, "******************************* TEST %s {\n", testid);
  //
  if(tl::su::seq(testid, "list")) {
    dbg_put(rsdl_SysTests, "list size shells tcl\n");
  } else if(tl::su::seq(testid, "size")) {
    dbg_put(rsdl_SysTests, "sizeof(DRMemAlloc)=%u sizeof(DRMemAlloc::DHeader)=%u\nsizeof(TMemAllocHeader<Void>)=%u sizeof(mem_headers::TMain<TMemAllocHeader<Void>>)=%u\n",
      (unsigned)sizeof(DRMemAlloc), (unsigned)sizeof(DRMemAlloc::DHeader), 
      (unsigned)sizeof(TMemAllocHeader<Void>), (unsigned)sizeof(mem_headers::TMain< TMemAllocHeader<Void> >)
    );
  } else if(tl::su::seq(testid, "shells")) {
    exec_test_shells();
  } else if(tl::su::seq(testid, "tcl")) {
    exec_test_tcl();
  } else if(tl::su::seq(testid, "json")) {
    exec_test_json();
  } else {
    dbg_put(rsdl_SysTests, "list size shells tcl\n");
  }
  //
  dbg_put(rsdl_SysTests, "******************************* } TEST %s\n", testid);
  return true;
}


//***************************************
// ::
//***************************************
