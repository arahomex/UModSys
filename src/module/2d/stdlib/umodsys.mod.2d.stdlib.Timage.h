//------------------------------------
// RImage_Memory::
//------------------------------------

bool RImage_Memory::set_info(const DInfo& inf, BCStr hint) 
{
  if(inf.size(0)<0 || inf.size(0)<0)
    return false;
  size_t binsize = 0, ps = GetPixelSize(inf.type), xs, es;
  State s;
  s.info = inf;
  switch(inf.type & it_Mask_Class) {
    case it_Class_Linear:
      es = ps * size_t(inf.size(0)) * size_t(inf.size(1));
      binsize += es;
      s.nplanes++;
      s.offsets[s.nplanes] = binsize;
      s.infos[s.nplanes] = inf;
      break;
    case it_Class_Palette:
      es = ps * size_t(inf.size(0)) * size_t(inf.size(1));
      binsize += es;
      s.nplanes++;
      s.offsets[s.nplanes] = binsize;
      s.infos[s.nplanes].set(eImageType(inf.type & it_Mask_Plane1), inf.size);
      //
      xs = size_t(1<<(UMODSYS_ARCH_BYTE_BITS * int(ps)));
      es = GetPixelSize(inf.type, 1) * size_t(xs);
      binsize += es;
      s.nplanes++;
      s.offsets[s.nplanes] = binsize;
      s.infos[s.nplanes].set( eImageType( (inf.type >> it_Sh_Plane2) & it_Mask_Plane1), DPoint(xs, 1));
      break;
    default:
      return false;
  }
  if(!s.bin.alloc(binsize, UMODSYS_SOURCEINFO))
    return false;
  //
  state = s;
  return true;
}

bool RImage_Memory::set_data(const DPatchInfo& cinf, const SCMem& cbin, int planeno) 
{
  if(planeno==0)
    return Convert(cinf.box(), state.info, state.bin, cinf.zerobox(), cinf, cbin);
  return false;
}
//
const SImageInfo& RImage_Memory::get_info(void) const 
{
  return state.info;
}

bool RImage_Memory::get_data(const DPatchInfo& winf, const SMem& wbin, int planeno) const 
{
  if(planeno==0)
    return Convert(winf.zerobox(), winf, wbin, winf.box(), state.info, state.bin);
  return false;
}

void* RImage_Memory::get_hwinfo(HUniquePointer hint) 
{
  return NULL;
}

const void* RImage_Memory::get_hwcinfo(HUniquePointer hint) const 
{
  return NULL;
}

//------------------------------------
//------------------------------------

RImage_Memory::RImage_Memory(DOwner *pv, const SParameters& args)
: refs(pv)
{
}


RImage_Memory::~RImage_Memory(void)
{
}

//------------------------------------
// 
//------------------------------------
