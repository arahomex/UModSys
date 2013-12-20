//------------------------------------
// RImage_Memory::
//------------------------------------

DPoint RImageFactory_Memory::image_get_maxsize(void) const
{
  return DPoint(0, 0);
}

IImage::P RImageFactory_Memory::image_new(void)
{
  IImage::P rv;
  TParametersA<267> args;
  if(!core::ValidateConstruction(rv, new(local_memory()) RImage_Memory(refs.owner, args)))
    return NULL;
  return rv;
}

IImage::P RImageFactory_Memory::image_new(const DInfo& inf, BCStr hint)
{
  IImage::P rv;
  TParametersA<267> args;
  if(!core::ValidateConstruction(rv, new(local_memory()) RImage_Memory(refs.owner, args)))
    return NULL;
  if(!rv->set_info(inf, hint))
    return NULL;
  return rv;
}

//------------------------------------
//------------------------------------

RImageFactory_Memory::RImageFactory_Memory(DOwner *pv, const SParameters& args)
: refs(pv)
{
}


RImageFactory_Memory::~RImageFactory_Memory(void)
{
}

//------------------------------------
// 
//------------------------------------
