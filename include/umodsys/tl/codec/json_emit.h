#ifndef __UMODSYS_TL_EMIT_JSON_H
#define __UMODSYS_TL_EMIT_JSON_H 1
/*************************************************************/
// file: umodsys/tl/codec/json_parse.h
// info: Generator of JSON
/*************************************************************/

#include <umodsys/common/stdtypedefs.h>
#include <umodsys/tl/util/res_lock.h>

namespace UModSys {
namespace tl {

/*************************************************************/

struct SJSON_Emit_Base;

template<typename Writer, typename StateArray> struct TJSON_Emit;


/*
  Writer must have inside:
    bool write_char(char ch);
    bool write_chars(const char *chs, size_t len);
    
  StateArray must support normal dynarray functions, type is at least byte
*/

/*************************************************************/

struct SJSON_Emit_Base {
  enum eError {
    eNoError,
    eWriteError,
    eInvalidSequence,
    eMemoryError,
    eUnknown,
  };
  //
  enum eState {
    sNone        = 0x01,
    sValue       = 0x02,
    sArray       = 0x04,
    sArrayValue  = 0x08, 
    sObject      = 0x10,
    sObjectKey   = 0x20,
    sObjectValue = 0x40,
    sError       = 0x80,
    //
    sMValue      = sNone | sArray | sArrayValue | sObjectKey,
    sMKey        = sObject | sObjectValue,
  };
  //
  struct Error {
    eError errorcode;
    int auxkey;
    //
    Error(eError e, int a) : errorcode(e), auxkey(a) {}
  };
  //
  typedef core::Bsint64 Dint;
  typedef core::Buint64 Duint;
  typedef core::Bfloat64 Dfloat;
};

/*************************************************************/

template<typename Writer, typename StateArray>
struct TJSON_Emit : SJSON_Emit_Base {
  struct Object;
  struct Array;
  //
  friend struct Object;
  friend struct Array;
  //
//  typedef SJSON_Emit_Base::eError eError;
//  typedef SJSON_Emit_Base::eState eState;
  //
  typedef TResLock<const Object> DObject;
  typedef TResLock<const Array> DArray;
  //
  typedef Writer DWriter;
  typedef StateArray DStateArray;
  typedef TJSON_Emit<Writer, StateArray> Self;
  //
  struct Array {
    Self& generator;
    mutable int idx;
    //
    Array(Self& gen, int i) : generator(gen), idx(i) {}
    Array(const Array& R) : generator(R.generator), idx(R.idx) { R.idx = -1; }
    ~Array(void) {}
    //
    void res_lock(void) const { if(idx<0) generator.raw_error(eInvalidSequence, __LINE__); }
    void res_unlock(void) const { 
      if(generator.last_error.errorcode!=eNoError)
        return; // do nothing
      end();
    }
    void end(void) const { if(idx>=0) { generator.ctx_end(idx); idx=-1; } }
    //
    void null(bool value) const { generator.ctx_val_null(idx); }
    void boolv(bool value) const { generator.ctx_val_bool(idx, value); }
    void numf(Dfloat value) const { generator.ctx_val_numf(idx, value); }
    void nums(Dint value) const { generator.ctx_val_nums(idx, value); }
    void numu(Duint value) const { generator.ctx_val_numu(idx, value); }
    void str(const char *value, size_t len) const { generator.ctx_val_str(idx, value, len); }
    void str(const char *value) const { generator.ctx_val_str(idx, value); }
    //
    Array arr(void) const { return Array(generator, generator.ctx_arr(idx)); }
    Object obj(void) const;
  };
  //
  struct Object : Array {
    Object(Self& gen, int i) : Array(gen, i) {}
    Object(const Object& R) : Array(R) {}
    //
    void key(const char *value, size_t len) const { Array::generator.ctx_key(Array::idx, value, len); }
    void key(const char *value) const { Array::generator.ctx_key(Array::idx, value); }
  };
  //
protected:
  Error last_error;
  Writer& writer;
  StateArray stack;
public:
  int padq;
  TJSON_Emit(Writer* wr, int pad=0) : writer(*wr), padq(pad), last_error(eNoError, 0) {}
  TJSON_Emit(Writer& wr, int pad=0) : writer(wr),  padq(pad), last_error(eNoError, 0) {}
  ~TJSON_Emit(void) {}
  //
public: // raw functions
  void raw_error(eError code, int aux=0);
  //
  void raw_pad(int len);
  void raw_number_s(Dint value);
  void raw_number_u(Duint value);
  void raw_number_f(Dfloat value);
  void raw_string(const char *value, size_t vlen);
  void raw_string(const char *value) { raw_string(value, su::slen(value)); }
  //
  void raw_atom(const char *text, size_t len) { if(!writer.write_chars(text, len)) raw_error(eWriteError, __LINE__); }
  void raw_atom(const char *text)             { raw_atom(text, su::slen(text)); }
  void raw_atom(char ch)                      { if(!writer.write_char(ch)) raw_error(eWriteError, __LINE__); }
  //
  void raw_null(void)         { raw_atom("null", 4); }
  void raw_bool(bool value)   { value ? raw_atom("true", 4) : raw_atom("false", 5); }
  void raw_object_begin(void) { raw_atom('{'); }
  void raw_object_end(void)   { raw_atom('}'); }
  void raw_array_begin(void)  { raw_atom('['); }
  void raw_array_end(void)    { raw_atom(']'); }
  void raw_delim_value(void)  { raw_atom(','); }
  void raw_delim_key(void)    { raw_atom(':'); }
protected: // ctx functions
  void ctx_val_null(int idx) { ctx_pre(idx, sMValue); raw_null(); ctx_post(idx); }
  void ctx_val_bool(int idx, bool value) { ctx_pre(idx, sMValue); raw_bool(value); ctx_post(idx); }
  void ctx_val_numf(int idx, Dfloat value) { ctx_pre(idx, sMValue); raw_number_f(value); ctx_post(idx); }
  void ctx_val_numu(int idx, Duint value) { ctx_pre(idx, sMValue); raw_number_u(value); ctx_post(idx); }
  void ctx_val_nums(int idx, Dint value) { ctx_pre(idx, sMValue); raw_number_s(value); ctx_post(idx); }
  void ctx_val_str(int idx, const char *value, size_t len) { ctx_pre(idx, sMValue); raw_string(value, len); ctx_post(idx); }
  void ctx_val_str(int idx, const char *value) { ctx_pre(idx, sMValue); raw_string(value); ctx_post(idx); }
  //
  void ctx_key(int idx, const char *value, size_t len) { ctx_pre(idx, sMKey); raw_string(value, len); ctx_post(idx, true); }
  void ctx_key(int idx, const char *value) { ctx_pre(idx, sMKey); raw_string(value); ctx_post(idx, true); }
  //
  int  ctx_obj(int idx)  { ctx_pre(idx, sMValue); raw_object_begin(); ctx_post(idx); return ctx_push(idx, sObject); }
  int  ctx_arr(int idx)  { ctx_pre(idx, sMValue); raw_array_begin(); ctx_post(idx); return ctx_push(idx, sArray); }
  void ctx_end(int idx);
  //
  void ctx_pre(int idx, int state);
  void ctx_post(int idx, bool isKey=false);
  int  ctx_push(int idx, int state);
  void ctx_init(void);
public: // ctx public functions
  Object obj(void) { ctx_init(); raw_object_begin(); return Object(*this, ctx_push(0, sObject)); }
  Array arr(void) { ctx_init(); raw_array_begin(); return Object(*this, ctx_push(0, sArray)); }
};

/*************************************************************/

template<typename Writer, typename StateArray>
void TJSON_Emit<Writer, StateArray>::raw_error(eError code, int aux)
{
  last_error.errorcode = code;
  last_error.auxkey = aux;
  throw last_error;
}

template<typename Writer, typename StateArray>
void TJSON_Emit<Writer, StateArray>::raw_pad(int len)
{
  raw_atom('\n');
  for(int i=0; i<len; i++) {
    raw_atom(' ');
  }
}

template<typename Writer, typename StateArray>
void TJSON_Emit<Writer, StateArray>::raw_number_s(SJSON_Emit_Base::Dint value)
{
  char buf[128];
  sprintf(buf, "%lld", value);
  raw_atom(buf);
}

template<typename Writer, typename StateArray>
void TJSON_Emit<Writer, StateArray>::raw_number_u(SJSON_Emit_Base::Duint value)
{
  char buf[128];
  sprintf(buf, "%llu", value);
  raw_atom(buf);
}

template<typename Writer, typename StateArray>
void TJSON_Emit<Writer, StateArray>::raw_number_f(SJSON_Emit_Base::Dfloat value)
{
  char buf[128];
  sprintf(buf, "%g", value);
  raw_atom(buf);
}

template<typename Writer, typename StateArray>
void TJSON_Emit<Writer, StateArray>::raw_string(const char *value, size_t len)
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

template<typename Writer, typename StateArray>
inline void TJSON_Emit<Writer, StateArray>::ctx_pre(int idx, int state)
{
  if(idx<0 || idx!=(~stack-1)) raw_error(eInvalidSequence, __LINE__);
  int ss = stack[idx];
  if((ss & state)!=ss) raw_error(eInvalidSequence, __LINE__);
  //
  if((ss & (sArrayValue | sObjectValue))) {
    raw_delim_value();
  } else if((ss & (sObjectKey))) {
    raw_delim_key();
  }
  if(padq) {
    if((ss & (sObject | sArray | sArrayValue | sObjectValue))) {
      raw_pad(padq * ~stack);
    }
  }
}

template<typename Writer, typename StateArray>
inline void TJSON_Emit<Writer, StateArray>::ctx_post(int idx, bool isKey)
{
  int ss = stack[idx];
  if(ss & (sObject | sObjectKey | sObjectValue)) {
    stack[idx] = isKey ? sObjectKey : sObjectValue;
  } else if(ss & (sArray | sArrayValue)) {
    stack[idx] = sArrayValue;
  } else {
    stack[idx] = sValue;
  }
}

template<typename Writer, typename StateArray>
inline int TJSON_Emit<Writer, StateArray>::ctx_push(int idx, int state)
{
  if(!stack.Push(state)) raw_error(eMemoryError, __LINE__);
  return int(~stack-1);
}

template<typename Writer, typename StateArray>
inline void TJSON_Emit<Writer, StateArray>::ctx_end(int idx)
{
  if(idx<0 || idx!=(~stack-1)) raw_error(eInvalidSequence, __LINE__);
  int ss = stack[idx];
  if(ss & sArray) {
    if(!stack.Pop()) raw_error(eMemoryError, __LINE__);
    raw_array_end();
  } else if(ss & sArrayValue) {
    if(!stack.Pop()) raw_error(eMemoryError, __LINE__);
    if(padq) raw_pad(padq * ~stack);
    raw_array_end();
  } else if(ss & sObject) {
    if(!stack.Pop()) raw_error(eMemoryError, __LINE__);
    raw_object_end();
  } else if(ss & sObjectValue) {
    if(!stack.Pop()) raw_error(eMemoryError, __LINE__);
    if(padq) raw_pad(padq * ~stack);
    raw_object_end();
  } else {
    raw_error(eInvalidSequence, __LINE__);
  }
}

template<typename Writer, typename StateArray>
inline void TJSON_Emit<Writer, StateArray>::ctx_init(void)
{
  if(~stack!=0) raw_error(eInvalidSequence, __LINE__);
}

/*************************************************************/

template<typename Writer, typename StateArray>
inline typename TJSON_Emit<Writer, StateArray>::Object TJSON_Emit<Writer, StateArray>::Array::obj(void) const
{
  return Object(generator, generator.ctx_obj(idx));
}


/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_EMIT_JSON_H
