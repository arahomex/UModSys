//************************************************************
// RSDLVideoTextureCellsGL::
//************************************************************

lib2d::IImage* RSDLVideoTextureCellsGL::get_layer(int idx)
{
  return idx<0 || idx>=~layers ? NULL : layers(idx);
}

bool RSDLVideoTextureCellsGL::set_layer_count(int c2)
{
  if(!layers.resize(c2))
    return false;
  for(int i=0; i<~layers; i++) {
    if(layers(i)==NULL) {
      layers[i] = new RSDLVideoTextureGL(parent);
      if(layers(i)==NULL)
        return false;
    }
  }
  fixed = false; 
  base = 0; count = 0;
  return true;
}

bool RSDLVideoTextureCellsGL::set_fixed_cell(int nx, int ny)
{
  // build from already loaded texture, nx & ny is cell size
  if(nx<=0 || ny<=0 || ~layers!=1 || layers(0)==NULL)
    return false; // incompatible one
  SSDLVideoTextureGL& tex = layers(0)->tex;
  if(tex.rwidth%nx || tex.rheight%ny)
    return false; // incompatible one
  //
  fixed = true;
  width = nx; height = ny;
  int cx = nx, cy = ny;
  nx = tex.rwidth/nx;
  ny = tex.rheight/ny;
  base = 0;
  count = cd.count = nx*ny;
  cd.xcount = nx; cd.ycount = ny;
  cd.cx = cx/double(tex.width);
  cd.cy = cy/double(tex.height);
  return true;
}

bool RSDLVideoTextureCellsGL::set_var_cell(const lib2d::SImageCellInfo* cells, int count, int base)
{
  fixed = false;
  if(count==0 || cells==NULL) {
    count = 0;
    return true;
  }
  this->count = 0;
  int i, nbox=0, nempty=0, k;
  for(i=0; i<count; i++) {
    const lib2d::SImageCellInfo &c = cells[i];
    switch(c.fill) {
      case lib2d::if_Null:
        continue;
      case lib2d::if_Ref:
        if(c.layer<0 || c.layer>=count || c.layer==i)
          return false; // bad ref
        continue;
      case lib2d::if_Empty:
        nempty++;
        continue;
      case lib2d::if_Rect:
        k = cells[i].layer;
        if(k<0 || k>=~layers || layers(k)==NULL)
          return false; // bad layer id
        nbox++;
        continue;
      default:
        return false; // bad fill
    }
  }
  if(!cboxes.resize(nbox) || !cemptys.resize(nempty) || !cindices.resize(count))
    return false; // alloc error
  nbox=0; nempty=0;
  maxsize.set(0);
  spacing.set(0);
  maxsizetex.set(0);
  for(i=0; i<count; i++) {
    const lib2d::SImageCellInfo &c = cells[i];
    switch(c.fill) {
      case lib2d::if_Null:
        cindices[i] = lib2d::if_Null<<24;
        continue;
      case lib2d::if_Ref:
        cindices[i] = (lib2d::if_Ref<<24) | c.layer;
        continue;
      case lib2d::if_Empty:
        cindices[i] = (lib2d::if_Empty<<24) | nempty;
        cemptys[nempty++].size = c.size;
        if(c.size(0)>maxsize(0)) maxsize[0] = c.size(0);
        if(c.size(1)>maxsize(1)) maxsize[1] = c.size(1);
        continue;
      case lib2d::if_Rect: {
        cindices[i] = (lib2d::if_Rect<<24) | nbox;
        CellBox& box = cboxes[nbox++];
        box.layer = c.layer;
        SSDLVideoTextureGL& tex = layers(c.layer)->tex;
        box.size = c.size;
        box.start = c.start;
        box.tstart.set(tex.itwidth*box.start(0), tex.itheight*box.start(1));
        box.tsize.set(tex.itwidth*box.size(0), tex.itheight*box.size(1));
        if(c.size(0)>maxsize(0)) maxsize[0] = c.size(0);
        if(c.size(1)>maxsize(1)) maxsize[1] = c.size(1);
      } break;
    }
  }
  this->count = count;
  return true;
}

