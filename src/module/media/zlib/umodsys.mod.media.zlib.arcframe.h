
//***************************************
// RBinArchiveFrameClient::
//***************************************

struct RBinArchiveFrameClient : public IBinArchiveFrame::IClient
{
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RBinArchiveFrameClient, 2, IBinArchiveFrame::IClient)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  typedef IBinArchiveFrame::NameNode NameNode;
  //
  bool valid;
  //
  RBinArchiveFrameClient(DOwner *own, const SParameters& args)
  : refs(own) {
    valid = true;
  }
  ~RBinArchiveFrameClient(void) {
  }
  // IBinArchiveFrame::IClient
  bool arc_init(IBinArchiveFrame* arch, const SParameters* params) 
  {
    return false;
  }
  bool arc_deinit(IBinArchiveFrame* arch) 
  {
    return false;
  }
  //
  bool filer_pre_scan(IBinArchiveFrame* arch, int &rspace, const SParameters* params)
  {
    return false;
  }
  bool filer_scan(IBinArchiveFrame* arch, SMem& memto, const SParameters* params)
  {
    return false;
  }
  //
  bool filew_init(IBinArchiveFrame* arch, const SParameters* params)
  {
    return false;
  }
  bool filew_copy_data(IBinArchiveFrame* arch, NameNode* node)
  {
    return false;
  }
  bool filew_writedir_start(IBinArchiveFrame* arch)
  {
    return false;
  }
  bool filew_writedir_entry(IBinArchiveFrame* arch, NameNode* node)
  {
    return false;
  }
  bool filew_writedir_end(IBinArchiveFrame* arch)
  {
    return false;
  }
  //
  bool data_load(IBinArchiveFrame* arch, SCMemShared& mem, NameNode* node, const SFlags& flags)
  {
    return false;
  }
  bool data_save(IBinArchiveFrame* arch, const SCMem& mem, const DCString& media_name, const SFlags& flags)
  {
    return false;
  }
  IStreamReader::P data_reader(IBinArchiveFrame* arch, NameNode* node, const SFlags& flags)
  {
    return NULL;
  }
  IStreamWriter::P data_writer(IBinArchiveFrame* arch, const DCString& media_name, const SFlags& flags)
  {
    return NULL;
  }
  //
  bool data_list(IBinArchiveFrame *arc, DIFileInfoArray& list, NameNode* node)
  {
    return false;
  }
  //
  // Shared
  inline bool validate_construction(void) const { return valid; }
};

//***************************************
// END
//***************************************
