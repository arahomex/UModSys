#ifndef __UMODSYS_BASE_RCONSOLE_STD_H
#define __UMODSYS_BASE_RCONSOLE_STD_H 1
/*************************************************************/
// file: src/base/console/umodsys.base.rconsole_std.h
// info: implementation of stdin/stdout console
/*************************************************************/

#include "umodsys.base.rcommon.h"

namespace UModSys {
namespace base {
namespace rsystem {

using namespace core;

//***************************************
// RConsole_std
//***************************************

struct RConsole_std : public IConsole
{
public:
  RConsole_std(void);
  ~RConsole_std(void);
public:
  void ref_add(void) const;
  void ref_remove(void) const;
  int  ref_links(void) const;
public:
public:
  bool vput(int level, const char* fmt, va_list va);
  bool put(int level, const char* fmt, ...);
  bool vget_c(int* ch, const char *fmt, va_list va);
  bool get_c(int* ch, const char *fmt, ...);
  bool vget_s(char* buf, size_t len, const char *fmt, va_list va);
  bool get_s(char* buf, size_t len, const char *fmt, ...);
public:
  bool reflect(const char *name);
public:
  UMODSYS_REFOBJECT_IMPLEMENT1(base::rsystem::RConsole_std, 2, IConsole);
public:
  FILE* dump_file;
public:
  static RConsole_std s_console;
};


//***************************************
// END
//***************************************

} // namespace rsystem
} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_RCONSOLE_STD_H
