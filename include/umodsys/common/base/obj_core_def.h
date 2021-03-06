
//***************************************
// DEFINES
//***************************************

#define UMODSYS_ROOT_INTIMPLEMENT_XNAME(_type, _verno, _interface) \
  protected: \
    const IRoot* _root_get_other_interface(TypeId type) const { return type==_root_get_interface_type() ? this : _interface::_root_get_other_interface(type); } \
    IRoot* _root_get_other_interface(TypeId type) { return type==_root_get_interface_type() ? this : _interface::_root_get_other_interface(type); } \
  public: \
    inline static DVersionNo _root_get_interface_infover(void) { return _verno; } \
    inline static const TypeInfo& _root_get_interface_info(void) { return tl::TObjectUniqueID< _type >::get_info(); } \
    inline static TypeId _root_get_interface_type(void) { return tl::TObjectUniqueID< _type >::get_id(); } \
    inline static size_t _root_get_interface_types(DPtrList& list) { list<<_root_get_interface_type(); return _interface::_root_get_interface_types(list)+1; } \
    inline static bool _root_is_interface_supported(TypeId type) { return type==_root_get_interface_type() || _interface::_root_is_interface_supported(type); } \
    inline const IRoot* _get_interface_p(void) const { return this; } \
    inline IRoot* _get_interface_p(void) { return this; } \
    inline static TypeId _get_interface_basetype(void) { return _root_get_interface_type(); } \
    typedef _type DInterface; \
  protected: \

#define UMODSYS_ROOT_INTIMPLEMENT_NONAME(_type, _verno, _interface) \
  UMODSYS_ROOT_INTIMPLEMENT_XNAME(_type, _verno, _interface) \
  public: \
    static const char* _root_get_interface_infoname(void); \
  protected: \

