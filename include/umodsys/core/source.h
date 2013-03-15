#ifndef __UMODSYS_CORE_SOURCE_H
#define __UMODSYS_CORE_SOURCE_H 1
/*************************************************************/
// file: umodsys/core/source.h
// info: source information structures
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>

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
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_SOURCE_H