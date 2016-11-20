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
      dbg_put(rsdl_SysTests, "JSON[%d]:\n%s\n/JSON.", int(wr.p-buf), buf);
    }
    {
      Writer wr(buf, sizeof(buf)-1);
      Generator gen(wr, 2);
      //
      {
        Generator::DObject obj(gen.obj());
        obj->key("array");
        {
          Generator::DArray arr(obj->arr());
          arr->str("value1");
          arr->str("value2");
          arr->nums(100);
        }
      }
      //
      *wr.p = 0;
      //
      dbg_put(rsdl_SysTests, "JSON[%d]:\n%s\n/JSON.", int(wr.p-buf), buf);
    }
  } catch(Generator::eError ge) {
    dbg_put(rsdl_SysTests, "JSON end with Generator::eError %d\n", ge);
    return false;
//  } catch(Parser::eError) {
  }
  //
  dbg_put(rsdl_SysTests, "JSON end.\n");
  return true;
}

//***************************************
// ::
//***************************************
