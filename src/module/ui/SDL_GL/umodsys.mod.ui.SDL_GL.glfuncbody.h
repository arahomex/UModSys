// 
// SGLFuncs::
// 

GLenum SGLFuncs::mapRenderMapFlagsFilter_Min[rmff_Unknown+1] = {
  GL_NEAREST,                 // rmff_Disabled,
  GL_NEAREST,                 // rmff_Nearest,
  GL_LINEAR,                  // rmff_Linear,
  GL_NEAREST_MIPMAP_NEAREST,  // rmff_MipNearest,
  GL_LINEAR_MIPMAP_NEAREST,   // rmff_MipLinear,
  GL_NEAREST_MIPMAP_LINEAR,   // rmff_LinearMipNearest,
  GL_LINEAR_MIPMAP_LINEAR,    // rmff_LinearMipLinear,
  GL_NEAREST                  // rmff_Unknown
};
GLenum SGLFuncs::mapRenderMapFlagsFilter_Mag[rmff_Unknown+1] = {
  GL_NEAREST,                 // rmff_Disabled,
  GL_NEAREST,                 // rmff_Nearest,
  GL_LINEAR,                  // rmff_Linear,
  GL_NEAREST,                 // rmff_MipNearest,
  GL_LINEAR,                  // rmff_MipLinear,
  GL_LINEAR,                  // rmff_LinearMipNearest,
  GL_LINEAR,                  // rmff_LinearMipLinear,
  GL_NEAREST                  // rmff_Unknown
};
GLenum SGLFuncs::mapRenderMapFlagsWrap_S[rmfw_Unknown+1] = {
  GL_CLAMP,           // rmfw_Disabled,
  GL_CLAMP,           // rmfw_NoWrap,
  GL_REPEAT,          // rmfw_UWrap,
  GL_CLAMP,           // rmfw_VWrap,
  GL_REPEAT,          // rmfw_UVWrap,
  GL_CLAMP_TO_EDGE,   // rmfw_Edge
  GL_CLAMP            // rmfw_Unknown
};
GLenum SGLFuncs::mapRenderMapFlagsWrap_T[rmfw_Unknown+1] = {
  GL_CLAMP,           // rmfw_Disabled,
  GL_CLAMP,           // rmfw_NoWrap,
  GL_CLAMP,           // rmfw_UWrap,
  GL_REPEAT,          // rmfw_VWrap,
  GL_REPEAT,          // rmfw_UVWrap,
  GL_CLAMP_TO_EDGE,   // rmfw_Edge
  GL_CLAMP            // rmfw_Unknown
};
GLenum SGLFuncs::mapRenderMapFlagsCombine[rmfc_Unknown+1] = {
  GL_REPLACE,     // rmfc_Disabled,
  GL_MODULATE,    // rmfc_Normal,
  GL_REPLACE,     // rmfc_Replace,
  GL_MODULATE,    // rmfc_Modulate,
  GL_BLEND,       // rmfc_Blend,
  GL_DECAL,       // rmfc_Decal,
  GL_ADD,         // rmfc_Add
  GL_ADD,         // rmfc_Enlight,
  GL_DECAL,       // rmfc_Complex,
  GL_BLEND,       // rmfc_Ghost
  GL_MODULATE,    // rmfc_Lightmap
  GL_REPLACE,     // rmfc_Unknown
};                                
GLenum SGLFuncs::mapTransparrentMode_SRC[lib2d::tm_Unknown+1] = {
  GL_ONE,                     // tm_Opaque      // no transparrency at all
  GL_DST_COLOR,               // tm_Glass       // light filter (darken other)
  GL_ONE_MINUS_DST_COLOR,     // tm_DarkGlass   // light filter (invert darken other)
  GL_SRC_ALPHA,               // tm_Lighten     // add color-to-color
  GL_ONE_MINUS_SRC_ALPHA,     // tm_AlphaLighten// add color-to-color
  GL_DST_COLOR,               // tm_ColorLighten// add color-to-color
  GL_SRC_ALPHA,               // tm_Glow        // blending old one & new
  GL_SRC_ALPHA,               // tm_SemiGlow    // light filter & some glow
  GL_DST_COLOR,               // tm_SemiGlass   // light filter & some glow
  0
};
GLenum SGLFuncs::mapTransparrentMode_DST[lib2d::tm_Unknown+1] = {
  GL_ZERO,                    // tm_Opaque      // no transparrency at all
  GL_ZERO,                    // tm_Glass       // light filter (darken other)
  GL_ZERO,                    // tm_DarkGlass   // light filter (invert darken other)
  GL_ONE,                     // tm_Lighten     // add color-to-color
  GL_ONE,                     // tm_AlphaLighten// add color-to-color
  GL_ONE,                     // tm_ColorLighten// add color-to-color
  GL_ONE_MINUS_SRC_ALPHA,     // tm_Glow        // blending old one & new
  GL_SRC_COLOR,               // tm_SemiGlow    // light filter & some glow
  GL_ONE_MINUS_SRC_ALPHA,     // tm_SemiGlass   // light filter & some glow
  0
};

