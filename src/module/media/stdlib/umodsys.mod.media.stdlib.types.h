
//***************************************
// RUtilities::
//***************************************

struct RUtilities : public IUtilities
{
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RUtilities, 1, IUtilities)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  struct Buffer {
    tl::TDynarrayDynamic<BByte> buf;
    bool locked;
    //
    inline Buffer(void) : locked(false) {}
    inline ~Buffer(void) {}
  };
  typedef tl::TDynarrayDynamic< Buffer > Buffers;
  //
  Buffers buffers;
  //
  RUtilities(DOwner *own, const SParameters& args);
  ~RUtilities(void);
  //
  // IUtilities
  // -- buffers
  bool buffer_free(SBuffer& B, const SSourceContext* sctx);
  bool buffer_alloc(SBuffer& B, size_t size, const SSourceContext* sctx);
  // -- sub streams
  IStreamReader::P sub_reader(IStreamReader* master, DFilePosition pos, DFilePosition size); 
  IStreamWriter::P sub_writer(IStreamWriter* master, DFilePosition pos, DFilePosition size); 
  // -- memory streams
  IStreamReader::P mem_reader(const SCMem& mem);
  IStreamReader::P mem_reader(const SCMemShared& mem);
  IStreamWriter::P mem_writer(const SMem* mem, IMemWriterCompleted* mw, IRefObject* mwh);
  //
  //
  inline bool validate_construction(void) const { return true; }
};

//***************************************
// RBinArchiveFrame::
//***************************************

struct RBinArchiveFrame : public IBinArchiveFrame
{
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RBinArchiveFrame, 1, IBinArchiveFrame)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  struct NN_Add;
  friend struct NN_Add;
  //
  struct NN_Add {
    int extra;
    SMem* memto;
    //
    inline NN_Add(int x, SMem* mt) 
      : extra(x), memto(mt) {}
    //
    inline NameNode* operator()(NameNodeCmpUC& cmp) const { return new_node(cmp.name, extra, memto); }
    inline NameNode* operator()(NameNodeCmpC& cmp) const { return new_node(cmp.name, extra, memto); }
  };
  //
  bool valid;
  SMemShared rdir;
  IClient::P client;
  NameNodes nodes;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
  //
  RBinArchiveFrame(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags());
  ~RBinArchiveFrame(void);
  //
  // SFlags::ISetter
  DMediaFlags::eStates get_flag(int shift) const;
  DMediaFlags get_flags_auto(void) const;
  DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag);
  //
  // IBinArchive 
  IStreamReader::P data_reader(const DCString& media_name, const SFlags& flags);
  IStreamWriter::P data_writer(const DCString& media_name, const SFlags& flags);
  bool data_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags);
  bool data_save(const DCString& media_name, const SCMem& mem, const SFlags& flags);
  bool data_list(const DCString& mask, size_t namestart, DIFileInfoArray& list, const SFlags& flags);
  bool data_info(const DCString& mask, size_t namestart, SFileInfo& list, const SFlags& flags);
  int get_permissions(void);
  //
  // IBinArchiveFrame
  bool open(IClient* client, const SParameters* params);
  bool close(void);
  //
  NameNode* add_node_r(const DCString& name, size_t extra, SMem& memto);
  NameNode* add_node_w(const DCString& name, size_t extra);
  bool remove_node(NameNode* node);
  NameNode* find_node(const DCString& name);
  size_t node_size(const DCString &name, int extra);
  //
  // utility
  bool open(const SParameters& args);
  //
  static bool kill_node(NameNode* node);
  bool fill_info(SFileInfo& fi, NameNode *node, const DCString& path, size_t namestart);
  //
  template<typename String> 
  static NameNode* new_node(const String& nodename, int extra, SMem* memto);
  //
  inline static size_t nodesize(int namelen, int extra) 
    { return UMODSYS_ARCH_SIZE_CORRECT( sizeof(NameNode) + extra + (namelen+1)*sizeof(BChar) ); }
  //
  bool openr(ILibrary* lib, const DCString& fname, const SParameters* params);
  bool closew(bool abort);
  //
  //
  inline bool validate_construction(void) const 
    { return valid; }
};


//***************************************
// RLibraryBinTree::
//***************************************

