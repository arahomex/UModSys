/*************************************************************
 CPU to LSB conversion
 *************************************************************/

template<typename T> 
struct TStreamDataElemLE {
  enum { Size = 0 };
  static void in(const void* src, T& value);
  static bool in(SCMem& stream, T& value);
  static bool out(SMem& stream, const T& value);
};

/*
  inline static void in(const void* src, T& value) {}
  inline static bool in(SCMem& stream, T& value) { return false; }
  inline static bool out(SMem& stream, const T& value) { return false; }
*/

/*************************************************************/
/*************************************************************/

template<> 
struct TStreamDataElemLE<BUI8> {
  typedef BUI8 T;
  enum { Size = 1 };
  //
  inline static bool in(SCMem& stream, T& value) { 
    const BByte* v;
    if(!stream.t_data_in(v, 1)) return false;
    value = BUI8(v[0]);
    return true;
  }
  inline static bool out(SMem& stream, T value) {
    BByte* v;
    if(!stream.t_data_out(v, 1)) return false;
    v[0] = value;
    return true;
  }
};

template<> 
struct TStreamDataElemLE<BSI8> {
  typedef BSI8 T;
  enum { Size = 1 };
  //
  inline static bool in(SCMem& stream, T& value) { 
    const BByte* v;
    if(!stream.t_data_in(v, 1)) return false;
    value = BSI8(v[0]);
    return true;
  }
  inline static bool out(SMem& stream, T value) {
    BByte* v;
    if(!stream.t_data_out(v, 1)) return false;
    v[0] = value;
    return true;
  }
};

template<> 
struct TStreamDataElemLE<BUI16> {
  typedef BUI16 T;
  enum { Size = 2 };
  //
  inline static void in(const void* src, T& value) { 
    const BByte* v = reinterpret_cast<const BByte*>(src);
    value = BUI16(v[0]) | (BUI16(v[1])<<8);
  }
  inline static bool in(SCMem& stream, T& value) { 
    const BByte* v;
    if(!stream.t_data_in(v, 2)) return false;
    value = BUI16(v[0]) | (BUI16(v[1])<<8);
    return true;
  }
  inline static bool out(SMem& stream, T value) {
    BByte* v;
    if(!stream.t_data_out(v, 2)) return false;
    v[0] = value & 0xff;
    v[1] = value >> 8;
    return true;
  }
};

template<> 
struct TStreamDataElemLE<BSI16> {
  typedef BSI16 T;
  enum { Size = 2 };
  //
  inline static bool in(SCMem& stream, T& value) { 
    const BByte* v;
    if(!stream.t_data_in(v, 2)) return false;
    value = BSI16(BUI16(v[0]) | (BUI16(v[1])<<8));
    return true;
  }
  inline static bool out(SMem& stream, T value) {
    BByte* v;
    if(!stream.t_data_out(v, 2)) return false;
    v[0] = value & 0xff;
    v[1] = value >> 8;
    return true;
  }
};

template<> 
struct TStreamDataElemLE<BUI32> {
  typedef BUI32 T;
  enum { Size = 4 };
  //
  inline static bool in(SCMem& stream, T& value) { 
    const BByte* v;
    if(!stream.t_data_in(v, 4)) return false;
    value = BUI32(v[0]) | (BUI32(v[1])<<8) | (BUI32(v[2])<<16) | (BUI32(v[3])<<24);
    return true;
  }
  inline static bool out(SMem& stream, T value) {
    BByte* v;
    if(!stream.t_data_out(v, 4)) return false;
    v[0] = value & 0xff;
    v[1] = (value >> 8) & 0xff;
    v[2] = (value >> 16) & 0xff;
    v[3] = (value >> 24) & 0xff;
    return true;
  }
};

template<> 
struct TStreamDataElemLE<BSI32> {
  typedef BSI32 T;
  enum { Size = 4 };
  //
  inline static bool in(SCMem& stream, T& value) { 
    const BByte* v;
    if(!stream.t_data_in(v, 4)) return false;
    value = BSI32(BUI32(v[0]) | (BUI32(v[1])<<8) | (BUI32(v[2])<<16) | (BUI32(v[3])<<24));
    return true;
  }
  inline static bool out(SMem& stream, T value) {
    BByte* v;
    if(!stream.t_data_out(v, 4)) return false;
    v[0] = value & 0xff;
    v[1] = (value >> 8) & 0xff;
    v[2] = (value >> 16) & 0xff;
    v[3] = (value >> 24) & 0xff;
    return true;
  }
};

template<> 
struct TStreamDataElemLE<BF32> {
  typedef BF32 T;
  union xlat { BF32 vf; BUI32 vi; };
  enum { Size = 4 };
  //
  inline static bool in(SCMem& stream, T& value) { 
    const BByte* v;
    if(!stream.t_data_in(v, 4)) return false;
    xlat x;
    x.vi = BUI32(v[0]) | (BUI32(v[1])<<8) | (BUI32(v[2])<<16) | (BUI32(v[3])<<24);
    value = x.vf;
    return true;
  }
  inline static bool out(SMem& stream, const T& value) {
    BByte* v;
    if(!stream.t_data_out(v, 4)) return false;
    xlat x;
    x.vf = value;
    v[0] = x.vi & 0xff;
    v[1] = (x.vi >> 8) & 0xff;
    v[2] = (x.vi >> 16) & 0xff;
    v[3] = (x.vi >> 24) & 0xff;
    return true;
  }
};

