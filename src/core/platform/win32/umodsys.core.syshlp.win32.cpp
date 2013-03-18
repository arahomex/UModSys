#include "stdafx.h"

#include "udebug.h"
#include "pars2hlp.h"
#include "syshlp.h"
#include "squirrel.h"

#include "winhandle.h"
#include "ui_controls.h"
#include "syshlp_win32.h"

#ifndef IO_REPARSE_TAG_SYMLINK
#define IO_REPARSE_TAG_SYMLINK 0xA000000C
#endif

#pragma warning (disable:4996)
#pragma warning (disable:4267)

#define is_arg parse2_is_arg
#define x_atoi parse2_xatoi

extern int sys_log_put(int severity, const char *lines);
extern int sys_log_put_dialog(int severity, const char *lines);

// ******************************************************************************************
// VARS
// ******************************************************************************************

static HANDLE hMutex;
static char first_time = 1;
//static DWORD console_attribute = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
static DWORD console_attribute = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_BLUE;

static FILE *dump_file[MAX_DEBUG_FILE_COUNT];
static char dump_file_name[MAX_DEBUG_FILE_COUNT][256];
static int dump_screen = 1;
unsigned char dump_levels[256], dump_level_redirect_default=0x03;
static simpledatabase_field_t debug_f[]={
  {sdbt_StringHash, 30, "name"},
  {sdbt_String, 256, "value"},
  {sdbt_Null}
};
static simpledatabase_t *sdb_debug;
static char log_eol = 1;

// ******************************************************************************************
// 
// ******************************************************************************************

static bool get_reg_string(HKEY root, const char* keyname, const char* valuename, char* value, int value_size)
{
  syshlp::U16String<> keynamew(keyname), valuenamew(valuename);
  HKEY sub;
  if(RegOpenKeyExW(root, keynamew, 0, KEY_READ, &sub)!=ERROR_SUCCESS)
    return false;
  syshlp::U16String<4096> valuew;
  DWORD type=REG_SZ, size=sizeof(valuew.buf);
  bool rv = RegQueryValueExW(sub, valuenamew, NULL, &type, (LPBYTE)&valuew.buf, &size)==ERROR_SUCCESS;
  if(type!=REG_SZ && type!=REG_EXPAND_SZ)
    rv = false;
  RegCloseKey(sub);
  if(rv) {
    ::ui::gracial_convert(value, value_size, valuew, wcslen(valuew));
  }
  return rv;
}

static const char* local_shell_folders[][2]={
  {"path:My Documents", "Personal"},
  {"path:My Desktop", "Desktop"},
  {"path:My Favorites", "Favorites"},
  {"path:My Recent", "Recent"},
  {"path:My Start Menu", "Start Menu"},
  {"path:My SendTo", "SendTo"},
  {"path:My Startup", "Startup"},
  {"path:My Music", "My Music"},
  {"path:My Pictures", "My Pictures"},
  {"path:My Video", "My Video"},
  {"path:My Local AppData", "Local AppData"},
  {"path:My Local Settings", "Local Settings"},
  {"path:My Templates", "Templates"},
  {NULL, NULL}
};

int sys_getvar(const char *name, char* value, int valuelen)
{
  for(int i=0; local_shell_folders[i][0]!=NULL; i++) {
    if(STREQ(local_shell_folders[i][0], name)) {
      if(get_reg_string(
        HKEY_CURRENT_USER, 
        "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
        local_shell_folders[i][1], 
        value,
        valuelen
        )) {
        //
        return 1;
      }
    }
  }
  return 0;
}

inline static int find_cp_no(const char* codepage)
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

inline static int find_cp_no(int codepage)
{
  if(codepage==0)
    codepage = CP_UTF8;
  return codepage;
}

inline static int codepage_convert(char* out, int outsize, const char *in, int insize, int codepage1, int codepage2)
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

void sys_err(char *buf, int sizebuf, int errcode) {
   char *z;

   FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errcode, 0, buf, sizebuf, NULL);

   while((z=strchr(buf, '\n'))!=NULL) {
     *z = ' '; // relpace LF symbols
   }
   while((z=strchr(buf, '\r'))!=NULL) {
     *z = ' '; // relpace CR symbols
   }
}

// ******************************************************************************************
// LOG
// ******************************************************************************************

