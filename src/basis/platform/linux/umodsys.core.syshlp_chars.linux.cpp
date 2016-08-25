#include <umodsys/common/syshlp.h>

#include <umodsys/common/platform/linux/syshlp_linux.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::core::syshlp;

//***************************************
// syshlp::
//***************************************

bool syshlp::gracial_cpy(char *dest, int destsize, const char* src[], int srcnum)
{
  int bp = 0;
  for(int i=0; i<srcnum; i++) {
    int n = src[i]!=NULL && *src[i] ? (int)strlen(src[i]) : 0;
    if(n+bp>=destsize) {
      n = destsize - bp - 1;
      memcpy(dest+bp, src[i], n);
      dest[bp+n] = 0;
      return false;
    } else {
      memcpy(dest+bp, src[i], n);
      dest[bp+n] = 0;
      bp += n;
    }
  }
  return true;
}

bool syshlp::gracial_cpy(wchar_t *dest, int destsize, const wchar_t* src[], int srcnum)
{
  int bp = 0;
  for(int i=0; i<srcnum; i++) {
    int n = src[i]!=NULL && *src[i] ? (int)wcslen(src[i]) : 0;
    if(n+bp>=destsize) {
      n = destsize - bp - 1;
      memcpy(dest+bp, src[i], n*sizeof(wchar_t));
      dest[bp+n] = 0;
      return false;
    } else {
      memcpy(dest+bp, src[i], n*sizeof(wchar_t));
      dest[bp+n] = 0;
      bp += n;
    }
  }
  return true;
}

//***************************************
//***************************************

void syshlp::path_os_uni(wchar_t* path)
{
}

void syshlp::path_os_uni(char* path)
{
}

void syshlp::path_uni_os(wchar_t* path)
{
}

void syshlp::path_uni_os(char* path)
{
}

//***************************************
//***************************************

int syshlp::eols_normalize(char *out, int outsize, const char* in, int insize)
{
  if(outsize<insize)
    outsize = insize;
  memcpy(out, in, outsize);
  return outsize;
}

int syshlp::eols_prepare(char *out, int outsize, const char* in, int insize)
{
  if(outsize<insize)
    outsize = insize;
  memcpy(out, in, outsize);
  return outsize;
}

//***************************************
//***************************************

size_t syshlp::safe_vsnprintf(char* buf, size_t nbuf, const char* fmt, va_list va)
{
  size_t rv = vsnprintf(buf, nbuf, fmt, va);
  if(rv<0) { *buf = 0; rv=0; } else if(rv==nbuf) buf[nbuf-1] = 0;
  return rv;
}

size_t syshlp::safe_vsnprintf(wchar_t* buf, size_t nbuf, const wchar_t* fmt, va_list va)
{
  size_t rv = vswprintf(buf, nbuf, fmt, va);
  if(rv<0) { *buf = 0; rv=0; } else if(rv==nbuf) buf[nbuf-1] = 0;
  return rv;
}

//***************************************
// ::
//***************************************

