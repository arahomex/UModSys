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
// Exception handler
//***************************************

struct IExceptionHandler {
  virtual bool consume(const EExceptionBase& e) =0;
  //
  template<typename Err>
  inline void process(const Err& e) {
    if(this!=NULL && consume(e))
      return;
    throw e;
  }
};

//***************************************
// Common exception codes
//***************************************

enum eCommonExceptionCodes {
  cec_Null               = 0x0001,
  cec_NotNull            = 0x0002,
  cec_NotInit            = 0x0010,
  cec_Unsupported        = 0x00ff,
  cec_Memory             = 0x0100,
  cec_MemoryDamaged      = 0x0101,
  cec_MemoryLow          = 0x0102,
  cec_FileOpenError      = 0x0200,
  cec_FileCreateError    = 0x0201,
};

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
