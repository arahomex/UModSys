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
bool RConsole_std::ref_weak(WeakPointer& wp) const { return false; }

//***************************************
//***************************************

bool RConsole_std::vput(eConsoleLevels level, const char* fmt, va_list va)
{
  bool rv = print_color(fmt, sva_list(va).va, level);
  if(dump_file!=NULL) {
    vfprintf(dump_file, fmt, sva_list(va).va);
  }
  return rv;
}

bool RConsole_std::put(eConsoleLevels level, const char* fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  bool rv = vput(level, fmt, va);
  va_end(va);
  return rv;
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

unsigned char RConsole_std::s_colors[RConsole_std::NumColors][8]={
  {0x11, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00 }, // cl_Fatal
  {0x11, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00 }, // cl_Error
  {0x11, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 }, // cl_Warning
  {0x11, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00 }, // cl_UI
  {0x10, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00 }, // cl_Info
  {0x10, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00 }, // cl_Aux
  {0x10, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00 }, // cl_Debug
  {0x10, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00 }, // cl_Debug1
  {0x10, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00 }, // cl_Debug2
};

bool RConsole_std::print_color(const char *fmt, va_list va, int lev)
{
  if(lev<0 || lev>=NumColors || (s_colors[lev][0]&0x10)==0) {
    lev = cl_Info;
  }
  const unsigned char* clr = s_colors[lev] + 2;
  FILE *f = (s_colors[lev][0] & 0x01) ? stderr : stdout;
  syshlp::con_setcolor(f, clr);
  vfprintf(f, fmt, sva_list(va).va);
  syshlp::con_setcolor(f, NULL);
  return true;
}

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
  dump_file = core::syshlp::u_fopen(name, mode);
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
