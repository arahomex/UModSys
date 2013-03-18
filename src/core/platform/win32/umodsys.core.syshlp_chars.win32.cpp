#include <umodsys/core/syshlp.h>

#include <umodsys/core/platform/win32/syshlp_win32.h>

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
  register wchar_t *p;
  for(p = path; *p; p++) {
    if(*p=='\\') *p = '/';
  }
}

void syshlp::path_os_uni(char* path)
{
  register char *p;
  for(p = path; *p; p++) {
    if(*p=='\\') *p = '/';
  }
}

void syshlp::path_uni_os(wchar_t* path)
{
  register wchar_t *p;
  for(p = path; *p; p++) {
    if(*p=='/') *p = '\\';
  }
}

void syshlp::path_uni_os(char* path)
{
  register char *p;
  for(p = path; *p; p++) {
    if(*p=='/') *p = '\\';
  }
}

//***************************************
//***************************************

int syshlp::eols_normalize(char *out, int outsize, const char* in, int insize)
{
  register const char *x = in;
  register int left = insize, rv=0;
  register char fln = 0;
  if(out==NULL) {
    // calculate only
    while(left>0) {
      register char c = *x++;
      left--;
      if(c=='\r') {
        if(fln=='n') {
          continue;
        }
        fln = 0; // replace \r to \n
      } else if(c=='\n') {
        if(fln=='r') {
          continue;
        }
        fln = 1;
      } else {
      }
      rv++;
    }
    return rv;
  }
  // fill data
  while(left>0) {
    register char c = *x++;
    left--;
    if(c=='\r') {
      if(fln=='n') {
        continue;
      }
      fln = 'r';
      c = '\n'; // replace \r to \n
      rv++;
    } else if(c=='\n') {
      if(fln=='r') {
        fln = 'n';
        continue;
      }
      fln = 'n';
      rv++;
    } else {
      rv++;
    }
    if(outsize--<=0)
      return -1;
    *out++ = c;
  }
  return rv;
}

int syshlp::eols_prepare(char *out, int outsize, const char* in, int insize)
{
  register const char *x = in;
  register int left = insize, rv=0;
  if(out==NULL) {
    // calculate only
    while(left>0) {
      left--;
      register char c = *x++;
      if(c=='\n') {
        rv+=2;
      } else {
        rv++;
      }
    }
    return rv;
  }
  // fill data
  while(left>0) {
    register char c = *x++;
    left--;
    if(c=='\n') {
      outsize -= 2;
      if(outsize<0)
        return -1;
      *out++ = '\r';
      *out++ = '\n';
      rv+=2;
    } else {
      outsize--;
      if(outsize<0)
        return -1;
      *out++ = c;
      rv++;
    }
  }
  return rv;
}


//***************************************
// ::
//***************************************

