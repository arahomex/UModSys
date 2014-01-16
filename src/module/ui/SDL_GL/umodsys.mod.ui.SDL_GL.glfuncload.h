
//***************************************
// SGLFunc::
//***************************************

SGLFunc::SGLFunc(void)
{
  verhi = verlo = 0;
  numfunc = loadfunc = numsec = loadsec = 0;
#define GL_LinkGroupBegin(type,name) is_##name = false; numsec++;
#define GL_LinkGroupEnd(type,name)
#define GL_Link(var,name,type) var = NULL; numfunc++;
  #include "umodsys.mod.ui.SDL_GL.gl_inc.ldr.h"
#undef GL_Link
#undef GL_LinkGroupEnd
#undef GL_LinkGroupBegin
}

bool SGLFunc::load(void)
{
  bool *cur; 
  loadsec  = loadfunc = 0;
#define GL_LinkGroupBegin(type,name) \
  if(check_##type(#name)) { do { cur = &is_##name; *cur = false;
#define GL_LinkGroupEnd(type,name) \
  *cur = true; loadsec++; } while(0); }
#define GL_Link(var,name,type) \
  var = static_cast<type>(::SDL_GL_GetProcAddress(name)); \
  if(var==NULL) break; else loadfunc++;
  #include "umodsys.mod.ui.SDL_GL.gl_inc.ldr.h"
#undef GL_Link
#undef GL_LinkGroupEnd
#undef GL_LinkGroupBegin
  s_dbg.put(0, cl_Info, "Loaded %d/%d functions from %d/%d sections.\n", loadfunc, numfunc, loadsec, numsec);
  return is_GL_VERSION_1_0;
}

bool SGLFunc::check_ext(const char *ext)
{
  return SDL_GL_ExtensionSupported(ext)!=0;
/*
  if(tl::su::sbegin(fea, "GL_VERSION_", &ver)) {
    if(tl::su::seq(ver, "1_0"))
      return true; // always defined
    if(!is_GL_VERSION_1_0)
      return false; // no OpenGL base
    sscanf("%d_%d",
  }
*/
  return false;
}

bool SGLFunc::check_base(const char *fea)
{
  int hi, lo;
  if(sscanf(fea, "GL_VERSION_%d_%d", &hi, &lo)==2) {
    if(hi==1 && lo==0)
      return true; // always defined
    if(!is_GL_VERSION_1_0)
      return false; // no OpenGL base
    if(verhi==0) {
      const char *verstr = (const char*)glGetString(GL_VERSION);
      int err = glGetError();
      if(err || sscanf(verstr, "%d.%d", &verhi, &verlo)!=2)
        return false;
    }
    if(hi>verhi)
      return false;
    if(hi==verhi && lo>verlo)
      return false;
    return true;
  }
  return false;
}

//***************************************
// END
//***************************************
