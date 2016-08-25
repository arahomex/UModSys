#ifndef __UMODSYS_CORE_PLATFORM_WIN32_SYSHLP_H
#define __UMODSYS_CORE_PLATFORM_WIN32_SYSHLP_H 1
/*************************************************************/
// file: umodsys/common/platform/win32/syshlp.h
// info: sys helper for win32 platform
/*************************************************************/

#include <umodsys/common/syshlp.h>

/*******************************************************************************/

#define UNICODE

// Change these values to use different versions
#define WINVER		0x0502
#define _WIN32_WINNT	0x0502
#define _WIN32_IE	0x0502
#define _RICHEDIT_VER	0x0200

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#pragma warning(disable : 4996) // 'sprintf' was declared deprecated
#pragma warning(disable : 4267) // 'sprintf' was declared deprecated

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include <winsock2.h>
#include <windows.h>
#include <tchar.h>
#include <shlobj.h>

#undef UNICODE

#ifdef _UNICODE
#undef _UNICODE
#endif

#ifndef IO_REPARSE_TAG_SYMLINK
#define IO_REPARSE_TAG_SYMLINK 0xA000000C
#endif

/*******************************************************************************/

namespace UModSys {
namespace core {
namespace syshlp {

const int MAX_PATH_LEN = 4096;

/////////////////////////////////////////////////////////////////////////////
// WINPATH

template<int N>
struct WinPath : public U16String<N> {
  typedef U16String<N> B;
public:
  WinPath(void) {}
  WinPath(const char *upath) : B(upath) { path_uni_os(*this); }
  WinPath(const wchar_t *wpath) : B(wpath) {}
  WinPath(const wchar_t *wpath, const wchar_t *wsuffix) : B(wpath, wsuffix) {}
  WinPath(const wchar_t *wpath, const wchar_t *wsuffix, const wchar_t *wsuffix2) : B(wpath, wsuffix) {}
};

template<int N>
struct UniPath : public U8String<N> {
  typedef U8String<N> B;
public:
  UniPath(void) {}
  UniPath(const wchar_t *wpath) : B(wpath) { path_os_uni(*this); }
  UniPath(const char *upath) : B(upath) {}
  UniPath(const char *upath, const char *usuffix) : B(upath, usuffix) {}
  UniPath(const char *upath, const char *usuffix, const char *usuffix2) : B(upath, usuffix, usuffix2) {}
};

/////////////////////////////////////////////////////////////////////////////
// OS PATH INLINES

struct SListContext {
  const char *path;
  const char *mask;
  HANDLE hScan;
  WIN32_FIND_DATAW ff;
  char found_name[MAX_PATH_LEN];
  int options;
  bool isParent;
  //
  SListContext(const char *p, const char *m, int options);
  ~SListContext(void);
  bool close(void);
  bool open(bool usemask);
  bool next(void);
  bool fill(IListAccepter::Filename& fn, bool checkmask);
  //
  template<typename Performer>
  bool use_normal(Performer& pf) {
    if(!open(true))
      return false;
    do {
      IListAccepter::Filename fn;
      if(!fill(fn, true))
        continue;
      if(pf(this, fn))
        break;
    } while(next());
    close();
    return true;
  }
  //
  template<typename Performer>
  bool use_recursive(Performer& pf) {
    if(!open(false))
      return false;
    do {
      IListAccepter::Filename fn;
      if(!fill(fn, false))
        continue;
      if(tl::su::wildcmp(mask, found_name)) {
        if(pf(this, fn))
          break;
      }
      if(!isParent && (ff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        SListContext sub(fn.filename, mask, options);
        if(!use_recursive(pf))
          break;
      }
    } while(next());
    close();
    return true;
  }
  //
  template<typename Performer>
  inline static bool use(const char *path, const char *mask, Performer& pf, int options) {
    if(options & IListAccepter::o_Recursive)
      return SListContext(path, mask, options).use_recursive(pf);
    return SListContext(path, mask, options).use_normal(pf);
  }
  //
  struct SList {
    IListAccepter& la;
    int counter;
    //
    inline SList(IListAccepter& a) : la(a), counter(0) {}
    inline bool operator()(SListContext* pctx, IListAccepter::Filename& fn) { return la.process_file(pctx->path, pctx->mask, fn, pctx->options, pctx->isParent); }
    inline int operator()(void) const { return counter; }
  };
};


/////////////////////////////////////////////////////////////////////////////
// INLINES

inline int UGetTimeFormat(LCID Locale, DWORD dwFlags, CONST SYSTEMTIME *lpTime, const char *format, char *buffer, int bufsize)
{
  U16String<> buf;
  if(GetTimeFormatW(Locale, dwFlags, lpTime, U16String<>(format), buf, buf.Len)<=0)
    return 0;
  U8String<> buf2(buf);
  int L = strlen(buf2);
  if(L>=bufsize)
    L = bufsize-1;
  strncpy(buffer, buf2, L);
  buffer[L] = 0;
  return L;
}

inline int UGetDateFormat(LCID Locale, DWORD dwFlags, CONST SYSTEMTIME *lpTime, const char *format, char *buffer, int bufsize)
{
  U16String<> buf;
  if(GetDateFormatW(Locale, dwFlags, lpTime, U16String<>(format), buf, buf.Len)<=0)
    return 0;
  U8String<> buf2(buf);
  int L = strlen(buf2);
  if(L>=bufsize)
    L = bufsize-1;
  strncpy(buffer, buf2, L);
  buffer[L] = 0;
  return L;
}

inline time_t filetime_to_timet(const FILETIME& ft) 
{
   ULARGE_INTEGER ull;
   ull.LowPart = ft.dwLowDateTime;
   ull.HighPart = ft.dwHighDateTime;
   return ull.QuadPart / 10000000ULL - 11644473600ULL;
}

inline SYSTEMTIME timet_to_systemtime(time_t t, bool local=true)
{
   FILETIME ft, ftl;
   // Note that LONGLONG is a 64-bit value
   LONGLONG ll;
   ll = Int32x32To64(t, 10000000) + 116444736000000000ULL;
   ft.dwLowDateTime = (DWORD)ll;
   ft.dwHighDateTime = ll >> 32;
   if(local)
    FileTimeToLocalFileTime(&ft, &ftl);
   else
     ftl = ft;
   SYSTEMTIME st;
   FileTimeToSystemTime(&ftl, &st);
   return st;
}

/////////////////////////////////////////////////////////////////////////////
// END

} // namespace syshlp
} // namespace core
} // namespace UModSys

#endif

