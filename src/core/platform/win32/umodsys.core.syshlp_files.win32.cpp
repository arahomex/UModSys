#include <umodsys/core/syshlp.h>

#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <umodsys/core/platform/win32/syshlp_win32.h>
#include <umodsys/core/stdstrings.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::core::syshlp;

//***************************************
// syshlp::
//***************************************

static DWORD s_oldCP = 0;
static bool no_colors = true;

void syshlp::con_setup(bool enable_colors)
{
  DWORD err;
  if(s_oldCP==0)
    s_oldCP = GetConsoleOutputCP();
  if(!SetConsoleOutputCP(CP_UTF8)) {
    err = GetLastError();         
  }
  no_colors = !enable_colors;
  if(!no_colors) {
    SetConsoleTextAttribute(stdout, NULL);
    SetConsoleTextAttribute(stderr, NULL);
  }
}

void syshlp::con_restore(void)
{
  DWORD err;
  if(s_oldCP==0)
    return;
  if(!SetConsoleOutputCP(s_oldCP)) {
    err = GetLastError();         
  }
  if(!no_colors) {
    SetConsoleTextAttribute(stdout, NULL);
    SetConsoleTextAttribute(stderr, NULL);
  }
}

void syshlp::con_setcolor(FILE* stream, const unsigned char *rgb)
{
  if(no_colors)
    return;
  DWORD err;
  if(stream==NULL)
    return;
  int fno = _fileno(stream);
  if(fno<0)
    return;
  HANDLE h = (HANDLE)_get_osfhandle(fno);
  if(h==NULL)
    return;
  DWORD attr = 0;
  if(rgb!=NULL) {
    if(rgb[0]>=0x80) attr |= FOREGROUND_RED;
    if(rgb[1]>=0x80) attr |= FOREGROUND_GREEN;
    if(rgb[2]>=0x80) attr |= FOREGROUND_BLUE;
    if(rgb[0]>=0xc0 || rgb[1]>=0xc0 || rgb[2]>=0xc0) attr |= FOREGROUND_INTENSITY;
    //
    if(rgb[3]>=0x80) attr |= BACKGROUND_RED;
    if(rgb[4]>=0x80) attr |= BACKGROUND_GREEN;
    if(rgb[5]>=0x80) attr |= BACKGROUND_BLUE;
    if(rgb[3]>=0xc0 || rgb[4]>=0xc0 || rgb[5]>=0xc0) attr |= BACKGROUND_INTENSITY;
  } else {
    attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
  }
  BOOL rv = SetConsoleTextAttribute(h, attr);
  if(!rv) {
    err = GetLastError();         
  }
}

//***************************************

FILE* syshlp::u_fopen(const char *cfilename, const char *cmode)
{
  U16String<4096> filename(cfilename);
  U16String<80> mode(cmode);
  path_uni_os(filename);
  return _wfopen(filename, mode);
}

FILE* syshlp::u_fopentemp(char* &handle, const char *msk)
{
  size_t L = strlen(msk)+1;
  handle = static_cast<char*>(local_memory().mem_alloc(L*2+10, UMODSYS_SOURCEINFO));
  if(handle==NULL) {
    return NULL;
  }
  strcpy(handle, msk);
  strcpy(handle+L, msk);
  strcpy(handle+L*2-1, ".tmp");
  FILE *f = u_fopen(handle+L, "wb");
  if(f==NULL) {
    local_memory().mem_free(handle, UMODSYS_SOURCEINFO);
    handle = NULL;
    return NULL;
  }
  return f;
}

bool syshlp::u_fendtemp(FILE* &f, char* &handle, bool gracial)
{
  bool ok = true;
  if(f) {
    ::fclose(f);
    f = NULL;
  }
  if(handle) {
    BCStr n1 = handle;
    BCStr n2 = handle+strlen(handle)+1;
    //
    if(gracial) {
      ok = ::remove(n1)==0 && ok;
      ok = ::rename(n2, n1)==0 && ok;
      ::remove(n2);
    } else {
      ok = ::remove(n2)==0 && ok;
    }
    local_memory().mem_free(handle, UMODSYS_SOURCEINFO);
    handle = NULL;
  }
  return ok;
}

//***************************************

/*
static int sys_list_fill(sys_list_context_t* ctx, WIN32_FIND_DATAW *ff)
{
  syshlp::gracial_convert(ctx->name, sizeof(ctx->name), ff->cFileName, wcslen(ff->cFileName));
  if(!tl::su::wildcmp(ctx->mask, ctx->name))
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

int syshlp::list_begin(sys_list_context_t* ctx, const char *path, const char *mask, int options)
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
  U16String<4096> dir(longdir);
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

int syshlp::list_next(sys_list_context_t* ctx)
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

int syshlp::list_end(sys_list_context_t* ctx)
{
  if(ctx==NULL)
    return -1;
  if(ctx->puser==NULL || ctx->puser==INVALID_HANDLE_VALUE)
    return -2;
  FindClose(HANDLE(ctx->puser));
  ctx->puser = INVALID_HANDLE_VALUE;
  return 0;
}
*/