static int log_rotate_num(const char *filename, int maxnum, int digits)
{
  const char *suffix = strrchr(filename, '.');
  char filefmt[1024];
  int i;
  char file2[1024], file1[1024];

  if(digits<0) {
    i=10; digits=1;
    while(i<=maxnum) {
      i*=10; digits++;
    }
  }

  if(suffix==NULL) {
    sprintf(filefmt, "%s.%%0%du", filename, digits);
  } else {
    strncpy(filefmt, filename, (suffix-filename));
    sprintf(filefmt+(suffix-filename), ".%%0%du.%s", digits, suffix+1);
  }
  sprintf(file2, filefmt, maxnum);
  for(i=maxnum-1; i>=-1; i--) {
    if(i<0) strcpy(file1, filename); else sprintf(file1, filefmt, i);
    remove(file2);
    rename(file1, file2);
    strcpy(file2, file1);
  }
  return 0;
}

static int log_rotate_date(const char *filename, int maxdeep)
{
  return 0;
}

int log_opendump(const char *value, const char *mode, int fid, const char* rotate)
{
  time_t t;
  char timebuf[256];
  char timebuf2[256];
  charcp_t rflag;
  const char *px, *py;
  char new_value[4][0x2000], *nv;
  int nvp;

  // do variables parsing
  if(value!=NULL) {
    while(1) {
      px = strstr(value, "$(");
      if(px==NULL)
        break;
      py = strchr(px, ')');
      if(py==NULL)
        break;
      nv = value==new_value[0] ? new_value[1] : new_value[0];
      memcpy(nv, value, nvp=px-value);
      //
      memcpy(new_value[2], px+2, py-px-2);
      new_value[2][py-px-2] = 0;
      if(sys_getvar(new_value[2], new_value[3], sizeof new_value[3])) {
        path_os_uni(new_value[3]);
        strcpy(nv+nvp, new_value[3]);
        nvp += strlen(new_value[3]);
      }
      //
      strcpy(nv+nvp, py+1);
      value = nv;
    }
    px = strrchr(value, '/');
    if(px!=NULL) {
      memcpy(new_value[3], value, px-value);
      new_value[3][px-value] = 0;
      sys_mkdir_recursive(new_value[3]);
    }
  }
  //
  if(rotate!=NULL || !STREQ(value, dump_file_name[fid])) {
    if(dump_file[fid]) {
      fclose(dump_file[fid]);
      dump_file[fid] = NULL;
      strcpy(dump_file_name[fid], "");
    }
    //
    if(rotate!=NULL) {
      if(STREQ(rotate, "")) { // number append 000
        log_rotate_num(value, 999, 3);
      } else if(is_arg(rotate, "num.", &rflag) || is_arg(rotate, "!", &rflag)) {
        log_rotate_num(value, x_atoi(rflag.p), -1);
      } else if(is_arg(rotate, "date.", &rflag) || is_arg(rotate, "@", &rflag)) {
        log_rotate_date(value, x_atoi(rflag.p));
      }
    }
    //
    if(*value!=0) {
      dump_file[fid] = fopen(value, mode);
      if(dump_file[fid]==NULL)
        return -1;
      strcpy(dump_file_name[fid], value);
      time(&t);
      strftime(timebuf, sizeof timebuf, "GMT %Y-%m-%d %H:%M:%S", gmtime(&t));
      strftime(timebuf2, sizeof timebuf2, "LOCAL %Y-%m-%d %H:%M:%S WEEK %W:%A/%w MONTH %B", localtime(&t));
      fprintf(dump_file[fid], "\n\n**********************************************\n%s\n%s\n**********************************************\n", timebuf, timebuf2);
      fflush(dump_file[fid]);
    }
  } else {
    fflush(dump_file[fid]);
  }
  return 0;
}

