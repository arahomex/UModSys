
//***************************************
// SGLFunc::
//***************************************

struct SGLFunc {
  int verhi, verlo, numfunc, loadfunc, numsec, loadsec;
  //
  #include "umodsys.mod.ui.SDL_GL.gl_inc.def.h"
  //
#define GL_LinkGroupBegin(type,name) bool is_##name;
#define GL_LinkGroupEnd(type,name)
#define GL_Link(var,name,type)
  #include "umodsys.mod.ui.SDL_GL.gl_inc.ldr.h"
#undef GL_Link
#undef GL_LinkGroupEnd
#undef GL_LinkGroupBegin
  //
  SGLFunc(void);
  bool load(void);
  bool check_ext(const char *ext);
  bool check_base(const char *fea);
};

//***************************************
// END
//***************************************