#define UMODSYS_ROOT_INTIMPLEMENT(_type, _verno, _interface) \
  UMODSYS_ROOT_INTIMPLEMENT_XNAME(_type, _verno, _interface) \
  public: \
    inline static const char* _root_get_interface_infoname(void) { return #_type; } \
  protected: \

#define UMODSYS_ROOT_IMPLEMENT1(_type, _verno, _interface) \
  typedef _interface DParent; \
  typedef _type Self; \
  inline static const char* _root_get_interface_infoname(void) { return #_type; } \
  inline static DVersionNo _root_get_interface_infover(void) { return _verno; } \
  inline static const TypeInfo& _root_get_interface_info(void) { return tl::TObjectUniqueID<_type>::get_info(); } \
  inline static TypeId _root_get_interface_type(void) { return tl::TObjectUniqueID<_type>::get_id(); } \
  inline static size_t _root_get_interface_types(DPtrList& list) { list<<_root_get_interface_type(); return _interface::_root_get_interface_types(list)+1; } \
  inline static bool _root_is_interface_supported(TypeId type) { return type==_root_get_interface_type() || _interface::_root_is_interface_supported(type); } \
  \
  inline const TypeInfo& root_get_interface_info(void) const { return _root_get_interface_info(); } \
  inline TypeId root_get_interface_type(void) const { return _root_get_interface_type(); } \
  inline size_t root_get_interface_types(DPtrList& list) const { return root_get_interface_types(list); } \
  inline const IRoot* root_get_other_interface(TypeId type) const { return type==_root_get_interface_type() ? _interface::_get_interface_p() : _interface::_root_get_other_interface(type); } \
  inline IRoot* root_get_other_interface(TypeId type) { return type==_root_get_interface_type() ? _interface::_get_interface_p() : _interface::_root_get_other_interface(type); } \
  inline bool root_is_interface_supported(TypeId type) const { return _root_is_interface_supported(type); } \

#define UMODSYS_ROOT_IMPLEMENT2(_type, _verno, _interface1, _interface2) \
  typedef _interface1 DParent1; \
  typedef _interface2 DParent2; \
  typedef _type Self; \
  static SUniquePointer s_interface_type; \
  inline static const char* _root_get_interface_infoname(void) { return #_type; } \
  inline static DVersionNo _root_get_interface_infover(void) { return _verno; } \
  inline static const TypeInfo& _root_get_interface_info(void) { return tl::TObjectUniqueID<_type>::get_info(); } \
  inline static TypeId _root_get_interface_type(void) { return tl::TObjectUniqueID<_type>::get_id(); } \
  inline static size_t _root_get_interface_types(DPtrList& list) { \
    list<<_root_get_interface_type(); \
    return _interface1::_root_get_interface_types(list)+_interface2::_root_get_interface_types(list)+1; \
  } \
  inline static bool _root_is_interface_supported(TypeId type) { \
    return type==_root_get_interface_type() || _interface1::_root_is_interface_supported(type) || _interface2::_root_is_interface_supported(type); \
  } \
  \
  inline const TypeInfo& root_get_interface_info(void) const { return _root_get_interface_info(); } \
  inline TypeId root_get_interface_type(void) const { return _root_get_interface_type(); } \
  inline size_t root_get_interface_types(DPtrList& list) const { return _root_get_interface_types(list); } \
  inline const IRoot* root_get_other_interface(TypeId type) const { \
    if(type==_root_get_interface_type()) return _interface1::_get_interface_p(); \
    const IRoot* rv = _interface1::_root_get_other_interface(type); if(rv) return rv; \
    return _interface2::_root_get_other_interface(type); \
  } \
  inline IRoot* root_get_other_interface(TypeId type) { \
    if(type==_root_get_interface_type()) return _interface1::_get_interface_p(); \
    IRoot* rv = _interface1::_root_get_other_interface(type); if(rv) return rv; \
    return _interface2::_root_get_other_interface(type); \
  } \
  inline bool root_is_interface_supported(TypeId type) const { return _root_is_interface_supported(type); } \

//*************************************** ANY REFOBJECT
//***************************************

#define UMODSYS_REFOBJECT_INTIMPLEMENT(_type, _verno, _interface) \
  UMODSYS_ROOT_INTIMPLEMENT(_type, _verno, _interface) \
  public: \
    typedef tl::TRefObject<_type> P, DInterfaceP; \
  protected: \

#define UMODSYS_REFOBJECT_IMPLEMENT1(_type, _verno, _interface) \
  UMODSYS_ROOT_IMPLEMENT1(_type, _verno, _interface) \
  typedef tl::TRefObject<_type> DSelfP, SelfP; \

#define UMODSYS_REFOBJECT_IMPLEMENT2(_type, _verno, _interface1, _interface2) \
  UMODSYS_ROOT_IMPLEMENT2(_type, _interface1, _interface2) \
  typedef tl::TRefObject<_type> DSelfP, SelfP; \

#define UMODSYS_REFOBJECT_UNIIMPLEMENT0() \
  inline void ref_add(void) const { refs.ref_add( const_cast<Self*>(this) ); } \
  inline void ref_remove(void) const { refs.ref_remove( const_cast<Self*>(this) ); } \
  inline int  ref_links(void) const { return refs.ref_links(); } \
  inline bool ref_weak(WeakPointer& wp) const { return refs.ref_weak( const_cast<Self*>(this), wp); } \

#define UMODSYS_REFOBJECT_UNIIMPLEMENT() \
  UMODSYS_REFOBJECT_UNIIMPLEMENT0() \
  inline void suicide(void) { refs.obj_delete(this); }

#if 0

#define UMODSYS_REFOBJECT_SIMPLEMENT1(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_IMPLEMENT1(_type, _verno, _interface) \
  UMODSYS_REFOBJECT_UNIIMPLEMENT() \
  UMODSYS_REFOBJECT_PIMPLEMENT() \
  /*UMODSYS_NEW_DELETE()*/ \
  inline virtual void suicide(void) { delete this; } \

#define UMODSYS_REFOBJECT_SIMPLEMENT2(_type, _verno, _interface1, _interface2) \
  UMODSYS_REFOBJECT_IMPLEMENT2(_type, _verno, _interface1, _interface2) \
  UMODSYS_REFOBJECT_UNIIMPLEMENT() \
  UMODSYS_REFOBJECT_PIMPLEMENT() \
  /*UMODSYS_NEW_DELETE()*/ \
  inline virtual void suicide(void) { delete this; } \

#endif

//*************************************** REFOBJECT types
//***************************************

#define UMODSYS_REFOBJECT_SINGLE() \
  tl::TRefObjectLinks<Self> refs; \
  UMODSYS_REFOBJECT_UNIIMPLEMENT() \

#define UMODSYS_REFOBJECT_REFOTHER(_type_owner) \
  typedef _type_owner DOwner; typedef tl::TRefObject<DOwner> DOwnerP; \
  tl::TRefObjectLinksParent<Self, DOwner> refs; \
  UMODSYS_REFOBJECT_UNIIMPLEMENT() \

#define UMODSYS_REFOBJECT_COMPOSITE(_type_owner) \
  typedef _type_owner DOwner; typedef tl::TRefObject<DOwner> DOwnerP; \
  tl::TRefObjectLinksComposite<Self, DOwner> refs; \
  UMODSYS_REFOBJECT_UNIIMPLEMENT() \

//***************************************
// END
//***************************************
