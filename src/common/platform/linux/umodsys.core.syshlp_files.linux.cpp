#include <umodsys/common/stdsyshlp.h>

#include <umodsys/common/platform/linux/syshlp_linux.h>
#include <umodsys/common/stdstrings.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::core::syshlp;

//***************************************
// syshlp::
//***************************************

static Buint32 s_ansi_colors[256] = 
#include "ansi-colors.h"
;

static int s_find_ansi_color(const unsigned char *rgb, int max=256)
{
  int rv = 0;
  int fdist = 1000000;
  for(int i=0; i<max; i++) {
    Buint32 c = s_ansi_colors[i];
    int dr = int((c>>16)&0xff)-rgb[0];
    int dg = int((c>>8)&0xff)-rgb[1];
    int db = int((c>>0)&0xff)-rgb[2];
    int dist = dr*dr + dg*dg + db*db;
    if(fdist>dist) {
      rv = i;
      fdist = dist;
    }
  }
  return rv;
}

static int ncolors = 0;

void syshlp::con_setup(bool enable_colors)
{
  if(!enable_colors) {
    ncolors = 0;
    return;
  }
  FILE *f = popen("tput colors", "r");
  if(f==NULL)
    return;
  char buf[256];
  if(!fgets(buf, sizeof(buf), f)) {
    *buf = 0;
  }
  pclose(f);
  //
  ncolors = atoi(buf);
  if(ncolors<16) {
    ncolors = 0;
  }
}

void syshlp::con_restore(void)
{
  if(ncolors!=0) {
    con_setcolor(stdout, NULL);
    con_setcolor(stderr, NULL);
  }
}

void syshlp::con_setcolor(FILE* stream, const unsigned char *rgb)
{
  if(ncolors>=16 && ncolors<=256) {
    if(rgb==NULL) {
      fprintf(stream, "\x1b[0m");
    } else {
      if(ncolors<256) {
      } else {
        int clrf = s_find_ansi_color(rgb, ncolors);
        int clrb = s_find_ansi_color(rgb+3, ncolors);
        fprintf(stream, "\x1b[38;5;%dm\x1b[48;5;%dm", clrf, clrb);
      }
    }
  }
}

//***************************************
//***************************************

FILE* syshlp::u_fopen(const char *cfilename, const char *cmode)
{
  return fopen(cfilename, cmode);
}

FILE* syshlp::u_fopentemp(char* &handle, const char *msk)
{
  size_t L = strlen(msk)+1;
  handle = static_cast<char*>(local_memory().mem_alloc(L*2+16, UMODSYS_SOURCEINFO));
  if(handle==NULL) {
    return NULL;
  }
  strcpy(handle, msk);
  strcpy(handle+L, msk);
  strcpy(handle+L*2-1, ".temp");
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
//***************************************

bool syshlp::u_erase(const char *filedir)
{
  return ::remove(filedir)==0;
}

bool syshlp::u_curdir(char *dira, size_t maxlen)
{
  return getcwd(dira, maxlen)!=NULL;
}

bool syshlp::u_chdir(const char *dira)
{
  return ::chdir(dira)==0;
}

bool syshlp::u_mkdir_recursive(const char *dira)
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
  return (err) ? false : true;
}

//***************************************
//***************************************

bool syshlp::get_file_size(const char *name, size_t& size, bool ftext)
{
  FILE *f = u_fopen(name, ftext ? "rt" : "rb");
  if(f==NULL)
    return false;
  size = 0;
  off_t endpos;
  if(::fseek(f, 0, SEEK_END) 
    || (endpos=::ftell(f))<0 
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
    off_t endpos;
    if(::fseek(f, 0, SEEK_END) 
       || (endpos=::ftell(f))<0 
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
  off_t endpos;
  if(::fseek(f, 0, SEEK_END) 
     || (endpos=::ftell(f))<0 
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

bool syshlp::u_fchsize(FILE *file, long pos)
{
 return ftruncate(fileno(file), pos)==0;
}

//***************************************
// ::
//***************************************

