#include <umodsys/stdbase.h>
#include <umodsys/tl/composite/dynarray.h>
#include <umodsys/core/syshlp.h>


#define TEST_SDL_GL 0
#define TEST_SDL_CORE 1
#define TEST_FRAMES 1

#include <umodsys/lib/media/libmedia.common.h>
#include <umodsys/lib/media/libmedia.library.h>
#include <umodsys/lib/media/libmedia.archive.h>
#include <umodsys/lib/media/libmedia.serialize.h>

#include <umodsys/lib/utils/libutils.time.h>

#include <umodsys/lib/ui/libui.common.h>
#include <umodsys/lib/ui/libui.keymap.h>
#include <umodsys/lib/ui/libui.frames.h>

#include <umodsys/lib/2d/lib2d.driver.h>
#include <umodsys/lib/2d/lib2d.image.h>

#include <umodsys/lib/3d/lib3d.common.h>
#include <umodsys/lib/3d/lib3d.driver.h>
#include <umodsys/lib/3d/lib3d.visual.h>
#include <umodsys/lib/3d/lib3d.objects.h>
#include <umodsys/lib/3d/lib3d.renderstate.h>

#include "version_plugin_test1.h"

static void refer(void);

using namespace UModSys;

namespace UModSys {
  UMODSYS_TL_TCONSTUNIQUEID_ATOM(base::eConsoleLevels, base::cl_Info)
  UMODSYS_TL_TCONSTUNIQUEID_ATOM(base::eConsoleLevels, base::cl_Debug)
  UMODSYS_TL_TCONSTUNIQUEID_ATOM(base::eConsoleLevels, base::cl_Error)
}



#define U_MOD UMODSYS_MODULE_NAME(test, test1)
UMODSYS_MODULE_START(test, test1)

using namespace core;
using namespace base;

struct RTest1_Generator;

struct ILines;
struct RLines;
struct RLines_Filter;

enum {
  d_Shell,
  d_Gen,
  d_SubGen,
  d_UI,
  d_File
};

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
  inline static typename T::P generate_type(const DCString &mask, const SParameters& args, BStr xinfo="") {
    TypeId found = NULL;
    typename T::P rv;
    if(!M.t_generate_first(rv, args, mask, &found)) {
      if(found!=NULL)
        s_dbg.put(d_Gen, cl_Error, "  fail generate %s(%s)\n", found->name, xinfo);
      else
        s_dbg.put(d_Gen, cl_Error, "  fail find %s(%s)\n", T::_root_get_interface_type()->name, mask());
      return NULL;
    }
    s_dbg.put(d_Gen, cl_Info, "  generated %s(%s) => %p\n", found->name, xinfo, rv());
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
  lib2d::IImageFactory::P new_mem_imagefactory(void);
  lib2d::IMultiImage::P new_mem_multiimage(lib2d::IImageFactory* imgf);
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
  struct UI_Info : libui::IKeyboardClient, libui::IMouseClient {
#pragma pack(push, 1)
    struct VertexPC {
      float32 x, y, z;
      uint8 r,g,b,a;
      //
      void set_xyz(float32 xx, float32 yy, float32 zz) {
        x = xx; y = yy; z = zz;
      }
      void set_rgba(uint8 rr, uint8 gg, uint8 bb, uint8 aa=255) {
        r = rr; g = gg; b = bb; a = aa;
      }
      void set(const lib3d::DPoint& p, const lib3d::DColor& c) {
        x = p(0); y = p(1); z = p(2);
        r = c(0)*255; g = c(1)*255; b = c(2)*255; a = c(3)*255;
      }
    };
    //
    struct VertexPCT {
      float32 x, y, z;
      uint8 r,g,b,a;
      float32 u, v;
      //
      void set_xyz(float32 xx, float32 yy, float32 zz) {
        x = xx; y = yy; z = zz;
      }
      void set_uv(float32 uu, float32 vv) {
        u=uu; v = vv;
      }
      void set_rgba(uint8 rr, uint8 gg, uint8 bb, uint8 aa=255) {
        r = rr; g = gg; b = bb; a = aa;
      }
      void set(const lib3d::DPoint& p, const lib3d::DColor& c) {
        x = p(0); y = p(1); z = p(2);
        r = c(0)*255; g = c(1)*255; b = c(2)*255; a = c(3)*255;
      }
    };
#pragma pack(pop)
    //
    UI_Info(RTest1_Shell& s) : shell(s) {}
    //
    RTest1_Shell& shell;
    libmedia::ILibrary::P media;
    //
    float ticks, frame_time;
    lib2d::IRenderDriver::P rd2d;
    lib2d::IMultiImage::P font;
    libui::ITerminal::P term;
    libui::IKeyboardController::P keyc;
    libui::IMouseController::P mouc;
    libui::ICollector::P frames;
    //
    lib3d::IRenderDriver::P rd3d;
    lib3d::IRenderer::P rdr;
    lib3d::IVisualizerScene::P visis;
    lib3d::IVertexArray::P va_tri, vas_tri, vas_cube, vas_cubechunk;
    lib3d::IVisualObject_Camera::P cam;
    lib3d::IVisualObject::P vocube;
    bool f_quit, fmouvis, fview_matrix, is_upz;
    //
    lib3d::DPoint3 fps_pos, fps_ori;
    lib3d::DScalar move_speed, mou_sensitivity;
    lib3d::DMatrix4 view_matrix, camera_T;
    lib3d::TMaterialST<1> mat_cube;
    //
    void fps_view_matrix(void);
    void fps_move(lib3d::DScalar x, lib3d::DScalar y, lib3d::DScalar z);
    void fps_add_rot(lib3d::DScalar x, lib3d::DScalar y);
    //
    void fps2_view_matrix(void);
    void fps2_move(int ort, lib3d::DScalar value);
    void fps2_add_rot(lib3d::DScalar x, lib3d::DScalar y);
    //
    bool font_pure(void);
    bool font_2d(void);
    bool init_term(const char *name);
    bool create_2d(const char *cap);
    bool create_3d(const char *cap);
    bool new_frames(libui::IController *ctrl);
    //
    bool new_va_1(void);
    bool new_va_qc(void);
    //
    void cycle(void);
    void cycle3d_a(void);
    void cycle3d_b(void);
    void shutdown(void);
    //
    bool key_pressed(const libui::SKeyboardInputRaw& key);
    bool key_text(const libui::SKeyboardInputText& key);
    bool mouse_event(const libui::SMouseInput& ms);
  };
  //
  static libui::ICollector::P ui_newframes(const DCString &mask, const SParameters& args);
  static libui::ITerminal::P ui_newterm(const DCString &mask, const SParameters& args);
  static lib2d::IMultiImage::P ui_newfont(const DCString &mask, const SParameters& args);
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
  struct Aux;
  void aux_tests(void);
  //
  RTest1_Shell(DOwner *own) : refs(own) {
    s_dbg.menable();
    //
    s_dbg.put(d_Shell, cl_Info, "RTest1_Shell() {\n");
    memblock = M().mem_alloc(1024, _UMODSYS_SOURCEINFO);
    M().mem_alloc(511, _UMODSYS_SOURCEINFO);
    //
    aux_tests();
    //
#if 0
    file_test1();
    file_test2();
    file_test3();
    file_test4();
    file_test5();
    file_test6();
    file_test7();
#endif
    //
#if TEST_SDL_GL
    ui_test2();
#elif TEST_SDL_CORE
    ui_test1();
#endif
    //
    s_dbg.put(d_Shell, cl_Info, "} // RTest1_Shell()\n");
  }
  ~RTest1_Shell(void) {
    M().mem_free(memblock, _UMODSYS_SOURCEINFO);
    s_dbg.put(d_Shell, cl_Info, "~RTest1_Shell()\n");
  }
  //
  bool connect(void) {
    s_dbg.put(d_Shell, cl_Info, "RTest1_Shell::connect()\n");
    return true;
  }
  bool disconnect(void) {
    s_dbg.put(d_Shell, cl_Info, "RTest1_Shell::disconnect()\n");
    return true;
  }
  bool process_tick(const STimeMsec& delta) {
    s_dbg.put(d_Shell, cl_Info, "RTest1_Shell::tick(%g)\n", delta.fsec());
    return true;
  }
  bool process_command(int argc, const core::DCString argv[]) {
    s_dbg.put(d_Shell, cl_Info, "RTest1_Shell::command(#%d)\n", argc);
    return true;
  }
};

