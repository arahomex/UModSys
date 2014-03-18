
//***************************************
// SGLFuncsBase::
//***************************************

const GLuint gl_no_texture = 0;
const GLuint gl_no_buffer = 0;
const GLuint gl_ARB_multitex_levels = 32;
const GLuint gl_multitex_levels = 32;

const GLenum gl_texture_bylevel[gl_multitex_levels+1] = {
  GL_TEXTURE0,  GL_TEXTURE1,  GL_TEXTURE2,  GL_TEXTURE3,  
  GL_TEXTURE4,  GL_TEXTURE5,  GL_TEXTURE6,  GL_TEXTURE7,
  GL_TEXTURE8,  GL_TEXTURE9,  GL_TEXTURE10, GL_TEXTURE11, 
  GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14, GL_TEXTURE15,
  GL_TEXTURE16, GL_TEXTURE17, GL_TEXTURE18, GL_TEXTURE19, 
  GL_TEXTURE20, GL_TEXTURE21, GL_TEXTURE22, GL_TEXTURE23,
  GL_TEXTURE24, GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27, 
  GL_TEXTURE28, GL_TEXTURE29, GL_TEXTURE30, GL_TEXTURE31,
  0
};
const GLenum gl_texture_bylevel_ARB[gl_ARB_multitex_levels+1] = {
  GL_TEXTURE0_ARB,  GL_TEXTURE1_ARB,  GL_TEXTURE2_ARB,  GL_TEXTURE3_ARB,  
  GL_TEXTURE4_ARB,  GL_TEXTURE5_ARB,  GL_TEXTURE6_ARB,  GL_TEXTURE7_ARB,
  GL_TEXTURE8_ARB,  GL_TEXTURE9_ARB,  GL_TEXTURE10_ARB, GL_TEXTURE11_ARB, 
  GL_TEXTURE12_ARB, GL_TEXTURE13_ARB, GL_TEXTURE14_ARB, GL_TEXTURE15_ARB,
  GL_TEXTURE16_ARB, GL_TEXTURE17_ARB, GL_TEXTURE18_ARB, GL_TEXTURE19_ARB, 
  GL_TEXTURE20_ARB, GL_TEXTURE21_ARB, GL_TEXTURE22_ARB, GL_TEXTURE23_ARB,
  GL_TEXTURE24_ARB, GL_TEXTURE25_ARB, GL_TEXTURE26_ARB, GL_TEXTURE27_ARB, 
  GL_TEXTURE28_ARB, GL_TEXTURE29_ARB, GL_TEXTURE30_ARB, GL_TEXTURE31_ARB,
  0
};

struct SGLFuncsBase {
  int verhi, verlo, numfunc, loadfunc, numsec, loadsec;
  const int *vic;
  BCStr version;
  //
  inline SGLFuncsBase(void) { 
    vic = NULL; version = NULL;
    clear(); 
  }
  inline void clear(void) {
    verhi = verlo = 0;
    numfunc = loadfunc = numsec = loadsec = 0;
  }
  inline bool check_ext(const char *ext) { 
    return SDL_GL_ExtensionSupported(ext)!=0; 
  }
};

//***************************************
// SGLFuncs::
//***************************************

