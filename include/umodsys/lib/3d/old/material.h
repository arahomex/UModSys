#ifndef __UMS_LIB3D_MATERIAL_H
#define __UMS_LIB3D_MATERIAL_H 1
//*****************************************************
// lib3d/maerial.h: common 3d library interfaces, material & texture
//*****************************************************

#include <ums/lib3d/objects.h>

#include <ums/lib2d/lib2d.h>
#include <ums/lib2d/image.h>

#include <ums/core/types/array.h>

namespace UMS {
namespace lib3d {

//***************************************

enum {
  max_MaterialTextures = 256
};

enum eRenderMapFlagsFilter {
  rmff_Disabled,
  rmff_Nearest,
  rmff_Linear,
  rmff_MipNearest,
  rmff_MipLinear,
  rmff_LinearMipNearest,
  rmff_LinearMipLinear,
  rmff_Unknown
};

enum eRenderMapFlagsCombine {
  rmfc_Disabled,
  rmfc_Normal,
  rmfc_Replace,
  rmfc_Modulate,
  rmfc_Blend,
  rmfc_Decal,
  rmfc_Add,
  rmfc_Enlight,
  rmfc_Complex,
  rmfc_Ghost,
  rmfc_Lightmap,
  rmfc_Unknown
};

enum eRenderMapFlagsWrap {
  rmfw_Disabled,
  rmfw_NoWrap,
  rmfw_UWrap,
  rmfw_VWrap,
  rmfw_UVWrap,
  rmfw_Edge,
  rmfw_Unknown
};

struct SRenderMapFlags {
  eRenderMapFlagsFilter filter;   // eRenderMapFlagsFilter
  eRenderMapFlagsCombine combine; // eRenderMapFlagsCombine
  eRenderMapFlagsWrap wrap;       // eRenderMapFlagsWrap
  byte mode;                      // reserved
  //
  SRenderMapFlags(void)
    : filter(rmff_Unknown), combine(rmfc_Unknown), wrap(rmfw_Unknown) {}
  SRenderMapFlags(eRenderMapFlagsFilter f, eRenderMapFlagsCombine c, eRenderMapFlagsWrap w)
    : filter(f), combine(c), wrap(w) {}
  //
  inline void merge(const SRenderMapFlags& R) {
    if(R.filter!=rmff_Unknown) filter = R.filter;
    if(R.combine!=rmfc_Unknown) combine = R.combine;
    if(R.wrap!=rmfw_Unknown) wrap = R.wrap;
    if(R.mode) mode = R.mode;
  }
  inline const SRenderMapFlags& operator<<(SRenderMapFlags& R) { merge(R); return *this; }
  inline const SRenderMapFlags& operator<<=(SRenderMapFlags& R) { merge(R); return *this; }
};

//***************************************

struct SPicture {
  DTexPoint p1, p2;
  DTexPoint t1, t2;
  ITexture *tex;
  //
  inline SPicture(void) : tex(NULL) {}
  inline ~SPicture(void) {}
};

//***************************************

struct SRenderDriverTexBox {
  SIntPoint offset, size;
  DTexPoint A, B;
  //
  inline void set_AB(const SIntPoint &texsize) {
    float ku = 1.f/texsize.v[0], kv = 1.f/texsize.v[1];
    A.set(ku*offset.v[0], kv*offset.v[1]);
    B.set(ku*size.v[0] + A.v[0], kv*size.v[1] + A.v[1]);
  }
};

//***************************************

struct SCellPicture {
  lib2d::eSubImageMode mode;
  DTexPoint t1, t2, size;
};

//***************************************
//***************************************

// single texture

struct ITexture : lib2d::IImage {
  virtual IRenderDriver* get_driver(void) =0;
  virtual bool set_flags(const SRenderMapFlags& flags) =0;
  virtual DTexPoint get_fsize(void) =0;
  //
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::ITexture, IImage)
};

// collection of textures or pictures

struct ITextureCells  : public lib2d::IMultiImage {
  virtual IRenderDriver* get_driver(void) =0;
  virtual bool get_cell_info(int idx, SPicture& pic, DTexPoint* start) =0;
  //
protected:
  UMS_ROOT_P_IMPLEMENT(lib3d::ITextureCells, IMultiImage)
};

//***************************************
//***************************************

struct SMaterialTexture {
  TRefObject<ITexture> texture;
  DTexPoint texsize;
  SIntPoint texisize;
  SRenderMapFlags flags;
  DColorAlpha multiply;
  //
  inline ~SMaterialTexture(void) {}
  //
  inline SMaterialTexture(void)
   : multiply(1,1,1,0), texsize(1)
  {
  }
  inline SMaterialTexture(ITexture* tex)
   : multiply(1,1,1,0), texture(tex)
  {
    if(tex) {
      texsize = tex->get_fsize();
      texisize = tex->get_size();
    }
  }
  inline SMaterialTexture(ITexture* tex, const SRenderMapFlags& rmf)
   : multiply(1,1,1,0), texture(tex), flags(rmf)
  {
    if(tex) {
      texsize = tex->get_fsize();
      texisize = tex->get_size();
    }
  }
  //
  inline void set(ITexture* tex) {
    texture = tex;
    if(tex) {
      texsize = tex->get_fsize();
      texisize = tex->get_size();
    } else {
      texsize.set(1);
      texisize.set(0);
    }
  }
  inline void set(ITexture* tex, const SRenderMapFlags& rmf) {
    texture = tex;
    flags = rmf;
    if(tex) {
      texsize = tex->get_fsize();
      texisize = tex->get_size();
    } else {
      texsize.set(1);
      texisize.set(0);
    }
  }
  //
  UMS_NEW_DELETE()
};

typedef types::TArray<SMaterialTexture> SMaterialTextureArray;

//***************************************

enum eMaterialComponents {
  mc_Ambient  = 0x000001,
  mc_Diffuse  = 0x000002,
  mc_Specular = 0x000004,
  mc_Emit     = 0x000008,
  mc_Colors   = mc_Ambient | mc_Diffuse | mc_Specular,
  mc_AllColors= mc_Ambient | mc_Diffuse | mc_Specular | mc_Emit,
  mc_NoLight  = 0x000800,
  mc_Texture  = 0x001000,
  mc_KDiffuse = 0x000100,
  mc_KSpecular= 0x000200,
  mc_None     = 0x000000
};

struct SMaterial {
protected:
  inline void update_textures(SMaterialTexture *tp, int nt) {
    textures = tp;
    num_textures = nt;
    if(nt==0) components &= ~mc_Texture;
    else components |= mc_Texture;
  }
public:
  int components;
  DColorAlpha ambient, diffuse, specular, emit;
  BScalar Kd, Ks;
  int num_textures;
  SMaterialTexture *textures;
  //
  inline SMaterial(void)
    : components(mc_None),
      ambient(1,1,1,1), diffuse(1,1,1,1),
      specular(1,1,1,1), emit(0,0,0,0),
      Kd(1), Ks(1),
      textures(NULL), num_textures(0)
  {
  }
  //
  inline void set_base(const SMaterial &R) {
    components = R.components &~mc_Texture;
    ambient = R.ambient; diffuse = R.diffuse;
    specular = R.specular; emit = R.emit;
    Kd = R.Kd; Ks = R.Ks;
  }
  //
  inline void set_alpha(const BColorElem& aa, const BColorElem& ad, const BColorElem& as) {
    components |= mc_Colors;
    ambient.set4(aa); diffuse.set4(as); specular.set4(as);
  }
  inline void set_alpha(const BColorElem& all) {
    components |= mc_Colors;
    ambient.set4(all); diffuse.set4(all); specular.set4(all);
  }
  inline void set_color(const DColor& a, const DColor& d, const DColor& s) {
    components |= mc_Colors;
    ambient.setp3(a.v); diffuse.setp3(d.v); specular.setp3(s.v);
  }
  inline void set_color(const DColor& all) {
    components |= mc_Colors;
    ambient.setp3(all.v); diffuse.setp3(all.v); specular.setp3(all.v);
  }
  //
  inline bool set_texture(ITexture* tex, int idx=0) const {
    if(idx<0 || idx>=num_textures)
      return false;
    textures[idx].set(tex);
    return true;
  }
  inline bool set_texture(ITexture* tex, const SRenderMapFlags& rmf, int idx=0) const {
    if(idx<0 || idx>=num_textures)
      return false;
    textures[idx].set(tex, rmf);
    return true;
  }
  //
  inline void set_nolight(void) { components |= mc_NoLight; }
  //
  static bool update_texpoints(DTexPoint *dest, const DTexPoint *src, int count, ITexture* tex);
};

// dynamic texturing material
struct SMaterialDT : public SMaterial {
protected:
    SMaterialTextureArray atextures;
public:
    inline void link(void) { update_textures(atextures.get_all(), ~atextures); }

    inline ~SMaterialDT(void) {}
    inline SMaterialDT(void) {}

    inline SMaterialTexture* add_texture(ITexture* tex) {
      atextures.push();
      SMaterialTexture* rv = &atextures.last();
      rv->set(tex);
      link();
      return rv;
    }
    inline SMaterialTexture* add_texture(ITexture* tex, const SRenderMapFlags& rmf) {
      atextures.push();
      SMaterialTexture* rv = &atextures.last();
      rv->set(tex, rmf);
      link();
      return rv;
    }
    inline bool remove_texture(int idx) {
      if(idx>=0 && idx<~atextures)
        return false;
      link();
      return true;
    }
};

// static texturing material
template<int size>
struct TMaterialST : public SMaterial {
protected:
    SMaterialTexture atextures[size];
public:
    inline void link(void) { update_textures(atextures, size); }

    inline ~TMaterialST(void) {}
    inline TMaterialST(void) { link(); }
};

//***************************************
//***************************************

} // namespace lib3d
} // namespace UMS

#endif
