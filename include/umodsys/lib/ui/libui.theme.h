#ifndef __UMODSYS_LIBUI_THEME_H
#define __UMODSYS_LIBUI_THEME_H 1
/*************************************************************/
// file: umodsys/lib/libui/libui.theme.h
// info: ui frames theme interface
/*************************************************************/

#include <umodsys/lib/ui/libui.frames.h>
#include <umodsys/tl/container/scatter_array.h>

namespace UModSys {
namespace libui {

//***************************************
// TYPES
//***************************************

struct ICollectorThemeTemplate : public ICollectorTheme {
protected:
  ICollectorThemeTree *parent;
  //
  tl::TScatterArray<lib2d::DColorAlphaf> colors;
  tl::TScatterArray<lib2d::DPointf> points;
  tl::TScatterArray<int> flags;
  //
  template<typename Arr, typename Elem>
  inline bool t_get(const Arr& arr, int idx1, int idx2, Elem& elem) const {
    int index = ((idx1 & 0xffff)<<16) | (idx2&0xffff);
    typedef const typename Arr::Value* DataCP;
    DataCP d = arr(index);
    if(d==NULL) {
      ICollectorTheme *up = parent->get_up_theme();
      return up ? up->get_property(idx1, idx2, elem) : false;
    }
    cast(elem, *d);
    return true;
  }
  //
  template<typename Arr, typename Elem>
  inline bool t_set(Arr& arr, int idx1, int idx2, const Elem& elem) {
    int index = ((idx1 & 0xffff)<<16) | (idx2&0xffff);
    typename Arr::Value *d = arr(index, core::void_null());
    if(d==NULL)
      return false;
    cast(*d, elem);
    return true;
  }
  //
  inline static void cast(lib2d::DColorAlphaf& A, const lib2d::DColorAlphaf& B) { A = B; }
  inline static void cast(lib2d::DColorAlpha& A, const lib2d::DColorAlphaf& B) { A = lib2d::c2c(B); }
  inline static void cast(lib2d::DColorAlphaf& A, const lib2d::DColorAlpha& B) { A = lib2d::c2c(B); }
  //
  inline static void cast(lib2d::DPointf& A, const lib2d::DPointf& B) { A = B; }
  inline static void cast(lib2d::DPoint& A, const lib2d::DPointf& B) { A = lib2d::p2p(B); }
  inline static void cast(lib2d::DPointf& A, const lib2d::DPoint& B) { A = lib2d::p2p(B); }
  //
  inline static void cast(int& A, int B) { A = B; }
public:
  //
  virtual int get_group_index(BCStr name) =0;
  virtual int get_elem_index(BCStr name) =0;
  //
  // get propeties
  //
  // color properties
  inline bool get_property(int group_idx, int elem_idx, lib2d::DColorAlphaf& elem) const
    { return t_get(colors, group_idx, elem_idx, elem); }
  inline bool get_property(int group_idx, int elem_idx, lib2d::DColorAlpha& elem) const
    { return t_get(colors, group_idx, elem_idx, elem); }
  // size property
  inline bool get_property(int group_idx, int elem_idx, lib2d::DPointf& elem) const 
    { return t_get(points, group_idx, elem_idx, elem); }
  inline bool get_property(int group_idx, int elem_idx, lib2d::DPoint& elem) const 
    { return t_get(points, group_idx, elem_idx, elem); }
  // flags property
  inline bool get_property(int group_idx, int elem_idx, int& elem) const 
    { return t_get(flags, group_idx, elem_idx, elem); }
  //
  // set propeties
  //
  // color properties
  inline bool set_property(int group_idx, int elem_idx, const lib2d::DColorAlphaf& elem) 
    { return t_set(colors, group_idx, elem_idx, elem); }
  inline bool set_property(int group_idx, int elem_idx, const lib2d::DColorAlpha& elem) 
    { return t_set(colors, group_idx, elem_idx, elem); }
  // size property
  inline bool set_property(int group_idx, int elem_idx, const lib2d::DPointf& elem) 
    { return t_set(points, group_idx, elem_idx, elem); }
  inline bool set_property(int group_idx, int elem_idx, const lib2d::DPoint& elem) 
    { return t_set(points, group_idx, elem_idx, elem); }
  // flags property
  inline bool set_property(int group_idx, int elem_idx, int elem) 
    { return t_set(flags, group_idx, elem_idx, elem); }
  //
  inline ICollectorThemeTemplate(ICollectorThemeTree *pv) : parent(pv) {
  }
  //
  inline ~ICollectorThemeTemplate(void) {
  }
};

//***************************************
// INLINES/OUTLINES
//***************************************

//***************************************
// END
//***************************************

} // namespace libui
} // namespace UModSys

#endif // __UMODSYS_LIBUI_THEME_H