#ifdef UMS_WCHAR_SEPARATED_TYPE
template<> 
struct TStreamDataElemLE<BCharW> {
  typedef BCharW T;
  enum { Size = sizeof(T) };
  //
  inline static void in(const void* src, T& value) { 
    const BByte* v = reinterpret_cast<const BByte*>(src);
    value = BUI16(v[0]) | (BUI16(v[1])<<8);
  }
  inline static bool in(SCMem& stream, T& value) { 
    const BByte* v;
    if(!stream.t_data_in(v, 2)) return false;
    value = BUI16(v[0]) | (BUI16(v[1])<<8);
    return true;
  }
  inline static bool out(SMem& stream, T value) {
    BByte* v;
    if(!stream.t_data_out(v, 2)) return false;
    v[0] = value & 0xff;
    v[1] = value >> 8;
    return true;
  }
};
#endif

/*************************************************************/
/*************************************************************/

/*************************************************************
 FROM/TO
 *************************************************************/

// conversion from le
inline void from_le(char *v) {}
inline void from_le(unsigned char *v) {}
inline void from_le(signed char *v) {}
inline void from_le(int *v) {}
inline void from_le(unsigned int *v) {}
inline void from_le(short int *v) {}
inline void from_le(unsigned short int *v) {}
inline void from_le(long int *v) {}
inline void from_le(unsigned long int *v) {}
inline void from_le(float *v) {}
inline void from_le(double *v) {}
#if defined(UMS_CPU_INTELTYPES)
  inline void from_le(sint8 *v) {}
  inline void from_le(uint8 *v) {}
  inline void from_le(sint16 *v) {}
  inline void from_le(uint16 *v) {}
  inline void from_le(sint32 *v) {}
  inline void from_le(uint32 *v) {}
  inline void from_le(sint64 *v) {}
  inline void from_le(uint64 *v) {}
#endif
#ifdef UMS_CPU_WCHAR
  inline void from_le(wchar_t *v) {}
#endif

// conversion to le
inline void to_le(char *v) {}
inline void to_le(unsigned char *v) {}
inline void to_le(signed char *v) {}
inline void to_le(int *v) {}
inline void to_le(unsigned int *v) {}
inline void to_le(short int *v) {}
inline void to_le(unsigned short int *v) {}
inline void to_le(long int *v) {}
inline void to_le(unsigned long int *v) {}
inline void to_le(float *v) {}
inline void to_le(double *v) {}

#if defined(UMS_CPU_INTELTYPES)
  inline void to_le(sint8 *v) {}
  inline void to_le(uint8 *v) {}
  inline void to_le(sint16 *v) {}
  inline void to_le(uint16 *v) {}
  inline void to_le(sint32 *v) {}
  inline void to_le(uint32 *v) {}
  inline void to_le(sint64 *v) {}
  inline void to_le(uint64 *v) {}
#endif
#ifdef UMS_CPU_WCHAR
  inline void to_le(wchar_t *v) {}
#endif

