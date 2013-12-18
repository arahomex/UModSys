//*****************************************************
// arch_def.h: defined CPU-related macros
//*****************************************************

// ************** Complex structure definition macroses:

#define UMODSYS_CPU_DEF_FIELD_CONST(field, bytesize, prevfield) \
    enum { OFS_##field = OFSE_##prevfield, SIZE_##field = bytesize, OFSE_##field = OFS_##field + SIZE_##field }

#define UMODSYS_CPU_DEF_FIELD(prevfield, field, type, storemode) \
    UMODSYS_CPU_DEF_FIELD_CONST(field, SIZE_##type, prevfield); \
    type field; /* cached value*/ \
    inline void g_##field(void) { g_##prevfield(); arch::from_##storemode<type>(field, bytes + OFS_##field); } \
    inline void s_##field(void) { s_##prevfield(); arch::to_##storemode<type>(bytes + OFS_##field, field); } \
    inline type get_##field(void) const { return arch::from_##storemode<type>(bytes + OFS_##field); } \
    inline void get_##field(type& val) const { val = arch::from_##storemode<type>(val, bytes + OFS_##field); } \
    inline void set_##field(const type& val) { arch::to_##storemode<type>(bytes + OFS_##field, val); } \
    inline static type get_##field(const void* r) { return arch::from_##storemode<type>(p_start(r) + OFS_##field); } \
    inline static void get_##field(const void* r, type& val) { arch::from_##storemode<type>(val, p_start(r) + OFS_##field); } \
    inline static void set_##field(void* r, const type& val) { arch::to_##storemode<type>(p_start(r) + OFS_##field, val); }

#define UMODSYS_CPU_DEF_FIELD_ARRAY(prevfield, field, type, storemode, _count) \
    UMODSYS_CPU_DEF_FIELD_CONST(field, (SIZE_##type*_count), prevfield); \
    type field[_count]; /* cached value*/ \
    inline void g_##field(void) { g_##prevfield(); for(int i=0; i<_count; i++) arch::from_##storemode<type>(field[i], bytes + OFS_##field + SIZE_##type*i); } \
    inline void s_##field(void) { s_##prevfield(); for(int i=0; i<_count; i++) arch::to_##storemode<type>(bytes + OFS_##field + SIZE_##type*i, field[i]); } \

#define UMODSYS_CPU_DEF_FIELD_START(ftypename) \
    struct ftypename { \
    enum { OFSE_START = 0 }; \
    inline void g_START(void) {} \
    inline void s_START(void) {} \
    inline ftypename(const void* b) { fill(b); } \
    inline ftypename(void) {} \


#define UMODSYS_CPU_DEF_FIELD_END(lastfield) \
    enum { SIZE = OFSE_##lastfield }; \
    BByte bytes[SIZE]; \
    inline void get(void) { g_##lastfield(); } \
    inline void set(void) { s_##lastfield(); } \
    inline static const BByte* p_start(const void* r) { return reinterpret_cast<const BByte*>(r); } \
    inline static const BByte* p_end(const void* r) { return reinterpret_cast<const BByte*>(r)+SIZE; } \
    inline static BByte* p_start(void* r) { return reinterpret_cast<BByte*>(r); } \
    inline static BByte* p_end(void* r) { return reinterpret_cast<BByte*>(r)+SIZE; } \
    inline void fill(const void* b) { memcpy(bytes, b, SIZE); get(); } \
  };




#define UMODSYS_CPU_DEF_FIELD_uint32_le(field, prevfield) UMODSYS_CPU_DEF_FIELD(prevfield, field, uint32, le)
#define UMODSYS_CPU_DEF_FIELD_uint16_le(field, prevfield) UMODSYS_CPU_DEF_FIELD(prevfield, field, uint16, le)
#define UMODSYS_CPU_DEF_FIELD_uint8_le(field, prevfield) UMODSYS_CPU_DEF_FIELD(prevfield, field, uint8, le)
#define UMODSYS_CPU_DEF_FIELD_sint32_le(field, prevfield) UMODSYS_CPU_DEF_FIELD(prevfield, field, sint32, le)
#define UMODSYS_CPU_DEF_FIELD_sint16_le(field, prevfield) UMODSYS_CPU_DEF_FIELD(prevfield, field, sint16, le)
#define UMODSYS_CPU_DEF_FIELD_sint8_le(field, prevfield) UMODSYS_CPU_DEF_FIELD(prevfield, field, sint8, le)
#define UMODSYS_CPU_DEF_FIELD_float32_le(field, prevfield) UMODSYS_CPU_DEF_FIELD(prevfield, field, float32, le)

#define UMODSYS_CPU_DEF_FIELDA_uint32_le(field, prevfield, _count) UMODSYS_CPU_DEF_FIELD_ARRAY(prevfield, field, uint32, le, _count)
#define UMODSYS_CPU_DEF_FIELDA_uint16_le(field, prevfield, _count) UMODSYS_CPU_DEF_FIELD_ARRAY(prevfield, field, uint16, le, _count)
#define UMODSYS_CPU_DEF_FIELDA_uint8_le(field, prevfield, _count) UMODSYS_CPU_DEF_FIELD_ARRAY(prevfield, field, uint8, le, _count)
#define UMODSYS_CPU_DEF_FIELDA_sint32_le(field, prevfield, _count) UMODSYS_CPU_DEF_FIELD_ARRAY(prevfield, field, sint32, le, _count)
#define UMODSYS_CPU_DEF_FIELDA_sint16_le(field, prevfield, _count) UMODSYS_CPU_DEF_FIELD_ARRAY(prevfield, field, sint16, le, _count)
#define UMODSYS_CPU_DEF_FIELDA_sint8_le(field, prevfield, _count) UMODSYS_CPU_DEF_FIELD_ARRAY(prevfield, field, sint8, le, _count)
#define UMODSYS_CPU_DEF_FIELDA_float32_le(field, prevfield, _count) UMODSYS_CPU_DEF_FIELD_ARRAY(prevfield, field, float32, le, _count)

// ************** /Complex structure definition macroses:


//*************************************** 

