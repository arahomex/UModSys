//***************************************
// RSystemKernel::
//***************************************

bool RSystemKernel::exec_test_tcl(void)
{
  dbg_put(rsdl_SysTests, "TCL begin:\n");
  //
  DCString spp(
    "set hello \"'Hello World!\\n'\"\n"
    "set {hello world} \"'Hello World!\\n'\"\n"
    "argdump \"\" \"'Hello World!\\n'\" \"\"\n"
    "vardump\n"
    "puts 1: \"'Hello World!\\n'\"\n"
    "puts 2: {'Hello World!\\n'} ; puts 3: [? {'Hello World!\\n'}]\n"
    "puts 4: $hello ; puts 5: [? ${hello world}]\n"
    "#if {< 0 1} {puts 0<1} ; if {< 1 0} {puts 1<0}\n"
    "puts Scan for signs\n"
    "for {= i 0} {< $i 3} {++ i} {\n"
    "  puts, $i:\n"
    "  if {< $i 1} {puts, $i<1 { }}\n"
    "  if {> $i 1} {puts, $i>1 { }}\n"
    "  if {<= $i 1} {puts, $i<=1 { }}\n"
    "  if {>= $i 1} {puts, $i>=1 { }}\n"
    "  if {== $i 1} {puts, $i==1 { }}\n"
    "  if {!= $i 1} {puts, $i!=1 { }}\n"
    "  puts\n"
    "}\n"
    "puts /Scan for signs\n"
    "#unknown_func\n"
    "#= i 0; puts ${i}; ++ i\n"
    "= i 0; while {< $i 5} {puts $i; ++ i}\n"
    "for {= i 0} {< $i 5} {++ i} {puts $i;}\n"
    "foreach range 5 {puts R$value ;}\n"
    "foreach range 1 6 {puts r$value ;}\n"
    "foreach range 5 5 26 {puts R$value ;}\n"
    "vardump;\n"
  );
  SExecTCL::Parser pp(spp.begin(), spp.end());
  SExecTCL::Thread thread;
  SExecTCL::IExecutor* exs[4]={ this, &tcl_ctx, &tcl_control, &tcl_ctxfail };
  SExecTCL col(thread, exs, 4);
  //
  dbg_put(rsdl_SysTests, "TCL data size: %d %d\n", int(sizeof(pp)+sizeof(col)), int(sizeof(thread)));
  //
  int rv = pp.Parse(col);
  if(rv!=SExecTCL::Parser::tEnd) {
    dbg_put(rsdl_SysTests, "TCL end with error %d\n", rv);
  } else {
    dbg_put(rsdl_SysTests, "TCL end.\n");
  }
  //
  return true;
}

//***************************************
// ::
//***************************************
