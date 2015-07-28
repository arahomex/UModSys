
//***************************************
// RSerializeReader::
//***************************************

struct RSerializeReader : public ISerializeReader {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RSerializeReader, 1, ISerializeReader)
  UMODSYS_REFOBJECT_REFMODULE()
public:
public:
  BStr reader_lasterror(void) const { return last_error; }
  //
  bool reader_parse(ISerializeParser* parser) 
  {
    if(parser==NULL)
      return err("No parser");
    return false;
  }
protected:
  TTextStreamReaderBuf<BChar, 4096> reader;
//  tl::TDynarrayDynamic<BChar> gathered;
  BStr last_error;
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
    args.ref_get("stream", reader.stream);
    int bufsize = 1024;
    args.get("buffer_size", bufsize);
//    gathered.Reserve(bufsize);
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
