#ifndef __UMODSYS_BASE_CONSOLE_H
#define __UMODSYS_BASE_CONSOLE_H 1
/*************************************************************/
// file: umodsys/base/console.h
// info: base console object
/*************************************************************/

#include <umodsys/core/stdcore.h>

namespace UModSys {
namespace base {

//***************************************
// IConsole
//***************************************

struct IConsole
: public core::IRefObject
{
public:
  ~IConsole(void);
public:
  virtual bool vput(int level, const char* fmt, va_list va) =0;
  virtual bool put(int level, const char* fmt, ...) =0;
  virtual bool vget_c(int* ch, const char *fmt, va_list va) =0;
  virtual bool get_c(int* ch, const char *fmt, ...) =0;
  virtual bool vget_s(char* buf, size_t len, const char *fmt, va_list va) =0;
  virtual bool get_s(char* buf, size_t len, const char *fmt, ...) =0;
public:
  UMODSYS_REFOBJECT_INTIMPLEMENT(base::IConsole, 2, IRefObject);
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_CONSOLE_H