struct RLibraryBinTree : public ILibraryBinTree
{
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RLibraryBinTree, 1, ILibraryBinTree)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  typedef SId SMountIndex;
  //
  struct SMountInfo : SPoint {
    DStringShared mount_name;
    //
    void operator=(const SPoint& r) { SPoint::operator=(r); }
  };
  typedef tl::TScatterArray< SMountInfo, SMountIndex, SMountIndex > DMounts;
  //
  DMounts mounts;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
  //
  RLibraryBinTree(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags());
  ~RLibraryBinTree(void);
  //
  // SFlags::ISetter
  DMediaFlags::eStates get_flag(int shift) const;
  DMediaFlags get_flags_auto(void) const;
  DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag);
  //
  // general data functions
  IStreamReader::P bin_reader(const DCString& media_name, const SFlags& flags);
  IStreamWriter::P bin_writer(const DCString& media_name, const SFlags& flags);
  bool bin_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags);
  bool bin_save(const DCString& media_name, const SCMem& mem, const SFlags& flags);
  bool bin_info(const DCString& media_name, SFileInfo& info, const SFlags& flags);
  bool bin_list(const DCString& media_mask, DIFileInfoArray& info, const SFlags& flags);
  //
  // sub-archives
  SPoint mount_get(const SId& id) const;
  bool mount_add(const SId& id, const SPoint& mp);
  bool mount_remove(const SId& id);
  bool mount_clear(void);
  //
  //
  // utility
  inline bool validate_construction(void) const { return true; }
};

//***************************************
// RLibraryBinCache::
//***************************************

struct RLibraryBinCache : public ILibraryBinCache
{
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RLibraryBinCache, 1, ILibraryBinCache)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  struct SCacheElem {
    DStringShared name;
    SCMemShared bin;
    bool valid;
    //
    SCacheElem(void) : valid(false) {}
  };
  typedef tl::TScatterArray< SCacheElem, DCString, TObjectCompare<DCString> > DCache;
  //
  DCache cache;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
  //
  RLibraryBinCache(DOwner *own, const SParameters& args, DMediaFlags av);
  ~RLibraryBinCache(void);
  //
  // SFlags::ISetter
  DMediaFlags::eStates get_flag(int shift) const;
  DMediaFlags get_flags_auto(void) const;
  DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag);
  //
  // general data functions
  IStreamReader::P bin_reader(const DCString& media_name, const SFlags& flags);
  IStreamWriter::P bin_writer(const DCString& media_name, const SFlags& flags);
  bool bin_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags);
  bool bin_get(const DCString& media_name, SCMemShared& mem, bool isinv);
  bool bin_put(const DCString& media_name, const SCMemShared* mem);
  //
  // binary cache
  bool bincache_get(const DCString& media_name, SCMemShared* mem, bool *valid) const;
  bool bincache_add(const DCString& media_name);
  bool bincache_add(const DCString& media_name, const SCMemShared& mem);
  bool bincache_remove(const DCString& media_name);
  bool bincache_clear(void);
  bool bincache_add(const DCString& media_name, const SCMemShared& mem, bool valid);
  //
  //
  // utility
  inline bool validate_construction(void) const { return true; }
};

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
  RLibraryLayered(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags());
  ~RLibraryLayered(void);
  //
  // SFlags::ISetter
  DMediaFlags::eStates get_flag(int shift) const;
  DMediaFlags get_flags_auto(void) const;
  DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag);
  //
  // general data functions
  IStreamReader::P bin_reader(const DCString& media_name, const SFlags& flags);
  IStreamWriter::P bin_writer(const DCString& media_name, const SFlags& flags);
  bool bin_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags);
  bool bin_save(const DCString& media_name, const SCMem& mem, const SFlags& flags);
  bool bin_info(const DCString& media_name, SFileInfo& info, const SFlags& flags);
  bool bin_list(const DCString& media_mask, DIFileInfoArray& info, const SFlags& flags);
  bool bin_get(const DCString& media_name, SCMemShared& mem, bool isinv);
  bool bin_put(const DCString& media_name, const SCMemShared* mem);
  // general object functions
  bool obj_fget(const IBinObjFilter::SInfo& info, IRefObject::P& obj);
  bool obj_fload(const IBinObjFilter::SInfo& info, IRefObject* obj);
  bool obj_fsave(IBinObjFilter::SInfo& info, const IRefObject* obj);
  bool obj_cget(const DCString& media_name, IRefObject::P& obj, bool inv);
  bool obj_cput(const DCString& media_name, IRefObject* obj);
  // universal object functions
  bool obj_get(const DCString& media_name, IRefObject::P& obj, const SObjOptions& opts);
  bool obj_load(const DCString& media_name, IRefObject* obj, const SObjOptions& opts);
  bool obj_save(const DCString& media_name, const IRefObject* obj, const SObjOptions& opts);
  //
  // sub-elements, use highest cast [IDataLibrary]
  size_t layer_count(const SLayer& lay) const;
  SLayer layer_get(size_t idx) const;
  size_t layer_find(const DCString &tag, size_t start) const;
  bool layer_insert(const SLayer& lay, size_t idx);
  bool layer_remove(size_t idx);
  bool layer_clear(void);
  //
  // utility
  inline bool validate_construction(void) const { return true; }
};

