#include <umodsys/core/syshlp.h>

#include <umodsys/core/platform/linux/syshlp_linux.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::core::syshlp;

//***************************************
// syshlp::
//***************************************

#if 0

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

#endif

//***************************************
// ::
//***************************************

