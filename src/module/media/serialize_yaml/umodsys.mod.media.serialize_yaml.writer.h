
//***************************************
// RSerializeWriter::
//***************************************

struct RSerializeWriter : public ISerializeWriter {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RSerializeWriter, 1, ISerializeWriter)
  UMODSYS_REFOBJECT_REFMODULE()
public:
public:
  BStr writer_lasterror(void) const { return last_error; }
  //
  bool put_value(const DCString& type, const Value& val, int flags) 
  {
    return false;
  }
  bool put_key(const DCString& type, const Value& val, int flags) 
  {
    return false;
  }
  bool put_begin(const DCString& type, Value::eType val, int flags) 
  {
    return false;
  }
  bool put_end(void) 
  {
    return false;
  }
protected:
  TTextStreamWriterBuf<BChar, 4096> writer;
  BStr last_error;
  //
public:
  RSerializeWriter(DOwner *own, const SParameters& args) 
  : refs(own), 
    writer(NULL), last_error(NULL)
//    root(Value::t_Undefined, 0), skip_step(2)
  {
    args.ref_get("stream", writer.stream);
//    args.get("skip_step", skip_step);
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