struct SGLFuncs : public SGLFuncsBase {
  //
  struct TextureInfo;
  //
  struct TexStageInfo {
    GLint composite;
    GLuint bound;
    byte tex2d, client_coords, a1, a2;
    //
    inline void clear(void) {
      bound = -1;
      composite = -1;
      tex2d = -1;
      client_coords = -1;
      a1 = a2 = -1;
    }
    inline TexStageInfo(void) {
      clear();
    }
  };
  //
#define GL_LinkGroupBegin(type,name) bool is_##name;
#define GL_LinkGroupEnd(type,name)
#define GL_Link_core(var,name,type,tdef) tdef;
#define GL_Link_compat(var,name,type,tdef) 
  #include "umodsys.mod.ui.SDL_GL.inc.gl.h"
#undef GL_Link_core
#undef GL_Link_compat
#undef GL_LinkGroupEnd
#undef GL_LinkGroupBegin
  //
  SGLFuncs(void);
  void clear_min(void);
  void clear(void);
  bool load(void);
  bool check_base(const char *fea);
  //
  void dumperror(int error) const;
  inline void e(void) const {
    int error = this->glGetError();
    if(error!=GL_NO_ERROR) {
      dumperror(error);
    }
  }
  //
  static int image_hint_format(const DCString &hinttype, int defvalue);
  static int image_hint_format(lib2d::eImageType type, int &fmt, int &size);
  //
  void set_flags(const SGLFuncs::TextureInfo* ti, const SRenderMapFlags& flags) const;
  void set_flags(const SGLFuncs::TextureInfo* ti, const SRenderMapFlags& flags, eRenderMapFlagsCombine combine) const;
  void set_tflags(const SGLFuncs::TextureInfo* ti, const SRenderMapFlags& flags) const;
  //
  static GLenum mapRenderMapFlagsFilter_Min[];
  static GLenum mapRenderMapFlagsFilter_Mag[];
  static GLenum mapRenderMapFlagsWrap_S[];
  static GLenum mapRenderMapFlagsWrap_T[];
  static GLenum mapRenderMapFlagsCombine[];
  static GLenum mapTransparrentMode_SRC[];
  static GLenum mapTransparrentMode_DST[];
  //
  mutable TexStageInfo texsinfos[gl_multitex_levels];
  mutable int texstage, texstage_client;
  mutable GLenum blend_source, blend_destination, blend_enabled;
  mutable int max_tex_level, tex_levels;
  // helpers
  //
  mutable GLuint ha_color;
  //
  virtual void h_glClientActiveTexture(GLint num) const;
  virtual void h_glColor4fv(const GLfloat val[4]) const;
  //
  virtual void h_DeleteTextures(GLsizei n, const GLuint* texs) const;
  virtual void h_Rect(GLint x1, GLint y1, GLint x2, GLint y2) const;
  //
  inline void set_blend(bool mode) const {
    if(blend_enabled!=int(mode)) {
      if(mode) this->glEnable(GL_BLEND); 
      else this->glDisable(GL_BLEND);
      blend_enabled = mode;
    }
  }
  inline void set_blend(GLenum s, GLenum d) const {
    if(blend_source!=s || blend_destination!=d) {
      this->glBlendFunc(blend_source=s, blend_destination=d);
    }
  }
  //
  inline void set_stage_t(int level) const {
    if(texstage!=level) {
      this->glActiveTexture(gl_texture_bylevel[texstage = level]);
    }
  }
  inline void set_stage_c(int level) const {
    if(texstage_client!=level) {
      h_glClientActiveTexture(gl_texture_bylevel[texstage_client = level]);
    }
  }
  inline void set_stage(int level) const {
    set_stage_t(level);
    set_stage_c(level);
  }
  inline bool get_tex2d(int level) const {
    TexStageInfo &ti = texsinfos[level];
    return ti.tex2d!=0;
  }
  inline bool get_tex2d(void) const {
    TexStageInfo &ti = texsinfos[texstage];
    return ti.tex2d!=0;
  }
  inline void set_tex2d(bool mode) const {
    TexStageInfo &ti = texsinfos[texstage];
    if(ti.tex2d!=byte(mode)) {
      if(mode) this->glEnable(GL_TEXTURE_2D); 
      else this->glDisable(GL_TEXTURE_2D);
      ti.tex2d = mode;
    }
  }
  inline void set_bound(GLuint name) const {
    TexStageInfo &ti = texsinfos[texstage];
    if(ti.bound!=name) {
      this->glBindTexture(GL_TEXTURE_2D, ti.bound = name);
    }
  }
};

//***************************************
// SGLFuncsLegacy::
//***************************************

struct SGLFuncsLegacy : public SGLFuncs {
  bool is_core;
#define GL_LinkGroupBegin(type,name)
#define GL_LinkGroupEnd(type,name)
#define GL_Link_core(var,name,type,tdef)
#define GL_Link_compat(var,name,type,tdef) tdef;
  #include "umodsys.mod.ui.SDL_GL.inc.gl.h"
#undef GL_Link_core
#undef GL_Link_compat
#undef GL_LinkGroupEnd
#undef GL_LinkGroupBegin
  //
  SGLFuncsLegacy(void);
  void clear_min(void);
  void clear(void);
  bool load(bool iscore=true);
  //
  inline void set_ctc(bool mode) const {
    TexStageInfo &ti = texsinfos[texstage];
    if(ti.client_coords!=byte(mode)) {
      if(mode) this->glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
      else this->glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      ti.client_coords = mode;
    }
  }
  inline void set_composite(GLint mode) const {
    TexStageInfo &ti = texsinfos[texstage];
    if(ti.composite!=mode) {
      this->glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, ti.composite = mode);
    }
  }
  inline void set_composite(GLint mode, int level) const { 
    set_stage_t(level); 
    set_composite(mode); 
  }
  inline void set_composite_all(GLint mode) const { 
    int s = texstage;
    for(int i=0; i<max_tex_level; i++) {
      set_stage_t(i);
      set_composite(mode); 
    }
    set_stage_t(s);
  }
  //
  void set_flags(const SGLFuncs::TextureInfo* ti, const SRenderMapFlags& flags) const;
  void set_flags(const SGLFuncs::TextureInfo* ti, const SRenderMapFlags& flags, eRenderMapFlagsCombine combine) const;
  void set_tflags(const SGLFuncs::TextureInfo* ti, const SRenderMapFlags& flags) const;
};