void SGLFuncs::delete_textures(GLsizei n, const GLuint* texs) const 
{
  for(int i, k=0; k<n; k++) {
    for(i=0; i<gl_ARB_multitex_levels; i++) {
      if(ARB_texsinfos[i].bound==texs[k])
        ARB_texsinfos[i].clear();
    }
    for(i=0; i<gl_multitex_levels; i++) {
      if(texsinfos[i].bound==texs[k])
        texsinfos[i].clear();
    }
  }
}


int SGLFuncs::image_hint_format(lib2d::eImageType type, int &fmt, int &size)
{
#define R(itype, rval, _fmt, _size) case lib2d::itype: fmt=_fmt; size=_size; return rval;
  switch(type) {
    R(it_R8G8B8, GL_RGB, GL_RGB, 3)
    R(it_R8G8B8A8, GL_RGBA, GL_RGBA, 4)
    R(it_L8, GL_LUMINANCE, GL_LUMINANCE, 1)
    R(it_L8A8, GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA, 2)
    R(it_I8_R8G8B8, GL_RGB, GL_RGB, 3)
    R(it_I8_R8G8B8A8, GL_RGBA, GL_RGBA, 4)
#undef R
    default: return 0;
  }
}

int SGLFuncs::image_hint_format(const DHString &hinttype, int defvalue) 
{
  // rgba formats
  if(defvalue==GL_RGBA) {
    if(hinttype=="rgba/8" || hinttype=="rgba/4")
      return GL_RGBA2;
    if(hinttype=="rgba/12" || hinttype=="rgba/16" || hinttype=="rgba/15")
      return GL_RGBA4;
    if(hinttype=="rgb/15,a1")
      return GL_RGB5_A1;
    if(hinttype=="rgba/32")
      return GL_RGBA8;
    return defvalue;
  }
  // rgb formats
  if(defvalue==GL_RGB) {
    if(hinttype=="rgb/8" || hinttype=="rgb/4")
      return GL_R3_G3_B2;
    if(hinttype=="rgba/12")
      return GL_RGB4;
    if(hinttype=="rgba/16" || hinttype=="rgba/15")
      return GL_RGB5;
    if(hinttype=="rgba/24")
      return GL_RGB8;
    return defvalue;
  }
  // i formats (L)
  if(defvalue==GL_LUMINANCE) {
    if(hinttype=="i4")
      return GL_LUMINANCE4;
    if(hinttype=="i8")
      return GL_LUMINANCE8;
    if(hinttype=="ia/2" || hinttype=="ia/4")
      return GL_LUMINANCE6_ALPHA2;
    if(hinttype=="ia/8")
      return GL_LUMINANCE4_ALPHA4;
    if(hinttype=="ia/16" || hinttype=="ia/12")
      return GL_LUMINANCE8_ALPHA8;
    return defvalue;
  }
  // i formats (I)
  if(defvalue==GL_INTENSITY) {
    if(hinttype=="i4")
      return GL_INTENSITY4;
    if(hinttype=="i8")
      return GL_INTENSITY8;
    return defvalue;
  }
  // a formats
  if(defvalue==GL_ALPHA) {
    if(hinttype=="a4" || hinttype=="i4")
      return GL_ALPHA4;
    if(hinttype=="a8" || hinttype=="i8")
      return GL_ALPHA8;
    return defvalue;
  }
  // i+a formats
  if(defvalue==GL_LUMINANCE_ALPHA) {
    if(hinttype=="ia/2" || hinttype=="ia/4")
      return GL_LUMINANCE6_ALPHA2;
    if(hinttype=="ia/8")
      return GL_LUMINANCE4_ALPHA4;
    if(hinttype=="ia/16" || hinttype=="ia/12")
      return GL_LUMINANCE8_ALPHA8;
    return defvalue;
  }
  // unknown
  return defvalue;
}