int log_setvar(const char *varname, const char *value)
{
  char i, num[80], ler[80], tmpvalue[1024];
  const char *openmode;
  const char *logrotate;

  if(varname==NULL || *varname==0)
    return 0; // empty set
  //
#if defined(_DEBUG) && defined(_MSC_VER)
  if(STREQ(varname, ".vc.leak_check")) {
    int f = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    if(atoi(value))  f |= _CRTDBG_LEAK_CHECK_DF; else f &= ~_CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(f);
    return 0;
  }
  if(STREQ(varname, ".vc.delay_free")) {
    int f = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    if(atoi(value))  f |= _CRTDBG_DELAY_FREE_MEM_DF; else f &= ~_CRTDBG_DELAY_FREE_MEM_DF;
    _CrtSetDbgFlag(f);
    return 0;
  }
  if(STREQ(varname, ".vc.alloc_check")) {
    int f = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    //
    int n = atoi(value);
    if(n<0) n = 0;
    else if(n<16) n = _CRTDBG_CHECK_ALWAYS_DF;
    else if(n<128) n = _CRTDBG_CHECK_EVERY_16_DF;
    else if(n<1024) n = _CRTDBG_CHECK_EVERY_128_DF;
    else n = _CRTDBG_CHECK_EVERY_1024_DF;
    //
    f &= ~(_CRTDBG_CHECK_ALWAYS_DF|_CRTDBG_CHECK_EVERY_16_DF | _CRTDBG_CHECK_EVERY_128_DF | _CRTDBG_CHECK_EVERY_1024_DF);
    _CrtSetDbgFlag(f | n);
    malloc(1);
    return 0;
  }
#endif
  //
#define LV(x) \
  } else if(STREQ(varname, #x)) { dump_levels[x] = x_atoi(value); return 0; \
  //
  if(STREQ(varname, "screen")) {
    dump_screen = atoi(value);
  } else if(STREQ(varname, "console_attribute")) {
    console_attribute = x_atoi(value);
    return 0;
  } else if(STREQ(varname, "redirect.default")) {
    dump_level_redirect_default = x_atoi(value);
    return 0;
  //
  LV(DL_MAIN)
  LV(DL_MAIN_ERROR)
  LV(DL_MAIN_ARGS)
  LV(DL_MAIN_HELP)
  LV(DL_MAIN_AUX)
  LV(DL_MAIN_CODE)
  LV(DL_MAIN_CODE_D)
  LV(DL_COM_CONNECT)
  LV(DL_CC_ERROR)
  LV(DL_CC_STREAM)
  LV(DL_CC_MODEM)
  LV(DL_CC_MODEMDUMP)
  LV(DL_CC_UDPDUMP)
  LV(DL_CC_TCPDUMP)
  LV(DL_CC_COMDUMP)
  LV(DL_CC_DEBUG)
  //
  } else {
    if(varname=="file") {
      varname = "file.1";
      sprintf(tmpvalue, "*R %s", value);
      value = tmpvalue;
    } else if(varname=="fileappend") { 
      varname = "file.1";
      sprintf(tmpvalue, "*A %s", value);
      value = tmpvalue;
    }
    //
    for(i=1; i<=MAX_DEBUG_FILE_COUNT; i++) {
      sprintf(num, "file.%d", i);
      if(STREQ(varname, num)) {
        openmode = "wt";
        logrotate = NULL;
        while(value!=NULL && value[0]=='*') {
          charcp_t v = {value+1}, v2;
          int L = strlen(value)-1;
          parse2_next_chars_buf(num, sizeof num, &v, &L, ' ', 0);
          if(STREQ(num, "rotate") || STREQ(num, "rot")) {
            logrotate = "";
          } else if(is_arg(num, "rotate:", &v2) || is_arg(num, "rot:", &v2)) {
            strcpy(ler, v2.p);
            logrotate = ler;
          } else if(STREQ(num, "append") || STREQ(num, "A")) {
            openmode = "at";
          } else if(STREQ(num, "replace") || STREQ(num, "R")) {
            openmode = "wt";
          }
          value = v.p;
        }
        return log_opendump(value, openmode, i-1, logrotate);
      }
    }
    // unknown varname
  }
#undef LV
  return 0;
}

static DWORD stime0;

int log_init(const char *filename, int append)
{
  int len, r;
  char buffer[4096];
  

  if(first_time) {
    MUTEX_INIT(hMutex);
    if(MUTEX_OPEN(hMutex))
      return -1;
    sdb_debug = SDB_create(debug_f, -1, 300);
    first_time = 0;
  }

  if(append==0) {
    log_deinit(0);
  }
  if(filename==NULL)
    return 0;
  len = sys_get_file(filename, buffer, sizeof buffer, 0);
  if(len<=0)
    return -1;
  SDB_load_ini(sdb_debug, "name", "value", buffer, len);
  //
  for(r=0; r<300; r++) {
    log_setvar(SDB_get_string(sdb_debug, r, 0, NULL), SDB_get_string(sdb_debug, r, 1, ""));
  }
  stime0 = timeGetTime();

  return 0;
}

void log_deinit(int final)
{
  int i;

  if(final) {
    SDB_destroy(sdb_debug);
    sdb_debug = NULL;
  } else {
    SDB_clean(sdb_debug);
  }
  for(i=0; i<MAX_DEBUG_FILE_COUNT; i++) {
    if(dump_file[i]) {
      fclose(dump_file[i]);
      dump_file[i] = NULL;
    }
  }
  if(final) {
    MUTEX_CLOSE(hMutex);
  }
}

int log_put(int severity, const char *text, ...)
{
  va_list va;
  int rv;

  va_start(va, text);
  rv = log_vput(severity, text, va);
  va_end(va);
  return rv;
}

int log_vput(int severity, const char *text, va_list va)
{
  int n, fid=1, ki;
  char buf[0x20000], *p, *e, *tt;

  if(severity>0) {
    if(dump_levels[severity & 0xff]==0)
      return 0; // skipped
    fid = severity>>8;
    if(fid==0) {
      fid = dump_levels[severity & 0xff];
      if(fid==0) {
        fid = dump_level_redirect_default;
        if(fid==0)
          fid = 3; // always use 0&1 channels if no channel
      }
    }
  }

  n = vsnprintf(buf, sizeof(buf), text, va);
  if(n<0) n=0;
  buf[n] = 0;

  MUTEX_STARTINF(hMutex);
  if(severity & DL_DIALOG) {
    sys_log_put_dialog(severity, buf);
  }
  p = buf;
  while(p!=NULL) {
    tt = p;
    if(log_eol && *tt) {
      DWORD tm = timeGetTime()-stime0;
      if(dump_screen && (fid&1)) {
//        printf("\n%09d,%04d::", tm, GetCurrentThreadId());
        char buf[256];
        sprintf(buf, "\n%09.3lf,%04d::", tm/1000.0, GetCurrentThreadId());
        sys_log_put(severity, buf);
      }
      for(ki=1; ki<=MAX_DEBUG_FILE_COUNT; ki++) {
        if((fid & (1<<ki)) && dump_file[ki-1]!=NULL) {
//          fprintf(dump_file[ki-1], "\n%09d,%04d::", tm, GetCurrentThreadId());
          fprintf(dump_file[ki-1], "\n%09.3lf,%04d::", tm/1000.0, GetCurrentThreadId());
          fflush(dump_file[ki-1]);
        }
      }
      log_eol = 0;
    }
    e = strchr(p, '\n');
    if(e!=NULL) {
      *e = 0;
      log_eol = 1;
      p = e+1;
    } else {
      p = NULL;
    }
    if(dump_screen && (fid&1)) {
      sys_log_put(severity, tt);
    }
    for(ki=1; ki<=MAX_DEBUG_FILE_COUNT; ki++) {
      if((fid & (1<<ki)) && dump_file[ki-1]!=NULL) {
        fprintf(dump_file[ki-1], "%s", tt);
      }
    }
  }
  //
  MUTEX_END(hMutex);
  return 0;
}

// ******************************************************************************************
// SYSTEM
// ******************************************************************************************

int sys_eols_normalize(char *out, int outsize, const char* in, int insize)
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

int sys_eols_prepare(char *out, int outsize, const char* in, int insize)
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

int sys_codepage_convert_cpi(char* out, int outsize, const char *in, int insize, int codepage1, int codepage2)
{
  int cp1 = find_cp_no(codepage1); if(cp1<0) return -1;
  int cp2 = find_cp_no(codepage2); if(cp2<0) return -2;
  return codepage_convert(out, outsize, in, insize, cp1, cp2);
}

int sys_codepage_convert_cps(char* out, int outsize, const char *in, int insize, const char *codepage1, const char *codepage2)
{
  int cp1 = find_cp_no(codepage1); if(cp1<0) return -1;
  int cp2 = find_cp_no(codepage2); if(cp2<0) return -2;
  return codepage_convert(out, outsize, in, insize, cp1, cp2);
}

// ******************************************************************************************

static int sys_list_fill(sys_list_context_t* ctx, WIN32_FIND_DATAW *ff)
{
  syshlp::gracial_convert(ctx->name, sizeof(ctx->name), ff->cFileName, wcslen(ff->cFileName));
  if(!parse2_wildcmp(ctx->mask, ctx->name))
    return 0;
  ctx->file_size = ((long long)ff->nFileSizeHigh<<32) + ff->nFileSizeLow;
  ctx->time_accessed = filetime_to_timet(ff->ftLastAccessTime);
  ctx->time_modified = filetime_to_timet(ff->ftLastWriteTime);
  ctx->time_created = filetime_to_timet(ff->ftCreationTime);
  //
  ctx->perm = 0;
  if(ff->dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
    ctx->perm = ~0 & ~slp_Write;
  } else {
    ctx->perm = ~0;
  }
  //
  if(ff->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
    ctx->type = slt_Directory;
  } else if(ff->dwFileAttributes & FILE_ATTRIBUTE_DEVICE) {
    ctx->type = slt_SerialDev;
  } else if(ff->dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
    if(ff->dwReserved0==IO_REPARSE_TAG_SYMLINK)
      ctx->type = slt_SymLink;
    else if(ff->dwReserved0==IO_REPARSE_TAG_MOUNT_POINT)
      ctx->type = slt_Mount;
    else
      ctx->type = slt_SymLink;
  } else {
    ctx->type = slt_File;
  }
  ctx->userattr = ff->dwFileAttributes;
  return 1;
}

int sys_list_begin(sys_list_context_t* ctx, const char *path, const char *mask, int options)
{
  if(ctx==NULL)
    return -1;
  if(path==NULL || *path==0) 
    path = ".";
  if(mask==NULL || *mask==0) 
    mask = "*";
  //
  ctx->path = path;
  ctx->mask = mask;
  ctx->options = options;
  //
  char longdir[4096];
  _snprintf(longdir, sizeof(longdir), "%s\\*", path, mask);
  ui::U16String<4096> dir(longdir);
  path_os_uni(dir);
  WIN32_FIND_DATAW ff;
  ctx->puser = INVALID_HANDLE_VALUE;
  //
  ctx->puser = FindFirstFileW(dir, &ff);
  if(ctx->puser == INVALID_HANDLE_VALUE)
    return -9;
  //
  do {
    if(sys_list_fill(ctx, &ff))
      return 0;
  } while(FindNextFileW(HANDLE(ctx->puser), &ff));
  FindClose(HANDLE(ctx->puser));
  ctx->puser = INVALID_HANDLE_VALUE;
  return -9;
}

int sys_list_next(sys_list_context_t* ctx)
{
  if(ctx==NULL)
    return -1;
  if(ctx->puser==NULL || ctx->puser==INVALID_HANDLE_VALUE)
    return -2;
  //
  WIN32_FIND_DATAW ff;
  while(FindNextFileW(HANDLE(ctx->puser), &ff)) {
    if(sys_list_fill(ctx, &ff))
      return 0;
  }
  FindClose(HANDLE(ctx->puser));
  ctx->puser = INVALID_HANDLE_VALUE;
  return -9;
}

int sys_list_end(sys_list_context_t* ctx)
{
  if(ctx==NULL)
    return -1;
  if(ctx->puser==NULL || ctx->puser==INVALID_HANDLE_VALUE)
    return -2;
  FindClose(HANDLE(ctx->puser));
  ctx->puser = INVALID_HANDLE_VALUE;
  return 0;
}


int sys_change_dir(const char *dira)
{
  ui::U16String<4096> dir(dira);
  path_uni_os(dir);
  BOOL rv = SetCurrentDirectoryW(dir);
  if(!rv) {
//    static char etext[4096];
//    sys_err(etext, sizeof etext, GetLastError());
//    dira = etext;
  }
  return rv!=0;
}

int sys_erase(const char *filedir)
{
  ui::U16String<4096> file(filedir);
  path_uni_os(file);
  return DeleteFileW(file) || RemoveDirectoryW(file) ? 0 : -1;
}

int sys_curdir(char *dira, size_t maxlen)
{
  ui::U16String<4096> file;
  if(::GetCurrentDirectoryW(4096, file)<=0 || !gracial_convert(dira, maxlen, file(), wcslen(file)))
    return -1;
  return 0;
}

int sys_chdir(const char *dira)
{
  ui::U16String<4096> file(dira);
  return ::SetCurrentDirectoryW(file) ? 0 : -1;
}

int sys_mkdir_recursive(const char *dira)
{
  ui::U16String<4096> dir(dira);
  path_uni_os(dir);
  wchar_t fn2[4096];
  const wchar_t *p1, *p2, *px = dir;
  
  if(wcsncmp(dir, L"\\", 2)==0) {
    px = wcschr(dir+2, L'\\');
    if(px==NULL)
      return -3; // bad NETWORK path
    px++;
  }
  while(1) {
    p1 = wcschr(px, L'\\');
    p2 = wcschr(px, L':');
    if(p2!=NULL && (p1==NULL || p1>p2)) 
      p1 = p2;
    if(p1==NULL)
      return CreateDirectory(dir, NULL) || GetLastError()==ERROR_ALREADY_EXISTS ? 0 : -1; // OK
    else if(*p1==L':') {
      px = p1+1;
      continue;
    }

    memcpy(fn2, dir, (p1-dir)*sizeof(wchar_t));
    fn2[p1-dir] = 0;
    if(!(CreateDirectory(fn2, NULL) || GetLastError()==ERROR_ALREADY_EXISTS))
      return -2; // failed create sub-dir
    px = p1+1;
  }
  return -1;
}

static bool create_pipe(HANDLE handles[3], DWORD size, DWORD maxwait, bool write)
{
  SECURITY_ATTRIBUTES sa;

  // Set up the security attributes struct.
  sa.nLength= sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle = TRUE;

   BOOL rv = CreatePipe(handles+0, handles+1, &sa, size);
   if(!rv)
     return false;
   if(!DuplicateHandle(GetCurrentProcess(), handles[write?0:1], 
                       GetCurrentProcess(), handles+2, 
                       0, FALSE, DUPLICATE_SAME_ACCESS)) {
     CloseHandle(handles[0]); handles[0] = INVALID_HANDLE_VALUE;
     CloseHandle(handles[1]); handles[1] = INVALID_HANDLE_VALUE;
     return false;
   } else {
//     CloseHandle(handles[write?0:1]);
//     handles[write?0:1] = INVALID_HANDLE_VALUE;
   }
   DWORD mode = PIPE_READMODE_BYTE|PIPE_WAIT;
   SetNamedPipeHandleState(handles[write?1:0], &mode, &size, &maxwait);
   mode = PIPE_READMODE_BYTE|PIPE_NOWAIT;
   SetNamedPipeHandleState(handles[2], &mode, &size, &maxwait);
   return true;
}

static bool destroy_pipe(HANDLE handles[3])
{
  CloseHandle(handles[0]); handles[0] = INVALID_HANDLE_VALUE;
  CloseHandle(handles[1]); handles[1] = INVALID_HANDLE_VALUE;
  CloseHandle(handles[2]); handles[2] = INVALID_HANDLE_VALUE;
  return true;
}

static bool prepare_pipe(HANDLE handles[3])
{
//  CloseHandle(handles[0]); handles[0] = INVALID_HANDLE_VALUE;
//  CloseHandle(handles[1]); handles[1] = INVALID_HANDLE_VALUE;
  return true;
}

static bool write_pipe(HANDLE handle, const char* &out, size_t &left, size_t block_size)
{
  static char err[1024];
  //
  DWORD rv;
  if(left) {
    size_t n = block_size<left ? block_size : left;
    if(WriteFile(handle, out, n, &rv, NULL)) {
      left -= rv;
      out += rv;
      return true;
    } else {
      sys_err(err, sizeof err, GetLastError());
    }
  }
  return false;
}

static bool read_pipe(HANDLE handle, char* &out, size_t &left, size_t block_size)
{
  static char err[1024];
  static bool ec;
  //
  DWORD rv;
  if(left) {
    size_t n = block_size<left ? block_size : left;
    if(ReadFile(handle, out, n, &rv, NULL)) {
      left -= rv;
      out += rv;
      return true;
    } else {
      ec = GetLastError() == ERROR_BROKEN_PIPE;
      sys_err(err, sizeof err, GetLastError());
    }
  }
  return false;
}

int sys_execute(const char *options, const char* cmd, const char *hstdin, char *hstdout, size_t nstdout, char *hstderr, size_t nstderr)
{
  ui::U16String<4096> cmdw(cmd);
  bool fhandles = hstdin && *hstdin;
  HANDLE handles[3][3];
  DWORD flags = 0;
  DWORD exitcode;
  STARTUPINFOW si;
  bool fuser = false;
  size_t nstdin = fhandles ? strlen(hstdin) : 0;
  size_t block_size = 4096;

  memset(&si, 0, sizeof si);
  si.cb = sizeof(si);
  si.dwFlags = 0;
  //
  if(strstr(options, "(cmd)")!=NULL) {
    ui::U16String<4096> cmdw2;
    wcscpy(cmdw2, L"cmd.exe /c ");
    wcscpy(cmdw2, cmdw);
    cmdw = cmdw2;
  }
  if(strstr(options, "(events)")!=NULL) {
    fuser = true;
  }
  if(strstr(options, "(nowindow)")!=NULL) {
    flags |= CREATE_NO_WINDOW;
  }
  if(strstr(options, "(detach)")!=NULL) {
    flags |= DETACHED_PROCESS;
  }
  if(strstr(options, "(new_console)")!=NULL) {
    flags |= CREATE_NEW_CONSOLE;
  }
  if(strstr(options, "(min)")!=NULL) {
    si.dwFlags |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_MINIMIZE;
  }
  if(strstr(options, "(max)")!=NULL) {
    si.dwFlags |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_MAXIMIZE;
  }
  if(strstr(options, "(hide)")!=NULL) {
    si.dwFlags |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
  }
  if(strstr(options, "(show)")!=NULL) {
    si.dwFlags |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;
  }
  //
  if(fhandles) {
    if(!create_pipe(handles[0], block_size, 1000, false))
      return false;
    if(!create_pipe(handles[1], block_size, 1000, true)) {
      destroy_pipe(handles[0]);
      return false;
    }
    if(!create_pipe(handles[2], block_size, 1000, true)) {
      destroy_pipe(handles[0]);
      destroy_pipe(handles[1]);
      return false;
    }
    //
    si.hStdInput = handles[0][0];
    si.hStdOutput = handles[1][1];
    si.hStdError = handles[2][1];
    si.dwFlags |= STARTF_USESTDHANDLES;
  }
  //
  //wcscpy(cmdw, L"cmd /c dir");
//  si.dwFlags &= ~STARTF_USESHOWWINDOW;
//  flags &= ~CREATE_NO_WINDOW;
//  flags |= CREATE_SUSPENDED;
  //
  PROCESS_INFORMATION pi;
  if(!CreateProcessW(NULL, cmdw, NULL, NULL, fhandles?TRUE:FALSE, flags, NULL, NULL, &si, &pi)) {
    if(fhandles) {
      destroy_pipe(handles[0]);
      destroy_pipe(handles[1]);
      destroy_pipe(handles[2]);
    }
    return -1;
  }
  prepare_pipe(handles[0]);
  prepare_pipe(handles[1]);
  prepare_pipe(handles[2]);
  //
  while(GetExitCodeProcess(pi.hProcess, &exitcode)) {
    if(fuser) {
      ui::PumpEvents();
    }
    Sleep(10);
    //
    if(fhandles) {
      bool repeat = false;
      do {
        repeat = false;
        repeat = write_pipe(handles[0][2], hstdin, nstdin, block_size) || repeat;
        repeat = read_pipe(handles[1][2], hstdout, nstdout, block_size) || repeat;
        repeat = read_pipe(handles[2][2], hstderr, nstderr, block_size) || repeat;
      } while(repeat);
    }
    //
    if(exitcode!=STILL_ACTIVE)
      break;
  }
  //
  write_pipe(handles[0][2], hstdin, nstdin, block_size);
  read_pipe(handles[1][2], hstdout, nstdout, block_size);
  read_pipe(handles[2][2], hstderr, nstderr, block_size);
  //
  if(hstderr) {
    if(nstderr) *hstderr = 0; else hstderr[-1] = 0;
  }
  if(hstdout) {
    if(nstdout) *hstdout = 0; else hstdout[-1] = 0;
  }
  //
  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);
  //
  if(fhandles) {
    if(fhandles) {
      destroy_pipe(handles[0]);
      destroy_pipe(handles[1]);
      destroy_pipe(handles[2]);
    }
  }
  //
  return 0;
}

