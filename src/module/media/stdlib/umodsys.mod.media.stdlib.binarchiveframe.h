
//***************************************
// RBinArchiveFrame::
//***************************************

struct RBinArchiveFrame : public IBinArchiveFrame
{
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RBinArchiveFrame, 1, IBinArchiveFrame)
  UMODSYS_REFOBJECT_REFMODULE()
  //
  SMemShared rdir;
  IClient::P client;
  NameNodes nodes;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
  //
  RBinArchiveFrame(DOwner *own, const SParameters& args, DMediaFlags av=DMediaFlags())
  : refs(own), flags(av)/*, cache(local_memory())*/ {
    open(args);
  }
  ~RBinArchiveFrame(void) {
  }
  //
  // SFlags::ISetter
  DMediaFlags::eStates get_flag(int shift) const { return flags.get_s(shift); }
  DMediaFlags get_flags_auto(void) const { return auto_flags; }
  DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag) { return flags.getset_s(shift, flag); }
  //
  // IBinArchive 
  IStreamReader::P data_reader(const DCString& media_name, const SFlags& flags)
  {
    return NULL;
  }
  IStreamWriter::P data_writer(const DCString& media_name, const SFlags& flags)
  {
    return NULL;
  }
  bool data_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags)
  {
    return false;
  }
  bool data_save(const DCString& media_name, const SCMem& mem, const SFlags& flags)
  {
    return false;
  }
  bool data_list(const DCString& mask, DIFileInfoArray& list, const SFlags& flags)
  {
    return false;
  }
  int get_permissions(void) 
  {
    return 0;
  }
  //
  // IBinArchiveFrame
  bool open(IClient* client, const SParameters* params)
  {
    return false;
  }
  bool close(void)
  {
    return false;
  }
  //
  NameNode* add_node_r(const DCString& name, size_t extra, SMem& memto)
  {
    return NULL;
  }
  NameNode* add_node_w(const DCString& name, size_t extra)
  {
    return NULL;
  }
  bool remove_node(NameNode* node)
  {
    return false;
  }
  NameNode* find_node(const DCString& name)
  {
    return NULL;
  }
  size_t node_size(const DCString &name, int extra)
  {
    return 0;
  }
  //
  // utility
  bool open(const SParameters& args)
  {
    return false;
  }
  inline bool validate_construction(void) const { return true; }
};

//***************************************
// END
//***************************************
