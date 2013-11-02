#ifndef __UMODSYS_TL_UTIL_REFPOINTER_H
#define __UMODSYS_TL_UTIL_REFPOINTER_H 1
/*************************************************************/
// file: umodsys/tl/util/ref_pointer.h
// info: template pointer types
/*************************************************************/

#include <umodsys/core/mem/mem_alloc.h>
#include <umodsys/core/stdtypedefs.h>

namespace UModSys {

namespace core {
  struct IRefObject;
}

namespace tl {

//***************************************
// *TYPEDEFS*
//***************************************

template<typename T> struct TRefObjectFunc;
template<typename T> struct TModObjectFunc;
template<typename T> struct TRefObjectWeakFunc;

template<typename T, typename RefFunc=TRefObjectFunc<T> > struct TRefObject;

template<typename T, typename Ref=core::IRefObject> struct TRefObjectComposite;
template<typename T, typename Extra, typename Ref=core::IRefObject> struct TRefObjectCompositeExtra;

template<typename Self> struct TRefObjectLinks;
template<typename Self, typename P> struct TRefObjectLinksParent;

//***************************************
// TRefObjectLinks::
// TRefObjectLinksParent::
// TRefObjectLinksModObject::
//***************************************

template<typename Self> 
struct TRefObjectLinks {
  typedef typename Self::WeakPointer Weak;
  //
  mutable int ref_count;
  mutable Weak ref_weak_root;
  //
  inline int  ref_links(void) const { return ref_count; }
  inline void ref_add(Self* p) const;
  void ref_remove(Self* p) const;
  bool ref_weak(Self* p, Weak& wp) const;
  void obj_delete(Self* p) const;
  //
  inline TRefObjectLinks(void) : ref_count(0) {}
};

template<typename Self, typename Owner> 
struct TRefObjectLinksParent : public TRefObjectLinks<Self> {
  mutable Owner* owner;
  //
  void obj_delete(Self* p) const;
  inline Owner* operator*(void) const { return owner; }
  //
  TRefObjectLinksParent(Owner* pv);
};

template<typename Self, typename Owner> 
struct TRefObjectLinksModObject : public TRefObjectLinks<Self> {
  mutable Owner* owner;
  //
  void obj_delete(Self* p) const;
  inline Owner* operator*(void) const { return owner; }
  //
  TRefObjectLinksModObject(Owner* pv);
};

//***************************************
// TRefObjectFunc::
// TRefObjectWeakFunc::
// TModObjectFunc::
//***************************************

template<typename T> 
struct TRefObjectFunc {
  typedef T* Object;
  typedef T* Data;
  //
  inline static void s_init(Data &obj, Object obj2);
  inline static void s_set(Data &obj, Object obj2);
  inline static void s_remove(Data &obj);
  inline static void s_add(Data &obj);
  //
  inline static Object s_get(const Data& obj) { return obj; }
  inline static Data s_detach(Data &obj) { Data rv = obj; obj=NULL; return rv; }
  inline static void s_attach(Data &obj, Object obj2) { s_remove(obj); obj = obj2; }
};

template<typename T> 
struct TModObjectFunc {
  typedef T* Object;
  typedef T* Data;
  //
  inline static void s_init(Data &obj, Object obj2);
  inline static void s_set(Data &obj, Object obj2);
  inline static void s_remove(Data &obj);
  inline static void s_add(Data &obj);
  //
  inline static Object s_get(const Data& obj) { return obj; }
  inline static Data s_detach(Data &obj) { Data rv = obj; obj=NULL; return rv; }
  inline static void s_attach(Data &obj, Object obj2) { s_remove(obj); obj = obj2; }
};

template<typename T> 
struct TRefObjectWeakFunc {
  typedef T* Object;
  typedef typename T::WeakPointer WeakPointer;
  //
  struct Data : public WeakPointer {
    T* get(void) { return WeakPointer::t_get(static_cast<T*>(NULL)); }
  };
  //
  inline static void s_init(Data &obj, Object obj2);
  inline static void s_set(Data &obj, Object obj2);
  inline static void s_init(Data &obj, const Data& obj2);
  inline static void s_set(Data &obj, const Data& obj2);
  inline static void s_remove(Data &obj);
  //
  inline static Object s_get(const Data& obj) { return obj.get(); }
  inline static Object s_detach(Data &obj);
  inline static void s_attach(Data &obj, Object obj2);
};

//***************************************
// TRefObject::
//***************************************

template<typename T, typename RefFunc>
struct TRefObject {
  typedef typename RefFunc::Data Data;
  typedef typename RefFunc::Object Object;
  Data object;
  //
  inline ~TRefObject(void) { clear(); }
  inline TRefObject(void) : object(NULL) {}
  inline TRefObject(Object obj) { RefFunc::s_init(object, obj); }
  inline TRefObject(Object obj, core::Void* flag) : object(obj) {}
  inline TRefObject(const TRefObject& R) { RefFunc::s_init(object, R.object); }
  inline const TRefObject& operator=(const TRefObject& R) { set(R.object); return *this; }
  //
  inline const TRefObject& operator=(T* R) { set(R); return *this; }
  inline const TRefObject& operator<<=(T* R) { attach(R); return *this; }
  //
  inline Object operator->(void) const { return RefFunc::s_get(object); }
  inline Object operator()(void) const { return RefFunc::s_get(object); }
  inline operator Object(void) const { return RefFunc::s_get(object); }
  inline bool operator~(void) const { return RefFunc::s_get(object)!=NULL; }
  inline bool operator!(void) const { return RefFunc::s_get(object)==NULL; }
  inline bool valid(void) const { return RefFunc::s_get(object)!=NULL; }
  inline bool invalid(void) const { return RefFunc::s_get(object)==NULL; }
  inline operator bool(void) const { return RefFunc::s_get(object)!=NULL; }
  //
  inline void clear(void) { RefFunc::s_remove(object);  }
  inline void set(Object obj) { RefFunc::s_set(object, obj); }
  //
  inline Object detach(void) { return RefFunc::s_detach(object); }
  inline void attach(Object obj) { RefFunc::s_remove(object); object=obj;  }
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
// *INLINES*
//***************************************

//***************************************
// TRefObjectFunc::

template<typename TRef>
inline void TRefObjectFunc<TRef>::s_init(Data &obj, Object obj2)
{
  if(obj2) {
    obj2->ref_add();
  }
  obj = obj2;
}

template<typename TRef>
inline void TRefObjectFunc<TRef>::s_set(Data &obj, Object obj2)
{
  if(obj2) {
    obj2->ref_add();
  }
  if(obj) {
    obj->ref_remove();
  }
  obj = obj2;
}

template<typename TRef>
inline void TRefObjectFunc<TRef>::s_remove(Data &obj)
{
  if(obj) {
    obj->ref_remove();
    obj = NULL;
  }
}

template<typename TRef>
inline void TRefObjectFunc<TRef>::s_add(Data &obj)
{
  if(obj) {
    obj->ref_add();
  }
}

//***************************************
// TModObjectFunc::

template<typename TRef>
inline void TModObjectFunc<TRef>::s_init(Data &obj, Object obj2)
{
  if(obj2) {
    obj2->ref_add();
    obj2->mod_inc();
  }
  obj = obj2;
}

template<typename TRef>
inline void TModObjectFunc<TRef>::s_set(Data &obj, Object obj2)
{
  if(obj2) {
    obj2->ref_add();
    obj2->mod_inc();
  }
  if(obj) {
    obj->ref_remove();
    obj->mod_dec();
  }
  obj = obj2;
}

template<typename TRef>
inline void TModObjectFunc<TRef>::s_remove(Data &obj)
{
  if(obj) {
    obj->ref_remove();
    obj->mod_dec();
    obj = NULL;
  }
}

template<typename TRef>
inline void TModObjectFunc<TRef>::s_add(Data &obj)
{
  if(obj) {
    obj->ref_add();
    obj->mod_inc();
  }
}

//***************************************
// TRefObjectWeakFunc::

template<typename TRef>
inline void TRefObjectWeakFunc<TRef>::s_init(Data &obj, Object obj2)
{
  if(obj2) {
    if(obj2->ref_weak(obj))
      obj.fix(obj2);
  }
}

template<typename TRef>
inline void TRefObjectWeakFunc<TRef>::s_set(Data &obj, Object obj2)
{
  obj.remove();
  if(obj2->ref_weak(obj))
    obj.fix(obj2);
}

template<typename TRef>
inline void TRefObjectWeakFunc<TRef>::s_init(Data &obj, const Data& obj2)
{
  s_init(obj, obj2.get());
}

template<typename TRef>
inline void TRefObjectWeakFunc<TRef>::s_set(Data &obj, const Data& obj2)
{
  s_set(obj, obj2.get());
}

template<typename TRef>
inline void TRefObjectWeakFunc<TRef>::s_remove(Data &obj)
{
  obj.remove();
}

template<typename TRef>
inline typename TRefObjectWeakFunc<TRef>::Object TRefObjectWeakFunc<TRef>::s_detach(Data &obj) 
{ 
  Object rv = obj.get(); 
  obj.remove(); 
  return rv; 
}

template<typename TRef>
inline void TRefObjectWeakFunc<TRef>::s_attach(Data &obj, Object obj2) 
{ 
  s_set(obj, obj2); 
}

//***************************************
// TRefObjectLinks::

template<typename Self> 
inline void TRefObjectLinks<Self>::ref_remove(Self* p) const 
{ 
  if(--ref_count==0) {
    p->suicide(); 
  }
}

template<typename Self> 
inline void TRefObjectLinks<Self>::ref_add(Self* p) const 
{ 
  ref_count++; 
}

template<typename Self> 
inline void TRefObjectLinks<Self>::obj_delete(Self* p) const
{
  delete p;
}

template<typename Self> 
inline bool TRefObjectLinks<Self>::ref_weak(Self* p, Weak& wp) const
{
  wp.insert(ref_weak_root, p);
  return true;
}

//***************************************
// TRefObjectLinksParent::

template<typename Self, typename Owner> 
inline TRefObjectLinksParent<Self, Owner>::TRefObjectLinksParent(Owner* pv) 
: owner(NULL) {
  TRefObjectFunc<Owner>::s_init(owner, pv);
}

template<typename Self, typename Owner> 
inline void TRefObjectLinksParent<Self, Owner>::obj_delete(Self* p) const
{
  TRefObject<Owner> pv(owner, core::void_null());
  owner = NULL;
  delete p;
}

//**********************
// TRefObjectLinksModObject::

template<typename Self, typename Owner> 
inline TRefObjectLinksModObject<Self, Owner>::TRefObjectLinksModObject(Owner* pv) 
: owner(NULL) {
  TModObjectFunc<Owner>::s_init(owner, pv);
}

template<typename Self, typename Owner> 
inline void TRefObjectLinksModObject<Self, Owner>::obj_delete(Self* p) const
{
  TRefObject<Owner, TModObjectFunc<Owner> > pv(owner, core::void_null());
  owner = NULL;
  delete p;
}

//***************************************
//
//***************************************

} // namespace tl
} // namespace UModSys


#endif // __UMODSYS_TL_UTIL_REFPOINTER_H