int sys_execute_tool(const char *toolpath, int async)
{
  if(async) {
    return spawnlp(P_NOWAIT, "cmd.exe", "cmd.exe", "/c", toolpath, NULL);
  } else {
    return spawnlp(P_WAIT, "cmd.exe", "cmd.exe", "/c", toolpath, NULL);
  }
  return -1;
}

static FILE* c_fopen(const char *cfilename, const char *cmode)
{
  ui::U16String<4096> filename(cfilename);
  ui::U16String<80> mode(cmode);
  path_uni_os(filename);
  return _wfopen(filename, mode);
}

int sys_get_file_alloc(const char *name, charp_t* buffer, int binmode)
{
  FILE *f;
  int rv, rv2;
  const char *mode="rb";

  switch(binmode) {
    case 0:
    case 't':
      mode = "rt"; 
      break;
    case 1:
    case 'b':
      mode = "rb"; 
      break;
  }

  f = c_fopen(name, mode);

  if(f==NULL)
    return -1; // not found

  fseek(f, 0, SEEK_END);
  rv = ftell(f);
  fseek(f, 0, SEEK_SET);

  if(rv==0 || buffer==NULL) {
    if(buffer)
      buffer->p = NULL;
    fclose(f);
    return rv;
  }

  buffer->p = (char*)ZA_alloc(rv);
  if(buffer->p==NULL)
  {
    buffer->p = NULL;
    fclose(f);
    return -1;
  }

  rv2 = fread(buffer->p, 1, rv, f);
  if(rv2<0)
  {
    free(buffer->p);
    fclose(f);
    return -1;
  }

  fclose(f);

  return rv2; // real size read
}

