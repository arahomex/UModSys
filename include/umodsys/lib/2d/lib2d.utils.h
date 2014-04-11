#ifndef __UMODSYS_LIB2D_UTILS_H
#define __UMODSYS_LIB2D_UTILS_H 1
/*************************************************************/
// file: umodsys/lib/lib2d/lib2d.utils.h
// info: utility functions
/*************************************************************/

#include <umodsys/lib/2d/lib2d.common.h>

namespace UModSys {
namespace lib2d {

//***************************************
// INLINES/OUTLINES
//***************************************

typedef bool (*pixel_convert_f)(void* dest, const void* src, size_t count, void *ctx1, const void *src2);

//***************************************
// general converters

inline bool bin_move(const SMem& dest, const SImagePatchInfo& idest, const SCMem& src, const SImagePatchInfo& isrc)
{
  if(idest.type!=isrc.type)
    return false;
  int qsize = (idest.type & it_Mask_Plane1)>>it_Sh_Plane1;
  int linesize = idest.size(0) * qsize, linesize2 = isrc.size(0) * qsize;
  if(dest.size<idest.size(1)*linesize || src.size<isrc.size(1)*linesize2)
    return false; // corrupted size
  if(isrc.isframe(idest)) { // simple copy
    memcpy(dest.data, src.data, idest.size(1)*linesize);
    return true;
  } else if(isrc.isinside(idest)) { // sub-item
    int start = ((isrc.start(1) - idest.start(1)) * idest.size(0)  + (isrc.start(0) - idest.start(0)))*qsize;
    int lineadd = (idest.size(0) - isrc.size(0)) * qsize;
    register const BByte* sp = src.get_data(0);
    register BByte* dp = dest.get_data(start);
    register int num = isrc.size(1);
    while(num>0) {
      memcpy(dp, sp, linesize2);
      dp += linesize;
      sp += linesize2;
      num--;
    }
    return true;
  } else {
    return false; // invalid size
  }
}

//***************************************

inline bool bin_move_cvt(const SMem& dest, const SImagePatchInfo& idest, const SCMem& src, const SImagePatchInfo& isrc, pixel_convert_f cvt, void *ctx)
{
  int qsize1 = (isrc.type & it_Mask_Plane1)>>it_Sh_Plane1;
  int qsize2 = (idest.type & it_Mask_Plane1)>>it_Sh_Plane1;
  int linesize = idest.size(0) * qsize2, linesize2 = isrc.size(0) * qsize1;
  if(dest.size<idest.size(1)*linesize || src.size<isrc.size(1)*linesize2)
    return false; // corrupted size
  if(isrc.isinside(idest)) { // sub-item
    int start = ((isrc.start(1) - idest.start(1)) * idest.size(0)  + (isrc.start(0) - idest.start(0)))*qsize1;
    int lineadd = (idest.size(0) - isrc.size(0)) * qsize2;
    register const BByte* sp = src.get_data(0);
    register BByte* dp = dest.get_data(start);
    register int num = isrc.size(1), fcount = isrc.size(0);
    while(num>0) {
      cvt(dp, sp, fcount, ctx, NULL);
      dp += linesize;
      sp += linesize2;
      num--;
    }
    return true;
  } else {
    return false; // invalid size
  }
}

//***************************************

inline bool bin_move_cvt2(const SMem& dest, const SImagePatchInfo& idest, const SCMem& src, const SCMem& src2, const SImagePatchInfo& isrc, pixel_convert_f cvt, void *ctx, pixel_convert_f chk=NULL)
{
  int qsize1 = (isrc.type & it_Mask_Plane1)>>it_Sh_Plane1;
  int qsize2 = (idest.type & it_Mask_Plane1)>>it_Sh_Plane1;
  int linesize = idest.size(0) * qsize2, linesize2 = isrc.size(0) * qsize1;
  if(dest.size<idest.size(1)*linesize || src.size<isrc.size(1)*linesize2)
    return false; // corrupted size
  if(isrc.isinside(idest)) { // sub-item
    if(chk!=NULL && !chk(NULL, NULL, 0, ctx, src2.data))
      return false;
    int start = ((isrc.start(1) - idest.start(1)) * idest.size(0)  + (isrc.start(0) - idest.start(0)))*qsize1;
    int lineadd = (idest.size(0) - isrc.size(0)) * qsize2;
    register const BByte* sp = src.get_data(0);
    register BByte* dp = dest.get_data(start);
    register int num = isrc.size(1), fcount = isrc.size(0);
    while(num>0) {
      cvt(dp, sp, fcount, ctx, src2.data);
      dp += linesize;
      sp += linesize2;
      num--;
    }
    return true;
  } else {
    return false; // invalid size
  }
}

//***************************************

inline bool bin_build_mipmap(SCMem* maps, const SMem& mem, const SImagePatchInfo& inf, int mlevels)
{
  int qsize = (inf.type & it_Mask_Plane1)>>it_Sh_Plane1;
  int linesize = inf.size(0) * qsize, nlines = inf.size(1), boxsize = nlines * linesize;
  register BByte* src = mem.get_data(0);
  if(maps!=NULL) {
    maps[0].assign(src, boxsize);
    maps++;
  }
  mlevels--;
  while(mlevels>0) {
    register BByte* dest = src + boxsize;
    if(maps!=NULL) {
      maps[0].assign(dest, boxsize>>2);
      maps++;
    }
    register BByte* src2 = src + linesize;
    int dy = nlines>>1;
    while(dy>0) {
      register int dx = linesize>>1;
      while(dx>0) {
        *dest++ = (int(src[0]) + int(src[1]) + int(src2[0]) + int(src2[1]))>>2;
        src+=2;
        src2+=2;
        dx--;
      }
      src += linesize;
      src2 += linesize;
      dy--;
    }
    linesize >>= 1;
    nlines  >>= 1;
    boxsize >>= 2;
    mlevels--;
  }
  return true;
}

//***************************************
// specialized converters

inline bool bin_cvt_L8_L8A8(void* dest, const void* src, size_t count, void *ctx, const void *src2)
{
  register const BByte *sp = static_cast<const BByte *>(src);
  register BByte *dp = static_cast<BByte *>(dest);
  while(count) {
    register BByte v = *sp++;
    *dp++ = v;
    *dp++ = v;
    count--;
  }
  return true;
}

//***************************************

inline bool bin_cvt_X24_X24A8(void* dest, const void* src, size_t count, void *ctx, const void *src2) 
// ctx=&alpha
{
  register const BByte *sp = static_cast<const BByte *>(src);
  register BByte *dp = static_cast<BByte *>(dest);
  register BByte alpha = *static_cast<const BByte *>(ctx);
  while(count) {
    *dp++ = *sp++;
    *dp++ = *sp++;
    *dp++ = *sp++;
    *dp++ = alpha;
    count--;
  }
  return true;
}

//***************************************

inline bool bin_cvt_X24A8_X24(void* dest, const void* src, size_t count, void *ctx, const void *src2)
{
  register const BByte *sp = static_cast<const BByte *>(src);
  register BByte *dp = static_cast<BByte *>(dest);
  while(count) {
    *dp++ = *sp++;
    *dp++ = *sp++;
    *dp++ = *sp++;
    sp++;
    count--;
  }
  return true;
}

//***************************************

inline bool bin_cvt_I8pX24_X24(void* dest, const void* src, size_t count, void *ctx, const void *src2) 
// src2=palette
{
  register const BByte *xp = static_cast<const BByte *>(src2);
  register const BByte *sp = static_cast<const BByte *>(src);
  register BByte *dp = static_cast<BByte *>(dest);
  while(count) {
    register int iv = *sp++;
    register const BByte *v = xp+(iv<<1)+iv;
    *dp++ = *v++;
    *dp++ = *v++;
    *dp++ = *v++;
    count--;
  }
  return true;
}

//***************************************

inline bool bin_cvt_I8pX24_X24A8(void* dest, const void* src, size_t count, void *ctx, const void *src2) 
// src2=palette, ctx=&alpha
{
  register const BByte *xp = static_cast<const BByte *>(src2);
  register const BByte *sp = static_cast<const BByte *>(src);
  register BByte *dp = static_cast<BByte *>(dest);
  register BByte alpha = *static_cast<const BByte *>(ctx);
  while(count) {
    register int iv = *sp++;
    register const BByte *v = xp+(iv<<1)+iv;
    *dp++ = *v++;
    *dp++ = *v++;
    *dp++ = *v++;
    *dp++ = alpha;
    count--;
  }
  return true;
}

//***************************************

inline bool bin_cvt_I8pX24A8_X24A8(void* dest, const void* src, size_t count, void *ctx, const void *src2)
// src2=palette
{
  register const BByte *xp = static_cast<const BByte *>(src2);
  register const BByte *sp = static_cast<const BByte *>(src);
  register BByte *dp = static_cast<BByte *>(dest);
  while(count) {
    register int iv = *sp++;
    register const BByte *v = xp+(iv<<2);
    *dp++ = *v++;
    *dp++ = *v++;
    *dp++ = *v++;
    *dp++ = *v++;
    count--;
  }
  return true;
}

//***************************************

inline bool bin_cvt_I8pX24A8_X24(void* dest, const void* src, size_t count, void *ctx, const void *src2) 
// src2=palette
{
  register const BByte *xp = static_cast<const BByte *>(src2);
  register const BByte *sp = static_cast<const BByte *>(src);
  register BByte *dp = static_cast<BByte *>(dest);
  while(count) {
    register int iv = *sp++;
    register const BByte *v = xp+(iv<<2);
    *dp++ = *v++;
    *dp++ = *v++;
    *dp++ = *v++;
    count--;
  }
  return true;
}

//***************************************
// END
//***************************************

} // namespace lib2d
} // namespace UModSys

#endif // __UMODSYS_LIB2D_UTILS_H