void SGLFuncs::dumperror(int error) const 
{
      const char *err = NULL;
      switch(error) {
#define E(x) case x: err = #x; break;
          E(GL_NO_ERROR)
          E(GL_INVALID_ENUM)
          E(GL_INVALID_VALUE)
          E(GL_INVALID_OPERATION)
          E(GL_STACK_OVERFLOW)
          E(GL_STACK_UNDERFLOW)
          E(GL_OUT_OF_MEMORY)
#undef E
          default:
            err = "GL_?_ERROR";
      }
      M.debug_put(dps_Error, "GL Error #%s\n", err);
}

bool SGLFuncs::link(void) 
{
    bool *flag = NULL;
#define F(x) if((*(void**)&this->x = ::SDL_GL_GetProcAddress(#x))==NULL) return false;
#define FX(x) if(*flag) { if((*(void**)&this->x = ::SDL_GL_GetProcAddress(#x))==NULL) *flag = false; }
#define FEXT(_flag) flag = &is_##_flag; *flag = true;
    F(glGetString)
    F(glGetError)
    F(glIsEnabled)
    F(glGetBooleanv)
    F(glGetDoublev)
    F(glGetFloatv)
    F(glGetIntegerv)
    F(glHint)
    F(glFlush)
    F(glFinish)
    F(glClear)
    F(glClearAccum)
    F(glClearColor)
    F(glClearDepth)
    F(glClearIndex)
    F(glClearStencil)
    F(glLineStipple)
    F(glLineWidth)
    //
    F(glMatrixMode)
    F(glPushMatrix)
    F(glPopMatrix)
    F(glFrustum)
    F(glOrtho)
    F(glLoadIdentity)
    F(glLoadMatrixd)
    F(glMultMatrixd)
    F(glRotated)
    F(glTranslated)
    F(glScaled)
    //
    F(glBegin)
    F(glEnd)
    F(glVertex3f)
    F(glVertex3fv)
    F(glVertex2f)
    F(glVertex2fv)
    F(glTexCoord2f);
    F(glTexCoord2fv);
    F(glColor4f)
    F(glColor4fv)
    F(glColor3f)
    F(glColor3fv)
    F(glVertex3d)
    F(glVertex3dv)
    F(glVertex2d)
    F(glVertex2dv)
    F(glTexCoord2d);
    F(glTexCoord2dv);
    F(glColor4d)
    F(glColor4dv)
    F(glColor3d)
    F(glColor3dv)
    F(glVertex2i)
    F(glVertex2iv)
    F(glNormal3d)
    F(glNormal3dv)
    F(glNormal3f)
    F(glNormal3fv)
    //
    F(glPushAttrib)
    F(glPopAttrib)
    F(glDisable)
    F(glEnable)
    F(glBlendFunc)
    F(glDepthFunc)
    F(glAlphaFunc)
    F(glDepthMask)
    F(glDepthRange)
    F(glCullFace)
    F(glFrontFace);
    F(glViewport)
    //
    F(glIsTexture)
    F(glGenTextures)
    F(glBindTexture)
    F(glDeleteTextures)
    F(glTexImage1D)
    F(glTexImage2D)
    F(glTexSubImage1D)
    F(glTexSubImage2D)
    F(glCopyTexImage1D)
    F(glCopyTexImage2D)
    F(glCopyTexSubImage1D)
    F(glCopyTexSubImage2D)
    F(glGetTexImage)
    F(glTexParameterf)
    F(glTexParameterfv)
    F(glTexParameteri)
    F(glTexParameteriv)
    F(glGetTexLevelParameterfv)
    F(glGetTexLevelParameteriv)
    F(glTexEnvi)
    F(glTexEnvf)
    F(glPixelTransferf)
    F(glPixelTransferi)
    //
    F(glLightModelf)
    F(glLightModelfv)
    F(glLightModeli)
    F(glLightModeliv)
    F(glLightf)
    F(glLightfv)
    F(glLighti)
    F(glLightiv)
    F(glMaterialf)
    F(glMaterialfv)
    F(glMateriali)
    F(glMaterialiv)
    F(glShadeModel);
    //
    // GL_VERSION_1_2
    FEXT(GL_VERSION_1_2)
    FX(glColorTable)
    FX(glColorTableParameterfv)
    FX(glColorTableParameteriv)
    //
    // GL_VERSION_1_3
    FEXT(GL_VERSION_1_3)
    FX(glActiveTexture)
    FX(glClientActiveTexture)
    FX(glMultiTexCoord2d)
    FX(glMultiTexCoord2dv)
    FX(glMultiTexCoord2f)
    FX(glMultiTexCoord2fv)
    max_GL_VERSION_1_3_multitexture = 0;
    if(is_GL_VERSION_1_3) {
      glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &max_GL_VERSION_1_3_multitexture);
    }
    //
    // EXT_vertex_array
    FEXT(GL_EXT_vertex_array)
    FX(glVertexPointer)
    FX(glTexCoordPointer)
    FX(glColorPointer)
    FX(glEdgeFlagPointer)
    FX(glNormalPointer)
    FX(glIndexPointer)
    FX(glInterleavedArrays)
    FX(glArrayElement)
    FX(glEnableClientState)
    FX(glDisableClientState)
    FX(glDrawArrays)
    FX(glDrawElements)
    FX(glPushClientAttrib)
    FX(glPopClientAttrib)
    //
    // GL_ARB_multitexture
    FEXT(GL_ARB_multitexture)
    FX(glActiveTextureARB)
    FX(glClientActiveTextureARB)
    FX(glMultiTexCoord2dARB)
    FX(glMultiTexCoord2dvARB)
    FX(glMultiTexCoord2fARB)
    FX(glMultiTexCoord2fvARB)
    max_GL_ARB_multitexture = 0;
    if(is_GL_ARB_multitexture) {
      glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &max_GL_ARB_multitexture);
    }
    //
    // GL_EXT_paletted_texture
    FEXT(GL_EXT_paletted_texture)
    FX(glColorTableEXT)
    FX(glGetColorTableEXT)
    FX(glGetColorTableParameterivEXT)
    FX(glGetColorTableParameterfvEXT)
    //
    // GL_ARB_vertex_buffer_object
    FEXT(GL_ARB_vertex_buffer_object);
    FX(glBindBufferARB)
    FX(glDeleteBuffersARB)
    FX(glGenBuffersARB)
    FX(glIsBufferARB)
    FX(glBufferDataARB)
    FX(glBufferSubDataARB)
    FX(glGetBufferSubDataARB)
    FX(glMapBufferARB)
    FX(glUnmapBufferARB)
    FX(glGetBufferParameterivARB)
    FX(glGetBufferPointervARB)
    //
//    F()
//    F()
#undef FEXT
#undef FX
#undef F
    //
    version = reinterpret_cast<BCStr>(glGetString(GL_VERSION));
    //
//    glEnd();
//    e();
    return true;
}




