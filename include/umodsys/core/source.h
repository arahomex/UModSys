#ifndef __UMODSYS_CORE_SOURCE_H
#define __UMODSYS_CORE_SOURCE_H 1
/*************************************************************/
// file: umodsys/core/source.h
// info: source information structures
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>
#include <umodsys/core/stdinlines.h>

namespace UModSys {
namespace core {

//***************************************
// Source context
//***************************************

struct SSourceContext {
  const char *file;
  const char *function;
  int line;
  //
  inline SSourceContext(const char *f, int l, const char *fu) : file(f), function(fu), line(l) {}
  inline explicit SSourceContext(void) : file(NULL), function(NULL), line(0) {}
};

//***************************************
// Source context adapter
//***************************************

struct ISourceContextAdapter {
  virtual const core::SSourceContext* persist_ctx(const core::SSourceContext* sc) =0;
};

//***************************************
// File Info Hash
//***************************************

struct SFileInfoHash {
  int date, time;
  int size;
  int crc;

  inline SFileInfoHash(void) : date(0), time(0), size(0), crc(0) {}
  inline void clear(void) { date=time=0; size=0; crc=0; }
};

//***************************************
// INLINES
//***************************************

inline int scalar_compare(const SSourceContext &p1, const SSourceContext &p2)
{
  int rv = scalar_compare(p1.file, p2.file);
  if(rv!=0)
    return rv;
  rv = scalar_compare(p1.function, p2.function);
  if(rv!=0)
    return rv;
  return scalar_compare(p1.line, p2.line);
}

inline int scalar_compare(const SSourceContext *p1, const SSourceContext *p2)
{
  if(p1==NULL)
    return p2!=NULL ? -1 : 0;
  if(p2==NULL)
    return 1;
  return scalar_compare(*p1, *p2);
}

//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_SOURCE_H
