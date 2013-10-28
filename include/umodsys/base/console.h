#ifndef __UMODSYS_BASE_CONSOLE_H
#define __UMODSYS_BASE_CONSOLE_H 1
/*************************************************************/
// file: umodsys/base/console.h
// info: base console object
/*************************************************************/

#include <umodsys/stdcore.h>

namespace UModSys {
namespace base {

//***************************************
// eConsoleLevels
//***************************************

enum eConsoleLevels {
  cl_Fatal     = 0,
  cl_Error     = 1,
  cl_Warning   = 2,
  cl_Info      = 3,
  cl_Aux       = 4,
  cl_Debug     = 5,
  cl_Debug1    = 6,
  cl_Debug2    = 7,
};

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
  //
  virtual bool vget_c(int* ch, const char *fmt, va_list va) =0;
  virtual bool get_c(int* ch, const char *fmt, ...) =0;
  virtual bool vget_s(char* buf, size_t len, const char *fmt, va_list va) =0;
  virtual bool get_s(char* buf, size_t len, const char *fmt, ...) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IConsole, 2, IRefObject);
};

//***************************************
// SDebug
//***************************************

struct SDebug {
public:
  IConsole::P console;
  core::uint32 mask;
public:
  SDebug(IConsole* ic=NULL, core::uint32 m=~core::uint32(0));
  ~SDebug(void);
  //
  inline void enable(core::byte src) { mask |= core::uint32(1)<<src; }
  inline void disable(core::byte src) { mask &= ~(core::uint32(1)<<src); }
  //
  inline bool vput(core::byte src, int lev, const char* fmt, va_list va) const {
    if((mask & (core::uint32(1)<<src))==0 || !console.valid())
      return false;
    return console->vput(lev, fmt, va);
  }
  inline bool dvput(core::byte src, const char* fmt, va_list va) const { 
    return vput(src, cl_Debug, fmt, va); 
  }
  //
  bool put(core::byte src, int lev, const char* fmt, ...) const;
  bool dput(core::byte src, const char* fmt, ...) const;
};

//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_CONSOLE_H
