#ifndef __UMODSYS_TL_UTIL_REFPOINTER_H
#define __UMODSYS_TL_UTIL_REFPOINTER_H 1
/*************************************************************/
// file: umodsys/tl/util/ref_pointer.h
// info: template pointer types
/*************************************************************/

#include <umodsys/core/mem/mem_alloc.h>

namespace UModSys {

namespace core {
  struct IRefObject;
}

namespace tl {

//***************************************
// *TYPEDEFS*
//***************************************

template<typename T> struct TRefObject;
template<typename T, typename Ref=core::IRefObject> struct TRefObjectComposite;
template<typename T, typename Extra, typename Ref=core::IRefObject> struct TRefObjectCompositeExtra;

template<typename Self> struct TRefObjectLinks;
template<typename Self, typename P> struct TRefObjectLinksParent;

//***************************************
// *INLINES*
//***************************************

template<typename TRef>
inline void _Ref_remove(TRef* &obj) {
  if(obj) {
    obj->ref_remove();
    obj = NULL;
  }
}

template<typename TRef>
inline void _Ref_add(TRef* obj) {
  if(obj) {
    obj->ref_add();
  }
}

template<typename TRef, typename TRef2>
inline void _Ref_set(TRef* &obj, TRef2* obj2) {
  if(obj2) {
    obj2->ref_add();
  }
  if(obj) {
    obj->ref_remove();
  }
  obj = obj2;
}

template<typename TRef, typename TRef2>
inline void _Ref_init(TRef* &obj, TRef2* obj2) {
  if(obj2) {
    obj2->ref_add();
  }
  obj = obj2;
}

template<typename TRef, typename TRef2>
inline void _Ref_init(TRef* &obj, TRef2* obj2, core::Void* flag) {
  obj = obj2;
}

//***************************************
// TRefObjectLinks::
// TRefObjectLinksParent::
// TRefObjectLinksPModule::
//***************************************

template<typename Self> 
struct TRefObjectLinks {
  mutable int ref_count;
  core::SIMemAlloc mem_allocator;
  //
  inline void ref_add(void) const { ref_count++; }
  inline void ref_remove(Self* p) const;
  inline void obj_delete(Self* p) const;
  inline int  ref_links(void) const { return ref_count; }
  //
  inline TRefObjectLinks(void) : ref_count(0) {}
};

template<typename Self, typename Owner> 
struct TRefObjectLinksParent : public TRefObjectLinks<Self> {
  Owner* owner;
  //
  inline TRefObjectLinksParent(Owner* pv);
};

template<typename Self, typename Owner> 
struct TRefObjectLinksPModule : public TRefObjectLinks<Self> {
  Owner* owner;
  //
  inline TRefObjectLinksPModule(Owner* pv);
};

//***************************************

template<typename Self> 
inline void TRefObjectLinks<Self>::ref_remove(Self* p) const 
{ 
  if(--ref_count==0) p->suicide(); 
}

template<typename Self> 
inline void TRefObjectLinks<Self>::obj_delete(Self* p) const
{
  p->_delete(mem_allocator);
}

template<typename Self, typename Owner> 
inline TRefObjectLinksParent<Self, Owner>::TRefObjectLinksParent(Owner* pv) 
: owner(pv) {
  owner->ref_add();
}

template<typename Self, typename Owner> 
inline TRefObjectLinksPModule<Self, Owner>::TRefObjectLinksPModule(Owner* pv) 
: owner(pv) {
  owner->ref_add();
  owner->mod_inc();
}

//***************************************
// TRefObject::
//***************************************

template<typename T>
struct TRefObject {
  T *object;
  //
  inline ~TRefObject(void) { clear(); }
  inline TRefObject(void) : object(NULL) {}
  inline TRefObject(T* obj) { _Ref_init(object, obj); }
  inline TRefObject(T* obj, core::Void* flag) : object(obj) {}
  inline TRefObject(const TRefObject& R) { _Ref_init(object, R.object); }
  inline const TRefObject& operator=(const TRefObject& R) { set(R.object); return *this; }
  //
  inline const TRefObject& operator=(T* R) { set(R); return *this; }
  inline const TRefObject& operator<<=(T* R) { attach(R); return *this; }
  //
  inline T* operator->(void) const { return object; }
  inline T* operator()(void) const { return object; }
  inline operator T*(void) const { return object; }
  inline bool operator~(void) const { return object!=NULL; }
  inline bool operator!(void) const { return object==NULL; }
  inline bool valid(void) const { return object!=NULL; }
  inline bool invalid(void) const { return object==NULL; }
  inline operator bool(void) const { return object!=NULL; }
  //
  inline void clear(void) { _Ref_remove(object);  }
  inline void set(T *obj) { _Ref_set(object, obj); }
  inline T* detach(void) { T* rv = object; object=NULL; return rv; }
  inline void attach(T* obj) { _Ref_remove(object); object=obj;  }
protected:
};

//***************************************
// TComposite::
//***************************************

template<typename T, typename Ref>
struct TRefObjectComposite {
  TRefObject<Ref> combo;
  T* ip;
  //
  inline ~TRefObjectComposite(void) { clear(); }
  inline TRefObjectComposite(void) : combo(NULL), ip(NULL) {}
  inline TRefObjectComposite(const TRefObjectComposite& R) : combo(R.combo), ip(R.ip) {}
  inline void operator=(const TRefObjectComposite& R) { combo=R.combo; ip=R.ip; }
  //
  inline T* operator->(void) const { return ip; }
  inline T* operator()(void) const { return ip; }
  inline operator T*(void) const { return ip; }
  inline bool operator~(void) const { return ip!=NULL; }
  inline void set(Ref *c, T* i) { combo=c; ip = i; }
  inline void clear(void) { combo.clear(); ip = NULL; }
};

template<typename T, typename Extra, typename Ref>
struct TRefObjectCompositeExtra {
  TRefObject<Ref> combo;
  T* ip;
  Extra extra;
  //
  inline ~TRefObjectCompositeExtra(void) { clear(); }
  inline TRefObjectCompositeExtra(void) : combo(NULL), ip(NULL) {}
  inline TRefObjectCompositeExtra(const TRefObjectCompositeExtra& R) : combo(R.combo), ip(R.ip), extra(R.extra) {}
  inline void operator=(const TRefObjectCompositeExtra& R) { combo=R.combo; ip=R.ip; extra=R.extra; }
  //
  inline T* operator->(void) const { return ip; }
  inline T* operator()(void) const { return ip; }
  inline operator T*(void) const { return ip; }
  inline bool operator~(void) const { return ip!=NULL; }
  inline void set(Ref *c, T* i) { combo=c; ip = i; }
  inline void clear(void) { combo.clear(); ip = NULL; }
};

//***************************************
//
//***************************************

} // namespace tl
} // namespace UModSys


#endif // __UMODSYS_TL_UTIL_REFPOINTER_H
