
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
  bool data_list(const DCString& mask, DIFileInfoArray& list, const SFlags& flags);
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
  bool bin_info(const DCString& media_mask, DIFileInfoArray& info, const SFlags& flags);
  bool bin_get(const DCString& media_name, SCMemShared& mem, bool isinv);
  bool bin_put(const DCString& media_name, const SCMemShared* mem);
  // general object functions
  bool obj_fget(const IBinObjFilter::SInfo& info, IRefObject::P& obj);
  bool obj_fload(const IBinObjFilter::SInfo& info, IRefObject* obj);
  bool obj_fsave(IBinObjFilter::SInfo& info, const IRefObject* obj);
  bool obj_cget(const DCString& media_name, IRefObject::P& obj, bool isinv);
  bool obj_cput(const DCString& media_name, IRefObject* obj);
  // universal object functions
  bool obj_get(const DCString& media_name, IRefObject::P& obj, const SObjOptions& opts);
  bool obj_load(const DCString& media_name, IRefObject* obj, const SObjOptions& opts);
  bool obj_save(const DCString& media_name, const IRefObject* obj, const SObjOptions& opts);
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
// END
//***************************************
