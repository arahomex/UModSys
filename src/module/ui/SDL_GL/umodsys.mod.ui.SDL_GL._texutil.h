//#define NO_PURE_ALPHA

// 
// SSDLVideoTextureGL::
// 

SSDLVideoTextureGL::SSDLVideoTextureGL(const SGLFuncs& agl) 
  : gl(agl), tinfo(this), inflags(rmff_Nearest, rmfc_Normal, rmfw_NoWrap) {
  width = height = palsize = 0;
  updated = false;
  mipmap = 0;
  //
  gl.glGenTextures(1, &tinfo.name);
  gl.e();
  saved_vic = *gl.vic;
}

SSDLVideoTextureGL::~SSDLVideoTextureGL(void) {
  if(tinfo.name!=gl_no_texture && saved_vic==*gl.vic)
    gl.delete_textures(1, &tinfo.name);
}

void SSDLVideoTextureGL::dorebind(bool fast)
{
  if(tinfo.name!=gl_no_texture && saved_vic==*gl.vic)
    gl.delete_textures(1, &tinfo.name);
  //
  gl.glGenTextures(1, &tinfo.name);
  gl.e();
  //
  saved_vic = *gl.vic;
  if(!fast) {
    upload();
  }
}

void SSDLVideoTextureGL::zinit(int v) 
{ 
  memset(mem.get_data(), v, mem.get_size()); 
  if(memtable.get_data()) 
    memset(memtable.get_data(), 0, memtable.get_size()); 
}

bool SSDLVideoTextureGL::alloc(void) {
  int size = width*height*elemsize;
  if(mipmap) {
    int s = size>>2;
    for(int i=1; i<mipmap; i++, s>>=2)
      size += s;
  } 
  //
  if(!M.mem_alloc(mem, size, UMS_SOURCEINFO))
    return false;
  if(palsize) {
    if(!M.mem_alloc(memtable, palsize*4, UMS_SOURCEINFO)) {
      return false; // RGBA
    }
  }
  return true;
}

void SSDLVideoTextureGL::set_mipmap(void) {
  mipmap = 0;
  int w = width, h = height;
  while(w && h) {
    w>>=1; h>>=1;
    mipmap++;
  }
}

void SSDLVideoTextureGL::set_size(int w, int h) 
{ 
  rwidth = w;
  rheight = h;
  width = math3d::one_bit_container(w);
  height = math3d::one_bit_container(h); 
  twidth = float(w)/width;
  theight = float(h)/height;
  itwidth = float(1)/width;
  itheight = float(1)/height;
  //
  if(tinfo.is_mipmap())
    set_mipmap();
}

void SSDLVideoTextureGL::set_palsize(int e) 
{ 
    palsize = e; 
    set_args(GL_COLOR_INDEX8_EXT, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, 1);
}

bool SSDLVideoTextureGL::reupload(void) 
{
  using namespace lib2d;
  if(resetsize) {
    return upload();
  }
  crebind();
  gl.set_bound_ARB(tinfo.name);
  gl.glTexSubImage2D(GL_TEXTURE_2D, 0, 
                     0, 0, width, height,
                     ext_format, ext_formatsize, mem.get_data());
  if(mipmap) { // create & upload mipmaps
    SCMem mips[256];
    if(!bin_build_mipmap(mips, mem, SImagePatchInfo(etype, width, height), mipmap))
      return false;
    int w = width, h = height;
    for(int i=1; i<mipmap; i++) {
      w>>=1; h>>=1;
      gl.glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0, w, h, ext_format, ext_formatsize, mips[i].data);
    }
  }
  updated = false;
  gl.e();
  return true;
}