//***************************************
// SGLFunc::TextureInfo::
//***************************************

struct SGLFuncs::TextureInfo {
  STextureGL* holder;
  mutable GLuint name;
  mutable GLint min, mag;
  mutable GLint wraps, wrapt;
  //
  inline TextureInfo(STextureGL* aholder) 
  : holder(aholder) {
    name = gl_no_texture;
    min = -1;
    mag = -1;
    wraps = -1;
    wrapt = -1;
  }
  //
  bool valid(void) const { return name!=gl_no_texture; }
  //
  inline void force(const SGLFuncs& gl) const {
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps);
    gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt);
  }
  //
  inline bool is_mipmap(void) const {
    return min==GL_NEAREST_MIPMAP_NEAREST
        || min==GL_LINEAR_MIPMAP_NEAREST
        || min==GL_NEAREST_MIPMAP_LINEAR
        || min==GL_LINEAR_MIPMAP_LINEAR;
  }
  //
  inline void set_minmag(const SGLFuncs& gl, GLint new_min, GLint new_mag) const {
    if(min!=new_min) {
      gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min = new_min);
    }
    if(mag!=new_mag) {
      gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag = new_mag);
    }
  }
  inline void set_wrap(const SGLFuncs& gl, GLint s, GLint t) const {
    if(wraps!=s) {
      gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps = s);
    }
    if(wrapt!=t) {
      gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapt = t);
    }
  }

};

//***************************************
// STextureGL::
//***************************************

struct STextureGL {
  const SGLFuncs* gl;
  SGLFuncs::TextureInfo tinfo;
  //
  SMemShared mem, memtable;
  GLenum int_format, ext_format, ext_formatsize;
  lib2d::eImageType etype;
  int elemsize;
  int width, height, palsize;
  int rwidth, rheight;
  float twidth, theight, itwidth, itheight;
  bool updated, resetsize; 
  byte mipmap;
  SRenderMapFlags inflags;
  int saved_vic;
  //
  bool valid(void) const { return tinfo.valid(); }
  //
  bool gen(void);
  void clear(void);
  void set_mipmap(void);
  void set_size(int w, int h);
  void set_palsize(int e);
  void set_args(GLenum i_f, GLenum e_f, GLenum e_fs, int es) {
    int_format=i_f; ext_format=e_f; ext_formatsize=e_fs; elemsize=es;
  }
  bool alloc(void);
  void zinit(int v=0);
  bool upload(void);
  bool reupload(void);
  void dorebind(bool fast);
  //
  bool set_reallocate(lib2d::eImageType type, int dx, int dy, BCStr hint);
  bool set_linear(const lib2d::SImagePatchInfo& info, const SCMem& linear);
  bool set_indexed(const lib2d::SImagePatchInfo& info, const SCMem& index, const SCMem& xtable);
  //
  inline void crebind(bool fast=true) { if(saved_vic!=*gl->vic) dorebind(fast); if(updated) { updated=false; reupload(); } }
  inline void bind(void) { crebind(false); gl->set_bound(tinfo.name); }
  void set_flags(const SRenderMapFlags& flags);
  //
  STextureGL(const SGLFuncs* gl);
  ~STextureGL(void);
  //
  inline static int hint_format(const DCString &h, int d) { return SGLFuncs::image_hint_format(h, d); }
  inline static int hint_format(lib2d::eImageType h, int &fmt, int &size) { return SGLFuncs::image_hint_format(h, fmt, size); }
};

//***************************************
// END
//***************************************
