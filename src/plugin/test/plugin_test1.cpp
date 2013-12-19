#include <umodsys/stdbase.h>
#include <umodsys/tl/composite/dynarray.h>

#include <umodsys/lib/media/libmedia.common.h>
#include <umodsys/lib/media/libmedia.library.h>
#include <umodsys/lib/media/libmedia.archive.h>
#include <umodsys/lib/media/libmedia.serialize.h>

#include <umodsys/lib/ui/libui.common.h>
#include <umodsys/lib/ui/libui.frames.h>
#include <umodsys/lib/2d/lib2d.driver.h>
//#include <umodsys/lib/2d/lib2d.image.h>
//#include <umodsys/lib/ui/lib3d.common.h>

#include "version_plugin_test1.h"

static void refer(void);

#define U_MOD UMODSYS_MODULE_NAME(test, test1)
UMODSYS_MODULE_BEGIN(test, test1)

using namespace core;
using namespace base;

struct RTest1_Generator;

struct ILines;
struct RLines;
struct RLines_Filter;

#include "plugin_test1.obj_load.h"

struct RTest1_Shell 
: public IShell, 
  public libui::IKeyboardClient, public libui::IMouseClient,
  public libui::IController
{
  //
  UMODSYS_BASE_SHELL_IMPLEMENT(U_MOD::RTest1_Shell, 1, IShell)
  //
  void* memblock;
  //
  // ----------------------------------------------------------------------------------
  //
  template<typename T>
  inline typename T::P generate_type(const DCString &mask, const SParameters& args, BStr xinfo="") {
    TypeId found = NULL;
    typename T::P rv;
    if(!M.t_generate_first(rv, args, mask, &found)) {
      if(found!=NULL)
        M.con().put(0, "  fail generate %s(%s)\n", found->name, xinfo);
      else
        M.con().put(0, "  fail find %s(%s)\n", T::_root_get_interface_type()->name, mask());
      return NULL;
    }
    M.con().put(0, "  generated %s(%s) => %p\n", found->name, xinfo, rv());
    return rv;
  }
  // ----------------------------------------------------------------------------------
  //
  void dump_str(const char *s, size_t n);
  //
  libmedia::IBinArchive::P media_arch_stdio(const DCString &path);
  libmedia::IBinArchive::P media_arch_zip(libmedia::ILibrary* root, libmedia::IUtilities* utl, const DCString &path, bool rw=false);
  libmedia::IBinObjFilter::P media_filter_new(const DCString &mask, const SParameters& args);
  lib2d::IImage::P new_mem_image(void);
  //
  libmedia::IUtilities::P media_utils(void);
  libmedia::ILibraryBinTree::P media_vfs(void);
  libmedia::ILibraryObjFilter::P media_flt(void);
  libmedia::ILibraryLayered::P media_lay(void);
  libmedia::ILibrary::P media_cache(bool isobj=false);
  void test_op_file(bool f, const DCString &fname, const DCString &operation);
  void test_op_file(bool f, const DCString &fname, SCMemShared& mem_block, bool isRead);
  //
  void file_test1(void);
  void file_test2(void);
  void file_test3(void);
  void file_test4(void);
  void file_test5(void);
  void file_test6(void);
  void file_test7(void);
  //
  // ----------------------------------------------------------------------------------
  //
  libui::ICollector::P ui_newframes(const DCString &mask, const SParameters& args);
  libui::ITerminal::P ui_newterm(const DCString &mask, const SParameters& args);
  lib2d::IMultiImage::P ui_newfont(const DCString &mask, const SParameters& args);
  //
  void ui_test1(void);
  void ui_test2(void);
  //
  bool key_pressed(const libui::SKeyboardInputRaw& key);
  bool key_text(const libui::SKeyboardInputText& key);
  bool mouse_event(const libui::SMouseInput& ms);
  //
  bool command(const libui::SController& ci, int command, const libui::SFrameDataIn& in);
  bool db_get(const libui::SController& ci, const BCStr hint, const libui::SFrameDataOut& out);
  bool db_get(const libui::SController& ci, const BCStr hint, int sid, const libui::SFrameDataOut& out);
  bool command_draw(const libui::SController& ci, lib2d::IRenderDriver *drv, const lib2d::DBox &bounds);
  //
  bool f_quit;
  // ----------------------------------------------------------------------------------
  //
  RTest1_Shell(DOwner *own) : refs(own) {
    M.con().put(0, "RTest1_Shell() {\n");
    memblock = M().mem_alloc(1024, _UMODSYS_SOURCEINFO);
    M().mem_alloc(511, _UMODSYS_SOURCEINFO);
    //
#if 0
    file_test1();
    file_test2();
    file_test3();
    file_test4();
    file_test5();
    file_test6();
#endif
    file_test7();
    //
#if 0
    ui_test1();
#endif
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

#include "plugin_test1.common.h"
#include "plugin_test1.file_tests.h"
#include "plugin_test1.ui_tests.h"

struct RGenerator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
    list<<RTest1_Shell::_root_get_interface_type();
    return 1;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    if(name==RTest1_Shell::_root_get_interface_type())
      return RTest1_Shell::_root_get_interface_types(list);
    return 0;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    if(name==RTest1_Shell::_root_get_interface_type()) {
      obj = new(M()) RTest1_Shell(this);
      return obj.valid();
    }
    return false;
  }
  //
  UMODSYS_BASE_GENERATOR_IMPLEMENT(U_MOD::RGenerator, 1, IGenerator)
};

struct RModuleReg : public IModuleReg {
  UMODSYS_BASE_MODREG_DEF(RModuleReg)
  //
  RModuleReg(void) 
  : UMODSYS_MODULEREG(
    "tests::test1", 
    UMODSYS_VERSION_PLUGIN_TEST_TEST1, 
    "Test 1 - module"
    ) {
    lost_func(refer);
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


UMODSYS_MODULE_DEF(lib2d,stdlib);
UMODSYS_MODULE_DEF(ui,frames);
UMODSYS_MODULE_DEF(ui,SDL_core);
UMODSYS_MODULE_DEF(media,images_std);
UMODSYS_MODULE_DEF(media,serialize_json);
UMODSYS_MODULE_DEF(media,zlib);
static void refer(void)
{
  UMODSYS_MODULE_USE(lib2d,stdlib);
  UMODSYS_MODULE_USE(ui,frames);
  UMODSYS_MODULE_USE(ui,SDL_core);
  UMODSYS_MODULE_USE(media,images_std);
  UMODSYS_MODULE_USE(media,serialize_json);;
  UMODSYS_MODULE_USE(media,zlib);;
}

