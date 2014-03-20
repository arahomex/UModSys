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
// IObject::

struct IObject : public IRefObject {
public:
  // state generation functions
  virtual bool valid(void) const =0;
  virtual PRenderObject render_new(IRenderer* r) =0; // generate static data
  virtual IObject* get_root(void) =0; // return root render object for states
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IObject, 2, IRefObject);
};

//***************************************
// IRenderObject::

struct IRenderObject : public IRefObject {
  PRenderer renderer;
  int seqno, vlinks;
  //
  virtual bool valid(void) const =0;
  virtual PObject object_get(void) const =0; // get object
  virtual PNodeObject node_new(void) =0; // generate dynamic data
  //
  inline bool is_unused(void) const { return vlinks==0; }
  inline void use(void) { vlinks++; }
  inline void unuse(void) { vlinks--; }
  //
  IRenderObject(void);
  ~IRenderObject(void);
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::IRenderObject, 2, IRefObject);
};

//***************************************
// IObjectVisualState::

struct INodeObject : public IRefObject {
  int seqno;
  //
  virtual bool valid(void) const =0;
  virtual PObject object_get(void) const =0; // get object
  virtual PRenderObject render_get(void) const =0; // get object
  virtual bool update(void) =0; // re-initalize links
  // depending of type functionality
  virtual bool render_state(SRenderState& state) =0;
  //
  INodeObject(void);
  ~INodeObject(void);
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::lib3d::INodeObject, 2, IRefObject);
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
// INLINES/EXTERNALS
//***************************************

extern sint8 cube_faceshift[6][4]; // [faces]{x,y,z,_}
extern uint8 cube_quad_texpoints[6][4][2]; // [faces][verts]{u,v}
extern uint8 cube_quad_points[6][4][4]; // [faces][verts]{x,y,z,_}

//***************************************
// END
//***************************************

} // namespace lib3d
} // namespace UModSys

#endif // __UMODSYS_LIB3D_COMMON_H
