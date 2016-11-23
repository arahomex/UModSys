#ifndef __UMODSYS_TL_PARSER_JSON_H
#define __UMODSYS_TL_PARSER_JSON_H 1
/*************************************************************/
// file: umodsys/tl/parser/json_parse.h
// info: Parser of JSON
/*************************************************************/

#include <umodsys/common/stdtypedefs.h>

namespace UModSys {
namespace tl {

/*************************************************************/

struct SJSON_Parse_Base;

template<typename Reader, typename Consumer, typename StateArray> struct TJSON_Parse;

/*
  Reader must have inside default text stream functions:
    bool eof(void);
    bool get_c(void);

  Consumer must have inside functions:
    bool consume_void(Self& sender);
    bool consume_bool(Self& sender, bool value);
    bool consume_nums(Self& sender, Dint value);
    bool consume_numu(Self& sender, Duint value);
    bool consume_numf(Self& sender, Dfloat value);
    bool consume_string(Self& sender, const char *value, size_t len);
    bool consume_array(Self& sender); // array follows
    bool consume_object(Self& sender); // object follows
    bool consume_key(Self& sender, const char *value, size_t len);
    bool consume_arrayend(Self& sender); // array terminated
    bool consume_objectend(Self& sender); // object terminated

  StateArray must support normal dynarray functions, type is at least byte
*/


/*************************************************************/

struct SJSON_Parse_Base {
  //
  typedef core::Bsint64 Dint;
  typedef core::Buint64 Duint;
  typedef core::Bfloat64 Dfloat;
};

/*************************************************************/

template<typename Writer, typename Consumer, typename StateArray>
struct TJSON_Parse : SJSON_Parse_Base {
public:
protected:
};

/*************************************************************/


} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_PARSER_TCL_H
