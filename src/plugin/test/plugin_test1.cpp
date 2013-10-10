#include <umodsys/stdbase.h>
#include <umodsys/lib/media/libmedia.common.h>

namespace UModSys {
namespace test1 {

using namespace core;
using namespace base;

struct RTest1_Generator;

struct RTest1_Shell : public IShell {
  UMODSYS_BASE_SHELL_IMPLEMENT(UModSys::test1::RTest1_Shell, 1, IShell)
  //
  void* memblock;
  media::IStreamReader::P fr;
  //
  RTest1_Shell(DOwner *own) : refs(own) {
    M.con().put(0, "RTest1_Shell() {\n");
    memblock = M().mem_alloc(1024, _UMODSYS_SOURCEINFO);
    //
    {
      tl::TArrayFixed<TypeId, 1000> lst;
      if(M.t_findobjname<media::IStreamReader>(lst)) {
        TypeId found = NULL;
        for(size_t i=0; i<~lst; i++) {
          if(tl::su::sstr(lst(i)->name, "_FILE")!=NULL) {
            found = lst(i);
            break;
          }
        }
        if(found!=NULL) {
          M.con().put(0, "  found reader: %s\n", found->name);
          TParametersA<1024> params;
          params.add("filename", "read-test.txt");
          M.t_generate(fr, found, params);
        }
      }
      if(fr.valid()) {
        char line[1024];
        size_t req = core::scalar_min(sizeof(line)-1, size_t(fr->reader_size()));
        line[req] = 0;
        if(fr->reader_read(line, req)) {
          M.con().put(0, "  read: {%s}\n", line);
        }
      }
    }
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

struct RTest1_Generator : public IGenerator {
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
  UMODSYS_BASE_GENERATOR_IMPLEMENT(UModSys::test1::RTest1_Generator, 1, IGenerator)
};

struct RTest1_ModuleReg : public IModuleReg {
  UMODSYS_BASE_MODREG_DEF(RTest1_ModuleReg)
  //
  RTest1_ModuleReg(void) {
    minfo.set("Test1", 0, 1, "Test 1 - module");
//    M.con().put(0, "RTest1_ModuleReg()\n");
  }
  ~RTest1_ModuleReg(void) {
//    M.con().put(0, "~RTest1_ModuleReg()\n");
  }
  bool do_open(void) {
    M.con().put(0, "RTest1_ModuleReg::open()\n");
    RTest1_Generator::s_reg(this);
    return true;
  }
  bool do_close(void) {
    M.con().put(0, "RTest1_ModuleReg::close()\n");
    RTest1_Generator::s_unreg(this);
    return true;
  }
};

UMODSYS_BASE_MODREG_BODY(RTest1_ModuleReg)
UMODSYS_BASE_GENERATOR_BODY(RTest1_Generator, RTest1_ModuleReg)

} // namespace test1
} // namespace UModSys


