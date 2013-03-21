#include <umodsys/core/syshlp.h>

#include <umodsys/core/platform/linux/syshlp_linux.h>
#include <umodsys/core/stdstrings.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::core::syshlp;

//***************************************
// syshlp::
//***************************************

FILE* syshlp::c_fopen(const char *cfilename, const char *cmode)
{
  return fopen(cfilename, cmode);
}

//***************************************

#if 0

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

#endif

//***************************************
//***************************************

int syshlp::change_dir(const char *dira)
{
  return ::chdir(dira)==0;
}

int syshlp::erase(const char *filedir)
{
  return ::remove(filedir)==0;
}

int syshlp::curdir(char *dira, size_t maxlen)
{
  return getcwd(dira, maxlen)!=NULL;
}

int syshlp::chdir(const char *dira)
{
  return ::chdir(dira)==0;
}

int syshlp::mkdir_recursive(const char *dira)
{
  int err = 0;
  size_t len;
  int rc = 0;
  int cnt = 0;
  char *slash;
  char buff[4096];
  struct stat statbuff;

  len = strlen(dira);
  if (len >= sizeof buff) return -EINVAL;
  memcpy(buff, dira, len);
  buff[len] = 0;
  //
  for(slash = buff; (slash = strchr(slash+1, '/' )); ) {
        /* this is to catch double / in paths */
    if (slash[-1] == '/') continue;
    *slash = 0 ;
    rc = stat(buff, &statbuff);
    if (!rc) err = EEXIST; /* this is used to skip existing prefix */
    else {
      rc = mkdir(buff, DEFAULT_DIR_MODE);
      err = (rc) ? errno: 0;
    }
    switch(err) {
    case 0:
      cnt++;
    case EEXIST:
      break;
    case ENOTDIR:
    case EACCES:
    default:
      goto quit;
    }
    *slash = '/' ;
  }
quit:
  return (err) ? -err : cnt;
}

//***************************************
//***************************************

int syshlp::get_file(const char *name, void* buffer, int bufsize, int binmode)
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

int syshlp::put_file(const char *name, const void* buffer, int bufsize, int binmode)
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

//***************************************
// ::
//***************************************

