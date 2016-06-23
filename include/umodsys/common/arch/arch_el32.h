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

#ifdef UMODSYS_WCHAR_SEPARATED_TYPE
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

/*************************************************************
 End
 *************************************************************/
