
//***************************************
// RVertexArray::
//***************************************

BByte RVertexArray::vat_sizes[vaet__Count] = {
  0,                // vaet_None,
  sizeof(sint8),    // vaet_Int8,
  sizeof(uint8),    // vaet_UInt8,
  sizeof(sint8),    // vaet_Int16,
  sizeof(uint8),    // vaet_UInt16,
  sizeof(sint8),    // vaet_Int32,
  sizeof(uint8),    // vaet_UInt32,
  sizeof(float32),  // vaet_Float32,
  sizeof(float64)   // vaet_Float64,
};

//***************************************

RVertexArray::RVertexArray(RRenderDriver3D *pv)
: refs(pv), 
  count(0), vbo(gl_no_buffer), 
  cache(NULL), uploaded(false), created(false)
{
  gl = &pv->gl;
}

RVertexArray::~RVertexArray(void) 
{
  Free();
}

//***************************************

bool RVertexArray::Free(void)
{
  layers.Clear();
  count = 0;
  uploaded = true;
  created = false;
  //
  if(cache!=NULL) {
    M.mem().mem_free(cache, UMODSYS_SOURCEINFO);
    cache = NULL;
  }
  elem_size = 0;
  cache_size = 0;
  //
  if(vbo!=gl_no_buffer) {
    gl->glDeleteBuffers(1, &vbo);
    vbo = gl_no_buffer;
    return true;
  }
  return false;
}

bool RVertexArray::Alloc(const SVertexElemInfo lays[], int lcount, int ecount, bool single)
{
  Free();
  //
  if(lcount<=0 || ecount<=0)
    return false;
  //
  fill.clear();
  if(!layers.Resize(lcount)) {
    Free();
    return false;
  }
  //
  int pos = 0;
  for(int i=0; i<lcount; i++) {
    SVertexElemInfo x = lays[i];
    if(!fill.check(x)) {
      Free();
      return false;
    }
    if(single && !fill.check_single(x)) {
      Free();
      return false;
    }
    SLayInfo& y = layers[i];
    //
    y.vei = x;
    y.start = pos;
    switch(x.aitype) {
      case vaet_Int8: y.type = GL_BYTE; break;
      case vaet_UInt8: y.type = GL_UNSIGNED_BYTE; break;
      case vaet_Int16: y.type = GL_SHORT; break;
      case vaet_UInt16: y.type = GL_UNSIGNED_SHORT; break;
      case vaet_Int32: y.type = GL_INT; break;
      case vaet_UInt32: y.type = GL_UNSIGNED_INT; break;
      case vaet_Float32: y.type = GL_FLOAT; break;
      case vaet_Float64: y.type = GL_DOUBLE; break;
      default:
        Free();
        return false;
    }
    y.num = x.acount;
    pos += vat_sizes[x.aitype]*x.acount;
  }
  for(int i=0; i<lcount; i++) {
    layers[i].stride = pos;
  }
  elem_size = pos;
  cache_size = pos * ecount;
  count = ecount;
  //
  gl->glGenBuffers(1, &vbo);
  if(!Alloc())
    return false;
  return true;
}

bool RVertexArray::Alloc(void)
{
  if(cache!=NULL)
    return true;
  void *p = M.mem().mem_alloc(cache_size, UMODSYS_SOURCEINFO);
  if(p==NULL) {
    Free();
    return false;
  }
  memset(p, 0, cache_size);
  cache = reinterpret_cast<BByte*>(p);
  uploaded = false;
  return true;
}

bool RVertexArray::Upload(bool final)
{
  if(uploaded)
    return true;
  if(vbo==gl_no_buffer || cache==NULL)
    return false;
  //
  gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);
  if(!created) {
    gl->glBufferData(GL_ARRAY_BUFFER, cache_size, cache, final ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    created = true;
  } else {
    gl->glBufferSubData(GL_ARRAY_BUFFER, 0, cache_size, cache);
  }
  //
  if(final) {
    M.mem().mem_free(cache, UMODSYS_SOURCEINFO);
    cache = NULL;
  }
  uploaded = true;
  return true;
}

bool RVertexArray::Use(unsigned laymask)
{
  if(!Upload())
    return false;
  gl->glBindBuffer(GL_ARRAY_BUFFER, vbo);
  for(int i=0; i<~layers; i++) {
    if((laymask & (1u<<i))==0)
      continue;
    const SLayInfo& y = layers[i];
    switch(y.vei.vclass) {
      case vc_Coord:
        gl->glVertexPointer(y.num, y.type, y.stride, (const GLvoid *)y.start);
        gl->glEnableClientState(GL_VERTEX_ARRAY);
        break;
      case vc_Normal:
//        gl->glNormalPointer(y.vei.acount, y.type, y.stride, (const GLvoid *)y.start);
        gl->glNormalPointer(y.type, y.stride, (const GLvoid *)y.start);
        gl->glEnableClientState(GL_NORMAL_ARRAY);
        break;
      case vc_Color:
        gl->glColorPointer(y.num, y.type, y.stride, (const GLvoid *)y.start);
        gl->glEnableClientState(GL_COLOR_ARRAY);
        break;
      case vc_TexCoord:
        gl->set_stage(y.vei.inf);
        gl->set_tex2d(true);
        gl->glTexCoordPointer(y.num, y.type, y.stride, (const GLvoid *)y.start);
        gl->glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        break;
      case vc_Edge:
//        gl->glEdgeFlagPointer(y.vei.acount, y.type, y.stride, (const GLvoid *)y.start);
        gl->glEdgeFlagPointer(y.stride, (const GLvoid *)y.start);
        gl->glEnableClientState(GL_EDGE_FLAG_ARRAY);
        break;
      case vc_Index:
        gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
//        gl->glIndexPointer(y.vei.acount, y.type, y.stride, (const GLvoid *)y.start);
//        gl->glEnableClientState(GL_INDEX_ARRAY);
        break;
    }
  }
  return true;
}

