//***************************************
// RSystem::
//***************************************

namespace UModSys {
  namespace kernel_test {
    namespace json {

      struct Writer {
        char *p;
        size_t s;
        //
        Writer(char* ap, size_t as) : p(ap), s(as) {}
        bool write_char(char ch) { if(s==0) return false; *p++=ch; s--; return true; }
        bool write_chars(const char *chs, size_t len) { if(s<len) return false; memcpy(p, chs, len); s-=len; p+=len; return true; }
      };

      typedef tl::TJSON_Emit<Writer, tl::TDynarrayStatic<BByte, 1024> > Generator;
    }
  }
}

bool RSystem::exec_test_json(void)
{
#define OP(x) dbg_put(rsdl_SysTests, "%s...", #x); x; dbg_put(rsdl_SysTests, "ok\n")
  using namespace kernel_test::json;
  char buf[1024];
  //
  dbg_put(rsdl_SysTests, "JSON begin:\n");
  //
  try {
    {
      Writer wr(buf, sizeof(buf)-1);
      Generator gen(wr);
      //
//      gen.raw_string(buf, 1024);
      gen.raw_object_begin();
       gen.raw_string("array");
        gen.raw_delim_key();
       gen.raw_array_begin();
        gen.raw_string("value1");
        gen.raw_delim_value();
        gen.raw_string("value2");
        gen.raw_delim_value();
        gen.raw_number_s(100);
       gen.raw_array_end();
      gen.raw_object_end();
      //
      *wr.p = 0;
      //
      dbg_put(rsdl_SysTests, "JSON[%d]:\n%s\n/JSON.\n", int(wr.p-buf), buf);
    }
    {
      Writer wr(buf, sizeof(buf)-1);
      Generator gen(wr, 2);
      //
      {
        OP(Generator::DObject obj(gen.obj()));
        OP(obj->key("key"));
        OP(obj->str("value"));
        OP(obj->key("array"));
        {
          OP(Generator::DArray arr(obj->arr()));
          OP(arr->str("value1"));
          OP(arr->str("value2"));
          OP(arr->nums(100));
        }
        OP(obj->key("key2"));
        OP(obj->str("value2"));
      }
      //
      *wr.p = 0;
      //
      dbg_put(rsdl_SysTests, "JSON[%d]:\n%s\n/JSON.\n", int(wr.p-buf), buf);
    }
/*
  } catch(Generator::Error ge) {
    dbg_put(rsdl_SysTests, "JSON end with Generator::eError %d:%d\n", ge.errorcode, ge.auxkey);
    return false;
*/
//  } catch(Parser::eError) {
  } catch(bool dummy) {
  }
  //
  dbg_put(rsdl_SysTests, "JSON end.\n");
  return true;
#undef OP
}

//***************************************
// ::
//***************************************
