
//***************************************
// STextureGL::
//***************************************

inline void STextureGL::set_flags(const SRenderMapFlags& flags) 
{ 
  if(flags.filter!=rmff_Unknown) {
    bool mips = (flags.filter>=rmff_MipNearest && flags.filter<=rmff_LinearMipLinear);
    if((mipmap!=0)!=mips) { if(mips) mipmap=0; else set_mipmap(); }
  }
  inflags.merge(flags); 
  gl->set_flags(&tinfo, inflags); 
}

//***************************************
// SGLFuncs::
//***************************************

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
    // ti->set_composite(mapRenderMapFlagsCombine[flags.combine]);
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
    //ti->set_composite(mapRenderMapFlagsCombine[combine]); e();
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

//***************************************
// SGLFuncsLegacy::
//***************************************

inline void SGLFuncsLegacy::set_flags(const SGLFuncs::TextureInfo* ti, const SRenderMapFlags& flags) const
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
    set_composite(mapRenderMapFlagsCombine[flags.combine]);
  }
}

inline void SGLFuncsLegacy::set_flags(const SGLFuncs::TextureInfo* ti, const SRenderMapFlags& flags, eRenderMapFlagsCombine combine) const
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
    set_composite(mapRenderMapFlagsCombine[combine]);
    e();
  }
}

inline void SGLFuncsLegacy::set_tflags(const SGLFuncs::TextureInfo* ti, const SRenderMapFlags& flags) const
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

//***************************************
// END
//***************************************
