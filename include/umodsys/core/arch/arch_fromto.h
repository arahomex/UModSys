/*************************************************************
 SWAP BYTES
 *************************************************************/

inline void type_swap_byte(BByte *a, BByte *b)
{
  register BByte t = *a; *a = *b; *b = t;
}

template<size_t num>
void type_swap_bytes(void* v)
{
  register BByte *p = reinterpret_cast<BByte*>(v);
  for(size_t i=0; i<num/2; i++) {
    type_swap_byte(p+i, (num-1)-i);
  }
}

template<> 
void type_swap_bytes<1>(void* v) {}

template<> 
void type_swap_bytes<2>(void* v) 
{
  register BByte *p = reinterpret_cast<BByte*>(v);
  type_swap_byte(p+0, p+1);
}

template<> 
void type_swap_bytes<4>(void* v) 
{
  register BByte *p = reinterpret_cast<BByte*>(v);
  type_swap_byte(p+0, p+3);
  type_swap_byte(p+1, p+2);
}

template<> 
void type_swap_bytes<8>(void* v) 
{
  register BByte *p = reinterpret_cast<BByte*>(v);
  type_swap_byte(p+0, p+7);
  type_swap_byte(p+1, p+6);
  type_swap_byte(p+2, p+5);
  type_swap_byte(p+3, p+4);
}

/*************************************************************
 FROM/TO LE and BE
 *************************************************************/

template<typename T>
inline T from_type_u(const void* p) 
{ 
  return *reinterpret_cast<const T*>(p); 
}
template<typename T>
inline void from_type_u(T& rv, const void* p) 
{ 
  v = *reinterpret_cast<const T*>(p); 
}
template<typename T>
inline void to_type_u(void* p, const T& v) 
{ 
  *reinterpret_cast<T*>(p) = v; 
}

template<typename T>
inline T from_type_a(const void* p) 
{ 
  T rv;
  memcpy(&rv, p, sizeof(T));
  return rv; 
}
template<typename T>
inline void from_type_a(T& rv, const void* p) 
{ 
  memcpy(&rv, p, sizeof(T));
}
template<typename T>
inline void to_type_a(void* p, const T& v) 
{ 
  memcpy(p, &v, sizeof(T));
}

template<typename T>
inline T from_type_s(const void* p) 
{ 
  T rv;
  memcpy(&rv, p, sizeof(T));
  type_swap_bytes<sizeof(T)>(&rv);
  return rv; 
}
template<typename T>
inline void from_type_s(T& rv, const void* p) 
{ 
  memcpy(&rv, p, sizeof(T));
  type_swap_bytes<sizeof(T)>(&rv);
}
template<typename T>
inline T to_type_s(void* p, const T& v) 
{ 
  memcpy(p, &v, sizeof(T));
  type_swap_bytes<sizeof(T)>(p);
}

#if defined(UMODSYS_ARCH_LE)
  #if defined(UMODSYS_ARCH_NONALIGNED)
    template<typename T> inline T from_le(const void* p) { return from_type_u<T>(p); }
    template<typename T> inline void from_le(T& rv, const void* p) { from_type_u<T>(rv, p); }
    template<typename T> inline void to_le(void* p, const T& v) { to_type_u<T>(p, v); }
  #else
    template<typename T> inline T from_le(const void* p) { return from_type_a<T>(p); }
    template<typename T> inline void from_le(T& rv, const void* p) { from_type_a<T>(rv, p); }
    template<typename T> inline void to_le(void* p, const T& v) { to_type_a<T>(p, v); }
  #endif
  template<typename T> inline T from_be(const void* p) { return from_type_s<T>(p); }
  template<typename T> inline void from_be(T& rv, const void* p) { from_type_s<T>(rv, p); }
  template<typename T> inline void to_be(void* p, const T& v) { to_type_s<T>(p, v); }
#elif defined(UMODSYS_ARCH_BE)
  #if defined(UMODSYS_ARCH_NONALIGNED)
    template<typename T> inline T from_be(const void* p) { return from_type_u<T>(p); }
    template<typename T> inline void from_be(T& rv, const void* p) { from_type_u<T>(rv, p); }
    template<typename T> inline void to_be(void* p, const T& v) { to_type_u<T>(p, v); }
  #else
    template<typename T> inline T from_be(const void* p) { return from_type_a<T>(p); }
    template<typename T> inline void from_be(T& rv, const void* p) { from_type_a<T>(rv, p); }
    template<typename T> inline void to_be(void* p, const T& v) { to_type_a<T>(p, v); }
  #endif
  template<typename T> inline T from_le(const void* p) { return from_type_s<T>(p); }
  template<typename T> inline void from_le(T& rv, const void* p) { from_type_s<T>(rv, p); }
  template<typename T> inline void to_le(void* p, const T& v) { to_type_s<T>(p, v); }
#else
  #error Unknown UMODSYS_ARCH endianess flag
#endif

template<> inline sint8 from_be<sint8>(const void* p) { return from_type_u<sint8>(p); }
template<> inline void from_be<sint8>(sint8& rv, const void* p) { return from_type_u<sint8>(rv, p); }
template<> inline void to_be<sint8>(void* p, const sint8& v) { return to_type_u<sint8>(p, v); }

template<> inline uint8 from_be<uint8>(const void* p) { return from_type_u<uint8>(p); }
template<> inline void from_be<uint8>(uint8& rv, const void* p) { return from_type_u<uint8>(rv, p); }
template<> inline void to_be<uint8>(void* p, const uint8& v) { return to_type_u<uint8>(p, v); }

template<> inline sint8 from_le<sint8>(const void* p) { return from_type_u<sint8>(p); }
template<> inline void from_le<sint8>(sint8& rv, const void* p) { return from_type_u<sint8>(rv, p); }
template<> inline void to_le<sint8>(void* p, const sint8& v) { return to_type_u<sint8>(p, v); }

template<> inline uint8 from_le<uint8>(const void* p) { return from_type_u<uint8>(p); }
template<> inline void from_le<uint8>(uint8& rv, const void* p) { return from_type_u<uint8>(rv, p); }
template<> inline void to_le<uint8>(void* p, const uint8& v) { return to_type_u<uint8>(p, v); }

/*************************************************************
 End
 *************************************************************/