//***************************************
//***************************************

/*
int syshlp::file_list(const char *pathmask, int (*use_fn)(void *ctx, const char *fn, const char *attr, int filesize), void *ctx)
{
  U16String<4096> wpm(pathmask);
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
    U8String<16384> filename(info.cFileName);
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
*/

int syshlp::u_list(const char *path, const char *mask, IListAccepter &fna, int options)
{
  if(path==NULL || *path==0) 
    path = ".";
  if(mask==NULL || *mask==0) 
    mask = "*";
  SListContext::SList ss(fna);
  if(SListContext::use(path, mask, ss, options))
    return -1;
  return ss();
}

//***************************************
//***************************************

bool syshlp::u_chdir(const char *dira)
{
  U16String<4096> dir(dira);
  path_uni_os(dir);
  BOOL rv = SetCurrentDirectoryW(dir);
  if(!rv) {
//    static char etext[4096];
//    sys_err(etext, sizeof etext, GetLastError());
//    dira = etext;
  }
  return rv!=0;
}

bool syshlp::u_curdir(char *dira, size_t maxlen)
{
  U16String<4096> file;
  if(::GetCurrentDirectoryW(4096, file)<=0 || !gracial_convert(dira, maxlen, file(), wcslen(file)))
    return false;
  return true;
}

bool syshlp::u_erase(const char *filedir)
{
  U16String<4096> file(filedir);
  path_uni_os(file);
  return DeleteFileW(file) || RemoveDirectoryW(file);
}

bool syshlp::u_mkdir_recursive(const char *dira)
{
  U16String<4096> dir(dira);
  path_uni_os(dir);
  wchar_t fn2[4096];
  const wchar_t *p1, *p2, *px = dir;
  
  if(wcsncmp(dir, L"\\", 2)==0) {
    px = wcschr(dir+2, L'\\');
    if(px==NULL)
      return false; // bad NETWORK path
    px++;
  }
  while(1) {
    p1 = wcschr(px, L'\\');
    p2 = wcschr(px, L':');
    if(p2!=NULL && (p1==NULL || p1>p2)) 
      p1 = p2;
    if(p1==NULL)
      return CreateDirectoryW(dir, NULL) || GetLastError()==ERROR_ALREADY_EXISTS; // OK
    else if(*p1==L':') {
      px = p1+1;
      continue;
    }
    memcpy(fn2, dir, (p1-dir)*sizeof(wchar_t));
    fn2[p1-dir] = 0;
    if(!(CreateDirectoryW(fn2, NULL) || GetLastError()==ERROR_ALREADY_EXISTS))
      return false; // failed create sub-dir
    px = p1+1;
  }
  return false;
}

bool syshlp::u_fullpath(const char* relname, char *dira, size_t maxlen)
{
  return false;
}

//***************************************
//***************************************

bool syshlp::get_file_size(const char *name, size_t& size, bool ftext)
{
  FILE *f = u_fopen(name, ftext ? "rt" : "rb");
  if(f==NULL)
    return false;
  size = 0;
  fpos_t endpos;
  if(::fseek(f, 0, SEEK_END) 
    || ::fgetpos(f, &endpos) 
    || endpos>mem_max_allocation) {
    ::fclose(f);
    return false;
  }
  size = size_t(endpos);
  ::fclose(f);
  return true;
}

bool syshlp::get_file(const char *name, size_t& size, void*& buffer, IMemAlloc* imem, bool ftext)
{
  if(name==NULL || imem==NULL)
    return false;
  buffer = NULL; size = 0;
  FILE *f = u_fopen(name, ftext ? "rt" : "rb");
  if(f==NULL)
    return false;
  try {
    fpos_t endpos;
    if(::fseek(f, 0, SEEK_END) 
       || ::fgetpos(f, &endpos) 
       || ::fseek(f, 0, SEEK_SET) 
       || endpos>mem_max_allocation) {
      ::fclose(f);
      return false;
    }
    size = size_t(endpos);
    if(size==0) {
      ::fclose(f);
      return true;
    }
    buffer = imem->mem_alloc(size + (ftext!=0), UMODSYS_SOURCEINFO);
    if(buffer==NULL) {
      ::fclose(f);
      return false;
    }
    if(ftext)
      static_cast<char*>(buffer)[size] = 0;
    if(fread(buffer, 1, size, f)!=size) {
      imem->mem_free(buffer, UMODSYS_SOURCEINFO);
      ::fclose(f);
      return false;
    }
    ::fclose(f);
    return true;
  } catch(...) {
    ::fclose(f);
    if(buffer!=NULL)
      imem->mem_free(buffer, UMODSYS_SOURCEINFO);
    throw;
  }
}

