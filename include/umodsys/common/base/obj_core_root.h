
//***************************************
// IRoot - all virtual classes base
//***************************************

struct IRoot {
  typedef HUniquePointer TypeId;
  typedef SUniquePointerInfo TypeInfo;
  typedef SUniquePtrList DPtrList;
  typedef int DVersionNo;
  //
  virtual ~IRoot(void);
  //
  virtual void suicide(void); // must be overloaded if custom memory allocation is used
  //
  virtual const TypeInfo& root_get_interface_info(void) const =0;
  virtual TypeId root_get_interface_type(void) const =0;
  virtual const IRoot* root_get_other_interface(TypeId type) const =0;
  virtual IRoot* root_get_other_interface(TypeId type) =0;
  virtual size_t root_get_interface_types(DPtrList& list) const =0;
  virtual bool root_is_interface_supported(TypeId type) const =0;
  // INLINES
  template<typename RData>
  inline bool t_root_get_other_interface(RData* &rv, TypeId type) {
    rv = static_cast<RData*>( root_get_other_interface(type) );
    return rv!=NULL;
  }
  template<typename RData>
  inline bool t_root_get_other_interface(RData* &rv) {
    rv = static_cast<RData*>( root_get_other_interface(RData::_root_get_interface_type()) );
    return rv!=NULL;
  }
  template<typename RData>
  inline RData* t_root_get_other_interface(void) {
    RData* rv = static_cast<RData*>( root_get_other_interface(RData::_root_get_interface_type()) );
    return rv;
  }
  template<typename RData>
  inline bool t_root_get_other_interface(const RData* &rv, TypeId type) const {
    rv = static_cast<const RData*>( root_get_other_interface(type) );
    return rv!=NULL;
  }
  template<typename RData>
  inline bool t_root_get_other_interface(const RData* &rv) const {
    rv = static_cast<const RData*>( root_get_other_interface(RData::_root_get_interface_type()) );
    return rv!=NULL;
  }
  template<typename RData>
  inline const RData* t_root_get_other_interface(void) const {
    const RData* rv = static_cast<const RData*>( root_get_other_interface(RData::_root_get_interface_type()) );
    return rv;
  }
  // INLINES
  template<typename RData>
  inline bool t_root_get_other_interface_n(RData* &rv, TypeId type) {
    rv = static_cast<RData*>( root_get_other_interface(type) );
    return rv!=NULL;
  }
  template<typename RData>
  inline bool t_root_get_other_interface_n(RData* &rv) {
    rv = static_cast<RData*>( root_get_other_interface(RData::_get_interface_name()) );
    return rv!=NULL;
  }
  template<typename RData>
  inline bool t_root_get_other_interface_n(const RData* &rv, TypeId type) const {
    rv = static_cast<const RData*>( root_get_other_interface(type) );
    return rv!=NULL;
  }
  template<typename RData>
  inline bool t_root_get_other_interface_n(const RData* &rv) const {
    rv = static_cast<const RData*>( root_get_other_interface(RData::_get_interface_name()) );
    return rv!=NULL;
  }
  template<typename RData>
  inline bool t_root_is_interface_supported(void) const {
    return root_is_interface_supported(RData::_get_interface_name());
  }
protected:
  inline const IRoot* _root_get_other_interface(TypeId type) const { return type==_root_get_interface_type() ? this : NULL; }
  inline IRoot* _root_get_other_interface(TypeId type) { return type==_root_get_interface_type() ? this : NULL; }
public:
  inline static const char* _root_get_interface_infoname(void) { return "UModSys::core::IRoot"; } \
  inline static DVersionNo _root_get_interface_infover(void) { return 2; } \
  inline static TypeId _root_get_interface_type(void) { return tl::TObjectUniqueID<IRoot>::get_id(); }
  inline static const TypeInfo& _root_get_interface_info(void) { return tl::TObjectUniqueID<IRoot>::get_info(); }
  inline static bool _root_is_interface_supported(TypeId type) { return type==_root_get_interface_type(); }
  inline static size_t _root_get_interface_types(DPtrList& list) { list<<_root_get_interface_type(); return 1; }
  inline const IRoot* _get_interface_p(void) const { return this; }
  inline IRoot* _get_interface_p(void) { return this; }
public:
  union UObjectHeader {
    size_t filler;
    IMemAlloc* mem;
  };
  // normal
  inline static void* operator new(size_t size, void *sp) UMODSYS_NOTHROW() { return sp; } 
  inline static void operator delete(void *op, void *sp) UMODSYS_NOTHROW() {} // normal
  // heap-based
  inline static void* operator new(size_t size, const SIMemAlloc& m) UMODSYS_NOTHROW() { return operator new(size, m.imem); }
  inline static void operator delete(void *op, const SIMemAlloc& m) UMODSYS_NOTHROW() { operator delete(op, m.imem); }
  //
  static void* operator new(size_t size, IMemAlloc* m) UMODSYS_NOTHROW();
  static void operator delete(void *op, IMemAlloc* m) UMODSYS_NOTHROW();
  //
  inline static void* operator new(size_t size) UMODSYS_NOTHROW() { return operator new(size, local_memory().imem); } // default
  static void operator delete(void *op) UMODSYS_NOTHROW();
};


//***************************************
// END
//***************************************
