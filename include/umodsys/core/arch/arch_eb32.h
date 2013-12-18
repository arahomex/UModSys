/*************************************************************
 CPU to MSB conversion
 *************************************************************/

template<typename T> 
struct TStreamDataElemBE {
  enum { Size = 0 };
  inline static void in(const void* src, T& value) {}
  inline static bool in(SCMem& stream, T& value) { return false; }
  inline static bool out(SMem& stream, const T& value) { return false; }
};

/*************************************************************/
/*************************************************************/

template<> 
struct TStreamDataElemBE<BUI8> {
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
struct TStreamDataElemBE<BSI8> {
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
struct TStreamDataElemBE<BUI16> {
  typedef BUI16 T;
  enum { Size = 2 };
  //
  inline static void in(const void* src, T& value) { 
    const BByte* v = reinterpret_cast<const BByte*>(src);
    value = BUI16(v[1]) | (BUI16(v[0])<<8);
  }
  inline static bool in(SCMem& stream, T& value) { 
    const BByte* v;
    if(!stream.t_data_in(v, 2)) return false;
    value = BUI16(v[1]) | (BUI16(v[0])<<8);
    return true;
  }
  inline static bool out(SMem& stream, T value) {
    BByte* v;
    if(!stream.t_data_out(v, 2)) return false;
    v[1] = value & 0xff;
    v[0] = value >> 8;
    return true;
  }
};

template<> 
struct TStreamDataElemBE<BSI16> {
  typedef BSI16 T;
  enum { Size = 2 };
  //
  inline static bool in(SCMem& stream, T& value) { 
    const BByte* v;
    if(!stream.t_data_in(v, 2)) return false;
    value = BSI16(BUI16(v[1]) | (BUI16(v[0])<<8));
    return true;
  }
  inline static bool out(SMem& stream, T value) {
    BByte* v;
    if(!stream.t_data_out(v, 2)) return false;
    v[1] = value & 0xff;
    v[0] = value >> 8;
    return true;
  }
};

template<> 
struct TStreamDataElemBE<BUI32> {
  typedef BUI32 T;
  enum { Size = 4 };
  //
  inline static bool in(SCMem& stream, T& value) { 
    const BByte* v;
    if(!stream.t_data_in(v, 4)) return false;
    value = BUI32(v[3]) | (BUI32(v[2])<<8) | (BUI32(v[1])<<16) | (BUI32(v[0])<<24);
    return true;
  }
  inline static bool out(SMem& stream, T value) {
    BByte* v;
    if(!stream.t_data_out(v, 4)) return false;
    v[3] = value & 0xff;
    v[2] = (value >> 8) & 0xff;
    v[1] = (value >> 16) & 0xff;
    v[0] = (value >> 24) & 0xff;
    return true;
  }
};

template<> 
struct TStreamDataElemBE<BSI32> {
  typedef BSI32 T;
  enum { Size = 4 };
  //
  inline static bool in(SCMem& stream, T& value) { 
    const BByte* v;
    if(!stream.t_data_in(v, 4)) return false;
    value = BSI32(BUI32(v[3]) | (BUI32(v[2])<<8) | (BUI32(v[1])<<16) | (BUI32(v[0])<<24));
    return true;
  }
  inline static bool out(SMem& stream, T value) {
    BByte* v;
    if(!stream.t_data_out(v, 4)) return false;
    v[3] = value & 0xff;
    v[2] = (value >> 8) & 0xff;
    v[1] = (value >> 16) & 0xff;
    v[0] = (value >> 24) & 0xff;
    return true;
  }
};

template<> 
struct TStreamDataElemBE<DCString> {
  typedef DCString T;
  enum { Size = 4 };
  //
  inline static bool in(SCMem& stream, T& value) { 
    T::Str line;
    stream.t_data(line);
    size_t len = tl::su::slen(line, stream.size);
    if(len==stream.size)
      return false;
    value = line;
    stream.move(len+1);
    return true;
  }
  inline static bool out(SMem& stream, const T& value) {
    return stream.t_data_out(value.c_str(), ~value+1);
  }
};

#ifdef UMODSYS_WCHAR_SEPARATED_TYPE
template<> 
struct TStreamDataElemBE<BCharW> {
  typedef BCharW T;
  enum { Size = sizeof(T) };
  //
  inline static void in(const void* src, T& value) { 
    const BByte* v = reinterpret_cast<const BByte*>(src);
    value = BUI16(v[1]) | (BUI16(v[0])<<8);
  }
  inline static bool in(SCMem& stream, T& value) { 
    const BByte* v;
    if(!stream.t_data_in(v, 2)) return false;
    value = BUI16(v[1]) | (BUI16(v[0])<<8);
    return true;
  }
  inline static bool out(SMem& stream, T value) {
    BByte* v;
    if(!stream.t_data_out(v, 2)) return false;
    v[1] = value & 0xff;
    v[0] = value >> 8;
    return true;
  }
};
#endif

/*************************************************************
 End
 *************************************************************/
