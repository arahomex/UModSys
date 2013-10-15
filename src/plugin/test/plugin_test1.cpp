#include <umodsys/stdbase.h>
#include <umodsys/lib/media/libmedia.common.h>

UMODSYS_MODULE_BEGIN(test, test1)

using namespace core;
using namespace base;

struct RTest1_Generator;

struct RTest1_Shell : public IShell {
  UMODSYS_BASE_SHELL_IMPLEMENT(UMODSYS_MODULE_NAME(test,test1)::RTest1_Shell, 1, IShell)
  //
  void* memblock;
  //
  void dump_str(const char *s, size_t n)
  {
    char bc[8];
    for(size_t i=0; i<n; i++) {
      unsigned char c = *s++;
      if(c<' ') {
        M.con().put(0, "\\x%02x", c);
      } else if(c=='\\') {
        M.con().put(0, "\\");
      } else if(c<0xc0) {
        M.con().put(0, "%c", c);
      } else if((c & 0xe0)==0xc0) {
        if(i+1<n) {
          bc[0] = c; 
          bc[1] = *s++;
          bc[2] = 0;
          M.con().put(0, "%s", bc);
        } else {
          M.con().put(0, "\\x%02x", c);
        }
        i++;
      } else if((c & 0xf0)==0xe0) {
        if(i+2<n) {
          bc[0] = c; 
          bc[1] = *s++;
          bc[2] = *s++;
          bc[3] = 0;
          M.con().put(0, "%s", bc);
        } else {
          M.con().put(0, "\\x%02x", c);
        }
        i+=2;
      } else if((c & 0xf8)==0xf0) {
        if(i+3<n) {
          bc[0] = c; 
          bc[1] = *s++;
          bc[2] = *s++;
          bc[3] = *s++;
          bc[4] = 0;
          M.con().put(0, "%s", bc);
        } else {
          M.con().put(0, "\\x%02x", c);
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
          M.con().put(0, "%s", bc);
        } else {
          M.con().put(0, "\\x%02x", c);
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
          M.con().put(0, "%s", bc);
        } else {
          M.con().put(0, "\\x%02x", c);
        }
        i+=5;
      }
    }
  }
  //
  void file_test1(void)
  {
    libmedia::IStreamReader::P fr;
    libmedia::IStreamWriter::P fw;
    if(TypeId found=M.t_firstobjname<libmedia::IStreamReader>("*::stdio::*")) {
      TParametersA<1024> params;
      params.add("filename", "read-test.txt");
      M.t_generate(fr, found, params);
      M.con().put(0, "  found reader: %s => %p\n", found->name, fr());
    }
    if(TypeId found=M.t_firstobjname<libmedia::IStreamWriter>("*::stdio::*")) {
      TParametersA<1024> params;
      params.add("filename", "write-test.txt");
      params.add("safe", true);
      M.t_generate(fw, found, params);
      M.con().put(0, "  found writer: %s => %p\n", found->name, fw());
    }
    if(fr.valid()) {
      char line[1024];
      size_t req = core::scalar_min(sizeof(line)-1, size_t(fr->reader_size()));
      line[req] = 0;
      if(fr->reader_read(line, req)) {
        M.con().put(0, "  read: {");
        dump_str(line, req);
        M.con().put(0, "}\n", line);
      }
      if(fw.valid()) {
        fw->writer_copy(fr, 0, fr->reader_size());
        fw->writer_close();
        M.con().put(0, "  written\n");
      }
    }
  }
  void file_test2(void)
  {
    libmedia::IStreamReader::P fr;
    libmedia::IStreamWriter::P fw;
    libmedia::IDataArchive::P arch;
    if(TypeId found=M.t_firstobjname<libmedia::IDataArchive>("*::stdio::*")) {
      TParametersA<1024> params;
      params.add("pathname", ".");
      M.t_generate(arch, found, params);
      M.con().put(0, "  found archive: %s => %p\n", found->name, arch());
    }
    if(arch.valid()) {
      fr = arch->load_reader("read-test.txt");
      M.con().put(0, "  gen reader: %p\n", fr());
      fw = arch->save_writer("write-test.txt", libmedia::SFlags(libmedia::mf_safe::yes()) );
      M.con().put(0, "  gen writer: %p\n", fw());
    }
    if(fr.valid()) {
      char line[1024];
      size_t req = core::scalar_min(sizeof(line)-1, size_t(fr->reader_size()));
      line[req] = 0;
      if(fr->reader_read(line, req)) {
        M.con().put(0, "  read: {");
        dump_str(line, req);
        M.con().put(0, "}\n");
      }
      if(fw.valid()) {
        fw->writer_copy(fr, 0, fr->reader_size());
        fw->writer_close();
        M.con().put(0, "  written\n");
      }
    }
  }
  void file_test3(void)
  {
    libmedia::IDataArchive::P arch;
    SCMemShared mem_block;
    if(TypeId found=M.t_firstobjname<libmedia::IDataArchive>("*::stdio::*")) {
      TParametersA<1024> params;
      params.add("pathname", ".");
      M.t_generate(arch, found, params);
      M.con().put(0, "  found archive: %s => %p\n", found->name, arch());
    }
    if(arch.valid()) {
      bool f = arch->load_data("read-test.txt", mem_block);
      if(f) {
        M.con().put(0, "  load file size:%u {", int(~mem_block));
        dump_str(mem_block.get_tdata<char>(), ~mem_block);
        M.con().put(0, "}\n");
      } else {
        M.con().put(0, "  not load file\n");
      }
      f = arch->save_data("write-test.txt", mem_block, libmedia::SFlags(libmedia::mf_safe::yes()) );
      if(f) {
        M.con().put(0, "  size file size:%u {", ~mem_block);
      } else {
        M.con().put(0, "  not save file\n");
      }
    }
  }
  //
  RTest1_Shell(DOwner *own) : refs(own) {
    M.con().put(0, "RTest1_Shell() {\n");
    memblock = M().mem_alloc(1024, _UMODSYS_SOURCEINFO);
    M().mem_alloc(511, _UMODSYS_SOURCEINFO);
    //
//    file_test1();
//    file_test2();
    file_test3();
    //
    M.con().put(0, "} // RTest1_Shell()\n");
  }
  ~RTest1_Shell(void) {
    M().mem_free(memblock, _UMODSYS_SOURCEINFO);
    M.con().put(0, "~RTest1_Shell()\n");
  }
  //
  bool connect(void) {
    M.con().put(0, "RTest1_Shell::connect()\n");
    return true;
  }
  bool disconnect(void) {
    M.con().put(0, "RTest1_Shell::disconnect()\n");
    return true;
  }
  bool process_tick(const STimeMsec& delta) {
    M.con().put(0, "RTest1_Shell::tick(%g)\n", delta.fsec());
    return true;
  }
  bool process_command(int argc, const core::DCString argv[]) {
    M.con().put(0, "RTest1_Shell::command(#%d)\n", argc);
    return true;
  }
};

