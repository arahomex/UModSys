#ifndef __UMODSYS_LIB3D_GEOM_H
#define __UMODSYS_LIB3D_GEOM_H 1
/*************************************************************/
// file: umodsys/lib/lib3d/lib3d.geom.h
// info: geometry
/*************************************************************/

#include <umodsys/lib/3d/lib3d.typedefs.h>

namespace UModSys {
namespace lib3d {

//***************************************
// FORWARD/TYPEDEF
//***************************************

struct SVertexElemInfo;
template<typename T> struct TVertexAType;

//***************************************
// TEMPLATES
//***************************************

template<typename T> 
struct TVertexAType {
  inline static eVertexAType type_of() { return vaet_None; }
  inline static int count_of() { return 1; }
};

//***************************************
//***************************************

template<> 
struct TVertexAType<sint32> {
  inline static eVertexAType type_of() { return vaet_Int32; }
  inline static int count_of() { return 1; }
};

template<> 
struct TVertexAType<sint16> {
  inline static eVertexAType type_of() { return vaet_Int16; }
  inline static int count_of() { return 1; }
};

template<> 
struct TVertexAType<sint8> {
  inline static eVertexAType type_of() { return vaet_Int8; }
  inline static int count_of() { return 1; }
};
template<> 
struct TVertexAType<uint8> {
  inline static eVertexAType type_of() { return vaet_UInt8; }
  inline static int count_of() { return 1; }
};

template<> 
struct TVertexAType<float32> {
  inline static eVertexAType type_of() { return vaet_Float32; }
  inline static int count_of() { return 1; }
};

template<> 
struct TVertexAType<float64> {
  inline static eVertexAType type_of() { return vaet_Float64; }
  inline static int count_of() { return 1; }
};

//***************************************

template<typename TT> 
struct TVertexAType< math3d::TVector2<TT> > {
  inline static eVertexAType type_of() { return TVertexAType<TT>::type_of(); }
  inline static int count_of() { return 2; }
};

template<typename TT> 
struct TVertexAType< math3d::TVector3<TT> > {
  inline static eVertexAType type_of() { return TVertexAType<TT>::type_of(); }
  inline static int count_of() { return 3; }
};

template<typename TT> 
struct TVertexAType< math3d::TVector4<TT> > {
  inline static eVertexAType type_of() { return TVertexAType<TT>::type_of(); }
  inline static int count_of() { return 4; }
};

//***************************************
// TYPES
//***************************************

struct SVertexElemInfo {
  eVertexClass vclass;
  eVertexAType aitype;
  uint8 acount, inf;
  //
  inline SVertexElemInfo(eVertexClass vc, eVertexAType ait, uint8 ac=1, uint8 ii=0) 
  : vclass(vc), aitype(ait), acount(ac), inf(ii) {
  }
  inline SVertexElemInfo(void) 
  : vclass(vc_Unknown), aitype(vaet_None), acount(0), inf(0) {
  }
};


//***************************************
// INTERFACES
//***************************************

//***************************************
// IRenderDriver::

struct IVertexArray : public IRefObject {
  virtual bool is_valid(void) const=0;
  virtual int get_array_count(void) const=0;
  virtual int get_layer_count(void) const=0;
  virtual SVertexElemInfo get_layer_info(int layid) const=0;
  //
  virtual bool get_layer_vdata(eVertexAType type, void* buf, int start, int ecount, int lay) const=0;
  virtual bool set_layer_vdata(eVertexAType type, const void* buf, int start, int ecount, int lay) =0;
  //
public:
  template<typename T> inline bool get_layer_data(T* buf, int start, int ecount, int lay) const {
    return get_layer_vdata(TVertexAType<T>::type_of(), buf, start, ecount, lay);
  }
  template<typename T> inline bool set_layer_data(T* buf, int start, int ecount, int lay) {
    return set_layer_vdata(TVertexAType<T>::type_of(), buf, start, ecount, lay);
  }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IVertexArray, 2, IRefObject);
};

//***************************************
// END
//***************************************

} // namespace lib3d
} // namespace UModSys

#endif // __UMODSYS_LIB3D_GEOM_H
