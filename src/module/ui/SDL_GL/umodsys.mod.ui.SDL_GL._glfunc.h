//const GLuint gl_no_texture = -1;
const GLuint gl_no_texture = 0;
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

// 
// SGLFuncs class
// 
struct SGLFuncs {
  const int *vic;
  BCStr version;
  //
  const GLubyte * (APIENTRY * glGetString)(GLenum name);
  GLenum (APIENTRY * glGetError)(void);
  GLboolean (APIENTRY * glIsEnabled)(GLenum cap);
  void (APIENTRY * glGetBooleanv)(GLenum pname, GLboolean *params);
  void (APIENTRY * glGetDoublev)(GLenum pname, GLdouble *params);
  void (APIENTRY * glGetFloatv)(GLenum pname, GLfloat *params);
  void (APIENTRY * glGetIntegerv)(GLenum pname, GLint *params);
  void (APIENTRY * glHint)(GLenum target, GLenum mode);
  void (APIENTRY * glFlush)(void);
  void (APIENTRY * glFinish)(void);
  void (APIENTRY * glClear)(GLbitfield mask);
  void (APIENTRY * glClearAccum)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
  void (APIENTRY * glClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
  void (APIENTRY * glClearDepth)(GLclampd depth);
  void (APIENTRY * glClearIndex)(GLfloat c);
  void (APIENTRY * glClearStencil)(GLint s);
  void (APIENTRY * glLineWidth)(GLfloat width);
  void (APIENTRY * glLineStipple)(GLint factor, GLushort pattern);
  //
  void (APIENTRY * glMatrixMode)(GLenum mode);
  void (APIENTRY * glPushMatrix)(void);
  void (APIENTRY * glPopMatrix)(void);
  void (APIENTRY * glFrustum)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
  void (APIENTRY * glOrtho)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
  void (APIENTRY * glLoadIdentity)(void);
  void (APIENTRY * glLoadMatrixd)(const GLdouble *m);
  void (APIENTRY * glMultMatrixd)(const GLdouble *m);
  void (APIENTRY * glRotated)(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
  void (APIENTRY * glTranslated)(GLdouble x, GLdouble y, GLdouble z);
  void (APIENTRY * glScaled)(GLdouble x, GLdouble y, GLdouble z);
  //
  void (APIENTRY * glBegin)(GLenum mode);
  void (APIENTRY * glEnd)(void);
  void (APIENTRY * glVertex2fv)(const GLfloat *v);
  void (APIENTRY * glVertex2f)(GLfloat x, GLfloat y);
  void (APIENTRY * glVertex3fv)(const GLfloat *v);
  void (APIENTRY * glVertex3f)(GLfloat x, GLfloat y, GLfloat z);
  void (APIENTRY * glVertex2dv)(const GLdouble *v);
  void (APIENTRY * glVertex2d)(GLdouble x, GLdouble y);
  void (APIENTRY * glVertex3dv)(const GLdouble *v);
  void (APIENTRY * glVertex3d)(GLdouble x, GLdouble y, GLdouble z);
  void (APIENTRY * glVertex2iv)(const GLint *v);
  void (APIENTRY * glVertex2i)(GLint x, GLint y);
  void (APIENTRY * glTexCoord2f)(GLfloat s, GLfloat t);
  void (APIENTRY * glTexCoord2fv)(const GLfloat *v);
  void (APIENTRY * glTexCoord2d)(GLdouble s, GLdouble t);
  void (APIENTRY * glTexCoord2dv)(const GLdouble *v);
  void (APIENTRY * glColor3f)(GLfloat red, GLfloat green, GLfloat blue);
  void (APIENTRY * glColor3fv)(const GLfloat *v);
  void (APIENTRY * glColor4f)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
  void (APIENTRY * glColor4fv)(const GLfloat *v);
  void (APIENTRY * glColor3d)(GLdouble red, GLdouble green, GLdouble blue);
  void (APIENTRY * glColor3dv)(const GLdouble *v);
  void (APIENTRY * glColor4d)(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
  void (APIENTRY * glColor4dv)(const GLdouble *v);
  void (APIENTRY * glNormal3d)(GLdouble nx, GLdouble ny, GLdouble nz);
  void (APIENTRY * glNormal3dv)(const GLdouble *v);
  void (APIENTRY * glNormal3f)(GLfloat nx, GLfloat ny, GLfloat nz);
  void (APIENTRY * glNormal3fv)(const GLfloat *v);
  //
  void (APIENTRY * glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
  void (APIENTRY * glPushAttrib)(GLbitfield mask);
  void (APIENTRY * glPopAttrib)(void);
  void (APIENTRY * glDisable)(GLenum cap);
  void (APIENTRY * glEnable)(GLenum cap);
  void (APIENTRY * glBlendFunc)(GLenum sfactor, GLenum dfactor);
  void (APIENTRY * glDepthFunc)(GLenum func);
  void (APIENTRY * glDepthMask)(GLboolean flag);
  void (APIENTRY * glDepthRange)(GLclampd zNear, GLclampd zFar);
  void (APIENTRY * glCullFace)(GLenum mode);
  void (APIENTRY * glFrontFace)(GLenum mode);
  void (APIENTRY * glAlphaFunc)(GLenum func, GLclampf ref);
  //
  GLboolean (APIENTRY * glIsTexture)(GLuint texture);
  void (APIENTRY * glGenTextures)(GLsizei n, GLuint *textures);
  void (APIENTRY * glDeleteTextures)(GLsizei n, const GLuint *textures);
  void (APIENTRY * glBindTexture)(GLenum target, GLuint texture);
  void (APIENTRY * glTexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
  void (APIENTRY * glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
  void (APIENTRY * glTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
  void (APIENTRY * glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
  void (APIENTRY * glCopyTexImage1D)(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
  void (APIENTRY * glCopyTexImage2D)(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
  void (APIENTRY * glCopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
  void (APIENTRY * glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
  void (APIENTRY * glGetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
  void (APIENTRY * glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
  void (APIENTRY * glTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
  void (APIENTRY * glTexParameteri)(GLenum target, GLenum pname, GLint param);
  void (APIENTRY * glTexParameteriv)(GLenum target, GLenum pname, const GLint *params);
  void (APIENTRY * glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
  void (APIENTRY * glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
  void (APIENTRY * glTexEnvi)(GLenum target, GLenum pname, GLint param);
  void (APIENTRY * glTexEnvf)(GLenum target, GLenum pname, GLfloat param);
  void (APIENTRY * glPixelTransferf)(GLenum pname, GLfloat param);
  void (APIENTRY * glPixelTransferi)(GLenum pname, GLint param);
  //
  void (APIENTRY * glLightModelf)(GLenum pname, GLfloat param);
  void (APIENTRY * glLightModelfv)(GLenum pname, const GLfloat *params);
  void (APIENTRY * glLightModeli)(GLenum pname, GLint param);
  void (APIENTRY * glLightModeliv)(GLenum pname, const GLint *params);
  void (APIENTRY * glLightf)(GLenum light, GLenum pname, GLfloat param);
  void (APIENTRY * glLightfv)(GLenum light, GLenum pname, const GLfloat *params);
  void (APIENTRY * glLighti)(GLenum light, GLenum pname, GLint param);
  void (APIENTRY * glLightiv)(GLenum light, GLenum pname, const GLint *params);
  void (APIENTRY * glMaterialf)(GLenum face, GLenum pname, GLfloat param);
  void (APIENTRY * glMaterialfv)(GLenum face, GLenum pname, const GLfloat *params);
  void (APIENTRY * glMateriali)(GLenum face, GLenum pname, GLint param);
  void (APIENTRY * glMaterialiv)(GLenum face, GLenum pname, const GLint *params);
  void (APIENTRY * glShadeModel)(GLenum mode);
  //             
  // GL_EXT_vertex_array
  bool is_GL_EXT_vertex_array;
  void (APIENTRY * glVertexPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
  void (APIENTRY * glTexCoordPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
  void (APIENTRY * glColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
  void (APIENTRY * glEdgeFlagPointer)(GLsizei stride,const GLboolean *pointer);
  void (APIENTRY * glNormalPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
  void (APIENTRY * glIndexPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
  void (APIENTRY * glInterleavedArrays)(GLsizei stride, const GLvoid *pointer);
  void (APIENTRY * glArrayElement)(GLint index);
  void (APIENTRY * glEnableClientState)(GLenum array);
  void (APIENTRY * glDisableClientState)(GLenum array);
  void (APIENTRY * glDrawArrays)(GLenum mode, GLint first, GLsizei count);
  void (APIENTRY * glDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
  void (APIENTRY * glPushClientAttrib)(GLbitfield mask);
  void (APIENTRY * glPopClientAttrib)(GLbitfield mask);
  //
  // GL_ARB_vertex_buffer_object
  bool is_GL_ARB_vertex_buffer_object;
  void (APIENTRY * glBindBufferARB)(GLenum, GLuint);
  void (APIENTRY * glDeleteBuffersARB)(GLsizei, const GLuint *);
  void (APIENTRY * glGenBuffersARB)(GLsizei, GLuint *);
  GLboolean (APIENTRY * glIsBufferARB)(GLuint);
  void (APIENTRY * glBufferDataARB)(GLenum, GLsizeiptr, const GLvoid *, GLenum);
  void (APIENTRY * glBufferSubDataARB)(GLenum, GLintptr, GLsizeiptr, const GLvoid *);
  void (APIENTRY * glGetBufferSubDataARB)(GLenum, GLintptr, GLsizeiptr, GLvoid *);
  GLvoid* (APIENTRY * glMapBufferARB)(GLenum, GLenum);
  GLboolean (APIENTRY * glUnmapBufferARB)(GLenum);
  void (APIENTRY * glGetBufferParameterivARB)(GLenum, GLenum, GLint *);
  void (APIENTRY * glGetBufferPointervARB)(GLenum, GLenum, GLvoid* *);
  //
  // GL_EXT_paletted_texture
  bool is_GL_EXT_paletted_texture;
  void (APIENTRY *glColorTableEXT)(GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
  void (APIENTRY *glGetColorTableEXT)(GLenum, GLenum, GLenum, GLvoid *);
  void (APIENTRY *glGetColorTableParameterivEXT)(GLenum, GLenum, GLint *);
  void (APIENTRY *glGetColorTableParameterfvEXT)(GLenum, GLenum, GLfloat *);
  //
  // GL_VERSION_1_2
  bool is_GL_VERSION_1_2;
  void (APIENTRY *glColorTable)(GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *);
  void (APIENTRY *glColorTableParameterfv)(GLenum, GLenum, const GLfloat *);
  void (APIENTRY *glColorTableParameteriv)(GLenum, GLenum, const GLint *);
  //
  // GL_VERSION_1_3
  bool is_GL_VERSION_1_3;
  GLint max_GL_VERSION_1_3_multitexture;
  void (APIENTRY *glActiveTexture)(GLenum);
  void (APIENTRY *glClientActiveTexture)(GLenum);
  void (APIENTRY *glMultiTexCoord2d)(GLenum, GLdouble, GLdouble);
  void (APIENTRY *glMultiTexCoord2dv)(GLenum, const GLdouble *);
  void (APIENTRY *glMultiTexCoord2f)(GLenum, GLfloat, GLfloat);
  void (APIENTRY *glMultiTexCoord2fv)(GLenum, const GLfloat *);
  // 
  // GL_ARB_multitexture
  bool is_GL_ARB_multitexture;
  GLint max_GL_ARB_multitexture;
  void (APIENTRY *glActiveTextureARB)(GLenum);
  void (APIENTRY *glClientActiveTextureARB)(GLenum);
  void (APIENTRY *glMultiTexCoord2dARB)(GLenum, GLdouble, GLdouble);
  void (APIENTRY *glMultiTexCoord2dvARB)(GLenum, const GLdouble *);
  void (APIENTRY *glMultiTexCoord2fARB)(GLenum, GLfloat, GLfloat);
  void (APIENTRY *glMultiTexCoord2fvARB)(GLenum, const GLfloat *);
  //
  bool link(void);
  bool load(void) {
    if(!link()) {
      if(SDL_GL_LoadLibrary(NULL))
        return false;
      return link();
    }
    return true;
  }
  void unload(void) {
  }
  //
  void dumperror(int error) const;
  inline void e(void) const {
    int error = glGetError();
    if(error!=GL_NO_ERROR) {
      dumperror(error);
    }
  }
  //
  SGLFuncs(const int *pvic) { 
    memset(this, 0, sizeof(*this)); 
    blend_enabled = -1;
    vic = pvic;
  }
  void clear(void) {
    const int *pvic = vic;
    memset(this, 0, sizeof(*this)); 
    blend_enabled = -1;
    vic = pvic;
  }
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
  struct TextureInfo;
  //
  mutable TexStageInfo ARB_texsinfos[gl_ARB_multitex_levels];
  mutable TexStageInfo texsinfos[gl_multitex_levels];
  mutable int ARB_texstage, ARB_texstage_client;
  mutable int texstage;
  mutable GLenum blend_source, blend_destination, blend_enabled;
  mutable int max_tex_level, tex_levels;
  //
  void delete_textures(GLsizei n, const GLuint* texs) const;
  //
  inline void set_blend(bool mode) const {
    if(blend_enabled!=int(mode)) {
      if(mode) glEnable(GL_BLEND); 
      else glDisable(GL_BLEND);
      blend_enabled = mode;
    }
  }
  inline void set_blend(GLenum s, GLenum d) const {
    if(blend_source!=s || blend_destination!=d) {
      glBlendFunc(blend_source=s, blend_destination=d);
    }
  }
  //
  inline void set_stage_ARB_t(int level) const {
    if(ARB_texstage!=level) {
      glActiveTextureARB(gl_texture_bylevel_ARB[ARB_texstage = level]);
    }
  }
  inline void set_stage_ARB_c(int level) const {
    if(ARB_texstage_client!=level) {
      glClientActiveTextureARB(gl_texture_bylevel_ARB[ARB_texstage_client = level]);
    }
  }
  inline void set_stage_ARB(int level) const {
    set_stage_ARB_t(level);
    set_stage_ARB_c(level);
  }
  inline void set_composite_ARB(GLint mode) const {
    TexStageInfo &ti = ARB_texsinfos[ARB_texstage];
    if(ti.composite!=mode) {
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, ti.composite = mode);
    }
  }
  inline void set_composite_ARB(GLint mode, int level) const { set_stage_ARB_t(level); set_composite_ARB(mode); }
  inline void set_composite_ARB_all(GLint mode) const { 
    int s = ARB_texstage;
    for(int i=0; i<max_tex_level; i++) {
      set_stage_ARB_t(i);
      set_composite_ARB(mode); 
    }
    set_stage_ARB_t(s);
  }
  inline bool get_tex2d_ARB(int level) const {
    TexStageInfo &ti = ARB_texsinfos[level];
    return ti.tex2d!=0;
  }
  inline bool get_tex2d_ARB(void) const {
    TexStageInfo &ti = ARB_texsinfos[ARB_texstage];
    return ti.tex2d!=0;
  }
  inline void set_tex2d_ARB(bool mode) const {
    TexStageInfo &ti = ARB_texsinfos[ARB_texstage];
    if(ti.tex2d!=byte(mode)) {
      if(mode) glEnable(GL_TEXTURE_2D); 
      else glDisable(GL_TEXTURE_2D);
      ti.tex2d = mode;
    }
  }
  inline void set_bound_ARB(GLuint name) const {
    TexStageInfo &ti = ARB_texsinfos[ARB_texstage];
    if(ti.bound!=name) {
      glBindTexture(GL_TEXTURE_2D, ti.bound = name);
    }
  }
  inline void set_ctc_ARB(bool mode) const {
    TexStageInfo &ti = ARB_texsinfos[ARB_texstage];
    if(ti.client_coords!=byte(mode)) {
      if(mode) glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
      else glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      ti.client_coords = mode;
    }
  }
  static int image_hint_format(const DHString &hinttype, int defvalue);
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
};

