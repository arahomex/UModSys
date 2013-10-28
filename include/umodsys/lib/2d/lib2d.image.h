#ifndef __UMODSYS_LIB2D_IMAGE_H
#define __UMODSYS_LIB2D_IMAGE_H 1
/*************************************************************/
// file: umodsys/lib/lib2d/lib2d.image.h
// info: 2d images (pictures)
/*************************************************************/

#include <umodsys/lib/2d/lib2d.common.h>

namespace UModSys {
namespace lib2d {

//***************************************
// TYPES
//***************************************

//***************************************
// -- Image common
// SImageCellInfo::  
// SImagePatchInfo::

struct SImageCellInfo {
  eImageFill fill;
  int layer;
  DPoint start, size;
  //
  inline SImageCellInfo(void) {}
  inline SImageCellInfo(eImageFill ifv, int mi, int ax, int ay, int adx, int ady)
    : fill(ifv), layer(mi), start(ax, ay), size(adx, ady) {}
};

struct SImagePatchInfo {
  eImageType type;
  DPoint start, size;
  //
  inline SImagePatchInfo(void) {}
  inline SImagePatchInfo(eImageType at, int adx, int ady, int ax=0, int ay=0)
    : start(ax, ay), type(at), size(adx, ady) {}
  inline SImagePatchInfo(eImageType at, const DPoint& sz, const DPoint& sp)
    : start(sp), type(at), size(sz) {}
  //
  inline bool isframe(int adx, int ady, int ax=0, int ay=0) const { 
    return size(0)==adx && size(1)==ady && start(0)==ax && start(1)==ay; 
  }
  inline bool isinside(int adx, int ady, int ax=0, int ay=0) const { 
    return start(0)+size(0)<=ax+adx && start(1)+size(1)<=ay+ady 
        && start(0)>=ax && start(1)>=ay; 
  }
  inline bool isframe(const SImagePatchInfo& R) const { 
    return size(0)==R.size(0) && size(1)==R.size(1) && start(0)==R.start(0) && start(1)==R.start(1); 
  }
  inline bool isinside(const SImagePatchInfo& R) const { 
    return start(0)+size(0)<=R.start(0)+R.size(0) && start(1)+size(1)<=R.start(1)+R.size(1) 
        && start(0)>=R.start(0) && start(1)>=R.start(1); 
  }
};

//***************************************
// TCellsDef2<T>::  -- Image Multi common

template<typename _Type>
struct TCellsDef2 {
  int xcount, ycount, count;
  _Type cx, cy;
  //
  TCellsDef2(void) {
    cx = cy = 0;
    xcount = ycount = count = 0;
  }
};

//***************************************
// INTERFACES
//***************************************

//***************************************
// IImage::  -- single image

struct IImage : public IRefObject {
  virtual bool set_reallocate(eImageType type, int dx, int dy, BCStr hint=NULL) =0;
  virtual bool set_linear(const SImagePatchInfo& info, const SCMem& linear) =0;
  virtual bool set_indexed(const SImagePatchInfo& info, const SCMem& index, const SCMem& xtable) =0;
  //
  virtual DPoint get_size(void) =0;
  virtual void* get_hwinfo(const void* hint) =0;
  //
  template<typename HWInfo>
  inline HWInfo* tget_hwinfo(const void* hint) { return static_cast<HWInfo*>(get_hwinfo(hint)); }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib2d::IImage, 2, IRefObject);
};

//***************************************
// IImage::  -- Image List

struct IMultiImage : public IRefObject {
  virtual IImage* get_layer(int idx=0) =0;
  virtual bool set_layer_count(int count) =0;
  virtual bool set_fixed_cell(int nx, int ny) =0;
  virtual bool set_var_cell(const SImageCellInfo* cells, int count, int base=0) =0;
  virtual bool set_hint(BCStr hint, BCStr value) =0;
  virtual bool get_cell_size(int idx, DPoint &size, DPoint* ofs=NULL) =0;
  virtual bool get_max_cell_size(DPoint &size) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib2d::IMultiImage, 2, IRefObject);
};

//***************************************
// INLINES/OUTLINES
//***************************************

//***************************************
// END
//***************************************

} // namespace lib2d
} // namespace UModSys

#endif // __UMODSYS_LIB2D_IMAGE_H
