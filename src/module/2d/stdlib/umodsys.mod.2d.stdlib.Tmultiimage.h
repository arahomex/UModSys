//------------------------------------
// RMultiImage::
//------------------------------------

DPoint RMultiImage::get_max_layer_size(void) const
{
  if(!factory.valid())
    return DPoint(0, 0);
  return factory->image_get_maxsize();
}

DPoint RMultiImage::get_max_cell_size(void) const
{
  if(!factory.valid())
    return DPoint(0, 0);
  return factory->image_get_maxsize();
}

DPoint RMultiImage::get_fixed_cell_size(void) const
{
  if(!factory.valid())
    return DPoint(0, 0);
  return fixed_size;
}

uint16 RMultiImage::get_layer_count(void) const
{
  if(!factory.valid())
    return 0;
  return ~layers;
}

IImage* RMultiImage::get_layer(Buint16 idx) const
{
  if(!factory.valid())
    return NULL;
  return idx<~layers ? layers(idx)() : NULL;
}

uint32 RMultiImage::get_cell_count(void) const
{
  if(!factory.valid())
    return 0;
  return ~cells;
}

bool RMultiImage::get_cell(uint32 idx, DPoint &size, DPoint* ofs, uint16* lay) const
{
  if(factory.valid())
    return false;
  if(idx>=~cells)
    return false;
  size = cells(idx).size;
  if(ofs!=NULL)
    *ofs = cells(idx).start;
  if(lay!=NULL)
    *lay = cells(idx).layer;
  return true;
}

bool RMultiImage::get_cell(uint32 idx, SImageCellInfo& info) const
{
  if(factory.valid())
    return false;
  info = cells(idx);
  return true;
}

//------------------------------------

bool RMultiImage::set_layer_count(uint16 num)
{
  if(!factory.valid())
    return false;
  size_t old = ~layers;
  if(!layers.Resize(num))
    return false;
  for(size_t i=old; i<num; i++) {
    layers[i] = factory->image_new();
    if(layers[i].invalid()) {
      layers.Resize(old);
      return false;
    }
  }
  return true;
}

bool RMultiImage::set_hint(BCStr hint, BCStr value)
{
  if(!factory.valid())
    return false;
  return false; // !!FIX
}

//------------------------------------

bool RMultiImage::setup_fixed_cell(int nx, int ny)
{
  if(!factory.valid())
    return false;
  is_fixed = true;
  fixed_size.set(nx, ny);
  return true;
}

bool RMultiImage::setup_variable_cell(const SImageCellInfo* xcells, Buint32 num, Buint32 base)
{
  if(!factory.valid())
    return false;
  is_fixed = false;
  fixed_size.set(0, 0);
  if(base+num>~cells) {
    if(!cells.Resize(base + num))
      return false;
  }
  if(xcells!=NULL) {
    for(size_t i=0; i<num; i++) {
      cells[base+i] = xcells[i];
    }
  }
  return true;
}

//------------------------------------
//------------------------------------

RMultiImage::RMultiImage(DOwner *pv, const SParameters& args)
: refs(pv)
{
  args.ref_get("factory", factory);
}

RMultiImage::~RMultiImage(void)
{
}

//------------------------------------
// 
//------------------------------------
