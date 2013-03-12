#ifndef __UMODSYS_CORE_COMMONEXCEPTIONS_H
#define __UMODSYS_CORE_COMMONEXCEPTIONS_H 1
/*************************************************************/
// file: umodsys/core/common_exceptions.h
// info: common exceptions
/*************************************************************/

#include <umodsys/core/source.h>

namespace UModSys {
namespace core {

//***************************************
// Exception base
//***************************************

struct EExceptionBase {
  SSourceContext context;
  int ucode;
  const char* sinfo;
  //
  EExceptionBase(const SSourceContext* ctx, int uc, const char* si=NULL)
  : ucode(uc), sinfo(si) { if(ctx) context=*ctx; }
  virtual ~EExceptionBase(void) {}
  //
  virtual int decode_error(char *buf, size_t bufmax) const
    { return snprintf(buf, bufmax, "#%08x \"%s\"", ucode, sinfo?sinfo:"NULL"); }
  //
  template<size_t n>
  void dump_stdout(void) const {
    char buf[n+1];
    decode_error(buf, n);
    printf(
      "\nException: %s\nat %s(%d) func %s\n", 
      buf,
      context.file?context.file:"none", context.line,
      context.function?context.function:"-"
    );
  }
};

//***************************************
// Exception fatal
//***************************************

struct EFatal : public EExceptionBase {
  EFatal(const SSourceContext* ctx, int uc, const char* si=NULL)
  : EExceptionBase(ctx, uc, si) {}
  //
  virtual int decode_error(char *buf, size_t bufmax) const
    { return snprintf(buf, bufmax, "EFatal #%08x \"%s\"", ucode, sinfo?sinfo:"NULL"); }
};

//***************************************
// Exception error
//***************************************

struct EError : public EExceptionBase {
  EError(const SSourceContext* ctx, int uc, const char* si=NULL)
  : EExceptionBase(ctx, uc, si) {}
  //
  virtual int decode_error(char *buf, size_t bufmax) const 
    { return snprintf(buf, bufmax, "EError #%08x \"%s\"", ucode, sinfo?sinfo:"NULL"); }
};

//***************************************
// Exception warning
//***************************************

struct EWarning : public EExceptionBase {
  EWarning(const SSourceContext* ctx, int uc, const char* si=NULL)
  : EExceptionBase(ctx, uc, si) {}
  //
  virtual int decode_error(char *buf, size_t bufmax) const
    { return snprintf(buf, bufmax, "EWarning #%08x \"%s\"", ucode, sinfo?sinfo:"NULL"); }
};

//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_COMMONEXCEPTIONS_H