bool RSDLVideoTextureCellsGL::set_hint(BCStr hint, BCStr value)
{
  if(su::seq(hint, "CHARMAP")) {
    if(value==NULL || *value==0 || su::seq(value, "ASCII")) {
      return true;
    } else if(su::seq(value, "UTF8")) {
      return true;
    } else if(su::seq(value, "UTF16")) {
      return true;
    }
  } else if(su::seq(hint, "FONTNAME")) {
    return true;
  }
  return false;
}

IRenderDriver* RSDLVideoTextureCellsGL::get_driver(void)
{
  return parent;
}

bool RSDLVideoTextureCellsGL::get_max_cell_size(lib2d::DPoint &size)
{
  size.set(maxsize(0), maxsize(1));
  return true;
}

bool RSDLVideoTextureCellsGL::get_cell_size(int idx, lib2d::DPoint &size, lib2d::DPoint* ofs)
{
  DTexPoint as, ao;
  if(!get_raw_cell(idx, &as, ofs ? &ao : NULL))
    return false;
  size.set(as(0), as(1));
  if(ofs)
    ofs->set(ao(0), ao(1));
  return true;
}

bool RSDLVideoTextureCellsGL::get_cell_info(int ch, SPicture& pic, DTexPoint* start)
{
  static DTexPoint zero(0,0);
  if(start==NULL)
    start = &zero;
  //
  if(fixed) {
    if(ch<0 || ch>=this->count || ~layers==0)
      return false;
    int ix = ch%cd.xcount, iy = ch/cd.xcount;
    pic.tex = layers(0);
    pic.p1.set(start->v[0], start->v[1]);
    pic.p2 = pic.p1 + maxsize;
    pic.t1.set(ix*cd.cx, iy*cd.cy);
    pic.t2 = pic.t1 + DTexPoint(cd.cx, cd.cy);
    return true;
  } else {
    while(1) {
      if(ch<0 || ch>=this->count)
        return false;
      int idx = cindices[ch];
      switch(idx>>24) {
        case lib2d::if_Null:
        case lib2d::if_Empty:
          break;
        case lib2d::if_Ref:
          ch = idx & 0xffffff;
          continue;
        case lib2d::if_Rect: {
          const CellBox& box = cboxes[idx & 0xffffff];
          if(box.layer<0 || box.layer>~layers || layers(box.layer)==NULL)
            return false;
          pic.tex = layers(box.layer);
          pic.p1.set(start->v[0], start->v[1]);
          pic.p2 = pic.p1 + DTexPoint(box.size(0), box.size(1));
          pic.t1 = box.tstart;
          pic.t2 = box.tstart + box.tsize;
          return true;
        }
      }
      break;
    }
  }
  return false;
}

//************************************************************
//************************************************************

/*
inline bool RSDLVideoTextureCellsGL::x_draw_init(DrawState& ds, int x, int y)
{
  ds.x = x; ds.y = y;
  ds.scale[0] = parent->kfont(0);
  ds.scale[1] = parent->kfont(1);
  if(fixed) {
    if(~layers==0 || layers(0)==NULL)
      return false;
    ds.tex = &layers(0)->tex;
    ds.dsize[0] = width*ds.scale[0];
    ds.dsize[1] = height*ds.scale[1];
    ds.dspace[0] = (width+spacing(0))*ds.scale[0];
  } else {
  }
  return true;
}
*/

inline SSDLVideoTextureGL* RSDLVideoTextureCellsGL::get_tex(int ch) const
{
  if(fixed) {
    return ~layers ? &layers(0)->tex : NULL;
  }
  ch -= base;
  while(1) {
    if(ch<0 || ch>=count)
      return NULL;
    int idx = cindices[ch];
    switch(idx>>24) {
      case lib2d::if_Null:
      case lib2d::if_Empty: 
        return NULL;
      case lib2d::if_Ref:
        ch = idx & 0xffffff;
        continue;
      case lib2d::if_Rect: {
        const CellBox& box = cboxes[idx & 0xffffff];
        if(box.layer<0 || box.layer>~layers || layers(box.layer)==NULL)
          return NULL;
        return &layers(box.layer)->tex;
      };
    }
    break;
  }
  return NULL;
}

