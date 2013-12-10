#ifndef __UMODSYS_LIBMEDIA_SERIALIZE_H
#define __UMODSYS_LIBMEDIA_SERIALIZE_H 1
/*************************************************************/
// file: umodsys/lib/libmedia/serialize.h
// info: media streams
/*************************************************************/

#include <umodsys/lib/media/libmedia.typedefs.h>


namespace UModSys {
namespace libmedia {

//***************************************
// TYPES
//***************************************

struct SSerializeValue;

struct ISerializeHash;
struct ISerializeArray;

struct ISerializeReader;
struct ISerializeWriter;

enum eSerializeOptions {
  sero_Compact = 0x0001,
  sero_Strict  = 0x0002,
  sero_Expand  = 0x0004,
  sero_Naked   = 0x0010,
  sero_Default = 0x0000
};

//***************************************
// SSerializeValue::

struct SSerializeValue {
  typedef SSerializeValue Value;
  typedef ISerializeHash  IHash;
  typedef ISerializeArray IArray;
  //
  enum eType {
    t_Null,
    t_Bool,
    t_Long,
    t_Double,
    t_String,
    t_Hash,
    t_Array,
    t_Undefined = 0x7f
  };
  struct Str {
    BStr p;
    size_t l;
    //
    inline void operator=(const tl::su::TSCoreConst<BChar>& s) { p=s.text; l=s.length; }
    inline void operator=(BStr s) { p=s; l=tl::su::slen(s); }
    inline DCString get(void) const { return DCString(p, l); }
  };
  union UValue {
    bool b;
    long l;
    double d;
    Str s;
    IHash *h;
    IArray *a;
  };
public:
  eType type;
  UValue value;
  //
  inline SSerializeValue(void) : type(t_Null) {}
  inline explicit SSerializeValue(eType t) : type(t) {}
  inline explicit SSerializeValue(eType t, bool val) : type(t_Bool) { (void)t; value.b = val; }
  inline SSerializeValue(int val) : type(t_Long) { value.l = val; }
  inline SSerializeValue(long val) : type(t_Long) { value.l = val; }
  inline SSerializeValue(double val) : type(t_Double) { value.d = val; }
  inline SSerializeValue(const DCString& val) : type(t_String) { value.s = val; }
  inline SSerializeValue(BStr val) : type(t_String) { value.s = val; }
  inline SSerializeValue(IHash* val) : type(t_Hash) { value.h = val; }
  inline SSerializeValue(IArray* val) : type(t_Array) { value.a = val; }
  //
  inline static Value s_null(void) { return Value(); }
  inline static Value s_true(void) { return Value(Value::t_Bool, true); }
  inline static Value s_false(void) { return Value(Value::t_Bool, false); }
  inline static Value s_bool(bool v) { return Value(Value::t_Bool, v); }
};

//***************************************
// INTERFACES
//***************************************

//***************************************
// ISerializeArray::

struct ISerializeArray
{
  typedef SSerializeValue Value;
  //
  virtual size_t array_get_length(void) const =0;
  virtual bool array_get_value(size_t id, Value &val) const =0;
};

//***************************************
// ISerializeArray::

struct ISerializeHash
{
  typedef SSerializeValue Value;
  //
  virtual size_t hash_get_length(void) const =0;
  virtual bool hash_get_keyvalue(size_t id, Value* key, Value* val) const =0;
};

//***************************************
// ISerizlizeParser::

struct ISerializeParser
{
  typedef SSerializeValue Value;
  //
  virtual bool put_value(const DCString& type, const Value& val, int flags) =0; // write value for hash
  virtual bool put_key(const DCString& type, const Value& val, int flags) =0;   // write key for hash
  virtual bool put_begin(const DCString& type, Value::eType val, int flags) =0; // begin hash/array
  virtual bool put_end(void) =0; // end hash/array
  //
  inline static Value s_null(void) { return Value(); }
  inline static Value s_true(void) { return Value(Value::t_Bool, true); }
  inline static Value s_false(void) { return Value(Value::t_Bool, false); }
  inline static Value s_bool(bool v) { return Value(Value::t_Bool, v); }
};


//***************************************
// ISerializeReader::

struct ISerializeReader
: public IRefObject
{
  typedef SSerializeValue Value;
  //
  virtual BStr reader_lasterror(void) const =0;
  virtual bool reader_parse(ISerializeParser* parser) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ISerializeReader, 1, IRefObject);
};

//***************************************
// ISerializeWriter::

struct ISerializeWriter
: public IRefObject, public ISerializeParser
{
public:
  virtual BStr writer_lasterror(void) const =0;
  //
  inline bool writer_value(const DCString& type, const Value& val, int flags=sero_Default) 
    { return put_value(type, val, flags); }
  inline bool writer_key(const DCString& type, const Value& val, int flags=sero_Default)
    { return put_key(type, val, flags); }
  inline bool writer_begin(const DCString& type, Value::eType val, int flags=sero_Default)
    { return put_begin(type, val, flags); }
  inline bool writer_end(void)
    { return put_end(); }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ISerializeWriter, 1, IRefObject);
};

//***************************************
// END
//***************************************

} // namespace libmedia
} // namespace UModSys

#endif // __UMODSYS_LIBMEDIA_SERIALIZE_H
