#ifndef __UMODSYS_LIB3D_COMMON_H
#define __UMODSYS_LIB3D_COMMON_H 1
/*************************************************************/
// file: umodsys/lib/lib3d/lib3d.common.h
// info: scene
/*************************************************************/

#include <umodsys/lib/3d/lib3d.typedefs.h>

namespace UModSys {
namespace lib3d {

//***************************************
// TYPES
//***************************************

//***************************************
// IGeneralObject::

struct IGeneralObject : public IRefObject {
public:
  // state generation functions
  virtual bool valid(void) const =0;
  virtual PVisualObject visual_new(IRenderer* r, IRenderObject* ro) =0; // generate visual dynamic data
  virtual PRenderObject render_new(IRenderer* r) =0; // generate visual static data
  virtual PLogicObject logic_new(void) =0; // generate logical (scene) data
//  virtual PPhysicsObject physics_new(void) =0; // generate physics data
  virtual IGeneralObject* get_root(void) =0; // return root render object for states
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IGeneralObject, 2, IRefObject);
};

//***************************************
// ILogicObject::

struct ILogicObject : public IRefObject {
  virtual bool valid(void) const =0;
  virtual PGeneralObject object_get(void) const =0; // get object
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::ILogicObject, 2, IRefObject);
};

//***************************************
// IRenderObject::

struct IRenderObject : public IRefObject {
  PRenderer renderer;
  int seqno, vlinks;
  //
  virtual bool valid(void) const =0;
  virtual PGeneralObject object_get(void) const =0; // get object
  virtual PVisualObject visual_new(void) =0; // generate dynamic data
  //
  inline bool is_unused(void) const { return vlinks==0; }
  inline void use(void) { vlinks++; }
  inline void unuse(void) { vlinks--; }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IRenderObject, 2, IRefObject);
};

//***************************************
// IVisualObject::

struct IVisualObject : public IRefObject {
  int seqno;
  PLogicObject logics;
  //
  virtual bool valid(void) const =0;
  virtual PGeneralObject object_get(void) const =0; // get object
  virtual PRenderObject render_get(void) const =0; // get object
  virtual bool visual_update(void) =0; // re-initalize links
  virtual bool logic_sync(void) =0; // update data
  // depending of type functionality
  virtual bool render_state(SRenderState& state) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IVisualObject, 2, IRefObject);
};

//***************************************
// IObjectBrushOutline::

struct IObjectBrushOutline {
  virtual eObjectBrushOutline get_type(void) =0; // eObjectBrushOutline
  //
  template<typename OutlineEx>
  inline bool t_as(OutlineEx* &outline) {
    if(get_type()!=OutlineEx::type)
      return NULL;
    outline = static_cast<OutlineEx*>(this);
    return true;
  }
  template<typename OutlineEx>
  inline bool t_asz(OutlineEx* &outline) {
    if(this==NULL)
      return NULL;
    if(get_type()!=OutlineEx::type)
      return NULL;
    outline = static_cast<OutlineEx*>(this);
    return true;
  }
};

//***************************************
// END
//***************************************

} // namespace lib3d
} // namespace UModSys

#endif // __UMODSYS_LIB3D_COMMON_H
