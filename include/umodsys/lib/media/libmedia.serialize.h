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
  union UValue {
    bool b;
    long l;
    double d;
    char _s[sizeof(DCString)];
    IHash *h;
    IArray *a;
    //
    DCString& s(void) { return *reinterpret_cast<DCString*>(_s); }
    const DCString& s(void) const { return *reinterpret_cast<const DCString*>(_s); }
  };
public:
  eType type;
  UValue value;
  //
  inline SSerializeValue(void) : type(t_Null) {}
  inline explicit SSerializeValue(eType t) : type(t) {}
  inline explicit SSerializeValue(bool val) : type(t_Bool) { value.b = val; }
  inline SSerializeValue(int val) : type(t_Long) { value.l = val; }
  inline SSerializeValue(long val) : type(t_Long) { value.l = val; }
  inline SSerializeValue(double val) : type(t_Double) { value.d = val; }
  inline SSerializeValue(const DCString& val) : type(t_String) { value.s() = val; }
  inline SSerializeValue(IHash* val) : type(t_Hash) { value.h = val; }
  inline SSerializeValue(IArray* val) : type(t_Array) { value.a = val; }
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
  virtual bool array_set_value(size_t id, const Value &val) =0;
};

//***************************************
// ISerializeArray::

struct ISerializeHash
{
  typedef SSerializeValue Value;
  //
  virtual size_t hash_get_length(void) const =0;
  virtual bool hash_get_keyvalue(size_t id, Value* key, Value* val) const =0;
  virtual bool hash_set_keyvalue(size_t id, const Value& key, const Value &val) =0;
};

//***************************************
// ISerizlizeParser::

struct ISerializeParser
{
  typedef SSerializeValue Value;
  //
  virtual bool put_value(const DCString& type, const Value& val, int flags=0) =0; // write value for hash
  virtual bool put_key(const DCString& type, const Value& val, int flags=0) =0;   // write key for hash
  virtual bool put_begin(const DCString& type, Value::eType val, int flags=0) =0; // begin hash/array
  virtual bool put_end(void) =0; // end hash/array
};


//***************************************
// ISerializeReader::

struct ISerializeReader
: public IRefObject
{
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
  inline bool writer_value(const DCString& type, const Value& val, int flags=0) 
    { return put_value(type, val, flags); }
  inline bool writer_key(const DCString& type, const Value& val, int flags=0)
    { return put_key(type, val, flags); }
  inline bool writer_begin(const DCString& type, Value::eType val, int flags=0)
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
