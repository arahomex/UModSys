#ifndef __UMODSYS_LIB2D_CONVERTOR_H
#define __UMODSYS_LIB2D_CONVERTOR_H 1
/*************************************************************/
// file: umodsys/lib/lib2d/lib2d.convertor.h
// info: 2d images conversion
/*************************************************************/

#include <umodsys/lib/2d/lib2d.common.h>

namespace UModSys {
namespace lib2d {

//***************************************
// TYPES
//***************************************

//***************************************
// types:
/*
type:
  "r8g8b8"      -> rgb24
  "r8g8b8a8"    -> rgba32
  "i8"          -> index8 or bw8
  "a8"          -> alphamap8
  "i8a8"        -> index8 or bw8, alphmap8
  "P8r8g8b8"    -> palette 8 bit rgb
  "P8r8g8b8a8"  -> palette 8 bit rgba
hinttype:
  "rgb/4"       -> rgb4
  "rgba/4"      -> rgba4
  "rgb/8"       -> rgb8
  "rgba/8"      -> rgba8
  "rgb/12"      -> rgb12
  "rgba/12"     -> rgba12
  "rgb/16"      -> rgb16
  "rgba/16"     -> rgba16
  "rgb/24"      -> rgb24
  "rgba/32"     -> rgba32
  "i1"          -> bw1
  "i2"          -> bw2
  "i4"          -> bw4
  "i8"          -> bw8
  "a1"          -> alpha1
  "a2"          -> alpha2
  "a4"          -> alpha4
  "a8"          -> alpha8
  "ia/2"        -> index+alpha 2
  "ia/4"        -> index+alpha 4
  "ia/8"        -> index+alpha 8
  "ia/16"       -> index+alpha 16
  prefixes:
  "C:"          -> compressed
  "UC:"         -> uncompressed
*/

//***************************************
// SComponentDef::  

struct SComponentDef {
  DCString type, hinttype;
  int hint;
  DPoint size;
  // this values can be 0 (zero)
  DPoint start;
  int bits, linebits;
  int bytes, linebytes;
  //
  SComponentDef(int dx=0, int dy=0, const DCString& t=NULL, int h=0) 
  : type(t), hint(h), size(dx, dy), start(0) {
    bits = linebits = 0;
    bytes = linebytes = 0;
  }
  void set(int dx, int dy, const DCString& t, int h=0) {
    type = t; hint=h; 
    size.v[0] = dx; 
    size.v[1] = dy;
  }
};

//***************************************
// SComponentW::  
// SComponentR::

struct SComponentW {
  SComponentDef def;
  SMem binary;
  SMemShared bintemp;
  //
  inline ~SComponentW(void) {}
  inline SComponentW(void) {}
  inline SComponentW(const SComponentW& r) 
  : def(r.def), binary(r.binary), bintemp(r.bintemp) {
  }
  inline const SComponentW& operator=(const SComponentW& r) {
    def = r.def;
    binary = r.binary;
    bintemp = r.bintemp;
    return *this;
  }
protected:
};

struct SComponentR {
  SComponentDef def;
  SCMem binary;
  SCMemShared bintemp;
  //
  inline ~SComponentR(void) {}
  inline SComponentR(void) {}
  inline SComponentR(const SComponentR& r) 
  : def(r.def), binary(r.binary), bintemp(r.bintemp) {
  }
  inline SComponentR(const SComponentW& r) 
  : def(r.def), binary(r.binary), bintemp(r.bintemp) {
  }
  inline const SComponentR& operator=(const SComponentW& r) {
    def = r.def;
    binary = r.binary;
    bintemp = r.bintemp;
    return *this;
  }
  //
protected:
};

//***************************************
// INTERFACES
//***************************************

//***************************************
// IConvertorDriver::  -- convert images

struct IConvertorDriver : public IRefObject {
  // -- allocate
  virtual bool alloc_i8p24(const DPoint &size, SMemChunk& i8, SMemChunk& pal) =0;
  virtual bool alloc_i8p32(const DPoint &size, SMemChunk& i8, SMemChunk& pal) =0;
  virtual bool alloc_rgb24(const DPoint &size, SMemChunk& rgb) =0;
  virtual bool alloc_rgb32(const DPoint &size, SMemChunk& rgba) =0;
  // -- bw8 => bw8
  virtual bool cvt_i8_i8(const DPoint &size, const SMem& i8, 
                         const DPoint &ssize, const SCMem& si8) =0;
  virtual bool cvt_i8_i8ri8(const DPoint &size, const SMem& i16, 
                            const DPoint &ssize, const SCMem& si8) =0;
  virtual bool cvt_i8_i8i8(const DPoint &size, const SMem& i16, 
                           const DPoint &ssize, const SCMem& si8) =0;
  // -- index8+pal => rgb/rgba
  virtual bool cvt_i8p24_rgb24(const DPoint &size, const SMem& rgb, 
                               const DPoint &ssize, const SCMem& i8, const SCMem& pal) =0;
  virtual bool cvt_i8p32_rgb24(const DPoint &size, const SMem& rgb, 
                               const DPoint &ssize, const SCMem& i8, const SCMem& pal) =0;
  virtual bool cvt_i8p24_rgba32(const DPoint &size, const SMem& rgba, 
                                const DPoint &ssize, const SCMem& i8, const SCMem& pal) =0;
  virtual bool cvt_i8p32_rgba32(const DPoint &size, const SMem& rgba, 
                                const DPoint &ssize, const SCMem& i8, const SCMem& pal) =0;
  // -- rgb/rgba => rgb/rgba
  virtual bool cvt_rgb24_rgb24(const DPoint &size, const SMem& rgb, 
                               const DPoint &ssize, const SCMem& srgb) =0;
  virtual bool cvt_rgb24_rgba32(const DPoint &size, const SMem& rgba, 
                                const DPoint &ssize, const SCMem& srgb) =0;
  virtual bool cvt_rgba32_rgb24(const DPoint &size, const SMem& rgb, 
                                const DPoint &ssize, const SCMem& srgba) =0;
  virtual bool cvt_rgba32_rgba32(const DPoint &size, const SMem& rgba, 
                                 const DPoint &ssize, const SCMem& srgba) =0;
  // -- index8+pal => index8+pal
  virtual bool cvt_i8p24_i8p24(const DPoint &size, const SMem& i8, const SMem& pal, 
                               const DPoint &ssize, const SCMem& si8, const SCMem& spal) =0;
  virtual bool cvt_i8p24_i8p32(const DPoint &size, const SMem& i8, const SMem& pal, 
                               const DPoint &ssize, const SCMem& si8, const SCMem& spal) =0;
  virtual bool cvt_i8p32_i8p24(const DPoint &size, const SMem& i8, const SMem& pal, 
                               const DPoint &ssize, const SCMem& si8, const SCMem& spal) =0;
  virtual bool cvt_i8p32_i8p32(const DPoint &size, const SMem& i8, const SMem& pal, 
                               const DPoint &ssize, const SCMem& si8, const SCMem& spal) =0;
  // -- rgb/rgba => rgb/rgba boxed
  virtual bool cvt_rgb24_rgb24(const DPoint &size, const SMem& rgb, const DBox& box,
                               const DPoint &ssize, const SCMem& srgb, const DBox& sbox) =0;
  virtual bool cvt_rgb24_rgba32(const DPoint &size, const SMem& rgba, const DBox& box, 
                                const DPoint &ssize, const SCMem& srgb, const DBox& sbox) =0;
  virtual bool cvt_rgba32_rgb24(const DPoint &size, const SMem& rgb, const DBox& box, 
                                const DPoint &ssize, const SCMem& srgba, const DBox& sbox) =0;
  virtual bool cvt_rgba32_rgba32(const DPoint &size, const SMem& rgba, const DBox& box, 
                                 const DPoint &ssize, const SCMem& srgba, const DBox& sbox) =0;
  //
  // -- rgb/rgba scale /2
  virtual bool cvt_rgb24_rgb24_scaleD2(const DPoint &size, const SMem& rgb, const SCMem& srgb) =0;
  virtual bool cvt_rgba32_rgba32_scaleD2(const DPoint &size, const SMem& rgba, const SCMem& srgba) =0;
  virtual bool cvt_i8i8_i8i8_scaleD2(const DPoint &size, const SMem& i16, const SCMem& si16) =0;
  virtual bool cvt_i8_i8_scaleD2(const DPoint &size, const SMem& i8, const SCMem& si8) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib2d::IConvertorDriver, 2, IRefObject);
};

//***************************************
// INLINES/OUTLINES
//***************************************

//***************************************
// END
//***************************************

} // namespace lib2d
} // namespace UModSys

#endif // __UMODSYS_LIB2D_CONVERTOR_H