inline lib2d::eSubImageMode RSDLVideoTextureCellsGL::get_raw_cell(int ch, DTexPoint* size, DTexPoint *ofs) const
{
  if(ofs)
    ofs->set(0,0);
  if(fixed) {
    if(ch<0 || ch>=this->count)
      return lib2d::sim_Null;
    int ix = ch%cd.xcount, iy = ch/cd.xcount;
    if(size)
      *size = maxsize;
    return lib2d::sim_Rect;
  } else {
    while(1) {
      if(ch<0 || ch>=this->count)
        return lib2d::sim_Null;
      int idx = cindices[ch];
      switch(idx>>24) {
        case lib2d::if_Null:
          return lib2d::sim_Null;
        case lib2d::if_Empty: {
          const CellEmpty& e = cemptys[idx & 0xffffff];
          if(size)
            size->set(e.size(0), e.size(1));
          return lib2d::sim_Empty;
        } break;
        case lib2d::if_Ref:
          ch = idx & 0xffffff;
          continue;
        case lib2d::if_Rect: {
          const CellBox& box = cboxes[idx & 0xffffff];
          if(size)
            size->set(box.size(0), box.size(1));
          return lib2d::sim_Rect;
        }
      }
      break;
    }
  }
  return lib2d::sim_Null;
}

/*
inline bool RSDLVideoTextureCellsGL::x_draw_elem(DrawState& ds, int ch) 
{
  if(fixed) {
    if(ch<0 || ch>=this->count)
      return false;
    int ix = ch%cd.xcount, iy = ch/cd.xcount;
    if(!parent->x_drawimage(
      ds.tex, 
      ds.x, ds.y, ds.dsize[0], ds.dsize[1], 
      ix*cd.cx, iy*cd.cy, cd.cx, cd.cy
    )) return false;
    ds.x += ds.dspace[0];
  } else {
    while(1) {
      if(ch<0 || ch>=this->count)
        return false;
      int idx = cindices[ch];
      switch(idx>>24) {
        case lib2d::if_Null:
          break;
        case lib2d::if_Empty: {
          const CellEmpty& e = cemptys[idx & 0xffffff];
          ds.x += (e.size(0)+spacing(0))*ds.scale[0];
        } break;
        case lib2d::if_Ref:
          ch = idx & 0xffffff;
          continue;
        case lib2d::if_Rect: {
          const CellBox& box = cboxes[idx & 0xffffff];
          if(box.layer<0 || box.layer>~layers || layers(box.layer)==NULL)
            return false;
          SSDLVideoTextureGL& tex = layers(box.layer)->tex;
          if(!parent->x_drawimage(
            &tex, 
            ds.x, ds.y, box.size(0)*ds.scale[0], box.size(1)*ds.scale[1], 
            box.tstart(0), box.tstart(1), box.tsize(0), box.tsize(1)
          )) return false;
          ds.x += (box.size(0)+spacing(0))*ds.scale[0];
        } break;
      }
      break;
    }
  }
  return true;
}

bool RSDLVideoTextureCellsGL::x_draw1(int x, int y, int ch)
{
  DrawState ds;
  return x_draw_init(ds, x, y) && x_draw_elem(ds, ch);
}

*/

//************************************************************
//************************************************************

UMS_REFOBJECT_RIMPLEMENT_BODY(RSDLVideoTextureCellsGL)

RSDLVideoTextureCellsGL::RSDLVideoTextureCellsGL(RSDLVideoSurfaceGL* pv)
: spacing(1,1) {
  rc_init(pv);
  fixed = false;
  count = 0;
}

RSDLVideoTextureCellsGL::~RSDLVideoTextureCellsGL(void)
{
}

//************************************************************
// ::
//************************************************************
