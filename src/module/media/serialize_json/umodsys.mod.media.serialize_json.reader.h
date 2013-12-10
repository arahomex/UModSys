
//***************************************
// RSerializeReader::
//***************************************

struct RSerializeReader : public ISerializeReader {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RSerializeReader, 1, ISerializeReader)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  struct Node {
    Value::eType type;
    char kind;
    //
    inline Node(Value::eType t=Value::t_Null, char k='v') : type(t), kind(k) {}
  };
public:
  BStr reader_lasterror(void) const { return last_error; }
  //
  bool reader_parse(ISerializeParser* parser) 
  {
    if(parser==NULL)
      return err("No parser");
    tl::TDynarrayDynamic<Node> nodes;
    if(!nodes.Push(Node(Value::t_Undefined)))
      return err("Memory error");
    Node *n = &nodes.Last();
    int is_str = 0;
    tl::su::utf32 uc;
    while(!reader.eof()) {
      BChar ch = reader.get_c();
      if(is_str==1) { // string
        if(ch=='\"') {
          is_str = 0;
          continue;
        }
        if(ch=='\\') {
          is_str = 2;
          continue;
        }
        if(!gathered.Push(ch))
          return err("Memory error");
      } else if(is_str==2) {
        switch(ch) {
          case 'b':  ch = '\b'; break;
          case 'f':  ch = '\f'; break;
          case 'n':  ch = '\n'; break;
          case 'r':  ch = '\r'; break;
          case 't':  ch = '\t'; break;
          case '\\': ch = '\\'; break;
          case '/':  ch = '/'; break;
          case '\"': ch = '\"'; break;
          case 'u':  is_str = 3; uc = 0; continue;
          default: return err("Bad escape sequence");
        }
        if(!gathered.Push(ch))
          return err("Memory error");
        is_str = 1;
      } else if(is_str>2) {
        if(ch>='a' && ch<='f') {
          uc <<= 4; uc += 10+(ch-'a');
          is_str++; 
          if(!uni(uc, is_str))
            return false;
          continue;
        }
        if(ch>='A' && ch<='F') {
          uc <<= 4; uc += 10+(ch-'A');
          is_str++; 
          if(!uni(uc, is_str))
            return false;
          continue;
        }
        if(ch>='0' && ch<='9') {
          uc <<= 4; uc += (ch-'0');
          is_str++; 
          if(!uni(uc, is_str))
            return false;
          continue;
        }
        return err("Bad escape sequence");
      } else {
        if(ch>=0 && ch<=' ')
          continue; // space
        switch(ch) {
          case '{': {
            if(n->kind!='v')
              return err("Structure error");
            if(!nodes.Push(Node(Value::t_Hash)))
              return err("Memory error");
            n = &nodes.Last();
            if(!parser->put_begin(NULL, Value::t_Hash, 0))
              return err("Parser error");
          } continue;
          case '}': {
            if(n->type!=Value::t_Hash || n->kind=='k')
              return err("Structure error");
            if(n->kind!='v') {
              if(!emit(parser, n))
                return false;
            }
            if(!parser->put_end())
              return err("Parser error");
            if(!nodes.Pop())
              return err("Memory error");
            n = &nodes.Last();
          } continue;
          case '[': {
            if(n->kind!='v')
              return err("Structure error");
            if(!nodes.Push(Node(Value::t_Array)))
              return err("Memory error");
            n = &nodes.Last();
            if(!parser->put_begin(NULL, Value::t_Array, 0))
              return err("Parser error");
          } continue;
          case ']': {
            if(n->type!=Value::t_Array)
              return err("Structure error");
            if(n->kind!='v') {
              if(!emit(parser, n))
                return false;
            }
            if(!parser->put_end())
              return err("Parser error");
            if(!nodes.Pop())
              return err("Memory error");
            n = &nodes.Last();
          } continue;
          case ',': {
            if(n->kind=='k')
              return err("Structure error");
            if(n->kind!='v') {
              if(!emit(parser, n))
                return false;
            }
            n->kind = n->type==Value::t_Hash ? 'k' : 'v';
          } continue;
          case ':': {
            if(n->type!=Value::t_Hash)
              return err("Structure error");
            if(n->kind!='v') {
              if(!emit(parser, n, true))
                return false;
            }
            n->kind = 'v';
          } continue;
          case '\"': {
            if(n->kind!='v')
              return err("Structure error");
            is_str = 1;
            n->kind = 's';
            gathered.Clear();
          } continue;
          default: {
            if(n->kind!='v' && n->kind!='w')
              return err("Structure error");
            if(n->kind=='v')
              gathered.Clear();
            if((ch>='a' && ch<='z') || (ch>='A' && ch<='Z')
               || (ch>='0' && ch<='9') || ch=='-') {
              n->kind = 'w';
              if(!gathered.Push(ch))
                return err("Memory error");
            }
          } continue;
        }
      }
    }
    if(~nodes!=1/* || n->kind=='v'*/)
      return err("Structure error");
    return true;
  }
protected:
  TTextStreamReaderBuf<BChar, 4096> reader;
  tl::TDynarrayDynamic<BChar> gathered;
  BStr last_error;
  //
  inline bool uni(tl::su::utf32 uc, int &is_str)
  {
    if(is_str!=7)
      return true;
    tl::su::utf8 chs[tl::su::utf_max8len];
    size_t l = tl::su::utf_32to8(chs, tl::su::utf_max8len, uc);
    size_t p = gathered.LastIndex();
    if(!gathered.PushN(l))
      return err("Memory error");
    for(size_t i=0; i<l; i++) {
      gathered[p+i] = chs[i];
    }
    is_str = 1;
    return true;
  }
  inline bool emit(ISerializeParser* parser, Node* n, bool iskey=false)
  {
    if(!gathered.Push(0))
      return err("Memory error");
    if(n->kind=='s') {
      Value v(gathered.All());
      return (iskey ? parser->put_key(NULL, v, 0) : parser->put_value(NULL, v, 0)) || err("Parser error");
    }
    if(n->kind=='w') {
      if(iskey)
        return err("Structure error");
      DCString kw(gathered.All());
      if(kw=="true")
        return parser->put_value(NULL, Value::s_true(), 0) || err("Parser error");
      if(kw=="false")
        return parser->put_value(NULL, Value::s_false(), 0) || err("Parser error");
      if(kw=="null")
        return parser->put_value(NULL, Value::s_null(), 0) || err("Parser error");
      BChar* ep;
      long lv = strtol(kw(), &ep, 10);
      if(*ep==0)
        return parser->put_value(NULL, Value(lv), 0) || err("Parser error");
      double dv = strtod(kw(), &ep);
      if(*ep==0)
        return parser->put_value(NULL, Value(dv), 0) || err("Parser error");
      return err("Value error");
    }
    return err("Structure error");
  }
  //
  bool err(BStr errcode) 
  { 
    last_error = errcode; 
    return false; 
  }
public:
  RSerializeReader(DOwner *own, const SParameters& args) 
  : refs(own), 
    reader(NULL), last_error(NULL)
  {
    args.get("stream", reader.stream);
    int bufsize = 1024;
    args.get("buffer_size", bufsize);
    gathered.Reserve(bufsize);
  }
  ~RSerializeReader(void) {
  }
  //
  inline bool validate_construction(void) const { return reader.stream.valid(); }
public:
};

//***************************************
// END
//***************************************
