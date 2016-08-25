#ifndef __UMODSYS_CORE_SYSHLP_H
#define __UMODSYS_CORE_SYSHLP_H 1
/*************************************************************/
// file: umodsys/common/syshlp.h
// info: sys helper for all platform
/*************************************************************/

#include <umodsys/common/stddefs.h>
#include <umodsys/common/mem/mem_alloc.h>

namespace UModSys {
namespace core {
namespace syshlp {

/////////////////////////////////////////////////////////////////////////////
// DEFS / FORWARDS

const int MAX_FILENAME_LEN = 1024;

template<size_t Length=4096> struct U8String;
template<size_t Length=1024> struct U16String;

extern bool gracial_convert(wchar_t *dest, int destsize, const char* src, int srcsize); // add NULL at end
extern bool gracial_convert(char *dest, int destsize, const wchar_t* src, int srcsize); // add NULL at end

extern bool gracial_cpy(char *dest, int destsize, const char* src[], int srcnum); // add NULL at end
extern bool gracial_cpy(wchar_t *dest, int destsize, const wchar_t* src[], int srcnum); // add NULL at end

extern size_t safe_vsnprintf(char* buf, size_t nbuf, const char* fmt, va_list va);
extern size_t safe_vsnprintf(wchar_t* buf, size_t nbuf, const wchar_t* fmt, va_list va);

/////////////////////////////////////////////////////////////////////////////
// arguments/options

extern bool opt_enable_colors;
extern const char* opt_stdlog_file;

bool validate_env(void);
bool validate_args(int &argc, char **&argv);

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
  void set(const char *text) { if(text==NULL) text=""; correct = gracial_convert(buf, Length+1, text, strlen(text)); }
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
  void set(const wchar_t *text) { if(text==NULL) text=L""; correct = gracial_convert(buf, Length+1, text, wcslen(text)); }
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

bool gracial_convert(wchar_t *dest, int destsize, const char* src, int srcsize); // add NULL at end
bool gracial_convert(char *dest, int destsize, const wchar_t* src, int srcsize); // add NULL at end

bool gracial_cpy(char *dest, int destsize, const char* src[], int srcnum); // add NULL at end
bool gracial_cpy(wchar_t *dest, int destsize, const wchar_t* src[], int srcnum); // add NULL at end

size_t safe_vsnprintf(char* buf, size_t nbuf, const char* fmt, va_list va);
size_t safe_vsnprintf(wchar_t* buf, size_t nbuf, const wchar_t* fmt, va_list va);

inline bool STREQ(const char *a, const char *b) { return strcmp(a,b)==0; }
inline bool STREQ(const wchar_t *a, const wchar_t *b) { return wcscmp(a,b)==0; }

int codepage_convert_cpi(char* out, int outsize, const char *in, int insize, int codepage1, int codepage2);
int codepage_convert_cps(char* out, int outsize, const char *in, int insize, const char *codepage1, const char *codepage2);

template<typename CharT>
size_t safe_snprintf(CharT* buf, size_t nbuf, const CharT* fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  size_t rv = safe_vsnprintf(buf, nbuf, fmt, va);
  va_end(va);
  return rv;
}


/////////////////////////////////////////////////////////////////////////////
// OS FILES

struct IListAccepter {
  enum {
    // type
    t_Unknown   = 0x01,
    t_File      = 0x01,
    t_Directory = 0x02,
    t_HardLink  = 0x03,
    t_SymLink   = 0x04,
    t_SerialDev = 0x05,
    t_BlockDev  = 0x06,
    t_Pipe      = 0x07,
    t_Socket    = 0x08,
    t_Mount     = 0x09,
    // perm
    p_Read      = 0x01,
    p_Write     = 0x02,
    p_ListExec  = 0x04,
    p_Options   = 0x08,
    // options
    o_Recursive  = 0x0001,
    o_ShowLink   = 0x0002,
    o_ShowParent = 0x0004,
    o_Normal     = 0x0000
  };
  //
  struct Filename {
    const char *filename;
    fpos_t file_size;
    unsigned char type, perm;
    unsigned userattr;
    time_t time_modified, time_created, time_accessed;
  };
  //
  virtual bool process_file(const char* path, const char* mask, const Filename& fn, int options, bool isParent) =0;
};

void con_setup(bool enable_colors=false);
void con_restore(void);
void con_setcolor(FILE* stream, const unsigned char *rgb); // f[r,g,b], b[r,g,b]

FILE* u_fopen(const char *cfilename, const char *cmode);
FILE* u_fopentemp(char* &handle, const char *msk);
bool u_fendtemp(FILE* &f, char* &handle, bool gracial);
bool u_fchsize(FILE *f, long pos);

bool u_mkdir_recursive(const char *dira);
bool u_erase(const char *filedir);
bool u_chdir(const char *dira);
bool u_curdir(char *dira, size_t maxlen);

bool u_fullpath(const char* relname, char *dira, size_t maxlen);

int u_list(const char *path, const char *mask, IListAccepter &fna, int options);

bool get_file_size(const char *name, size_t& size, bool ftext=false);
bool get_file(const char *name, size_t& size, void*& buffer, IMemAlloc* imem, bool ftext=false);
bool get_file(const char *name, size_t& size, void* buffer, size_t bufsize, bool ftext=false);
bool put_file(const char *name, const void* buffer, size_t bufsize, size_t& size, bool ftext=false, bool append=false);
bool put_file(const char *name, const void* buffer, size_t bufsize, bool ftext=false, bool append=false);


/////////////////////////////////////////////////////////////////////////////
// OS thread

void t_delay(int msec);
unsigned long t_msec(void); // overflowed msec timer

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

