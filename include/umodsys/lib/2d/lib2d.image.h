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
  inline SImageCellInfo(void) 
    {}
  inline SImageCellInfo(eImageFill ifv, int mi, int ax, int ay, int adx, int ady)
    : fill(ifv), layer(mi), start(ax, ay), size(adx, ady) {}
};

struct SImageInfo {
  eImageType type;
  DPoint size;
  //
  inline SImageInfo(void)
    : size(0,0), type(it_Undefined) {}
  inline SImageInfo(eImageType at, int adx, int ady)
    : size(adx, ady), type(at) {}
  inline SImageInfo(eImageType at, const DPoint& sz)
    : size(sz), type(at) {}
  //
  inline void set(eImageType at, const DPoint& sz)
    { type = at; size = sz; }
  inline void set(eImageType at, int dx, int dy)
    { type = at; size.set(dx, dy); }
  //
  inline DBox zerobox(void) const 
    { return DBox(0, 0, size(0), size(1)); }
  inline int width(void) const 
    { return size(0); }
  inline int height(void) const 
    { return size(1); }
  inline size_t pixelsize(int plane=0) const 
    { return GetPixelSize(type, plane); }
  inline size_t getbinsize(int plane=0) const 
    { return size(0)*size(1)*GetPixelSize(type, plane); }
};

struct SImagePatchInfo : public SImageInfo {
  DPoint start;
  //
  inline SImagePatchInfo(void) 
    : start(0) {}
  inline SImagePatchInfo(eImageType at, int adx, int ady, int ax=0, int ay=0)
    : SImageInfo(at, adx, ady), start(ax, ay) {}
  inline SImagePatchInfo(eImageType at, const DPoint& sz, const DPoint& sp=DPoint(0,0))
    : SImageInfo(at, sz), start(sp) {}
  inline SImagePatchInfo(const SImageInfo& inf, const DPoint& sp=DPoint(0,0))
    : SImageInfo(inf), start(sp) {}
  //
  inline void set(eImageType at, const DPoint& sz, const DPoint& sp=DPoint(0,0))
    { SImageInfo::set(at, sz); start = sp; }
  inline void set(eImageType at, int dx, int dy, int x=0, int y=0)
    { SImageInfo::set(at, dx, dy); start.set(x,y); }
  inline void set(const SImageInfo& inf, const DPoint& sp)
    { SImageInfo::operator=(inf); start = sp; }
  inline void set(const SImageInfo& inf, int x, int y)
    { SImageInfo::operator=(inf); start.set(x,y); }
  //
  inline bool isframe(int adx, int ady, int ax=0, int ay=0) const 
    { return size(0)==adx && size(1)==ady && start(0)==ax && start(1)==ay; }
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
  inline DBox box(void) const 
    { return DBox(start(0), start(1), size(0), size(1)); }
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
  typedef SImagePatchInfo DPatchInfo;
  typedef SImagePatchInfo DInfo;
public:
  virtual bool set_info(const DInfo& inf, BCStr hint=NULL) =0;
  virtual bool set_data(const DPatchInfo& inf, const SCMem& bin, int planeno=0) =0;
  //
  virtual const SImageInfo& get_info(void) const =0;
  virtual bool get_data(const DPatchInfo& inf, const SMem& bin, int planeno=0) const =0;
  //
  virtual void* get_hwinfo(HUniquePointer hint) =0;
  virtual const void* get_hwcinfo(HUniquePointer hint) const =0;
  //
  template<typename HWInfo> inline HWInfo* tget_hwinfo(const void* hint) 
    { return static_cast<HWInfo*>(get_hwinfo(hint)); }
  template<typename HWInfo> inline const HWInfo* tget_hwcinfo(const void* hint) const 
    { return static_cast<HWInfo*>(get_hwcinfo(hint)); }
  inline bool set_info(eImageType t, int dx, int dy, BCStr hint=NULL)
    { return set_info(DInfo(t, dx, dy), hint); }
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

inline size_t GetPixelSize(eImageType type, int plane) 
{
  return (plane<0 || plane>4) ? 0 : (type>>(plane<<2)) & it_Mask_Plane1;
}


//***************************************
// END
//***************************************

} // namespace lib2d
} // namespace UModSys

#endif // __UMODSYS_LIB2D_IMAGE_H
