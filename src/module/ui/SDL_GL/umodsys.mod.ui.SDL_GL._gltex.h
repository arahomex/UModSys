
struct SSDLVideoTextureGL;

struct SGLFuncs::TextureInfo {
  SSDLVideoTextureGL* holder;
  mutable GLuint name;
  mutable GLint min, mag;
  mutable GLint wraps, wrapt;
  //
  inline TextureInfo(SSDLVideoTextureGL* aholder) : holder(aholder) {
    name = gl_no_texture;
    min = -1;
    mag = -1;
    wraps = -1;
    wrapt = -1;
  }
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

inline void SGLFuncs::set_flags(const SGLFuncs::TextureInfo* ti, const SRenderMapFlags& flags) const
{
  if(!ti || ti->name==gl_no_texture)
    return;
  //
  if(flags.filter<rmff_Unknown) {
    ti->set_minmag(*this, mapRenderMapFlagsFilter_Min[flags.filter], mapRenderMapFlagsFilter_Mag[flags.filter]);
  }
  if(flags.wrap<rmfw_Unknown) {
    ti->set_wrap(*this, mapRenderMapFlagsWrap_S[flags.wrap], mapRenderMapFlagsWrap_T[flags.wrap]);
  }
  if(flags.combine<rmfc_Unknown) {
    set_composite_ARB(mapRenderMapFlagsCombine[flags.combine]);
  }
}

inline void SGLFuncs::set_flags(const SGLFuncs::TextureInfo* ti, const SRenderMapFlags& flags, eRenderMapFlagsCombine combine) const
{
  if(!ti || ti->name==gl_no_texture)
    return;
  //
  if(flags.filter<rmff_Unknown) {
    ti->set_minmag(*this, mapRenderMapFlagsFilter_Min[flags.filter], mapRenderMapFlagsFilter_Mag[flags.filter]);
  }
  if(flags.wrap<rmfw_Unknown) {
    ti->set_wrap(*this, mapRenderMapFlagsWrap_S[flags.wrap], mapRenderMapFlagsWrap_T[flags.wrap]);
  }
  if(combine<rmfc_Unknown) {
    set_composite_ARB(mapRenderMapFlagsCombine[combine]);
    e();
  }
}

inline void SGLFuncs::set_tflags(const SGLFuncs::TextureInfo* ti, const SRenderMapFlags& flags) const
{
  if(!ti || ti->name==gl_no_texture)
    return;
  //
  if(flags.filter<rmff_Unknown) {
    ti->set_minmag(*this, mapRenderMapFlagsFilter_Min[flags.filter], mapRenderMapFlagsFilter_Mag[flags.filter]);
  }
  if(flags.wrap<rmfw_Unknown) {
    ti->set_wrap(*this, mapRenderMapFlagsWrap_S[flags.wrap], mapRenderMapFlagsWrap_T[flags.wrap]);
  }
}
