#include <umodsys/stdbase.h>
#include <umodsys/lib/media/libmedia.common.h>
#include <umodsys/core/syshlp.h>

UMODSYS_DEFINE_MODULE(Media_StdIo)
UMODSYS_BODY_MODULE(Media_StdIo)

namespace UModSys {
namespace media {

using namespace core;
using namespace base;

struct RStdIo_Generator;

struct RStreamReader_FILE : public IStreamReader {
  UMODSYS_REFOBJECT_IMPLEMENT1(UModSys::media::RStreamReader_FILE, 1, IStreamReader)
  UMODSYS_REFOBJECT_REFOTHER(RStdIo_Generator);
  //
  FILE *file;
  DFilePosition endsize;
  //
  RStreamReader_FILE(DOwner *own, const SParameters& args) 
  : refs(own), file(NULL), endsize(0) {
    open(args);
  }
  ~RStreamReader_FILE(void) {
    reader_close();
  }
  //
  inline void open(const SParameters& args) { 
    BCStr filename;
    if(args.get("filename", filename)) {
      open(filename);
    }
  }
  //
  inline void open(const BCStr osname) { file = syshlp::c_fopen(osname, "rb"); get_size(); }
  inline void open(FILE *f) { file = f; get_size(); }
  inline bool validate_construction(void) const { return file!=NULL; }
  //
  inline void get_size(void) {
    if(file) {
      fseek(file, 0, SEEK_END);
      endsize = ftell(file);
      fseek(file, 0, SEEK_SET);
    }
  }
  //
  bool reader_seek(DFilePosition pos) {
    return file && !fseek(file, pos, SEEK_SET);
  }
  bool reader_seekend(DFilePosition pos) {
    return file && !fseek(file, -pos, SEEK_END);
  }
  bool reader_seekdelta(DFilePosition pos) {
    return file && !fseek(file, pos, SEEK_CUR);
  }
  DFilePosition reader_pos(void) {
    return file ? ftell(file) : 0;
  }
  DFilePosition reader_size(void) {
    return endsize;
  }
  bool reader_read(void* data, size_t size) {
    if(size==0)
      return true;
    if(!file || !data)
      return false;
    DFilePosition p = ftell(file);
    if(fread(data, 1, size, file)==size)
      return true; // ok
    //
    fseek(file, p, SEEK_SET);
    return false;
  }
  void reader_close(void) {
    if(file) {
      fclose(file);
      file = NULL;
    }
  }
};

struct RStdIo_Generator : public IGenerator {
  //
  int get_generated_names(DPtrList& list) const {
    list<<RStreamReader_FILE::_get_interface_type();
    return 1;
  }
  int get_generated_types(DPtrList& list, TypeId name) const {
    if(name==RStreamReader_FILE::_get_interface_type())
      return RStreamReader_FILE::_get_interface_types(list);
    return 0;
  }
  bool generate(IRefObject::P& obj, TypeId name, const SParameters& args) {
    if(name==RStreamReader_FILE::_get_interface_type()) {
      return ValidateConstruction(obj, new(M()) RStreamReader_FILE(this, args));
    }
    return false;
  }
  //
  UMODSYS_BASE_GENERATOR_IMPLEMENT(UModSys::media::RStdIo_Generator, 1, IGenerator)
};

struct RStdIo_ModuleReg : public IModuleReg {
  UMODSYS_BASE_MODREG_DEF(RStdIo_ModuleReg)
  //
  RStdIo_ModuleReg(void) {}
  ~RStdIo_ModuleReg(void) {}
  bool do_open(void) { RStdIo_Generator::s_reg(this); return true; }
  bool do_close(void) { RStdIo_Generator::s_unreg(this); return true; }
};

UMODSYS_BASE_MODREG_BODY(RStdIo_ModuleReg)
UMODSYS_BASE_GENERATOR_BODY(RStdIo_Generator, RStdIo_ModuleReg)

} // namespace media
} // namespace UModSys