#include "plugin_test1.common.h"

#include "plugin_test1.file_utils.h"
#include "plugin_test1.file_tests.h"

#include "plugin_test1.ui_funcs.h"
#include "plugin_test1.ui_ctx.h"
#include "plugin_test1.ui_ctx3.h"
#include "plugin_test1.ui_ctx3u.h"
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
    s_dbg.put(d_Shell, cl_Info, "RModuleReg::open()\n");
    RGenerator::s_reg(this);
    return true;
  }
  bool do_close(void) {
    s_dbg.put(d_Shell, cl_Info, "RModuleReg::close()\n");
    RGenerator::s_unreg(this);
    return true;
  }
};

UMODSYS_BASE_MODREG_BODY(RModuleReg)
UMODSYS_BASE_GENERATOR_BODY(RGenerator, RModuleReg)

UMODSYS_MODULE_END()


UMODSYS_MODULE_DEF(lib2d,stdlib);
UMODSYS_MODULE_DEF(lib3d,stdlib);
UMODSYS_MODULE_DEF(ui,frames);
UMODSYS_MODULE_DEF(ui,SDL_core);
UMODSYS_MODULE_DEF(ui,SDL_GL);
UMODSYS_MODULE_DEF(media,images_std);
UMODSYS_MODULE_DEF(media,serialize_json);
UMODSYS_MODULE_DEF(media,zlib);

static void refer(void)
{
//  UMODSYS_MODULE_USE(lib2d,stdlib);
//  UMODSYS_MODULE_USE(lib3d,stdlib);
//  UMODSYS_MODULE_USE(ui,frames);
#if TEST_SDL_CORE
//  UMODSYS_MODULE_USE(ui,SDL_core);
#endif
#if TEST_SDL_GL
//  UMODSYS_MODULE_USE(ui,SDL_GL);
#endif
//  UMODSYS_MODULE_USE(media,images_std);
//  UMODSYS_MODULE_USE(media,serialize_json);
//  UMODSYS_MODULE_USE(media,zlib);
}

