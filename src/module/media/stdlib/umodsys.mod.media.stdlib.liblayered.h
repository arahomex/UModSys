
//***************************************
// RLibraryLayered::
//***************************************

struct RLibraryLayered : public ILibraryLayered
{
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RLibraryLayered, 1, ILibraryLayered)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  struct SLayerX : public SLayer {
    DStringShared mtag;
    //
    inline SLayerX(void) : SLayer(NULL) {}
    inline void operator=(const SLayer& r) {
      mtag = r.tag;
      tag = mtag();
      lib = r.lib;
      pm = r.pm;
    }
  };
  typedef tl::TDynarrayDynamic< SLayerX  > DLayers;
  //
  DLayers layers;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
  //
  RLibraryLayered(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags())
  : refs(own), flags(av)/*, cache(local_memory())*/ {
  }
  ~RLibraryLayered(void) {
  }
  //
  // SFlags::ISetter
  DMediaFlags::eStates get_flag(int shift) const { return flags.get_s(shift); }
  DMediaFlags get_flags_auto(void) const { return auto_flags; }
  DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag) { return flags.getset_s(shift, flag); }
  //
  // general data functions
  IStreamReader::P bin_reader(const DCString& media_name, const SFlags& flags) {
    SFlagsChain f2(flags, this);
    for(DLayers::const_iterator x=layers.begin(), e=layers.end(); x!=e; ++x) {
      const SLayerX& lx = *x;
      if(!(lx.pm & mp_Read) || lx.lib.invalid())
        continue;
      IStreamReader::P rv = lx.lib->bin_reader(media_name, f2);
      if(rv.valid())
        return rv;
    }
    return NULL;
  }
  IStreamWriter::P bin_writer(const DCString& media_name, const SFlags& flags) {
    SFlagsChain f2(flags, this);
    for(DLayers::const_iterator x=layers.begin(), e=layers.end(); x!=e; ++x) {
      const SLayerX& lx = *x;
      if(!(lx.pm & mp_Write) || lx.lib.invalid())
        continue;
      IStreamWriter::P rv = lx.lib->bin_writer(media_name, f2);
      if(rv.valid())
        return rv;
    }
    return NULL;
  }
  bool bin_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags) {
    mem.clear();
    if(flags.yes<mf_cache>(this) && bin_get(media_name, mem, flags.yes<mf_failuse>(this)))
      return true;
    SFlagsChain f2(flags, this);
    for(DLayers::const_iterator x=layers.begin(), e=layers.end(); x!=e; ++x) {
      const SLayerX& lx = *x;
      if(!(lx.pm & mp_Read) || lx.lib.invalid())
        continue;
      if(lx.lib->bin_load(media_name, mem, f2)) {
        if(flags.yes<mf_cachesave>(this)) {
          bin_put(media_name, &mem);
        }
        return true;
      }
    }
    if(flags.yes<mf_failwrite>(this)) {
      bin_put(media_name, NULL);
    }
    return false;
  }
  bool bin_save(const DCString& media_name, const SCMem& mem, const SFlags& flags) {
    SFlagsChain f2(flags, this);
    for(DLayers::const_iterator x=layers.begin(), e=layers.end(); x!=e; ++x) {
      const SLayerX& lx = *x;
      if(!(lx.pm & mp_Write) || lx.lib.invalid())
        continue;
      if(lx.lib->bin_save(media_name, mem, f2)) {
        if(flags.yes<mf_cachesave>(this)) {
          SCMemShared ms = mem;
          bin_put(media_name, &ms);
        }
        return true;
      }
    }
    if(flags.yes<mf_failwrite>(this)) {
      bin_put(media_name, NULL);
    }
    return false;
  }
  bool bin_info(const DCString& media_name, SFileInfo& info, const SFlags& flags) {
    SFlagsChain f2(flags, this);
    for(DLayers::const_iterator x=layers.begin(), e=layers.end(); x!=e; ++x) {
      const SLayerX& lx = *x;
      if(!(lx.pm & mp_List) || lx.lib.invalid())
        continue;
      if(lx.lib->bin_info(media_name, info, f2))
        return true;
    }
    return false;
  }
  bool bin_info(const DCString& media_mask, DIFileInfoArray& info, const SFlags& flags) {
    SFlagsChain f2(flags, this);
    bool rv = false;
    for(DLayers::const_iterator x=layers.begin(), e=layers.end(); x!=e; ++x) {
      const SLayerX& lx = *x;
      if(!(lx.pm & mp_List) || lx.lib.invalid())
        continue;
      rv = lx.lib->bin_info(media_mask, info, f2) || rv;
    }
    return rv;
  }
  bool bin_get(const DCString& media_name, SCMemShared& mem, bool isinv) {
    for(DLayers::const_iterator x=layers.begin(), e=layers.end(); x!=e; ++x) {
      const SLayerX& lx = *x;
      if(!(lx.pm & mp_Read) || lx.lib.invalid())
        continue;
      if(lx.lib->bin_get(media_name, mem, isinv))
        return true;
    }
    return false;
  }
  bool bin_put(const DCString& media_name, const SCMemShared* mem) {
    for(DLayers::const_iterator x=layers.begin(), e=layers.end(); x!=e; ++x) {
      const SLayerX& lx = *x;
      if(!(lx.pm & mp_Write) || lx.lib.invalid())
        continue;
      if(lx.lib->bin_put(media_name, mem))
        return true;
    }
    return false;
  }
  // general object functions
  bool obj_fget(const IBinObjFilter::SInfo& info, IRefObject::P& obj) {
//    SFlagsChain f2(flags, this);
    for(DLayers::const_iterator x=layers.begin(), e=layers.end(); x!=e; ++x) {
      const SLayerX& lx = *x;
      if(!(lx.pm & mp_Read) || lx.lib.invalid())
        continue;
      if(lx.lib->obj_fget(info, obj))
        return true;
    }
    return false;
  }
  bool obj_fload(const IBinObjFilter::SInfo& info, IRefObject* obj) {
//    SFlagsChain f2(flags, this);
    for(DLayers::const_iterator x=layers.begin(), e=layers.end(); x!=e; ++x) {
      const SLayerX& lx = *x;
      if(!(lx.pm & mp_Read) || lx.lib.invalid())
        continue;
      if(lx.lib->obj_fload(info, obj))
        return true;
    }
    return false;
  }
  bool obj_fsave(IBinObjFilter::SInfo& info, const IRefObject* obj) {
//    SFlagsChain f2(flags, this);
    for(DLayers::const_iterator x=layers.begin(), e=layers.end(); x!=e; ++x) {
      const SLayerX& lx = *x;
      if(!(lx.pm & mp_Write) || lx.lib.invalid())
        continue;
      if(lx.lib->obj_fsave(info, obj))
        return true;
    }
    return false;
  }
  bool obj_cget(const DCString& media_name, IRefObject::P& obj, bool inv) {
    for(DLayers::const_iterator x=layers.begin(), e=layers.end(); x!=e; ++x) {
      const SLayerX& lx = *x;
      if(!(lx.pm & mp_Read) || lx.lib.invalid())
        continue;
      if(lx.lib->obj_cget(media_name, obj, inv))
        return true;
    }
    return false;
  }
  bool obj_cput(const DCString& media_name, IRefObject* obj) {
    for(DLayers::const_iterator x=layers.begin(), e=layers.end(); x!=e; ++x) {
      const SLayerX& lx = *x;
      if(!(lx.pm & mp_Write) || lx.lib.invalid())
        continue;
      if(lx.lib->obj_cput(media_name, obj))
        return true;
    }
    return false;
  }
  // universal object functions
  bool obj_get(const DCString& media_name, IRefObject::P& obj, const SObjOptions& opts) {
    SFlagsChain f2(opts, this);
    if(f2.yes<mf_objects>(this) && obj_cget(media_name, obj, f2.yes<mf_nulluse>(this)))
      return true;
    IBinObjFilter::SInfo info;
    if(!bin_load(media_name, info.binary, flags)) {
      if(f2.yes<mf_nullwrite>(this)) {
        obj_cput(media_name, NULL);
      }
      return false;
    }
    info.flags = f2;
    info.common_type = opts.typehint;
    info.media_name = media_name;
    info.params = opts.params;
    info.reqtype = opts.reqtype;
    if(!obj_fget(info, obj)) {
      if(f2.yes<mf_nullwrite>(this)) {
        obj_cput(media_name, NULL);
      }
      return false;
    }
    if(f2.yes<mf_objsave>(this)) {
      obj_cput(media_name, obj);
    }
    return true;
  }
  bool obj_load(const DCString& media_name, IRefObject* obj, const SObjOptions& opts) {
    SFlagsChain f2(opts, this);
    IBinObjFilter::SInfo info;
    if(!bin_load(media_name, info.binary, flags))
      return false;
    info.flags = f2;
    info.common_type = opts.typehint;
    info.media_name = media_name;
    info.params = opts.params;
    info.reqtype = opts.reqtype;
    if(!obj_fload(info, obj))
      return false;
    return true;
  }
  bool obj_save(const DCString& media_name, const IRefObject* obj, const SObjOptions& opts) {
    SFlagsChain f2(opts, this);
    IBinObjFilter::SInfo info;
    info.flags = f2;
    info.common_type = opts.typehint;
    info.media_name = media_name;
    info.params = opts.params;
    info.reqtype = opts.reqtype;
    if(!obj_fsave(info, obj))
      return false;
    if(!bin_save(media_name, info.binary, flags))
      return false;
    if(f2.yes<mf_objsave>(this)) {
      obj_cput(media_name, const_cast<IRefObject*>(obj));
    }
    return true;
  }
  //
  // sub-elements, use highest cast [IDataLibrary]
  size_t layer_count(const SLayer& lay) const {
    return ~layers;
  }
  SLayer layer_get(size_t idx) const {
    return layers(idx);
  }
  size_t layer_find(const DCString &tag, size_t start) const {
    if(start==array_index_none) start = 0; else start++;
    for(; start<~layers; ++start) {
      const SLayerX& lx = layers(start);
      if(tag==lx.tag)
        return start;
    }
    return array_index_none;
  }
  bool layer_insert(const SLayer& lay, size_t idx) {
    if(idx==array_index_none)
      idx = ~layers;
    if(!layers.InsertAt(idx))
      return false;
    SLayerX& lx = layers[idx];
    lx = lay;
    return true;
  }
  bool layer_remove(size_t idx) {
    return layers.RemoveAt(idx);
  }
  bool layer_clear(void) {
    return layers.Clear();
  }
  //
  //
  // utility
  inline bool validate_construction(void) const { return true; }
};

//***************************************
// END
//***************************************