#if defined(UMS_CPU_NONALIGNED)

  inline sint8 le_sint8(const void* p) { return *reinterpret_cast<const sint8*>(p); }
  inline uint8 le_uint8(const void* p) { return *reinterpret_cast<const uint8*>(p); }
  inline sint16 le_sint16(const void* p) { return *reinterpret_cast<const sint16*>(p); }
  inline uint16 le_uint16(const void* p) { return *reinterpret_cast<const uint16*>(p); }
  inline sint32 le_sint32(const void* p) { return *reinterpret_cast<const sint32*>(p); }
  inline uint32 le_uint32(const void* p) { return *reinterpret_cast<const uint32*>(p); }
  inline sint64 le_sint64(const void* p) { return *reinterpret_cast<const sint64*>(p); }
  inline uint64 le_uint64(const void* p) { return *reinterpret_cast<const uint64*>(p); }
  inline float32 le_float32(const void* p) { return *reinterpret_cast<const float32*>(p); }
  inline float64 le_float64(const void* p) { return *reinterpret_cast<const float64*>(p); }

  inline void le_sint8(sint8 &value, const void* p) { value = *reinterpret_cast<const sint8*>(p); }
  inline void le_uint8(uint8 &value, const void* p) { value = *reinterpret_cast<const uint8*>(p); }
  inline void le_sint16(sint8 &value, const void* p) { value = *reinterpret_cast<const sint16*>(p); }
  inline void le_uint16(uint16 &value, const void* p) { value = *reinterpret_cast<const uint16*>(p); }
  inline void le_sint32(sint32 &value, const void* p) { value = *reinterpret_cast<const sint32*>(p); }
  inline void le_uint32(uint32 &value, const void* p) { value = *reinterpret_cast<const uint32*>(p); }
  inline void le_sint64(sint64 &value, const void* p) { value = *reinterpret_cast<const sint64*>(p); }
  inline void le_uint64(uint64 &value, const void* p) { value = *reinterpret_cast<const uint64*>(p); }
  inline void le_float32(float32 &value, const void* p) { value = *reinterpret_cast<const float32*>(p); }
  inline void le_float64(float64 &value, const void* p) { value = *reinterpret_cast<const float64*>(p); }

  inline void sint8_le(void* p, const sint8 &value) { *reinterpret_cast<sint8*>(p) = value; }
  inline void uint8_le(void* p, const uint8 &value) { *reinterpret_cast<uint8*>(p) = value; }
  inline void sint16_le(void* p, const sint16 &value) { *reinterpret_cast<sint16*>(p) = value; }
  inline void uint16_le(void* p, const uint16 &value) { *reinterpret_cast<uint16*>(p) = value; }
  inline void sint32_le(void* p, const sint32 &value) { *reinterpret_cast<sint32*>(p) = value; }
  inline void uint32_le(void* p, const uint32 &value) { *reinterpret_cast<uint32*>(p) = value; }
  inline void sint64_le(void* p, const sint64 &value) { *reinterpret_cast<sint64*>(p) = value; }
  inline void uint64_le(void* p, const uint64 &value) { *reinterpret_cast<uint64*>(p) = value; }
  inline void float32_le(void* p, const float32& value) { *reinterpret_cast<float32*>(p) = value; }
  inline void float64_le(void* p, const float64& value) { *reinterpret_cast<float64*>(p) = value; }

#else // !defined(UMS_CPU_NONALIGNED)

  inline sint8 le_sint8(const void* p) { return *reinterpret_cast<const sint8*>(p); }
  inline uint8 le_uint8(const void* p) { return *reinterpret_cast<const uint8*>(p); }
  inline void le_sint8(sint8 &value, const void* p) { value = *reinterpret_cast<const sint8*>(p); }
  inline void le_uint8(uint8 &value, const void* p) { value = *reinterpret_cast<const uint8*>(p); }
  inline void sint8_le(void* p, sint8 value) { *reinterpret_cast<sint8*>(p) = value; }
  inline void uint8_le(void* p, uint8 value) { *reinterpret_cast<uint8*>(p) = value; }


  inline sint16 le_sint16(const void* p) { sint16 rv; memcpy(&rv, p, 2); return rv; }
  inline uint16 le_uint16(const void* p) { uint16 rv; memcpy(&rv, p, 2); return rv; }
  inline sint32 le_sint32(const void* p) { sint32 rv; memcpy(&rv, p, 4); return rv; }
  inline uint32 le_uint32(const void* p) { uint32 rv; memcpy(&rv, p, 4); return rv; }
  inline sint64 le_sint64(const void* p) { sint64 rv; memcpy(&rv, p, 8); return rv; }
  inline uint64 le_uint64(const void* p) { uint64 rv; memcpy(&rv, p, 8); return rv; }
  inline float32 le_float32(const void* p) { float32 rv; memcpy(&rv, p, 4); return rv; }
  inline float64 le_float64(const void* p) { float64 rv; memcpy(&rv, p, 8); return rv; }

  inline void le_sint16(sint8 &value, const void* p) { memcpy(&value, p, 2); }
  inline void le_uint16(uint16 &value, const void* p) { memcpy(&value, p, 2); }
  inline void le_sint32(sint32 &value, const void* p) { memcpy(&value, p, 4); }
  inline void le_uint32(uint32 &value, const void* p) { memcpy(&value, p, 4); }
  inline void le_sint64(sint64 &value, const void* p) { memcpy(&value, p, 8); }
  inline void le_uint64(uint64 &value, const void* p) { memcpy(&value, p, 8); }
  inline void le_float32(float32 &value, const void* p) { memcpy(&value, p, 16); }
  inline void le_float64(float64 &value, const void* p) { memcpy(&value, p, 16); }

  inline void sint16_le(void* p, const sint16 &value) { memcpy(p, &value, 2); }
  inline void uint16_le(void* p, const uint16 &value) { memcpy(p, &value, 2); }
  inline void sint32_le(void* p, const sint32 &value) { memcpy(p, &value, 4); }
  inline void uint32_le(void* p, const uint32 &value) { memcpy(p, &value, 4); }
  inline void sint64_le(void* p, const sint64 &value) { memcpy(p, &value, 8); }
  inline void uint64_le(void* p, const uint64 &value) { memcpy(p, &value, 8); }
  inline void float32_le(void* p, const float32& value) { memcpy(p, &value, 4); }
  inline void float64_le(void* p, const float64& value) { memcpy(p, &value, 8); }

#endif // defined(UMS_CPU_NONALIGNED)

/*************************************************************
 End
 *************************************************************/
