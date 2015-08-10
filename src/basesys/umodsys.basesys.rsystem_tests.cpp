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
// RSystem::
//***************************************

bool RSystem::exec_test_tcl(void)
{
  dbg_put(rsdl_System, "TCL begin:\n");
  //
  DCString spp(
    "set hello \"'Hello World!\\n'\"\n"
    "set {hello world} \"'Hello World!\\n'\"\n"
    "argdump \"\" \"'Hello World!\\n'\" \"\"\n"
    "vardump\n"
    "puts 1: \"'Hello World!\\n'\"\n"
    "puts 2: {'Hello World!\\n'} ; puts 3: [? {'Hello World!\\n'}]\n"
    "puts 4: $hello ; puts 5: [? ${hello world}]\n"
    "if {< 0 1} {puts 0<1} ; if {< 1 0} {puts 1<0}\n"
    "= i 0; while {< $i 10} {puts $i; ++ i}\n"
    "for {= i 0} {< $i 10} {++ i} {puts $i;}\n"
    "vardump;\n"
  );
  SExecTCL::Parser pp(spp.begin(), spp.end());
  SExecTCL col(tcl_ss, this);
  //
  pp.Parse(col);
  //
  dbg_put(rsdl_System, "TCL end.\n");
  return true;
}

//***************************************
//***************************************

bool RSystem::exec_test_shells(void)
{
  IRefObject::TypeId tids[0x100];
  size_t ns = find_shells(tids, 0x100, NULL);
  dbg_put(rsdl_System, "shells found: %d\n", ns);
  for(size_t i=0; i<ns; i++) {
    dbg_put(rsdl_System, "  shell: %p %s\n", tids[i], tids[i]->name);
    IShell::P sh;
    TParametersA<1024> pars;
    if(!moddb.t_moduledb_generate(sh, tids[i], pars)) {
      dbg_put(rsdl_System, "    shell gen error\n");
      continue;
    } else {
      dbg_put(rsdl_System, "    shell generated as %p\n", sh());
    }
    set_shell(tids[i]->name, sh);
  }
  dbg_put(rsdl_System, "/shells found: %d\n", ns);
  return true;
}

//***************************************
//***************************************

bool RSystem::exec_tests(void)
{
//  dump_upis();
  dbg_put(rsdl_SystemTests, "******************************* TESTS {\n");
  dbg_put(rsdl_SystemTests, "sizeof(DRMemAlloc)=%u sizeof(DRMemAlloc::DHeader)=%u\nsizeof(TMemAllocHeader<Void>)=%u sizeof(mem_headers::TMain<TMemAllocHeader<Void>>)=%u\n",
    (unsigned)sizeof(DRMemAlloc), (unsigned)sizeof(DRMemAlloc::DHeader), 
    (unsigned)sizeof(TMemAllocHeader<Void>), (unsigned)sizeof(mem_headers::TMain< TMemAllocHeader<Void> >)
  );
  //
//  exec_test_shells();
  exec_test_tcl();
  //
  dbg_put(rsdl_SystemTests, "******************************* } TESTS\n");
  return true;
}

//***************************************
// ::
//***************************************
