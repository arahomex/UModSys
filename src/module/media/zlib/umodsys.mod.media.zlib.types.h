
//***************************************
// RBinArchiveFrameClient_ZIP::
//***************************************

struct RBinArchiveFrameClient_ZIP : public IBinArchiveFrame::IClient
{
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RBinArchiveFrameClient_ZIP, 2, IBinArchiveFrame::IClient)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  typedef IBinArchiveFrame::NameNode NameNode;
  struct FileInfo {
    uint32 pos;
    uint32 csize;
    uint32 compression;
    uint32 crc;
    uint32 mt, md;
    uint32 attr, xattr;
  };
  //
  bool valid;
  //
  inline FileInfo* fi(NameNode* n) { return reinterpret_cast<FileInfo*>(n->extra); }
  //
  RBinArchiveFrameClient_ZIP(DOwner *own, const SParameters& args)
  : refs(own) {
    valid = true;
  }
  ~RBinArchiveFrameClient_ZIP(void) {
  }
  // IBinArchiveFrame::IClient
  bool arc_init(IBinArchiveFrame* arch, const SParameters* params);
  bool arc_deinit(IBinArchiveFrame* arch);
  //
  bool filer_pre_scan(IBinArchiveFrame* arch, size_t &rspace, const SParameters* params);
  bool filer_scan(IBinArchiveFrame* arch, SMem& memto, const SParameters* params);
  bool filew_init(IBinArchiveFrame* arch, const SParameters* params);
  bool filew_copy_data(IBinArchiveFrame* arch, NameNode* node);
  bool filew_writedir_start(IBinArchiveFrame* arch);
  bool filew_writedir_entry(IBinArchiveFrame* arch, NameNode* node);
  bool filew_writedir_end(IBinArchiveFrame* arch);
  //
  bool data_load(IBinArchiveFrame* arch, SCMemShared& mem, NameNode* node, const SFlags& flags);
  bool data_save(IBinArchiveFrame* arch, const SCMem& mem, const DCString& media_name, const SFlags& flags);
  IStreamReader::P data_reader(IBinArchiveFrame* arch, NameNode* node, const SFlags& flags);
  IStreamWriter::P data_writer(IBinArchiveFrame* arch, const DCString& media_name, const SFlags& flags);
  //
  bool node_info(IBinArchiveFrame *arc, SFileInfo& info, NameNode* node);
  //
  // Shared
  inline bool validate_construction(void) const { return valid; }
};

//***************************************
// END
//***************************************
