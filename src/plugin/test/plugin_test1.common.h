// RTest1_Shell::

void RTest1_Shell::dump_str(const char *s, size_t n)
{
  char bc[8];
  for(size_t i=0; i<n; i++) {
    unsigned char c = *s++;
    if(c<' ') {
      s_dbg.put(d_Shell, cl_Debug, "\\x%02x", c);
    } else if(c=='\\') {
      s_dbg.put(d_Shell, cl_Debug, "\\");
    } else if(c<0xc0) {
      s_dbg.put(d_Shell, cl_Debug, "%c", c);
    } else if((c & 0xe0)==0xc0) {
      if(i+1<n) {
        bc[0] = c; 
        bc[1] = *s++;
        bc[2] = 0;
        s_dbg.put(d_Shell, cl_Debug, "%s", bc);
      } else {
        s_dbg.put(d_Shell, cl_Debug, "\\x%02x", c);
      }
      i++;
    } else if((c & 0xf0)==0xe0) {
      if(i+2<n) {
        bc[0] = c; 
        bc[1] = *s++;
        bc[2] = *s++;
        bc[3] = 0;
        s_dbg.put(d_Shell, cl_Debug, "%s", bc);
      } else {
        s_dbg.put(d_Shell, cl_Debug, "\\x%02x", c);
      }
      i+=2;
    } else if((c & 0xf8)==0xf0) {
      if(i+3<n) {
        bc[0] = c; 
        bc[1] = *s++;
        bc[2] = *s++;
        bc[3] = *s++;
        bc[4] = 0;
        s_dbg.put(d_Shell, cl_Debug, "%s", bc);
      } else {
        s_dbg.put(d_Shell, cl_Debug, "\\x%02x", c);
      }
      i+=3;
    } else if((c & 0xfc)==0xf8) {
      if(i+4<n) {
        bc[0] = c; 
        bc[1] = *s++;
        bc[2] = *s++;
        bc[3] = *s++;
        bc[4] = *s++;
        bc[5] = 0;
        s_dbg.put(d_Shell, cl_Debug, "%s", bc);
      } else {
        s_dbg.put(d_Shell, cl_Debug, "\\x%02x", c);
      }
      i+=4;
    } else {
      if(i+5<n) {
        bc[0] = c; 
        bc[1] = *s++;
        bc[2] = *s++;
        bc[3] = *s++;
        bc[4] = *s++;
        bc[5] = *s++;
        bc[6] = 0;
        s_dbg.put(d_Shell, cl_Debug, "%s", bc);
      } else {
        s_dbg.put(d_Shell, cl_Debug, "\\x%02x", c);
      }
      i+=5;
    }
  }
}


struct RTest1_Shell::Aux {
  struct Prop : IPropAdapter {
    bool set_value(const SPropValueR& value) {
      const Bsint32 *iv;
      if(value.get_as(iv)) {
        s_dbg.put(d_Shell, cl_Info, "set {Bsint32 %d}\n", *iv);
        return true;
      }
      const float32 *fv;
      if(value.get_as(fv)) {
        s_dbg.put(d_Shell, cl_Info, "set {float32 %g}\n", *fv);
        return true;
      }
      s_dbg.put(d_Shell, cl_Info, "set {?'%s' %p:%p}\n", value.id->name, value.value.data, value.value.size);
      return false;
    }
    bool get_value(const SPropValueW& value) const {
      return false;
    }
  };
  struct PropX : IPropIdAdapter {
    bool set_value(HUniquePointer id, const SPropValueR& value) {
      if(id==tl::TConstUniqueID<eConsoleLevels, cl_Info>::get_id()) {
        const Bsint32 *iv;
        if(value.get_as(iv)) {
          s_dbg.put(d_Shell, cl_Info, "info::set {Bsint32 %d}\n", *iv);
          return true;
        }
        const float32 *fv;
        if(value.get_as(fv)) {
          s_dbg.put(d_Shell, cl_Info, "info::set {float32 %g}\n", *fv);
          return true;
        }
        s_dbg.put(d_Shell, cl_Info, "info::set {?'%s' %p:%p}\n", value.id->name, value.value.data, value.value.size);
        return false;
      }
      if(id==tl::TConstUniqueID<eConsoleLevels, cl_Debug>::get_id()) {
        const Bsint32 *iv;
        if(value.get_as(iv)) {
          s_dbg.put(d_Shell, cl_Info, "debug::set {Bsint32 %d}\n", *iv);
          return true;
        }
        const float32 *fv;
        if(value.get_as(fv)) {
          s_dbg.put(d_Shell, cl_Info, "debug::set {float32 %g}\n", *fv);
          return true;
        }
        s_dbg.put(d_Shell, cl_Info, "debug::set {?'%s' %p:%p}\n", value.id->name, value.value.data, value.value.size);
        return false;
      }
      s_dbg.put(d_Shell, cl_Info, "?'%s'::set {?'%s' %p:%p}\n", id->name, value.id->name, value.value.data, value.value.size);
      return false;
    }
    bool get_value(HUniquePointer id, const SPropValueW& value) const {
      return false;
    }
  };
};

void RTest1_Shell::aux_tests(void)
{
  {
    s_dbg.put(d_Shell, cl_Info, "Begin prop test\n");
    Aux::Prop ap;
    ap.set_value(TPropValueRV<Bsint32>(10));
    ap.set_value(TPropValueRV<Bsint16>(10));
    ap.set_value(TPropValueRV<float32>(10));
    s_dbg.put(d_Shell, cl_Info, "End prop test\n");
  }
  {
    s_dbg.put(d_Shell, cl_Info, "Begin prop id test\n");
    Aux::PropX ap;
    ap.set_value(tl::const_unique_id<eConsoleLevels, cl_Info>(), TPropValueRV<Bsint32>(10));
    ap.set_value(tl::const_unique_id<eConsoleLevels, cl_Debug>(), TPropValueRV<Bsint16>(10));
    ap.set_value(tl::const_unique_id<eConsoleLevels, cl_Error>(), TPropValueRV<float32>(10));
//    ap.set_value(tl::const_unique_id<eConsoleLevels, cl_Warning>(), TPropValueRV<float32>(10));
    s_dbg.put(d_Shell, cl_Info, "End prop id test\n");
  }
}

