#ifndef __UMS_LIB3D_FONT_EMULATION_H
#define __UMS_LIB3D_FONT_EMULATION_H 1
//*****************************************************
// lib3d/emul_font.h: common 3d library interfaces, font display emulation
//*****************************************************

#include <ums/lib3d/material.h>
#include <ums/lib3d/driver3.h>

namespace UMS {
namespace lib3d {
namespace emulate_font {

//***************************************
// List of must-defined-helper-functions
//***************************************
/*
typedef TFontContext<Driver, Font, Index> DFontContext;

bool Driver::font_init(DFontContext& CC);
bool Driver::font_draw(DFontContext& CC, int cid)

int Font::base;
int Font::count;
STexPoint Font::spacing;
STexPoint Font::maxsize;
bool Font::get_raw_cell(int cid, DTexPoint* size, DTextPoint* ofs);
*/

//***************************************
// Font emulation state
//***************************************

struct SFontContextCommon {
  bool is_draw, is_calc_box, is_simple, is_caretpos, is_caretsize, is_aligned;
  int options;
  int count;
  const SIntPoint* a;
  const SIntPoint* b;
  const DTexPoint* scale;
  float ax, ay, bx, by, dx, dy, nx, ny, mdy, bx2, maxx;
  int shift, shift2;
  IRenderDriver::TextInfo& info;
  SPicture picture;
  int current;
  //
  inline void next(void) { ax += dx; if(ax>maxx) maxx=ax; }
  //
  void shift_start_by_align(void);
  //
  SFontContextCommon(IRenderDriver::TextInfo& inf, int cnt);
};

template<typename Driver, typename Font, typename Index=BCharU>
struct TFontContext : public SFontContextCommon {
  Driver* driver;
  Font *font;
  const Index *piclist, *piclist_caret;
  //
  TFontContext(Driver* drv, Font *fnt, IRenderDriver::TextInfo& inf, const Index *ee, int cnt);
  //
  bool reset(int key, int mode);
  void font_init(void);
  //
  bool draw(void);
  bool draw_complex(void);
  bool draw_simple(void);
  bool draw1(int cid);
  //
  inline void check_caret_size(const Index *piclist);
};

//***************************************
// SFontContextCommon::
//***************************************

inline SFontContextCommon::SFontContextCommon(IRenderDriver::TextInfo& inf, int cnt)
  : info(inf), count(cnt), options(inf.options) {
    is_draw = (options & lib2d::to_DontDraw)==0;
    is_calc_box = (options & lib2d::to_CalcSize)!=0;
    is_simple = (options & lib2d::to_FixedBox)==0;
    is_caretpos = (options & lib2d::to_CalcCaretPos)!=0;
    is_caretsize = (options & lib2d::to_CalcCaretSize)!=0;
    is_aligned = (options & lib2d::to_HMask)!=0 || (options & lib2d::to_VMask)!=0;
    a = info.a; 
    b = info.b; 
    scale = info.scale;
    shift = shift2 = 0;
    maxx = ax = a->v[0]; 
    ay = a->v[1]; 
}

inline void SFontContextCommon::shift_start_by_align(void) 
{
  switch(options & lib2d::to_HMask) {
    case lib2d::to_Left:
      ax = a->v[0];
      break;
    case lib2d::to_Right:
      ax = a->v[0] + floor(b->v[0] - maxx);
      break;
    case lib2d::to_Center:
      ax = a->v[0] + floor((b->v[0] - maxx)/2);
      break;
  } 
  maxx = ax;
  switch(options & lib2d::to_VMask) {
    case lib2d::to_VLeft:
      ay = a->v[1];
      break;
    case lib2d::to_VRight:
      ay = a->v[1] + floor(b->v[1] - by);
      break;
    case lib2d::to_VCenter:
      ay = a->v[1] + floor((b->v[1] - by)/2);
      break;
  } 
}

//***************************************
// TFontContext::
//***************************************

template<typename Driver, typename Font, typename Index>
inline TFontContext<Driver, Font, Index>::TFontContext(Driver* drv, Font *fnt, IRenderDriver::TextInfo& inf, const Index *ee, int cnt)
: SFontContextCommon(inf, cnt), driver(drv), font(fnt), piclist(ee) { 
    font_init();
    if(is_draw) {
      driver->font_init(*this); 
    }
    if(is_caretsize)
      piclist_caret = piclist + info.caret_pos;
}

template<typename Driver, typename Font, typename Index>
inline void TFontContext<Driver, Font, Index>::font_init(void)
{
  bx2 = ax;
  if(scale) { 
    nx = scale->v[0];
    ny = scale->v[1];
    dy = scale->v[1];
  } else {
    nx = 1; 
    ny = 1; 
    dy = 1;
  }
  dx = 0;
  mdy = 0;
  //
  bx = ax;
  by = ay + ceil(font->maxsize.v[1]*ny + font->spacing.v[1]*ny);
}

template<typename Driver, typename Font, typename Index>
inline void TFontContext<Driver, Font, Index>::check_caret_size(const Index *piclist) 
{
  if(is_caretsize && piclist==piclist_caret) {
    info.calc_caret.A.set(ax, ay);
    info.calc_caret.B.set(bx, by);
    is_caretsize=false;
  }
}

template<typename Driver, typename Font, typename Index>
inline bool TFontContext<Driver, Font, Index>::draw_complex(void)
{
  const Index* piclist = this->piclist;
  int count = this->count;

  // main loop
  for(; count>0; piclist++, count--) {
    BCharU n = *piclist;
    if(options & lib2d::to_Wrap) {
      if(reset(n, lib2d::to_Wrap)) {
        for(int i=-shift; i<=0; i++) {
          BCharU n = piclist[i];
          if(n=='\n') {
            reset('\n', lib2d::to_LF);
          } else {
            draw1(n);
            check_caret_size(piclist);
            next();
          }
        }
        bx2 = bx;
        shift = 0;
      }
    } else if((options & lib2d::to_LF) && n=='\n') {
      reset('\n', lib2d::to_LF);
    } else {
      draw1(n);
      check_caret_size(piclist);
      next();
    }
  }
  // draw the rest
  if((options & lib2d::to_Wrap) && shift) {
    reset('\n', lib2d::to_Wrap);
    for(int i=-shift; i<0; i++) {
      BCharU n = piclist[i];
      if(n=='\n') {
        reset('\n', lib2d::to_LF);
      } else {
        draw1(n);
        check_caret_size(piclist);
        next();
      }
    }
    shift = 0;
  }
  // end complex
  return true;
}

template<typename Driver, typename Font, typename Index>
inline bool TFontContext<Driver, Font, Index>::draw_simple(void)
{
  const Index* piclist = this->piclist;
  int count = this->count;

  for(; count>0; piclist++, count--) {
    BCharU n = *piclist;
    draw1(n);
    check_caret_size(piclist);
    next();
  }
  return true;
}

template<typename Driver, typename Font, typename Index>
inline bool TFontContext<Driver, Font, Index>::draw(void)
{
    if(is_simple) {
      if(is_calc_box)
        info.calc_size.A = *a;
      //
      draw_simple();
      //
      if(is_calc_box)
        info.calc_size.B.set( bx, by );
    } else {
      if(is_calc_box)
        info.calc_size.A = *a;
      //
      if(!is_aligned || !is_draw) {
        draw_complex();
      } else {
        is_draw = false;
        draw_complex();
        shift_start_by_align();
        is_draw = true;
        draw_complex();
      }
      if(is_calc_box)
        info.calc_size.B.set(maxx, by);
    }
    //
    if(is_caretsize) {
      ax = bx;
      check_caret_size(piclist_caret);
    }
    //
    return true;
}

template<typename Driver, typename Font, typename Index>
inline bool TFontContext<Driver, Font, Index>::reset(int key, int mode)
{
  switch(mode) {
    case lib2d::to_Wrap: {
      if(key<0 || key>=font->count) {
        reset(0, lib2d::to_LF);
        return true; // space (out of chars)
      }
      float dx = 0;
      if(key=='\n' || key==' ') {
        dx = font->spacing.v[0]*nx;
        bx2 += dx;
        reset(0, lib2d::to_LF);
        return true;
      }
      DTexPoint cdsize, cdofs;
      lib2d::eSubImageMode sim = font->get_raw_cell(key, &cdsize, &cdofs);
      if(sim==lib2d::sim_Null) {
        dx = font->spacing.v[0]*nx;
        bx2 += dx;
        reset(0, lib2d::to_LF);
        return true;
      }
      dx = (cdofs.v[0]+cdsize.v[0])*nx;
      bx2 += dx;
      if(sim==lib2d::sim_Empty) {
        reset(0, lib2d::to_LF);
        return true;
      }
      shift++;
      return false;
    } break;

    case lib2d::to_LF:
      if(key==0) {
        if(bx2<b->v[0])
          return false; // not enough
      }
      ax = (*a)(0); 
      ay += ceil(font->maxsize(1)*ny + font->spacing(1)*ny);
      bx2 = ax;
      return true;
  }
  return false;
}

template<typename Driver, typename Font, typename Index>
inline bool TFontContext<Driver, Font, Index>::draw1(int cid)
{
  current = cid;
  cid -= font->base; 
  if(cid<0 || cid >= font->count) {
    dx = font->spacing.v[0]*nx;
    return false;
  }
  //
  DTexPoint cdsize, cdofs;
  lib2d::eSubImageMode sim = font->get_raw_cell(cid, &cdsize, &cdofs);
  if(sim==lib2d::sim_Null) {
    dx = font->spacing.v[0]*nx;
    return true;
  }
  dx = cdsize.v[0]*nx;
  if(sim==lib2d::sim_Empty) {
    return true;
  }
  bx = ax+dx; 
  by = ay+cdsize.v[1]*dy;
  dx += (font->spacing.v[0]+cdofs.v[0])*nx;
  float ky = by - bx;
  if(mdy<ky) mdy = ky;
  //
  if(is_draw && sim==lib2d::sim_Rect) {
    font->get_cell_info(cid, picture, &DTexPoint(ax, ay));
    driver->font_draw(*this);
  }
  return true;
}

//***************************************
// END
//***************************************

} // namespace emulate_font
} // namespace lib3d
} // namespace UMS

#endif
