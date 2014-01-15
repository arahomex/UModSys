
//***************************************
// RSerializeWriter::
//***************************************

struct RSerializeWriter : public ISerializeWriter {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RSerializeWriter, 1, ISerializeWriter)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  struct Node {
    int idx, skip, flags;
    Value::eType type;
    char last_mode;
    int last_skip;
    //
    inline Node(Value::eType t=Value::t_Null, int s=0, int f=0, int i=0) 
      : type(t), skip(s), flags(f), idx(i), last_mode(0), last_skip(s) {}
  };
public:
  BStr writer_lasterror(void) const { return last_error; }
  //
  bool put_value(const DCString& type, const Value& val, int flags) 
  {
s_dbg.put(0, cl_Debug, "{%d}", val.type);
if(val.type==Value::t_String) s_dbg.put(0, cl_Debug, "{%u\n\"%s\"}", (unsigned)val.value.s.l, val.value.s.p);
    if(val.type==Value::t_Array) {
      if(!put_begin(type, val.type, flags))
        return false;
      for(size_t i=0, n=val.value.a->array_get_length(); i<n; i++) {
        Value v;
        if(!val.value.a->array_get_value(i, v))
          return false;
        if(put_value(NULL, v, flags))
          return false;
      }
      if(!put_end())
        return false;
      return true;
    } else if(val.type==Value::t_Hash) {
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
    //
    if(!begin_elem(flags, true))
      return false;
    //
    switch(val.type) {
      case Value::t_Null:
        return writer.put_z("null");
      case Value::t_Bool:
        return writer.put_z(val.value.b ? "true" : "false");
      case Value::t_Long:
        return writef("%ld", val.value.l);
      case Value::t_Double:
        return writef("%e", val.value.d);
      case Value::t_String:
        return write_s(val.value.s);
    }
    return false;
  }
  bool put_key(const DCString& type, const Value& val, int flags) 
  {
    if(val.type!=Value::t_String)
      return false;
    if(!begin_elem(flags, false))
      return false;
    return write_s(val.value.s) && writer.put_c(':');
  }
  bool put_begin(const DCString& type, Value::eType val, int flags) 
  {
    if(!begin_elem(flags, true))
      return false;
    switch(val) {
      case Value::t_Array:
        if(!writer.put_c('['))
          return false;
        break;
      case Value::t_Hash:
        if(!writer.put_c('{'))
          return false;
        break;
      default:
        return false;
    }
    Node *node = top();
//    if(!delim(*node, flags))
//      return false;
    if(!nodes.Push(Node(val, node->skip + skip_step, flags)))
      return false;
//    if(!delim(*top(), flags))
//      return false;
    return true;
  }
  bool put_end(void) 
  {
    if(~nodes==0)
      return false;
    Node node = *top();
    if(!nodes.Pop())
      return false;
    Node *x = top();
    if(!delim(*x, node.flags))
      return false;
    switch(node.type) {
      case Value::t_Array:
        if(!writer.put_c(']'))
          return false;
        break;
      case Value::t_Hash:
        if(!writer.put_c('}'))
          return false;
        break;
      default:
        return false;
    }
    return true;
  }
protected:
  Node root;
  int skip_step;
  tl::TDynarrayDynamic<Node> nodes;
  TTextStreamWriterBuf<BChar, 4096> writer;
  BStr last_error;
  //
  Node* top(void) { return ~nodes ? &nodes.Last() : &root; }
  //
  bool begin_elem(int flags, bool isvalue)
  {
    Node &node = *top();
    if(node.type==Value::t_Hash) {
      if(isvalue && node.last_mode!=1)
        return false;
      if(!isvalue && node.last_mode==1)
        return false;
      node.last_mode = isvalue+1;
    } else {
      if(!isvalue)
        return false;
    }
    //
    if(isvalue) {
      if(node.idx) {
        if(!writer.put_c(','))
          return false;
      }
      node.idx++;
    }
    if(!isvalue || node.type!=Value::t_Hash) {
      if(!delim(node, flags))
        return false;
    }
    return true;
  }
  bool write_s(const Value::Str &s) 
  {
    if(!writer.put_c('\"'))
      return false;
    for(BStr p=s.p, e=s.p+s.l; p!=e; p++) {
      BChar c = *p;
      if(s_escapes[c][0]!='\0') {
        if(!writer.put_z(s_escapes[c]))
          return false;
      } else {
        if(!writer.put_c(c))
          return false;
      }
    }
    if(!writer.put_c('\"'))
      return false;
    return true;
  }
  inline bool skip(int n) 
  {
    if(!writer.put_c('\n'))
      return false;
    for(;n>0; n--) {
      if(!writer.put_c(' '))
        return false;
    }
    return true;
  }
  inline bool delim(Node& n, int flags)
  {
    if(flags & sero_Compact) {
      if(!writer.put_c(' '))
        return false;
    } else {
      if(!skip(n.skip))
        return false;
    }
    return true;
  }
  bool writef(BStr fmt, ...) 
  {
    va_list va;
    char buf[256];
    va_start(va, fmt);
    int rv = vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);
    return rv>=0 && writer.put_n(buf, rv);
  }
public:
  RSerializeWriter(DOwner *own, const SParameters& args) 
  : refs(own), 
    writer(NULL), last_error(NULL), 
    root(Value::t_Undefined, 0), skip_step(2)
  {
    args.ref_get("stream", writer.stream);
    args.get("skip_step", skip_step);
  }
  ~RSerializeWriter(void) {
    writer.put_c('\n');
    writer.flush();
s_dbg.put(0, cl_Debug, "{SIZE %ld}", (long)writer.stream->writer_size());
  }
  //
  inline bool validate_construction(void) const { return writer.stream.valid(); }
  //
public:
};

//***************************************
// END
//***************************************
