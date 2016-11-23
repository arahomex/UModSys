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
        bool put_c(char ch) { if(s==0) return false; *p++=ch; s--; return true; }
        bool put_n(const char *chs, size_t len) { if(s<len) return false; memcpy(p, chs, len); s-=len; p+=len; return true; }
      };

      typedef tl::TJSON_Emit<Writer, tl::TDynarrayStatic<BByte, 1024> > Generator;

      void test_one_pass(Writer &wr)
      {
        Generator gen(wr, 2);
        //
        {
          Generator::DObject obj(gen.obj());
          obj->key("key");
          obj->str("value");
          obj->key("array");
          {
            Generator::DArray arr(obj->arr());
            arr->str("value1");
            arr->str("value2");
            arr->nums(100);
          }
          obj->key("key2");
          obj->str("value2");
        }
        //
        *wr.p = 0;
      }
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
      dbg_put(rsdl_SysTests, "1.JSON[%d]:\n%s\n/JSON.\n", int(wr.p-buf), buf);
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
      dbg_put(rsdl_SysTests, "2.JSON[%d]:\n%s\n/JSON.\n", int(wr.p-buf), buf);
    }
    //
    {
#ifdef _DEBUG
      const int count = 256*1024;
#else
      const int count = 4096*1024;
#endif
      clock_t t1, t2;
      t1 = clock();
      size_t bytes = 0;
      for(int i=0; i<count; i++) {
        Writer wr(buf, sizeof(buf)-1);
        test_one_pass(wr);
        bytes += wr.p - buf;
      }
      t2 = clock();
      double dt = double(t2-t1)/CLOCKS_PER_SEC;
      //
      dbg_put(rsdl_SysTests, "3.JSON:\n%s\n/JSON. -- %.3f sec %.3fkOps/sec %.3fMb %.3fMb/sec\n", buf, dt, count/1e3/dt, bytes/1e6, bytes/1e6/dt);
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
