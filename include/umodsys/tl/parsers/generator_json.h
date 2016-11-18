#ifndef __UMODSYS_TL_GENERATOR_JSON_H
#define __UMODSYS_TL_GENERATOR_JSON_H 1
/*************************************************************/
// file: umodsys/tl/parser/generator_json.h
// info: Generator of JSON
/*************************************************************/

#include <umodsys/common/stdtypedefs.h>

namespace UModSys {
namespace tl {

/*************************************************************/

template<typename Writer> struct TGenerator_JSON;


/*
  Writer must have inside:
    bool write_char(char ch);
    bool write_chars(const char *chs, size_t len);
*/


/*************************************************************/

template<typename Writer>
struct TGenerator_JSON {
  typedef Writer DWriter;
  typedef TGenerator_JSON<Writer> Self;
  //
  enum eError {
    eNoError,
    eWriteError,
    eInvalidSequence,
    eUnknown
  };
  //
  enum eType {
    tUndefined,
    tNull,
    tBool,
    tNumberInt,
    tNumberFloat,
    tString,
    tObject,
    tObjectKey,
    tArray
  };
  //
/*
  struct Context {
    Self& generator;
    Context* up;
    volatile eType type;
    //
    Context(Self* pthis) : generator(*pthis), up(NULL), type(tUndefined) {}
    Context(Context* aup, eType atype) : generator(up.generator), up(aup), type(atype) {}
    Context(const Context& R) : generator(R.generator), up(R.up), type(R.type) { R.type = tUndefined; }
    ~Context(void) { end(); }
    //
    Context begin_object(void) {
      if(type!=tUndefined) generator.err
    }
    //
    void number(core::Bsint64 value) { raw_number
  };
*/
  //
  eError errorcode;
  Writer& writer;
//  Context top;
//  Context* current;
  int padq;
  int pad_level;
  bool need_pad;
  //
//  TGenerator_JSON(Writer* wr) : top(this) { current = &top; padq=0; }
//  TGenerator_JSON(Writer& wr) : top(this) { current = &top; padq=0; }
  TGenerator_JSON(Writer* wr) : writer(*wr) { padq=0; need_pad=false; }
  TGenerator_JSON(Writer& wr) : writer(wr)  { padq=0; need_pad=false; }
  ~TGenerator_JSON(void) {}
  //
//  Context start_object(void) { return top.begin_object(); }
//  Context start_array(void) { return top.start_array(); }
  //
  void raw_error(eError code);
  //
  void raw_pad(int len);
  void raw_number_s(core::Bsint64 value);
  void raw_number_u(core::Buint64 value);
  void raw_number_f(core::Bfloat64 value);
  void raw_string(const char *value, size_t vlen);
  void raw_string(const char *value) { raw_string(value, su::slen(value)); }
  //
  void raw_atom(const char *text, size_t len) { if(!writer.write_chars(text, len)) raw_error(eWriteError); }
  void raw_atom(const char *text) { raw_atom(text, su::slen(text)); }
  void raw_atom(char ch) { if(!writer.write_char(ch)) raw_error(eWriteError); }
  void raw_void(bool value) { value ? raw_atom("true", 4) : raw_atom("false", 5); }
  void raw_null(void) { raw_atom("null", 4); }
  void raw_object_begin(void) { raw_atom('{'); raw_pad_inc(); need_pad=false; }
  void raw_object_end(void) { if(need_pad) raw_pad(); raw_atom('}'); raw_pad_dec(); }
  void raw_array_begin(void) { raw_atom('['); raw_pad_inc(); }
  void raw_array_end(void) { if(need_pad) raw_pad(); raw_atom(']'); raw_pad_dec(); }
  void raw_delim_value(void) { raw_atom(','); raw_pad_dec(); }
  void raw_delim_key(void) { raw_atom(':'); }
  void raw_pad(void) { raw_pad(pad_level); }
  void raw_pad_inc(void) { if(padq) raw_pad(pad_level += padq); need_pad=false; }
  void raw_pad_dec(void) { if(padq) raw_pad(pad_level -= padq); }
};

/*************************************************************/

template<typename Writer>
void TGenerator_JSON<Writer>::raw_error(eError code)
{
  errorcode = code;
  throw code;
}

template<typename Writer>
void TGenerator_JSON<Writer>::raw_pad(int len)
{
  raw_atom('\n');
  for(int i=0; i<len; i++) {
    raw_atom(' ');
  }
}

template<typename Writer>
void TGenerator_JSON<Writer>::raw_number_s(core::Bsint64 value)
{
  char buf[128];
  sprintf(buf, "%lld", value);
  raw_atom(buf);
}

template<typename Writer>
void TGenerator_JSON<Writer>::raw_number_u(core::Buint64 value)
{
  char buf[128];
  sprintf(buf, "%llu", value);
  raw_atom(buf);
}

template<typename Writer>
void TGenerator_JSON<Writer>::raw_number_f(core::Bfloat64 value)
{
  char buf[128];
  sprintf(buf, "%g", value);
  raw_atom(buf);
}

template<typename Writer>
void TGenerator_JSON<Writer>::raw_string(const char *value, size_t len)
{
  raw_atom('\"');
  for(int i=0; i<len; i++) {
    char ch = value[i];
    switch(ch) {
      case '\"': raw_atom("\\\"", 2); break;
      case '\\': raw_atom("\\/", 2); break;
      case '/':  raw_atom("\\\\", 2); break;
      case '\b': raw_atom("\\\b", 2); break;
      case '\f': raw_atom("\\\f", 2); break;
      case '\n': raw_atom("\\\n", 2); break;
      case '\r': raw_atom("\\\r", 2); break;
      case '\t': raw_atom("\\\t", 2); break;
      default:   raw_atom(ch); break;
    }
  }
  raw_atom('\"');
}

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_PARSER_TCL_H
