#include "umodsys.basesys.rconsole_std.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RConsole_std::
//***************************************

void RConsole_std::ref_add(void) const {}
void RConsole_std::ref_remove(void) const {}
int  RConsole_std::ref_links(void) const { return 0; }

//***************************************
//***************************************

bool RConsole_std::vput(int level, const char* fmt, va_list va)
{
  if(stdout!=NULL) {
    vfprintf(stdout, fmt, va);
  }
  if(dump_file!=NULL) {
    vfprintf(dump_file, fmt, va);
  }
  return true;
}

bool RConsole_std::put(int level, const char* fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  //
  if(stdout!=NULL) {
    vfprintf(stdout, fmt, va);
  }
  if(dump_file!=NULL) {
    vfprintf(dump_file, fmt, va);
  }
  //
  va_end(va);
  return true;
}

bool RConsole_std::vget_c(int* ch, const char *fmt, va_list va)
{
  return false;
}

bool RConsole_std::get_c(int* ch, const char *fmt, ...)
{
  return false;
}

bool RConsole_std::vget_s(char* buf, size_t len, const char *fmt, va_list va)
{
  return false;
}

bool RConsole_std::get_s(char* buf, size_t len, const char *fmt, ...)
{
  return false;
}

//***************************************
//***************************************

bool RConsole_std::reflect(const char *name)
{
  if(dump_file!=NULL) {
    fclose(dump_file);
    dump_file = NULL;
  }
  if(name==NULL)
    return true;
  if(name[0]=='>')
    name++;
  const char *mode = "wt";
  if(name[0]=='>') {
    name++;
    mode = "wt";
  }
  dump_file = core::syshlp::c_fopen(name, mode);
  if(dump_file==NULL)
    return false;
  return true;
}

//***************************************

RConsole_std::RConsole_std(void)
: dump_file(NULL)
{
}

RConsole_std::~RConsole_std(void)
{
  reflect(NULL);
}

//***************************************

RConsole_std RConsole_std::s_console;

//***************************************
// ::
//***************************************