//***************************************
// RLibraryObjCache::
//***************************************

struct RLibraryObjCache : public ILibraryObjCache
{
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RLibraryObjCache, 1, ILibraryObjCache)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  struct SCacheElem {
    DStringShared name;
    IRefObject::P obj;
    bool valid;
    //
    SCacheElem(void) : valid(false) {}
  };
  typedef tl::TScatterArray< SCacheElem, DCString, TObjectCompare<DCString> > DCache;
  //
  DCache cache;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
  //
  RLibraryObjCache(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags());
  ~RLibraryObjCache(void);
  //
  // SFlags::ISetter
  DMediaFlags::eStates get_flag(int shift) const;
  DMediaFlags get_flags_auto(void) const;
  DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag);
  //
  // general object functions
  bool obj_cget(const DCString& media_name, IRefObject::P& obj, bool isinv);
  bool obj_cput(const DCString& media_name, IRefObject* obj);
  // universal object functions
  bool obj_get(const DCString& media_name, IRefObject::P& obj, const SObjOptions& opts);
  //
  // object cache 
  bool objcache_get(const DCString& media_name, IRefObject::P* obj, bool *valid) const;
  bool objcache_add(const DCString& media_name, IRefObject* obj);
  bool objcache_remove(const DCString& media_name);
  bool objcache_clear(void);
  //
  // utility
  inline bool validate_construction(void) const { return true; }
};

//***************************************
// RLibraryObjFilter::
//***************************************

struct RLibraryObjFilter : public ILibraryObjFilter
{
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RLibraryObjFilter, 1, ILibraryObjFilter)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  struct SId {
    int order;
    IBinObjFilter* filter;
    //
    SId(int o, IBinObjFilter* f) : order(o), filter(f) {}
    //
    static int compare(const SId& L, const SId& R) { 
      int rv = -scalar_compare(L.order, R.order); // revert order, a>b mean a before b
      if(rv) return rv;
      return scalar_compare(L.filter, R.filter);
    }
  };
  struct SFilter {
//    SParameters params;
    IBinObjFilter::P filter;
  };
  typedef tl::TScatterArray< SFilter, SId, SId > DFilters;
  //
  DFilters filters;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
  //
  RLibraryObjFilter(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags());
  ~RLibraryObjFilter(void);
  //
  // SFlags::ISetter
  DMediaFlags::eStates get_flag(int shift) const;
  DMediaFlags get_flags_auto(void) const;
  DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag);
  //
  // general object functions
  bool obj_fget(const IBinObjFilter::SInfo& info, IRefObject::P& obj);
  bool obj_fload(const IBinObjFilter::SInfo& info, IRefObject* obj);
  bool obj_fsave(IBinObjFilter::SInfo& info, const IRefObject* obj);
  //
  // object filtering
  bool filters_add(IBinObjFilter::P filter, const SParameters* filter_params, int order);
  bool filters_remove(IBinObjFilter* filter, int order);
  bool filters_clear(void);
  bool filters_load(const IBinObjFilter::SInfo& info, IRefObject::P& obj);
  bool filters_load(const IBinObjFilter::SInfo& info, IRefObject* obj);
  bool filters_save(IBinObjFilter::SInfo& info, const IRefObject* obj);
  //
  // utility
  inline bool validate_construction(void) const { return true; }
};

//***************************************
// END
//***************************************