bool SSDLVideoTextureGL::upload(void) 
{
  using namespace lib2d;
  crebind();
  gl.set_bound_ARB(tinfo.name);
  if(memtable.get_data()) { // upload colortable
    gl.e();
    gl.glColorTableEXT(GL_TEXTURE_2D, GL_RGBA, palsize, 
                       GL_RGBA, GL_UNSIGNED_BYTE, memtable.get_data());
    gl.e();
  }
  //memset(mem.get_data(), 255, width*height*elemsize/4);
  //memset(mem.get_data(), 255, 4*3);
  gl.glTexImage2D(GL_TEXTURE_2D, 0, int_format, width, height, 0, 
                  ext_format, ext_formatsize, mem.get_data());
  gl.e();
  //
  if(mipmap) { // create & upload mipmaps
    SCMem mips[24];
    if(!bin_build_mipmap(mips, mem, SImagePatchInfo(etype, width, height), mipmap))
      return false;
    int w = width, h = height;
    for(int i=1; i<mipmap; i++) {
      w>>=1; h>>=1;
      gl.glTexImage2D(GL_TEXTURE_2D, i, int_format, w, h, 0, ext_format, ext_formatsize, mips[i].data);
      gl.e();
    }
  }
  gl.set_flags(&tinfo, inflags);
  tinfo.force(gl);
  updated = false;
  resetsize = false;
  gl.e();
  return true;
}

bool SSDLVideoTextureGL::set_reallocate(lib2d::eImageType aetype, int dx, int dy, BCStr hint)
{
  using namespace lib2d;
  if(aetype==lib2d::it_I8_R8G8B8) aetype = lib2d::it_R8G8B8;
  else if(aetype==lib2d::it_I8_R8G8B8A8) aetype = lib2d::it_R8G8B8A8;
  int ofmt, osize;
  int type = hint_format(hint, hint_format(aetype, ofmt, osize));
  if(type==0)
    return false;
  set_size(dx, dy);
  etype = aetype; 
  set_args(type, ofmt, GL_UNSIGNED_BYTE, osize);
  resetsize = updated = true;
  return alloc();
}

bool SSDLVideoTextureGL::set_linear(const lib2d::SImagePatchInfo& info, const SCMem& linear)
{
  using namespace lib2d;
  BByte alpha0 = 0xff;
  updated = true;
  if(etype==info.type)
    return bin_move(mem, SImagePatchInfo(etype, width, height), linear, info);
  switch(etype) {
    case it_L8A8:
      switch(info.type) {
        case it_L8:
          return bin_move_cvt(mem, SImagePatchInfo(etype, width, height), linear, info, bin_cvt_L8_L8A8, NULL);
      } break;
    case it_R8G8B8:
      switch(info.type) {
        case it_R8G8B8A8:
          return bin_move_cvt(mem, SImagePatchInfo(etype, width, height), linear, info, bin_cvt_X24A8_X24, NULL);
      } break;
    case it_R8G8B8A8:
      switch(info.type) {
        case it_R8G8B8:
          return bin_move_cvt(mem, SImagePatchInfo(etype, width, height), linear, info, bin_cvt_X24_X24A8, &alpha0);
      } break;
  }
  return false;
}

bool SSDLVideoTextureGL::set_indexed(const lib2d::SImagePatchInfo& info, const SCMem& index, const SCMem& xtable)
{
  using namespace lib2d;
  BByte alpha0 = 0xff;
  updated = true;
  switch(etype) {
    case it_R8G8B8:
      switch(info.type) {
        case it_I8_R8G8B8:
          if(xtable.size<0x100*3)
            return false;
          return bin_move_cvt2(mem, SImagePatchInfo(etype, width, height), index, xtable, info, bin_cvt_I8pX24_X24, NULL);
        case it_I8_R8G8B8A8:
          if(xtable.size<0x100*4)
            return false;
          return bin_move_cvt2(mem, SImagePatchInfo(etype, width, height), index, xtable, info, bin_cvt_I8pX24_X24, NULL);
      } break;
    case it_R8G8B8A8:
      switch(info.type) {
        case it_I8_R8G8B8:
          if(xtable.size<0x100*3)
            return false;
          return bin_move_cvt2(mem, SImagePatchInfo(etype, width, height), index, xtable, info, bin_cvt_I8pX24A8_X24A8, &alpha0);
        case it_I8_R8G8B8A8:
          if(xtable.size<0x100*4)
            return false;
          return bin_move_cvt2(mem, SImagePatchInfo(etype, width, height), index, xtable, info, bin_cvt_I8pX24A8_X24A8, NULL);
      } break;
  }
  return false;
}
