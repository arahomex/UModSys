#include <umodsys/common/stdsyshlp.h>

#include <umodsys/common/platform/win32/syshlp_win32.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::core::syshlp;

//***************************************
// syshlp::
//***************************************

#if 0
static int find_cp_no(const char* codepage)
{
  if(codepage==NULL || STREQ(codepage, "UTF8"))
    return CP_UTF8;
  if(STREQ(codepage, "ANSI") || STREQ(codepage, "WIN"))
    return CP_ACP;
  if(STREQ(codepage, "OEM") || STREQ(codepage, "DOS"))
    return CP_OEMCP;
  if(STREQ(codepage, "MAC"))
    return CP_MACCP;
  return -1; // unknown
}

static int find_cp_no(int codepage)
{
  if(codepage==0)
    codepage = CP_UTF8;
  return codepage;
}


static int codepage_convert(char* out, int outsize, const char *in, int insize, int codepage1, int codepage2)
{
  using namespace data_res;
  static SMemoryCacheBuffer u_buffer(def_memory_cache);

  int len1 = MultiByteToWideChar(codepage1, 0, in, insize, NULL, 0);
  if(len1<=0 || !u_buffer.valid() || !u_buffer->resize(len1*sizeof(wchar_t)))
    return -10;
  int len2 = MultiByteToWideChar(codepage1, 0, in, insize, (wchar_t*)u_buffer->get(), len1);
  if(len2<=0)
    return -11;
  int len3 = WideCharToMultiByte(codepage2, 0, (wchar_t*)u_buffer->get(), len2, NULL, 0, NULL, NULL);
  if(len3<=0)
    return -12;
  if(out==NULL) {
    return len3;
  }
  int len4 = WideCharToMultiByte(codepage2, 0, (wchar_t*)u_buffer->get(), len2, out, outsize, NULL, NULL);
  return len4<=0 ? -13 : len4;
}


int syshlp::codepage_convert_cpi(char* out, int outsize, const char *in, int insize, int codepage1, int codepage2)
{
  int cp1 = find_cp_no(codepage1); if(cp1<0) return -1;
  int cp2 = find_cp_no(codepage2); if(cp2<0) return -2;
  return codepage_convert(out, outsize, in, insize, cp1, cp2);
}

int syshlp::codepage_convert_cps(char* out, int outsize, const char *in, int insize, const char *codepage1, const char *codepage2)
{
  int cp1 = find_cp_no(codepage1); if(cp1<0) return -1;
  int cp2 = find_cp_no(codepage2); if(cp2<0) return -2;
  return codepage_convert(out, outsize, in, insize, cp1, cp2);
}

int syshlp::codepage_convert_cpi(char* out, int outsize, const char *in, int insize, int codepage1, int codepage2)
{
  int cp1 = find_cp_no(codepage1); if(cp1<0) return -1;
  int cp2 = find_cp_no(codepage2); if(cp2<0) return -2;
  return codepage_convert(out, outsize, in, insize, cp1, cp2);
}

int syshlp::codepage_convert_cps(char* out, int outsize, const char *in, int insize, const char *codepage1, const char *codepage2)
{
  int cp1 = find_cp_no(codepage1); if(cp1<0) return -1;
  int cp2 = find_cp_no(codepage2); if(cp2<0) return -2;
  return codepage_convert(out, outsize, in, insize, cp1, cp2);
}
#endif

bool syshlp::gracial_convert(wchar_t *dest, int destsize, const char* src, int srcsize)
{
  int rv = MultiByteToWideChar(CP_UTF8, 0, src, srcsize, dest, destsize-1);
  if(rv<0) {
    *dest = 0;
    return false;
  }
  dest[rv] = 0;
  return true;
}

bool syshlp::gracial_convert(char *dest, int destsize, const wchar_t* src, int srcsize)
{
  int rv = WideCharToMultiByte(CP_UTF8, 0, src, srcsize, dest, destsize-1, NULL, NULL);
  if(rv<0) {
    *dest = 0;
    return false;
  }
  dest[rv] = 0;
  return true;
}


//***************************************
// ::
//***************************************

