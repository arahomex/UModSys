#ifndef __UMODSYS_CORE_SYSHLP_H
#define __UMODSYS_CORE_SYSHLP_H 1
/*************************************************************/
// file: umodsys/core/syshlp.h
// info: sys helper for all platform
/*************************************************************/

#include <umodsys/core/stddefs.h>

namespace UModSys {
namespace core {
namespace syshlp {

/////////////////////////////////////////////////////////////////////////////
// DEFS

const int MAX_FILENAME_LEN = 1024;

template<size_t Length=4096> struct U8String;
template<size_t Length=1024> struct U16String;

extern bool gracial_convert(wchar_t *dest, int destsize, const char* src, int srcsize); // add NULL at end
extern bool gracial_convert(char *dest, int destsize, const wchar_t* src, int srcsize); // add NULL at end

extern bool gracial_cpy(char *dest, int destsize, const char* src[], int srcnum); // add NULL at end
extern bool gracial_cpy(wchar_t *dest, int destsize, const wchar_t* src[], int srcnum); // add NULL at end

typedef struct sys_list_context_s sys_list_context_t;

enum {
  slt_Unknown   = 0x01,
  slt_File      = 0x01,
  slt_Directory = 0x02,
  slt_HardLink  = 0x03,
  slt_SymLink   = 0x04,
  slt_SerialDev = 0x05,
  slt_BlockDev  = 0x06,
  slt_Pipe      = 0x07,
  slt_Socket    = 0x08,
  slt_Mount     = 0x09,
  //
  slp_Read      = 0x01,
  slp_Write     = 0x02,
  slp_ListExec  = 0x04,
  slp_Options   = 0x08,
  //
  slo_Recursive  = 0x001,
  slo_ShowLink   = 0x002,
  slo_Normal     = 0x000
};

struct sys_list_context_s {
  char name[MAX_FILENAME_LEN];
  long long file_size;
  unsigned char type, perm, userattr;
  time_t time_modified, time_created, time_accessed;
  //
  const char *path;
  const char *mask;
  int options;
  //
  void *puser;
  int *iuser;
};

/////////////////////////////////////////////////////////////////////////////
// STRING TYPES

template<size_t Length> 
struct U16String {
  enum { Len = Length };
  bool correct;
  wchar_t buf[Length+1];

  U16String(const char *text) 
    { if(text==NULL) text=""; correct = gracial_convert(buf, Length+1, text, strlen(text)); }
  U16String(const char *text, int tlen) 
    { correct = gracial_convert(buf, Length+1, text, tlen); }
  U16String(const wchar_t *t1) 
    { correct = gracial_cpy(buf, Length+1, &t1, 1); }
  U16String(const wchar_t *t1, const wchar_t *t2) 
    { const wchar_t *a[]={t1, t2}; correct = gracial_cpy(buf, Length+1, a, 2); }
  U16String(const wchar_t *t1, const wchar_t *t2, const wchar_t *t3) 
    { const wchar_t *a[]={t1, t2, t3}; correct = gracial_cpy(buf, Length+1, a, 3); }
  U16String(const wchar_t *a[], size_t n) 
    { correct = gracial_cpy(buf, Length+1, a, (int)n); }
  U16String(void) : correct(false) {}
  //
  operator const wchar_t*(void) const { return buf; }
  operator wchar_t*(void) { return buf; }
  const wchar_t* operator*(void) const { return buf; }
  wchar_t* operator*(void) { return buf; }
  const wchar_t* operator()(void) const { return buf; }
  wchar_t* operator()(void) { return buf; }
  bool operator!(void) const { return !correct; }
  void ok(bool f=true) { correct=f; }
};

template<size_t Length> 
struct U8String {
  enum { Len = Length };
  bool correct;
  char buf[Length+1];

  U8String(const wchar_t *text) 
    { if(text==NULL) text=L""; correct = gracial_convert(buf, Length+1, text, wcslen(text)); }
  U8String(const wchar_t *text, int tlen) 
    { correct = gracial_convert(buf, Length+1, text, tlen); }
  U8String(const char *t1) 
    { correct = gracial_cpy(buf, Length+1, &t1, 1); }
  U8String(const char *t1, const char *t2) 
    { const char *a[]={t1, t2}; correct = gracial_cpy(buf, Length+1, a, 2); }
  U8String(const char *t1, const char *t2, const char *t3) 
    { const char *a[]={t1, t2, t3}; correct = gracial_cpy(buf, Length+1, a, 3); }
  U8String(const char *a[], size_t n) 
    { correct = gracial_cpy(buf, Length+1, a, (int)n); }
  U8String(void) : correct(false) {}
  //
  operator const char*(void) const { return buf; }
  operator char*(void) { return buf; }
  const char* operator*(void) const { return buf; }
  char* operator*(void) { return buf; }
  const char* operator()(void) const { return buf; }
  char* operator()(void) { return buf; }
  bool operator!(void) const { return !correct; }
  void ok(bool f=true) { correct=f; }
};


/////////////////////////////////////////////////////////////////////////////
// OS PATH

void path_os_uni(wchar_t* path);
void path_os_uni(char* path);
void path_uni_os(wchar_t* path);
void path_uni_os(char* path);

int eols_normalize(char *out, int outsize, const char* in, int insize); 
int eols_prepare(char *out, int outsize, const char* in, int insize); 

/////////////////////////////////////////////////////////////////////////////
// OS STRING

inline bool STREQ(const char *a, const char *b) { return strcmp(a,b)==0; }
inline bool STREQ(const wchar_t *a, const wchar_t *b) { return wcscmp(a,b)==0; }

int codepage_convert_cpi(char* out, int outsize, const char *in, int insize, int codepage1, int codepage2);
int codepage_convert_cps(char* out, int outsize, const char *in, int insize, const char *codepage1, const char *codepage2);

/////////////////////////////////////////////////////////////////////////////
// OS FILES

FILE* c_fopen(const char *cfilename, const char *cmode);

int get_file(const char *name, void* buffer, int bufsize, int binmode);
int put_file(const char *name, const void* buffer, int bufsize, int binmode);
int get_file_list(const char *pathmask, int (*use_fn)(void *ctx, const char *fn, const char *attr, int filesize), void *ctx);

int change_dir(const char *dir);
int mkdir_recursive(const char *dira);
int erase(const char *filedir);
int curdir(char *dira, size_t maxlen);
int chdir(const char *dira);

int list_begin(sys_list_context_t* ctx, const char *path, const char *mask, int options);
int list_next(sys_list_context_t* ctx);
int list_end(sys_list_context_t* ctx);

/////////////////////////////////////////////////////////////////////////////
// OS thread

void delay(int msec);

/////////////////////////////////////////////////////////////////////////////
// OS process

int execute(const char *options, const char* cmd, const char *hstdin, char *hstdout, size_t nstdout, char *hstderr, size_t nstderr);

/////////////////////////////////////////////////////////////////////////////
// OS VARS

int getvar(const char *name, char* value, int valuelen);

/////////////////////////////////////////////////////////////////////////////
// END

} // namespace syshlp
} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_SYSHLP_H

