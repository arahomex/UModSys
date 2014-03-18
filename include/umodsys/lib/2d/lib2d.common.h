#ifndef __UMODSYS_LIB2D_COMMON_H
#define __UMODSYS_LIB2D_COMMON_H 1
/*************************************************************/
// file: umodsys/lib/lib2d/lib2d.common.h
// info: 2d common imterfaces
/*************************************************************/

#include <umodsys/lib/2d/lib2d.typedefs.h>

namespace UModSys {
namespace lib2d {

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

inline DColorElemf c2c(DColorElemi c) { return DColorElemf(c/255.0); }
inline DColorElemf c2c(DColorElemb c) { return DColorElemf(c/255.0); }
inline DColorElemi c2c(DColorElemf c) { return DColorElemi(c*255); }

inline DColorf c2c(const DColor &c) { return DColorf(c2c(c.v[0]), c2c(c.v[1]), c2c(c.v[2])); }
inline DColorf c2c(const DColorb &c) { return DColorf(c2c(c.v[0]), c2c(c.v[1]), c2c(c.v[2])); }
inline DColor c2c(const DColorf &c) { return DColor(c2c(c.v[0]), c2c(c.v[1]), c2c(c.v[2])); }

inline DColorAlphaf c2c(const DColorAlpha &c) { return DColorAlphaf(c2c(c.v[0]), c2c(c.v[1]), c2c(c.v[2]), c2c(c.v[3])); }
inline DColorAlphaf c2c(const DColorAlphab &c) { return DColorAlphaf(c2c(c.v[0]), c2c(c.v[1]), c2c(c.v[2]), c2c(c.v[3])); }
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
