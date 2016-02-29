#include <umodsys/lib/media/libmedia.common.h>
#include <umodsys/core/syshlp.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::libmedia;

//***************************************
// SVFileName::
//***************************************

SVFileName::SVFileName(BCStr whole, bool dir)
: valid(false) {
  *namebuf = 0;
  purenamelen = 0;
  init(whole, whole ? tl::su::slen(whole) : 0, dir);
}

SVFileName::SVFileName(const DCString &whole, bool dir) 
: valid(false) {
  *namebuf = 0;
  purenamelen = 0;
  init(*whole, whole.length, dir);
}

SVFileName::SVFileName(const DCString &prefix, BCStr suffix, bool dir)
: valid(false) {
  *namebuf = 0;
  valid = (syshlp::safe_snprintf(namebuf, MaxFilenameSize, "%s%s", prefix.text, suffix)!=0);
  if(valid)
    name = namebuf;
}

SVFileName::SVFileName(const DCString &prefix, const DCString& suffix, bool dir)
: valid(false) {
  *namebuf = 0;
  valid = (syshlp::safe_snprintf(namebuf, MaxFilenameSize, "%s%s", prefix.text, suffix.text)!=0);
  if(valid)
    name = namebuf;
}

void SVFileName::operator=(BCStr whole)
{
  valid = false;
  *namebuf = 0;
  purenamelen = 0;
  init(whole, whole ? tl::su::slen(whole) : 0, false);
}


bool SVFileName::relative_up(bool fpath, bool fname)
{
  if(!valid)
    return valid;
  if(purenamelen && fpath) {
    int poss[MaxFilenameSize], possn = 0;
    for(BCStr x=tl::su::schr(namebuf, '/'); x; x=tl::su::schr(x+1, '/')) {
      if(name.length-(x-namebuf)<=purenamelen)
        continue; // don't count name
      poss[possn++] = x-namebuf;
    }
    if(possn<=1) {
      valid = false;
      return valid;
    }
    int pos1 = poss[possn-1], pos2=poss[possn-2];
    int size = name.length - pos1 + 1;
    tl::su::smemmove(namebuf+pos2, namebuf+pos1, size);
    name = namebuf;
  } else if(fname) {
    int poss[MaxFilenameSize], possn = 0;
    for(BCStr x=tl::su::schr(namebuf, '/'); x; x=tl::su::schr(x+1, '/')) {
      if(x==namebuf + (name.length-1))
        continue; // don't count last '/'
      poss[possn++] = x-namebuf;
    }
    if(possn<=1) {
      valid = false;
      return valid;
    }
    int pos1 = poss[possn-1], pos2=poss[possn-2];
    int size = name.length - pos1 + 1;
    tl::su::smemmove(namebuf+pos2, namebuf+pos1, size);
    name = namebuf;
  }
  return valid;
}

void SVFileName::init(BCStr whole, int L, bool dir)
{
  if(whole==NULL || *whole==0) {
    name = namebuf;
    valid = true;
  } else {
    if(L>MaxFilenameSize-3)
      return; // no data
    BWStr p = namebuf, p2;
    if(whole[0]!='/')
      *p++ = '/';
    tl::su::smemcpy(p, whole, L);
    p += L;
    p2 = tl::su::sstr(namebuf, "//");
    if(p2!=NULL) {
      tl::su::smemcpy(p2, p2+1, (p-p2));
      p--;
    }
    if(dir) {
      if(whole[L-1]!='/')
        *p++ = '/';
    } else {
      if(whole[L-1]=='/')
        --p;
    }
    *p++ = 0;
    name = namebuf;
    if(p2!=NULL) {
      purenamelen = name.length - (p2-namebuf)-1;
    }
    valid = true;
  }
}

//***************************************
// ::
//***************************************