bool RVertexArray::StrideConvert(eVertexAType t1, uint8 c1, void* p1, int s1, eVertexAType t2, uint8 c2, const void* p2, int s2, int num)
{
  const BByte* src = reinterpret_cast<const BByte*>(p2);
  BByte* dest = reinterpret_cast<BByte*>(p1);
  if(t1==t2) {
    BByte n = vat_sizes[t1]*c1;
    for(int i=0; i<num; i++) {
      memcpy(dest, src, n);
      src += s2; 
      dest += s1;
    }
    return true;
  }
  return false;
}

//***************************************

bool RVertexArray::is_valid(void) const
{
  return vbo!=gl_no_buffer;
}

int RVertexArray::get_array_count(void) const
{
  return count;
}

int RVertexArray::get_layer_count(void) const
{
  return ~layers;
}

SVertexElemInfo RVertexArray::get_layer_info(int layid) const
{
  if(layid<0 || layid>=~layers)
    return SVertexElemInfo();
  return layers[layid].vei;
}

//***************************************

bool RVertexArray::get_layer_vdata(eVertexAType type, void* buf, int start, int ecount, int lay) const
{
  return false;
}

bool RVertexArray::set_layer_vdata(eVertexAType type, const void* buf, int start, int ecount, int lay)
{
  if(type==vaet_None || type>=vaet__Count)
    return false;
  if(lay<0 || lay>=~layers || buf==NULL)
    return false;
  if(start<0 || ecount<=0 || start+ecount>count)
    return false;
  if(!Alloc())
    return false;
  //
  const SLayInfo& y = layers[lay];
  return StrideConvert(
    y.vei.aitype, y.vei.acount, cache+(y.stride*start+y.start), y.stride, 
    type, y.vei.acount, buf, vat_sizes[type],
    ecount
  );
}

//***************************************
// RVertexArray::SArrayFill::
//***************************************

void RVertexArray::SArrayFill::clear(void)
{
  coord = color = normal = edge = index = false;
  for(int i=0; i<gl_multitex_levels; i++) {
    texcoords[i] = false;
  }
}

bool RVertexArray::SArrayFill::check(SVertexElemInfo& x)
{
    switch(x.vclass) {
      case vc_Coord:
        if(x.aitype==vaet_None || x.aitype>=vaet__Count) {
          x.aitype = vaet_Float32;
        } else if(x.aitype==vaet_Int8 || x.aitype==vaet_UInt8) {
          x.aitype = vaet_Int16;
        } else if(x.aitype==vaet_UInt16) {
          x.aitype = vaet_Int32;
        } else if(x.aitype==vaet_UInt32) {
          x.aitype = vaet_Float64;
        }
        if(x.acount<2 || x.acount>4)
          return false;
        break;
      case vc_Normal:
        if(x.aitype==vaet_None || x.aitype>=vaet__Count) {
          x.aitype = vaet_Float32;
        } else if(vaet_UInt8) {
          x.aitype = vaet_Int16;
        } else if(x.aitype==vaet_UInt16) {
          x.aitype = vaet_Int32;
        } else if(x.aitype==vaet_UInt32) {
          x.aitype = vaet_Float64;
        }
        if(x.acount!=3)
          return false;
        break;
      case vc_Color:
        if(x.aitype==vaet_None || x.aitype>=vaet__Count) {
          x.aitype = vaet_Float32;
        }
        if(x.acount<3 || x.acount>4)
          return false;
        break;
      case vc_TexCoord:
        if(x.aitype==vaet_None || x.aitype>=vaet__Count) {
          x.aitype = vaet_Float32;
        } else if(x.aitype==vaet_Int8 || x.aitype==vaet_UInt8) {
          x.aitype = vaet_Int16;
        } else if(x.aitype==vaet_UInt16) {
          x.aitype = vaet_Int32;
        } else if(x.aitype==vaet_UInt32) {
          x.aitype = vaet_Float64;
        }
        if(x.acount<1 || x.acount>4)
          return false;
        break;
      case vc_Edge:
        x.aitype = vaet_UInt8;
        if(x.acount!=1)
          return false;
        break;
      case vc_Index:
        if(x.aitype==vaet_None || x.aitype>=vaet__Count || x.aitype==vaet_Float32 || x.aitype==vaet_Float64 || x.aitype==vaet_Int32) {
          x.aitype = vaet_UInt32;
        } else if(x.aitype==vaet_Int8) {
          x.aitype = vaet_UInt8;
        } else if(x.aitype==vaet_Int16) {
          x.aitype = vaet_UInt16;
        }
        if(x.acount!=1)
          return false;
        break;
      default:
        return false;
    }
    return true;
}

bool RVertexArray::SArrayFill::check_single(const SVertexElemInfo& x)
{
    switch(x.vclass) {
      case vc_Coord:
        if(coord || x.inf!=0)
          return false;
        coord = true;
        break;
      case vc_Normal:
        if(normal || x.inf!=0)
          return false;
        normal = true;
        break;
      case vc_Color:
        if(color || x.inf!=0)
          return false;
        color = true;
        break;
      case vc_TexCoord:
        if(x.inf>=gl_multitex_levels || texcoords[x.inf])
          return false;
        texcoords[x.inf] = true;
        break;
      case vc_Edge:
        if(edge || x.inf!=0)
          return false;
        edge = true;
        break;
      case vc_Index:
        if(index || x.inf!=0)
          return false;
        index = true;
        break;
      default:
        return false;
    }
    return true;
}

//***************************************
// END
//***************************************