bool syshlp::get_file(const char *name, size_t& size, void* buffer, size_t bufsize, bool ftext)
{
  if(buffer==NULL)
    return get_file_size(name, size);
  if(name==NULL || bufsize<=(ftext-1))
    return false;
  size = 0;
  FILE *f = u_fopen(name, ftext ? "rt" : "rb");
  if(f==NULL)
    return false;
  fpos_t endpos;
  if(::fseek(f, 0, SEEK_END) 
     || ::fgetpos(f, &endpos) 
     || ::fseek(f, 0, SEEK_SET) 
     || endpos>bufsize-(ftext!=0)) {
    ::fclose(f);
    return false;
  }
  size = size_t(endpos);
  if(size==0) {
    if(ftext)
      static_cast<char*>(buffer)[size] = 0;
    ::fclose(f);
    return true;
  }
  if(fread(buffer, 1, bufsize-1, f)!=size) {
    ::fclose(f);
    return false;
  }
  if(ftext)
    static_cast<char*>(buffer)[size] = 0;
  ::fclose(f);
  return true;
}

bool syshlp::put_file(const char *name, const void* buffer, size_t bufsize, bool ftext, bool append)
{
  return put_file(name, buffer, bufsize, *static_cast<size_t*>(NULL), ftext, append);
}

bool syshlp::put_file(const char *name, const void* buffer, size_t bufsize, size_t& size, bool ftext, bool append)
{
  if(name==NULL || buffer==NULL)
    return false;
  FILE *f = u_fopen(name, ftext ? (append ? "at" : "wt" ) : (append ? "ab" : "wb"));
  if(f==NULL)
    return false;
  size_t rv = ::fwrite(buffer, 1, bufsize, f);
  ::fclose(f);
  if(rv==size_t(-1) || rv!=bufsize && &size==NULL)
    return false;
  return true;  
}

//***************************************
// SListContext::
//***************************************

SListContext::SListContext(const char *p, const char *m, int o) 
: options(o), path(p), mask(m), hScan(INVALID_HANDLE_VALUE) 
{
/*
  if(path==NULL || *path==0) 
    path = ".";
  if(mask==NULL || *mask==0) 
    mask = "*";
*/
}

SListContext::~SListContext(void) 
{
  close();
}

//***************************************
//***************************************

bool SListContext::close(void)
{
  if(hScan==INVALID_HANDLE_VALUE)
    return false;
  FindClose(hScan);
  hScan = INVALID_HANDLE_VALUE;
  return true;
}

bool SListContext::open(bool usemask)
{
  close();
  char longdir[MAX_PATH_LEN*2];
  safe_snprintf(longdir, MAX_PATH_LEN, "%s/%s", path, usemask ? mask : "*");
  U16String<MAX_PATH_LEN> dir(longdir);
  path_os_uni(dir);
  hScan = FindFirstFileW(dir, &ff);
  if(hScan == INVALID_HANDLE_VALUE)
    return false;
  return true;
}

bool SListContext::fill(IListAccepter::Filename& fn, bool checkmask)
{
  if(hScan==INVALID_HANDLE_VALUE)
    return false;
  syshlp::gracial_convert(found_name, sizeof(found_name), ff.cFileName, wcslen(ff.cFileName));
  isParent = tl::su::seq(found_name, ".") || tl::su::seq(found_name, "..");
  if((options & IListAccepter::o_ShowParent)==0 && isParent)
    return false;
  if(checkmask && !tl::su::wildcmp(mask, found_name))
    return false;
  fn.file_size = ((long long)ff.nFileSizeHigh<<32) + ff.nFileSizeLow;
  fn.time_accessed = filetime_to_timet(ff.ftLastAccessTime);
  fn.time_modified = filetime_to_timet(ff.ftLastWriteTime);
  fn.time_created = filetime_to_timet(ff.ftCreationTime);
  //
  fn.perm = 0;
  if(ff.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
    fn.perm = ~0 & ~IListAccepter::p_Write;
  } else {
    fn.perm = ~0;
  }
  //
  if(ff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
    fn.type = IListAccepter::t_Directory;
  } else if(ff.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) {
    fn.type = IListAccepter::t_SerialDev;
  } else if(ff.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
    if(ff.dwReserved0==IO_REPARSE_TAG_SYMLINK)
      fn.type = IListAccepter::t_SymLink;
    else if(ff.dwReserved0==IO_REPARSE_TAG_MOUNT_POINT)
      fn.type = IListAccepter::t_Mount;
    else
      fn.type = IListAccepter::t_SymLink;
  } else {
    fn.type = IListAccepter::t_File;
  }
  fn.userattr = ff.dwFileAttributes;
  return true;
}

bool SListContext::next(void)
{
  if(hScan==INVALID_HANDLE_VALUE)
    return false;
  if(!FindNextFileW(hScan, &ff)) {
    close();
    return false;
  }
  return true;
}


bool syshlp::u_fchsize(FILE *file, long pos)
{
 return _chsize(_fileno(file), pos)==0;
}

//***************************************
// ::
//***************************************

