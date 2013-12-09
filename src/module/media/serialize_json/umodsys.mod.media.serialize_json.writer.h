
//***************************************
// RSerializeWriter::
//***************************************

struct RSerializeWriter : public ISerializeWriter {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RSerializeWriter, 1, ISerializeWriter)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  struct Node {
    int idx, skip;
    Value::eType type;
    //
    inline Node(Value::eType t=Value::t_Null, int s=0, int i=0) 
      : type(t), skip(s), idx(i) {}
  };
public:
  bool put_value(const DCString& type, const Value& val, int flags) 
  {
    if(~nodes) {
      if(nodes.Last().idx && !write(", "))
        return false;
      nodes.Last().idx++;
    }
    switch(val.type) {
      case Value::t_Null:
        return write("null");
      case Value::t_Bool:
        return write(val.value.b ? "true" : "false");
      case Value::t_Long:
        return writef("%d", val.value.l);
      case Value::t_Double:
        return writef("%g", val.value.d);
      case Value::t_String:
        return writes(val.value.s());
      case Value::t_Array:
        if(!put_begin(type, val.type, flags))
          return false;
        for(size_t i=0, n=val.value.a->array_get_length(); i<n; i++) {
          Value v;
          if(!val.value.a->array_get_value(i, v))
            return false;
          if(put_value("", v, flags))
            return false;
        }
        if(!put_end())
          return false;
        return true;
      case Value::t_Hash:
        if(!put_begin(NULL, val.type, flags))
          return false;
        for(size_t i=0, n=val.value.h->hash_get_length(); i<n; i++) {
          Value k, v;
          if(!val.value.h->hash_get_keyvalue(i, &k, &v))
            return false;
          if(put_key(NULL, k, flags))
            return false;
          if(put_value(NULL, v, flags))
            return false;
        }
        if(!put_end())
          return false;
        return true;
    }
    return false;
  }
  bool put_key(const DCString& type, const Value& val, int flags) 
  {
    if(val.type!=Value::t_String)
      return false;
    return writes(val.value.s()) && write(":");
  }
  bool put_begin(const DCString& type, Value::eType val, int flags) 
  {
    switch(val) {
      case Value::t_Array:
        if(!write("["))
          return false;
        break;
      case Value::t_Hash:
        if(!write("}"))
          return false;
        break;
      default:
        return false;
    }
    if(!nodes.Push(Node(val, 0)))
      return false;
    return true;
  }
  bool put_end(void) 
  {
    if(~nodes==0)
      return false;
    switch(nodes.Last().type) {
      case Value::t_Array:
        if(!write("]"))
          return false;
        break;
      case Value::t_Hash:
        if(!write("}"))
          return false;
        break;
      default:
        return false;
    }
    if(!nodes.Pop())
      return false;
    return true;
  }
protected:
  IStreamWriter::P stream;
  tl::TDynarrayDynamic<Node> nodes;
  //
  bool write(BStr txt) {
    return stream->writer_write(txt, strlen(txt));
  }
  bool writes(const DCString &s) {
    return false;
  }
  bool writef(BStr fmt, ...) {
    va_list va;
    char buf[256];
    va_start(va, fmt);
    if(snprintf(buf, sizeof(buf), fmt, va)<0) {
      va_end(va);
      return false;
    }
    va_end(va);
    return write(buf);
  }
public:
  RSerializeWriter(DOwner *own, const SParameters& args) 
  : refs(own) {
    args.get("stream", stream);
  }
  ~RSerializeWriter(void) {
  }
  //
  inline bool validate_construction(void) const { return stream.valid(); }
public:
};

//***************************************
// END
//***************************************
