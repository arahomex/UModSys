#ifndef __UMODSYS_LIB2D_COMMON_H
#define __UMODSYS_LIB2D_COMMON_H 1
/*************************************************************/
// file: umodsys/lib/lib2d/lib2d.common.h
// info: 2d common imterfaces
/*************************************************************/

#include <umodsys/stdcore.h>
//#include <umodsys/tl/composite/isockets.h>
#include <umodsys/tl/math/math_3d.h>


namespace UModSys {
namespace lib2d {

using namespace core;

//***************************************
// FORWARD/TYPEDEF
//***************************************

struct SImageCellInfo;
struct SImagePatchInfo;
template<typename _Type> struct TCellsDef2;

//***************************************

struct IImage;            // general 2d bitmap image
struct IMultiImage;       // general 2d bitmap multi-image (eg. font)
struct IConvertor;        // convert bitmaps
struct IConvertorDriver;  // convert bitmaps base functions
struct IRenderDriver;     // render driver object

struct SComponentDef;     // component definition
struct SComponentW;       // component r/w
struct SComponentR;       // component r/o

//***************************************

typedef math3d::TVector2f   DPointf;
typedef math3d::TVector3f   DColorf;
typedef math3d::TVector4f   DColorAlphaf;

typedef math3d::TVector2i   DPoint;
typedef math3d::TVector3i   DColor;
typedef math3d::TVector4i   DColorAlpha;

typedef math3d::TBoundBox2i DBox;

typedef float BColorElemf;
typedef int BColorElem;

//***************************************
// ENUM/CONST
//***************************************

enum eTransparrentMode {
  tm_Disabled     = -1,     // disabled blending process
  tm_Opaque       = 0,      // no transparrency at all
  tm_Glass        = 1,      // light filter (darken other)
  tm_DarkGlass    = 2,      // light filter (invert darken other)
  tm_Lighten      = 3,      // add color-to-color
  tm_AlphaLighten = 4,      // add color-to-color (rev alpha)
  tm_ColorLighten = 5,      // add color-to-color (multiplied by old color)
  tm_Glow         = 6,      // blending old one & new
  tm_SemiGlow     = 7,      // light filter & some glow
  tm_SemiGlass    = 8,      // light filter & some glow
  tm_Unknown      = 9
};

enum eAlign {
  a_Left          = 0,
  a_Right         = 1,
  a_Center        = 2,
  a_Justify       = 3
};

enum eTextOptions {
  to_HShift       = 0,
  to_HMask        = 0x00000f,
  to_Left         = a_Left,
  to_Right        = a_Right,
  to_Center       = a_Center,
  to_Justify      = a_Justify,

  to_VShift       = 4,
  to_VMask        = 0x0000f0,
  to_VLeft        = a_Left<<to_VShift,
  to_VRight       = a_Right<<to_VShift,
  to_VCenter      = a_Center<<to_VShift,
  to_VJustify     = a_Justify<<to_VShift,

  to_Wrap         = 0x000100,
  to_LF           = 0x000200,
  to_MultiLine    = to_LF,
  //
  to_CalcSize     = 0x001000,
  to_CalcCaretSize= 0x002000,
  to_CalcCaretPos = 0x004000,
  //
  to_FixedBox     = 0x100000,
  to_CropBox      = 0x200000,
  to_DontDraw     = 0x800000,
  //
  to_None         = 0x000000
};

enum eImageFill {
  if_Null,
  if_Empty,
  if_Rect,
  if_Ref,
  if_Unknown
};

enum eSubImageMode {
  sim_Null,
  sim_Empty,
  sim_Rect,
  sim_Unknown
};

enum eImageType {
  it_Sh_Plane1  =4*0,
  it_Sh_Plane2  =4*1,
  it_Sh_Plane3  =4*2,
  it_Sh_Plane4  =4*3,
  it_Mask_Plane1=0x0000000F,
  it_Mask_Plane2=0x000000F0,
  it_Mask_Plane3=0x00000F00,
  it_Mask_Plane4=0x0000F000,
  it_Mask_Number=0x0FF00000,
  it_Mask_Planes=0x000F0000,
  //
  it_Null       =0x00000000,
  it_R8G8B8     =0x00110003,
  it_R8G8B8A8   =0x00210004,
  it_L8         =0x01110001,
  it_L8A8       =0x01210002,
  it_I8_R8G8B8  =0x08120031,
  it_I8_R8G8B8A8=0x08220041,
  it_Undefined  =0x80000000
};


//***************************************
// TYPES
//***************************************

struct SBlendColorf {
  DColorAlphaf color;
  eTransparrentMode transmode;
  //
  operator int(void) const { return transmode; }
  operator const DColorAlphaf&(void) const { return color; }
  const DColorAlphaf* operator->(void) const { return &color; }
  DColorAlphaf* operator->(void) { return &color; }
  //
  inline SBlendColorf(void) {}
  inline SBlendColorf(const DColorAlphaf& c, eTransparrentMode tm) : color(c), transmode(tm) {}
  inline SBlendColorf(bool tflag) : color(1), transmode(tflag ? tm_Opaque : tm_Glow) {}
  inline SBlendColorf(eTransparrentMode tm) : color(1), transmode(tm) {}
};

struct SBlendColor {
  DColorAlpha color;
  eTransparrentMode transmode;
  //
  operator int(void) const { return transmode; }
  operator const DColorAlpha&(void) const { return color; }
  const DColorAlpha* operator->(void) const { return &color; }
  DColorAlpha* operator->(void) { return &color; }
  //
  inline SBlendColor(void) {}
  inline SBlendColor(const DColorAlpha& c, eTransparrentMode tm) : color(c), transmode(tm) {}
  inline SBlendColor(bool tflag) : color(1), transmode(tflag ? tm_Opaque : tm_Glow) {}
  inline SBlendColor(eTransparrentMode tm) : color(1), transmode(tm) {}
};

//***************************************
// INLINES/OUTLINES
//***************************************

inline BColorElemf c2c(BColorElem c) { return BColorElemf(c/255.0); }
inline BColorElem c2c(BColorElemf c) { return BColorElem(c*255); }

inline DColorf c2c(const DColor &c) { return DColorf(c2c(c.v[0]), c2c(c.v[1]), c2c(c.v[2])); }
inline DColor c2c(const DColorf &c) { return DColor(c2c(c.v[0]), c2c(c.v[1]), c2c(c.v[2])); }

inline DColorAlphaf c2c(const DColorAlpha &c) { return DColorAlphaf(c2c(c.v[0]), c2c(c.v[1]), c2c(c.v[2]), c2c(c.v[3])); }
inline DColorAlpha c2c(const DColorAlphaf &c) { return DColorAlpha(c2c(c.v[0]), c2c(c.v[1]), c2c(c.v[2]), c2c(c.v[3])); }

inline DPointf p2p(const DPoint &p) { return DPointf(p.v[0], p.v[1]); }
inline DPoint p2p(const DPointf &p) { return DPoint(p.v[0], p.v[1]); }

//***************************************

// note - prefix no more than 200 chars
extern bool _util_getcolor(const SParameters& params, DColorAlphaf& color, BCStr prefix);
extern bool _util_getcolor(const SParameters& params, DColorf& color, BCStr prefix);


//***************************************
// END
//***************************************

} // namespace lib2d
} // namespace UModSys

#endif // __UMODSYS_LIB2D_COMMON_H