int sys_get_file(const char *name, void* buffer, int bufsize, int binmode)
{
  FILE *f;
  int rv;
  const char *mode;

  switch(binmode) {
    case 0:
    case 't':
      mode = "rt"; 
      break;
    case 1:
    case 'b':
      mode = "rb"; 
      break;
  }

  f = c_fopen(name, mode);

  if(f==NULL)
    return -1; // not found

  if(buffer==NULL) {
    fseek(f, 0, SEEK_END);
    rv = ftell(f);
    fclose(f);
    if(!binmode)
      rv++;
    return rv;
  }

  if(binmode)
  {
    rv = fread(buffer, 1, bufsize, f);
    if(rv<0) {
      fclose(f);
      return 0; // fail
    }
    fclose(f);
    return rv; // real size read
  }

  rv = fread(buffer, 1, bufsize-1, f);
  if(rv<0) {
    *(char*)buffer = 0;
    fclose(f);
    return 0; // fail
  }

  ((char*)buffer)[rv] = 0;
  fclose(f);
  return rv+1; // real size read
}

int sys_put_file(const char *name, const void* buffer, int bufsize, int binmode)
{
  FILE *f;
  int rv;
  const char* mode = "wt";

  // root is com cache path
  switch(binmode) {
    case 'a':
    case 'A':
      mode = "at"; 
      break;
    case 0:
    case 't':
      mode = "wt"; 
      break;
    case 1:
    case 'b':
      mode = "wb"; 
      break;
  }
  

  f = c_fopen(name, mode);

  if(f==NULL)
    return -1; // not found

  if(bufsize>0) {
    rv = fwrite(buffer, 1, bufsize, f);
    if(rv<0) {
      fclose(f);
      return 0; // fail
    }
  } else {
    rv = 0;
  }

  fclose(f);
  return rv; // real size written
}

