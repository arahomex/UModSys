#ifndef __UMODSYS_TL_PARSER_TCL_H
#define __UMODSYS_TL_PARSER_TCL_H 1
/*************************************************************/
// file: umodsys/tl/parser/parser_tcl.h
// info: Parser of TCL-like languages
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>

namespace UModSys {
namespace tl {

/*************************************************************/

template<typename Collector> struct TParser_TCL;

/*************************************************************/

template<typename Collector>
struct TParser_TCL {
public:
  typedef typename Collector::StringP StringP;
  typedef TParser_TCL<Collector> Self;
  //
  enum eToken {
    tLine        =0, // '\n' ';'
    tSeparator   =2, // ' '
    tString      =3, // '\"' .. '\"'
    tEnd         =5, // end of stream
    tError       =6, // error in syntax
    tErrorResult =7, // error in execute command, can be even more
  };
  //
  StringP p, e;
  eToken token, ptoken;
  bool isnested;
  //
  TParser_TCL(StringP begin, StringP end, bool ins = false) {
    p = begin;
    e = end;
    token = ptoken = tLine;
    isnested = ins;
  }
  //
  eToken Parse(Collector& c) {
    c.parse_start();
    for(ptoken=token; true; ptoken=token) {
      switch(nextToken(c)) {
        case tSeparator:
          if(ptoken==tString)
            c.next_arg();
          continue;
        case tLine:
          if(ptoken==tString || c.stream_size())
            c.next_arg();
          if(!c.execute())
            return token = tErrorResult;
          continue;
        case tEnd:
          if(ptoken==tString || c.stream_size())
            c.next_arg();
          if(!c.execute())
            return token = tErrorResult;
          return token;
        case tError:
          return token;
        case tErrorResult:
          return token;
      }
    }
  }
  //
  eToken nextToken(Collector &c) {
    eToken rv = _nextToken(c);
    c.next_token(rv);
    return rv;
  }
  eToken _nextToken(Collector &c) {
    if(token>=tEnd)
      return token;
    if(p==e)
      return (token=tEnd);
    //
    switch(*p) {
      case ' ': case '\t': case '\r':
        return token = ptsSeparator(c);
      case '\n': case ';':
        return token = ptsLine(c);
      case '[':
        return token = ptsCommand(c);
      case ']':
        if(isnested) {
          *p++;
          return tEnd;
        }
        return token = ptsString(c);
      case '$':
        return token = ptsVar(c);
      case '#':
        if(token==tLine)
          return token = ptsComment(c);
        return token = ptsString(c);
      default:
        return token = ptsString(c);
    }
    //
    return token;
  }
  //
  bool ptsStringBrace(StringP& a, StringP& b) {
      int level = 1;
      a = ++p;
      if(p==e) 
        return false;
      for(; true; ++p) {
        if(p==e || *p=='}') {
          level--;
          if (level == 0 || p==e) {
            b = p;
            if(p!=e)
              ++p;
            break;
          }
        } else if(*p=='\\') {
          if(++p==e)
            return false;
          // c.add(p2, p-1); p2 = p;
        } else if (*p=='{') {
          level++;
        }
      }
      return true;
  }
  eToken ptsString(Collector &c) {
    bool newword = (token == tSeparator || token == tLine || token == tString);
    bool quote = false;
    StringP p2;
    if (newword && *p == '{') {
      StringP pe;
      if(!ptsStringBrace(p2, pe))
        return tError;
      c.add(p2, pe);
      return tString;
    } else if (newword && *p == '"') {
      quote = true;
      if(++p==e)
        return tError;
    }
    eToken rv;
    p2 = p;
    for(; true; ++p) {
        if (p==e) {
          c.add(p2, p);
          return tString;
        }
        switch(*p) {
          case '\\':
            c.add(p2, p);
            if(++p==e)
              return tError;
            else {
              int idx = 0;
              while(c.add_esc(p, idx)) {
                if(p+idx==e)
                  return tError;
              }
              p = p+idx;
            }
            p2 = p+1;
            break;
          case '$': 
            c.add(p2, p);
            rv = ptsVar(c);
            if(rv>tEnd)
              return rv;
            p2 = p--;
            break;
          case '[':
            c.add(p2, p);
            rv = ptsCommand(c);
            if(rv>tEnd)
              return rv;
            p2 = p;
            break;
          case ' ': case '\t': case '\n': case '\r': case ';':
            if (!quote) {
              c.add(p2, p);
              return tString;
            }
            break;
          case '"':
            if (quote) {
              c.add(p2, p++);
              return tString;
            }
            break;
        }
    }
  }
  eToken ptsCommand(Collector &c) {
    if(++p==e)
      return tError;
    return c.exec_command(*this) ? tString : tError;
  }
  eToken ptsVar(Collector &c) {
    StringP p2 = ++p, pe;
    if(p!=e) {
      if(*p=='{') {
        if(!ptsStringBrace(p2, pe))
          return tError;
        c.add_var(p2, pe);
        return tString;
      }
      while(1) {
        if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_') {
          if(++p!=e)
            continue;
        }
        break;
      }
    }
    if(p2 == p) { /* It's just a single char string "$" */
      c.add('$');
      return tString;
    } else {
      c.add_var(p2, p);
      return tString;
    }
  }
  eToken ptsComment(Collector &c) {
    StringP p2 = ++p;
    while(p!=e && *p != '\n') {
      ++p;
    }
    c.add_cmt(p2, p);
    return tLine;
  }
  eToken ptsLine(Collector &c) {
    while(*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' || *p == ';') {
      if(++p==e)
        break;
    }
    return tLine;
  }
  eToken ptsSeparator(Collector& c) {
    while(*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
      if(++p==e)
        break;
    }
    return tSeparator;
  }
  //
};

/*************************************************************/


} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_PARSER_TCL_H
