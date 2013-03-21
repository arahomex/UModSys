#include "umodsys.base.rconsole_std.h"

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
  vfprintf(stdout, fmt, va);
  return true;
}

bool RConsole_std::put(int level, const char* fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  vfprintf(stdout, fmt, va);
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

RConsole_std::RConsole_std(void)
{
}

RConsole_std::~RConsole_std(void)
{
}

//***************************************

RConsole_std RConsole_std::s_console;

//***************************************
// ::
//***************************************