void delay(int msec)
{
  if(msec) {
    log_put(DL_MAIN_AUX, "{wait %d msec...}\n", msec);
    Sleep(msec);
    log_put(DL_MAIN_AUX, "{done %d}\n", msec);
  }
}

int get_file_list(const char *pathmask, int (*use_fn)(void *ctx, const char *fn, const char *attr, int filesize), void *ctx)
{
  ui::U16String<4096> wpm(pathmask);
  path_uni_os(wpm);
  WIN32_FIND_DATAW info;
  int n = 0;
  HANDLE find = FindFirstFileW(wpm, &info);

  if(find==INVALID_HANDLE_VALUE)
    return 0;
  do {
    if(wcscmp(info.cFileName, L".")==0 || wcscmp(info.cFileName, L"..")==0)
      continue; // skip system files
    //
    ui::U8String<16384> filename(info.cFileName);
    char attr[16] = {0};
    if(info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      strcat(attr, "D");
    }
    if(info.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
      strcat(attr, "L");
    }
    if(info.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED) {
      strcat(attr, "C");
    }
    if(info.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED) {
      strcat(attr, "E");
    }
    if(info.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
      strcat(attr, "R");
    } else {
      strcat(attr, "RW");
    }
    int rv = use_fn(ctx, filename, attr, info.nFileSizeLow);
    if(rv<0) {
      n = rv;
      break;
    }
    if(rv==0)
      break;
    n++;
  } while(FindNextFile(find, &info));
  FindClose(find);
  return n;
}

/****************************************/

void* SDB_memalloc(int size) 
{ 
  return malloc(size); 
}

void SDB_memfree(void* dat) 
{
  free(dat);
}

void* SDB_memrealloc(void* dat, int size) 
{
  return realloc(dat, size);
}

void* ZA_alloc(int size) 
{
  return malloc(size); 
}

void ZA_free(void* dat) 
{
  free(dat);
}

// ******************************************************************************************
// /SYSTEM
// ******************************************************************************************
