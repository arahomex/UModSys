//***************************************
// RSystemKernel::
//***************************************

bool RSystemKernel::exec_test_shells(void)
{
  IRefObject::TypeId tids[0x100];
  size_t ns = find_shells(tids, 0x100, NULL);
  dbg_put(rsdl_SysTests, "shells found: %d\n", ns);
  for(size_t i=0; i<ns; i++) {
    dbg_put(rsdl_SysTests, "  shell: %p %s\n", tids[i], tids[i]->name);
    IShell::P sh;
    TParametersA<1024> pars;
    if(!moddb.t_moduledb_generate(sh, tids[i], pars)) {
      dbg_put(rsdl_SysTests, "    shell gen error\n");
      continue;
    } else {
      dbg_put(rsdl_SysTests, "    shell generated as %p\n", sh());
    }
    set_shell(tids[i]->name, sh);
  }
  dbg_put(rsdl_SysTests, "/shells found: %d\n", ns);
  return true;
}

//***************************************
// ::
//***************************************