struct RGenerator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
    list<<RTest1_Shell::_get_interface_type();
    return 1;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    if(name==RTest1_Shell::_get_interface_type())
      return RTest1_Shell::_get_interface_types(list);
    return 0;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    if(name==RTest1_Shell::_get_interface_type()) {
      obj = new(M()) RTest1_Shell(this);
      return obj.valid();
    }
    return false;
  }
  //
  UMODSYS_BASE_GENERATOR_IMPLEMENT(UMODSYS_MODULE_NAME(test,test1)::RGenerator, 1, IGenerator)
};

struct RModuleReg : public IModuleReg {
  UMODSYS_BASE_MODREG_DEF(RModuleReg)
  //
  RModuleReg(void) {
    minfo.set("Test1", 0, 1, "Test 1 - module");
//    M.con().put(0, "RModuleReg()\n");
  }
  ~RModuleReg(void) {
//    M.con().put(0, "~RModuleReg()\n");
  }
  bool do_open(void) {
    M.con().put(0, "RModuleReg::open()\n");
    RGenerator::s_reg(this);
    return true;
  }
  bool do_close(void) {
    M.con().put(0, "RModuleReg::close()\n");
    RGenerator::s_unreg(this);
    return true;
  }
};

UMODSYS_BASE_MODREG_BODY(RModuleReg)
UMODSYS_BASE_GENERATOR_BODY(RGenerator, RModuleReg)

UMODSYS_MODULE_END()


