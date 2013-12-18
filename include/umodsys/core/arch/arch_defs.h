//*****************************************************
// arch_def.h: defined CPU-related macros
//*****************************************************

// ************** Complex structure definition macroses:

#define UMODSYS_CPU_DEF_FIELD_START(ftypename) \
  struct ftypename { \
    enum { OFSE_START = 0 }; \
    inline void g_START(const BByte *bytes) {} \
    inline void s_START(BByte *bytes) {} \
    inline ftypename(void) {} \

#define UMODSYS_CPU_DEF_FIELD_CONST(field, bytesize, prevfield) \
    enum { OFS_##field = OFSE_##prevfield, SIZE_##field = bytesize, OFSE_##field = OFS_##field + SIZE_##field }

#define UMODSYS_CPU_DEF_FIELD(prevfield, field, type, storemode) \
    UMODSYS_CPU_DEF_FIELD_CONST(field, SIZE_##type, prevfield); \
    type field; /* cached value*/ \
    inline void g_##field(const BByte *bytes) { g_##prevfield(bytes); arch::from_##storemode<type>(field, bytes + OFS_##field); } \
    inline void s_##field(BByte *bytes) { s_##prevfield(bytes); arch::to_##storemode<type>(bytes + OFS_##field, field); } \

#define UMODSYS_CPU_DEF_FIELD_ARRAY(prevfield, field, type, storemode, _count) \
    UMODSYS_CPU_DEF_FIELD_CONST(field, (SIZE_##type*_count), prevfield); \
    type field[_count]; /* cached value*/ \
    inline void g_##field(const BByte *bytes) { g_##prevfield(bytes); for(size_t i=0; i<_count; i++) arch::from_##storemode<type>(field[i], bytes + OFS_##field + SIZE_##type*i); } \
    inline void s_##field(BByte *bytes) { s_##prevfield(bytes); for(size_t i=0; i<_count; i++) arch::to_##storemode<type>(bytes + OFS_##field + SIZE_##type*i, field[i]); } \

#define UMODSYS_CPU_DEF_FIELD_END(ftypename, lastfield) \
    enum { SIZE = OFSE_##lastfield }; \
    struct Holder; \
    inline void get(const BByte *bytes) { g_##lastfield(bytes); } \
    inline void set(BByte *bytes) { s_##lastfield(bytes); } \
    inline static const BByte* p_end(const BByte *bytes) { return bytes + SIZE; } \
    inline static BByte* p_end(BByte *bytes) { return bytes + SIZE; } \
  }; \
  struct ftypename::Holder : public ftypename { \
    BByte bytes[SIZE]; \
    inline void get(void) { ftypename::get(bytes); } \
    inline void set(void) { ftypename::set(bytes); } \
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

