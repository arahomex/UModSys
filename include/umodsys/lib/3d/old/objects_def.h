#ifndef __UMS_LIB3D_OBJECTS_DEFINES_H
#define __UMS_LIB3D_OBJECTS_DEFINES_H 1
//*****************************************************
// lib3d/objects_def.h: defines for object implementation
//*****************************************************

#include <ums/lib3d/lib3d.h>

namespace UMS {
namespace lib3d {

//***************************************
// DEFINES
//***************************************

//*************************************** 
// IMPORT

#define LIB3D_IMPORT_IOBJECT(RObject) \
  typedef RObject::Object Object; \
  typedef IObjectClass IClass; \
  typedef RObject::Class Class;

#define LIB3D_IMPORT_IOBJECT_RENDER(RObject) \
  typedef RObject::IRenderState IRenderState; \
  typedef RObject::RenderState RenderState; \
  typedef RObject::IVisualState IVisualState; \
  typedef RObject::VisualState VisualState;

#define LIB3D_IMPORT_IOBJECT_SHAPE(RObject) \
  typedef RObject::IShapeState IShapeState; \
  typedef RObject::ShapeState ShapeState; 


//*************************************** 
// CLASS

#define LIB3D_DEFINE_IOBJECT_CLASS(RObject, IXObjectParent, _no, _ns, _nr, _nv) \
  typedef RObject Object; \
  struct Class; \
  UMS_REFOBJECT_RIMPLEMENT_DEF(RObject, IXObjectParent, Class) \
  struct Class : public IObjectClass { \
    UMS_MODOBJECT_IMPLEMENT(RObject::Class, IObjectClass) \
    Class(BCStr name=NULL, int no=_no, int ns=_ns, int nr=_nr, int nv=_nv) \
    : IObjectClass(name?name:_get_interface_name(), no, ns, nr, nv) { \
      rc_init(); \
    } \
    ~Class(void) {} \
  };

//*************************************** 
// RENDER STATE

#define LIB3D_DEFINE_IOBJECT_RENDERSTATE_BEGIN(RObject) \
  struct RenderState : public IRenderState { \
    UMS_REFOBJECT_RIMPLEMENT_DEF(RObject::RenderState, IRenderState, Object) \
    RenderState(Object* obj, IObject *robj, IRenderer* r); \
    ~RenderState(void); \
    bool valid(void) const;

#define LIB3D_DEFINE_IOBJECT_RENDERSTATE_END \
  };

//*************************************** 
// VISUAL STATE

#define LIB3D_DEFINE_IOBJECT_VISUALSTATE_BEGIN(RObject) \
  struct VisualState : public IVisualState { \
    UMS_REFOBJECT_RIMPLEMENT_DEF(RObject::VisualState, IVisualState, RenderState) \
    VisualState(RenderState* base); \
    ~VisualState(void); \
    bool render_state(SRenderState& state); \
    bool update(void); \
    bool valid(void) const;

#define LIB3D_DEFINE_IOBJECT_VISUALSTATE_END \
  };

//*************************************** 
// SHAPE STATE

#define LIB3D_DEFINE_IOBJECT_SHAPESTATE_BEGIN(RObject) \
  struct ShapeState : public IShapeState { \
    UMS_REFOBJECT_RIMPLEMENT_DEF(RObject::ShapeState, IShapeState, Object) \
    ShapeState(Object* base); \
    ~ShapeState(void); \
    IObjectCollider* get_collider(void); \
    IObjectBrushOutline* get_outlineindex(int aid); \
    IObjectBrushOutline* get_outline(eObjectBrushOutline id); \
    bool valid(void) const;

#define LIB3D_DEFINE_IOBJECT_SHAPESTATE_END \
  };

//***************************************
//
//***************************************

} // namespace lib3d
} // namespace UMS

#endif
