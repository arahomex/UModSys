#ifndef __UMS_LIB3D_LIGHT_EMULATION_H
#define __UMS_LIB3D_LIGHT_EMULATION_H 1
//*****************************************************
// lib3d/emul_light.h: common 3d library interfaces, light emulation
//*****************************************************

#include <ums/lib3d/objects.h>
#include <ums/lib3d/light.h>
#include <ums/core/types/array.h>

namespace UMS {
namespace lib3d {
namespace emulate_light {

//***************************************

struct SStateGeneral {
  int hwid, swid;
  bool enabled, special, r1, r2;
  DMatrix4 T;
  //
  inline SStateGeneral(void)
  : enabled(0), special(0), swid(-1), hwid(-1) {
  }
};

struct SHardwareInfo {
  eLightType type;
  SStateGeneral* state;
  SLight* light;
  int local_idx;
  int ui1, ui2, ui3, ui4;
  void *up1, *up2, *up3, *up4;
  //
  inline SHardwareInfo(void)
    : type(lt_Unknown), state(NULL), light(NULL), local_idx(-1)
  {
  }
  //
  inline void clear(void) {
    type = lt_Unknown; state = NULL; light = NULL; local_idx = -1;
  }
};

template<typename Light>
struct TState : SStateGeneral {
  Light light;

  inline TState(void) {}
  //
  inline void set(const SLight& L, const DMatrix4& TT, int sw, int hw, SHardwareInfo* inf) {
    T = TT;
    set(L, sw, hw, inf);
  }
  inline void set(const SLight& L, int sw, int hw, SHardwareInfo* info) {
    if(info) {
      info->light = &light;
      info->state = this;
    }
    enabled = true;
    light = *static_cast<const Light*>(&L);
    hwid = hw;
    swid = sw;
  }
};

typedef TState<SLight>         SStateOmni;
typedef TState<SLightCone>     SStateCone;
typedef TState<SLightSphere>   SStateSphere;
typedef TState<SLightCylinder> SStateCylinder;

enum eBases {
  eb_Omni       = 0x10000,
  eb_Omni_Last  = 0x1FFFF,
  eb_HW         = 0x20000,
  eb_HW_Last    = 0x2FFFF,
  eb_SW         = 0x30000,
  eb_SW_Last    = 0x3FFFF,
  eb_None       = 0
};

struct ISystemHardware {
  virtual bool els_clear(void) =0;
  virtual bool els_init_info(SHardwareInfo* info, int id) =0;
  virtual bool els_set_omni(const DColor& color) =0;
  virtual bool els_set_omni(SHardwareInfo* info, const SLight& L) =0;
  virtual bool els_set_cone(SHardwareInfo* info, const SLightCone& L, const DMatrix4& T) =0;
  virtual bool els_set_cylinder(SHardwareInfo* info, const SLightCylinder& L, const DMatrix4& T) =0;
  virtual bool els_set_sphere(SHardwareInfo* info, const SLightSphere& L, const DMatrix4& T) =0;
};

struct SSystem {
  ISystemHardware *hw;
  types::TArray< SStateOmni > omnis;
  types::TArray< SStateCone > cones;
  types::TArray< SStateSphere > spheres;
  types::TArray< SStateCylinder > cylinders;
  //
  types::TArray< SHardwareInfo > hwinfos;
  types::TArray< SHardwareInfo > swinfos;
  //
  bool omni_changed;
  DColor omni;
  //
  SSystem(void);
  ~SSystem(void);
  //
  bool configure(ISystemHardware *hw, int nomni, int nhw, int nsw);
  bool prepare(void);
  bool get_omni_color(DColor& color);
  bool get_point_color(DColor& color, const DPoint& point, const DNormal& normal);
  //
  bool light_clear(eLightType type, bool emulated, bool hardware);
  int  light_add(eLightType type, const SLight& light, const DMatrix4& T, bool hw);
  bool light_set(int idx, eLightType type, const SLight& light, const DMatrix4& T);
  bool light_enable(int idx, bool enabled);
  bool light_setspecial(int idx, bool enabled);
};

//***************************************

} // namespace emulate_light
} // namespace lib3d
} // namespace UMS

#endif
