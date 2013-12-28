#ifndef __UMODSYS_LIB2D_DRIVER_H
#define __UMODSYS_LIB2D_DRIVER_H 1
/*************************************************************/
// file: umodsys/lib/lib2d/lib2d.driver.h
// info: 2d render driver
/*************************************************************/

#include <umodsys/lib/2d/lib2d.common.h>
#include <umodsys/lib/2d/lib2d.image.h>

namespace UModSys {
// foreign ones
namespace libui { struct ITerminal; } // namespace libui
// end foreign ones

namespace lib2d {

//***************************************
// INTERFACES
//***************************************

//***************************************
// IRenderDriver::  -- draw 2d things

struct IRenderDriver : public IRefObject {
public:
  struct TextInfo {
    int options, caret_pos;
    const DPoint *a, *b;
    const DPointf *scale;
    DBox calc_size, calc_caret;
    //
    inline TextInfo(const DPoint &aa, const DPointf *ascale=NULL) 
      : a(&aa), b(NULL), scale(ascale), options(to_None), caret_pos(-1) {}
    inline TextInfo(const DPoint &aa, const DPoint &ab, const DPointf *ascale=NULL) 
      : a(&aa), b(&ab), scale(ascale), options(to_None), caret_pos(-1) {}
    inline TextInfo(const DPoint &aa, const DPoint &ab, const DPointf &ascale) 
      : a(&aa), b(&ab), scale(&ascale), options(to_None), caret_pos(-1) {}
    //
    inline void set_option(int oropt, int andnotopt=0) { options &= ~andnotopt; options |= oropt; }
  };
public:
  // -- UI
  virtual libui::ITerminal* get_terminal(void) const =0;
  virtual IRefObject* get_controller(TypeId ctrl) const =0;
  virtual IMultiImage::P new_font(const SParameters& params, const DCString &tagname = NULL) =0;
  virtual IImage::P new_picture(const SParameters& params, const DCString &tagname = NULL) =0;
  //
  // -- main ones
  virtual void begin(void) =0; // frame begin
  virtual void end(void) =0; // frame end and show
  // -- information
  virtual const SParameters* get_max_values(void) const =0;
  virtual const SParameters* get_frame_values(void) const =0;
  virtual bool set_parameters(BCStr mode, const SParameters& P) =0;
  //
  // -- setup next primitives
  virtual bool setup_font(IMultiImage* font, const DPointf* scale=NULL) = 0;
  virtual void setup_color(const DColorf& c) =0;
  virtual void setup_color(const DColor& c) =0;
  virtual void setup_coloralpha(const DColorAlphaf& c, int transmode=tm_Glow) =0;
  virtual void setup_coloralpha(const DColorAlpha& c, int transmode=tm_Glow) =0;
  virtual void setup_blendcolor(const SBlendColor& c) =0;
  virtual void setup_blendcolor(const SBlendColorf& c) =0;
  virtual void setup_alpha(BColorElem alpha, int transmode=tm_Glow) =0;
  virtual void setup_alpha(BColorElemf alpha, int transmode=tm_Glow) =0;
  // -- render text
  //
  virtual bool render_text_advanced(TextInfo &info, BCStrL piclist, int count) =0;
  //
  // -- render quad/box/tri
  virtual void render_box(const DPoint& a, const DPoint& b) =0;
  virtual void render_tri(const DPoint& a, const DPoint& b, const DPoint& c) =0;
  virtual void render_quad(const DPoint& a, const DPoint& b, const DPoint& c, const DPoint& d) =0;
  // -- render quad/box/tri textured
  virtual void render_box(IImage* image, const DPoint& a, const DPoint& b, const DPointf& ta, const DPointf& tb) =0;
  // -- render line/rect
  virtual void render_line(const DPoint& a, const DPoint& b) =0;
  virtual void render_rect(const DPoint& a, const DPoint& b) =0;
  //
public:
  template<typename T>
  inline T* t_get_controller(void) const {
    IRefObject* rv = get_controller(T::_root_get_interface_type());
    return rv ? rv->t_root_get_other_interface<T>() : NULL;
  }
  //
  inline int get_max_int(BCStr name) { int rv=0; get_max_values()->get(name, rv); return rv; }
  inline int get_frame_int(BCStr name) { int rv=0; get_frame_values()->get(name, rv); return rv; }
  //
  inline bool render_text(const DPoint& a, BCStrL piclist, int count, const DPointf* scale=NULL) { 
    TextInfo ti(a, scale); 
    return render_text_advanced(ti, piclist, count); 
  }
  //
  inline bool render_text_size(DBox &size, const DPoint& a, BCStrL piclist, int count, const DPointf* scale=NULL) { 
    TextInfo info(a, scale); info.set_option(to_DontDraw|to_CalcSize); 
    bool rv = render_text_advanced(info, piclist, count);
    if(rv) size = info.calc_size;
    return rv;
  }
  //
  inline bool render_textbox(const DPoint& a, const DPoint& b, BCStrL piclist, int count, int options=to_None, const DPointf* scale=NULL) {
    TextInfo info(a, b, scale); info.set_option(options|to_FixedBox); 
    return render_text_advanced(info, piclist, count);
  }
  inline bool render_textbox(const DPoint& a, const DPoint& b, BCStrL piclist, int count, int options, const DPointf& scale) {
    TextInfo info(a, b, &scale); info.set_option(options|to_FixedBox); 
    return render_text_advanced(info, piclist, count);
  }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib2d::IRenderDriver, 2, IRefObject);
};

//***************************************
// TYPES
//***************************************

//***************************************
// INLINES/OUTLINES
//***************************************

//***************************************
// END
//***************************************

} // namespace lib2d
} // namespace UModSys

#endif // __UMODSYS_LIB2D_DRIVER_H
