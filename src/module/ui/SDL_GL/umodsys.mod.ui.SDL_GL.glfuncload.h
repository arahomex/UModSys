
//***************************************
// SGLFuncs::
//***************************************

SGLFuncs::SGLFuncs(void)
{
  texstage = texstage_client = 0;
  clear_min();
}

void SGLFuncs::clear_min(void)
{
#define GL_LinkGroupBegin(type,name) is_##name = false; numsec++;
#define GL_LinkGroupEnd(type,name)
#define GL_Link_core(var,name,type,tdef) var = NULL; numfunc++;
#define GL_Link_compat(var,name,type,tdef)
  #include "umodsys.mod.ui.SDL_GL.inc.gl.h"
#undef GL_Link_core
#undef GL_Link_compat
#undef GL_LinkGroupEnd
#undef GL_LinkGroupBegin
}

void SGLFuncs::clear(void)
{
  SGLFuncsBase::clear();
  clear_min();
}

bool SGLFuncs::load(void)
{
  bool *cur; 
  //
  ha_color = 0;
  //
  loadsec  = loadfunc = 0;
#define GL_LinkGroupBegin(type,name)   if(check_##type(#name)) { do { cur = &is_##name; *cur = false;
#define GL_LinkGroupEnd(type,name)     *cur = true; loadsec++; } while(0); }
#define GL_Link_core(var,name,type,tdef)    var = reinterpret_cast<type>(::SDL_GL_GetProcAddress(name)); \
  if(var==NULL) { s_dbg.put(0, cl_Error, "Error link '%s'.\n", name); break; } else loadfunc++;
#define GL_Link_compat(var,name,type,tdef)
  #include "umodsys.mod.ui.SDL_GL.inc.gl.h"
#undef GL_Link_core
#undef GL_Link_compat
#undef GL_LinkGroupEnd
#undef GL_LinkGroupBegin
  s_dbg.put(0, cl_Info, "Loaded %d/%d functions from %d/%d sections.\n", loadfunc, numfunc, loadsec, numsec);
  return is_GL_VERSION_1_0;
}

bool SGLFuncs::check_base(const char *fea)
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
// SGLFuncsLegacy::
//***************************************

SGLFuncsLegacy::SGLFuncsLegacy(void)
{
  is_core = true;
  clear_min();
}

void SGLFuncsLegacy::clear_min(void)
{
#define GL_LinkGroupBegin(type,name)
#define GL_LinkGroupEnd(type,name)
#define GL_Link_core(var,name,type,tdef) 
#define GL_Link_compat(var,name,type,tdef) var = NULL; numfunc++;
  #include "umodsys.mod.ui.SDL_GL.inc.gl.h"
#undef GL_Link_core
#undef GL_Link_compat
#undef GL_LinkGroupEnd
#undef GL_LinkGroupBegin
}


void SGLFuncsLegacy::clear(void)
{
  SGLFuncs::clear();
  clear_min();
}

bool SGLFuncsLegacy::load(bool iscore)
{
  is_core = iscore;
  if(!SGLFuncs::load())
    return false;
  if(is_core)
    return true; // no more
  bool *cur; 
#define GL_LinkGroupBegin(type,name)   if(is_##name) { do { cur = &is_##name; *cur = false; loadsec--;
#define GL_LinkGroupEnd(type,name)     *cur = true; loadsec++; } while(0); }
#define GL_Link_compat(var,name,type,tdef)    var = reinterpret_cast<type>(::SDL_GL_GetProcAddress(name)); \
  if(var==NULL) { s_dbg.put(0, cl_Error, "Error link '%s'.\n", name); break; } else loadfunc++;
#define GL_Link_core(var,name,type,tdef)
  #include "umodsys.mod.ui.SDL_GL.inc.gl.h"
#undef GL_Link_core
#undef GL_Link_comat
#undef GL_LinkGroupEnd
#undef GL_LinkGroupBegin
  s_dbg.put(0, cl_Info, "Loaded compatibility %d/%d  functions from %d/%d sections.\n", loadfunc, numfunc, loadsec, numsec);
  return true;
}

//***************************************
// END
//***************************************
