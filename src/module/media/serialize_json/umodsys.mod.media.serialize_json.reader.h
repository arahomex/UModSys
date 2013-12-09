
//***************************************
// RSerializeReader::
//***************************************

struct RSerializeReader : public ISerializeReader {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RSerializeReader, 1, ISerializeReader)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  bool reader_parse(ISerializeParser* parser) {
    return false;
  }
protected:
public:
  RSerializeReader(DOwner *own, const SParameters& args) 
  : refs(own) {
  }
  ~RSerializeReader(void) {
  }
  //
  inline bool validate_construction(void) const { return true; }
public:
};

//***************************************
// END
//***************************************
