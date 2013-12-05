//------------------------------------
// RImage_Memory
//------------------------------------

struct RImage_Memory : public IImage {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RImage_Memory, 2, IImage)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  struct State {
    SMemShared bin;
    int nplanes;
    SImageInfo info;
    SImageInfo infos[4];
    size_t offsets[5];
    //
    inline State(void)
      : nplanes(0) 
    {
      offsets[0] = 0;
    }
  };
public:
  bool set_info(const DInfo& inf, BCStr hint) 
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
        xs = (1<<(UMODSYS_ARCH_BYTE_BITS * ps));
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
  bool set_data(const DPatchInfo& cinf, const SCMem& cbin, int planeno) 
  {
    if(planeno==0)
      return Convert(cinf.box(), state.info, state.bin, cinf.zerobox(), cinf, cbin);
    return false;
  }
  //
  const SImageInfo& get_info(void) const 
  {
    return state.info;
  }
  bool get_data(const DPatchInfo& winf, const SMem& wbin, int planeno) const 
  {
    if(planeno==0)
      return Convert(winf.zerobox(), winf, wbin, winf.box(), state.info, state.bin);
    return false;
  }
  //
  void* get_hwinfo(HUniquePointer hint) 
  {
    return NULL;
  }
  const void* get_hwcinfo(HUniquePointer hint) const 
  {
    return NULL;
  }
public:
  State state;
public:
  RImage_Memory(DOwner *pv, const SParameters& args)
  : refs(pv)
  {
  }
  bool validate_construction(void)
  {
    return true;
  }
};

//------------------------------------
// 
//------------------------------------
