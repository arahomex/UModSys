//***************************************
// IRefObject - all reference classes base
//***************************************

struct IRefObject : public IRoot {
public:
  struct WeakPointer {
    IRefObject* obj;
    WeakPointer *next, *prev;
    //
    inline WeakPointer(void) : obj(NULL), next(NULL), prev(NULL) {}
    //
    inline void root_mode(void) { next = prev = this; }
    inline void fix(IRefObject* p) { obj=p; }
    inline void insert(WeakPointer& r, IRefObject* p) { insert(r); fix(p); }
    //
    template<typename X> inline X* t_obj(void) const { return static_cast<X*>(obj); }
    template<typename X> inline X* t_obj(X* hint) const { return static_cast<X*>(obj); }
    //
    inline void insert(WeakPointer& r) { 
      WeakPointer &fw = *r.next;
      next = &fw; prev = &r;
      fw.prev = this; r.next = this;
    }
    inline void remove(void) { 
      if(next && prev) { 
        next->prev = prev; 
        prev->next = next; 
        next = prev = NULL; 
      } 
      obj=NULL; 
    }
  };
public:
  ~IRefObject(void);
  virtual void ref_add(void) const =0;
  virtual void ref_remove(void) const =0;
  virtual int  ref_links(void) const =0;
  virtual bool ref_weak(WeakPointer& wp) const =0;
public:
  template<typename RData, typename RefFunc>
  inline bool t_ref_get_other_interface(tl::TRefObject<RData, RefFunc> &rv) {
    rv = static_cast<RData*>( root_get_other_interface(RData::_root_get_interface_type()) );
    return rv.valid();
  }
  template<typename RData, typename RefFunc>
  inline bool t_ref_get_other_interface(tl::TRefObject<const RData, RefFunc> &rv) const {
    rv = static_cast<const RData*>( root_get_other_interface(RData::_root_get_interface_type()) );
    return rv.valid();
  }
  template<typename RData, typename RefFunc>
  inline bool t_ref_get_other_interface_n(tl::TRefObject<RData, RefFunc> &rv) {
    rv = static_cast<RData*>( root_get_other_interface(RData::_get_interface_name()) );
    return rv.valid();
  }
  template<typename RData, typename RefFunc>
  inline bool t_ref_get_other_interface_n(tl::TRefObject<const RData, RefFunc> &rv) const {
    rv = static_cast<const RData*>( root_get_other_interface(RData::_get_interface_name()) );
    return rv.valid();
  }
public:
  //
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::core::IRefObject, 2, IRoot);
};

//***************************************
//***************************************

template<typename Obj, typename XObj>
bool ValidateConstruction(tl::TRefObject<Obj> &rv, tl::TRefObject<XObj> pobj) 
{
  if(!pobj.valid())
    return false;
  if(!pobj->validate_construction()) {
    return false;
  }
  rv = pobj();
  return true;
}

template<typename Obj, typename XObj>
bool ValidateConstruction(tl::TRefObject<Obj> &rv, XObj* pobj) 
{
  if(pobj == NULL)
    return false;
  tl::TRefObject<XObj> obj = pobj;
  if(!pobj->validate_construction()) {
    return false;
  }
  rv = pobj;
  return true;
}

//***************************************
// END
//***************************************
