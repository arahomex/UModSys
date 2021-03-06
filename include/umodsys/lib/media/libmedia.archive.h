#ifndef __UMODSYS_LIBMEDIA_ARCHIVE_H
#define __UMODSYS_LIBMEDIA_ARCHIVE_H 1
/*************************************************************/
// file: umodsys/lib/libmedia/common.h
// info: media common structures
/*************************************************************/

#include <umodsys/lib/media/libmedia.common.h>
#include <umodsys/tl/container/tree_rb.h>

namespace UModSys {
namespace libmedia {

using namespace core;

//***************************************
// INTERFACES
//***************************************

//***************************************
// IBinArchiveFrame

struct IBinArchiveFrame : public IBinArchive {
public:
  struct IClient;
  //
  struct NameNode;
  typedef tl::TTreeNodeRB<NameNode> NameNodeBase;
  typedef tl::TTreeHoldRBD<NameNode> NameNodes;
  //
  struct NameNode : public NameNodeBase {
    // node_flag1 -> rnode=0, wnode=1
    DCString name;
    DCNocaseString namenc;
    DFilePosition length; // ==-1 for directory only
    void *extra;
    //
    inline void* user_data(void) { return this+1; }
    //
    inline void setname(void* data, int ofs, const DCString& source) 
      { name.set_ofs(data, ofs, source); name.update(); namenc.set(*name, ~name); }
    inline void setname(void* data, int ofs, const DCNocaseString& source) 
      { namenc.set_ofs(data, ofs, source); namenc.update(); name.set(*namenc, ~namenc); }
  };
  //
  struct NameNodeCmpC {
    DCString name;
    //
    inline NameNodeCmpC(const DCString& s) 
      : name(s) {}
    //
    inline int operator()(const NameNode* node) const 
      { return name.cmp(node->name); }
  };
  //
  struct NameNodeCmpUC {
    DCNocaseString name;
    //
    inline NameNodeCmpUC(const DCString& s) 
      : name(*s, ~s) {}
    //
    inline int operator()(const NameNode* node) const 
      { return name.cmp(node->namenc); }
  };
public:
  virtual bool open(IClient* client, const SParameters* params) =0;
  virtual bool close(void) =0;
  //
  virtual NameNode* add_node_r(const DCString& name, size_t extra, SMem& memto) =0;
  virtual NameNode* add_node_w(const DCString& name, size_t extra) =0;
  virtual bool remove_node(NameNode* node) =0;
  virtual NameNode* find_node(const DCString& name) =0;
  virtual size_t node_size(const DCString &name, int extra) =0;
public:
  IStreamReader::P filer;
  IStreamWriter::P filew;
  IStreamReader::P filerw;
  bool ignore_case;
  int permission;
  int user_flags[8];
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IBinArchiveFrame, 2, IBinArchive);
};

//***************************************
// IBinArchiveFrame::IClient

struct IBinArchiveFrame::IClient : public IRefObject {
  virtual bool arc_init(IBinArchiveFrame* arch, const SParameters* params) =0;
  virtual bool arc_deinit(IBinArchiveFrame* arch) =0;
  //
  virtual bool filer_pre_scan(IBinArchiveFrame* arch, size_t &rspace, const SParameters* params) =0;
  virtual bool filer_scan(IBinArchiveFrame* arch, SMem& memto, const SParameters* params) =0;
  //
  virtual bool filew_init(IBinArchiveFrame* arch, const SParameters* params) =0;
  virtual bool filew_copy_data(IBinArchiveFrame* arch, NameNode* node) =0;
  virtual bool filew_writedir_start(IBinArchiveFrame* arch) =0;
  virtual bool filew_writedir_entry(IBinArchiveFrame* arch, NameNode* node) =0;
  virtual bool filew_writedir_end(IBinArchiveFrame* arch) =0;
  //
  virtual bool data_load(IBinArchiveFrame* arch, SCMemShared& mem, 
                         NameNode* node, const SFlags& flags) =0;
  virtual bool data_save(IBinArchiveFrame* arch, const SCMem& mem, 
                         const DCString& media_name, const SFlags& flags) =0;
  virtual IStreamReader::P data_reader(IBinArchiveFrame* arch, NameNode* node, const SFlags& flags) =0;
  virtual IStreamWriter::P data_writer(IBinArchiveFrame* arch, const DCString& media_name, const SFlags& flags) =0;
  //
  virtual bool node_info(IBinArchiveFrame *arc, SFileInfo& info, NameNode* node) = 0;
//  virtual bool entry_permission(SMediaArchiveEntryInfo* info, NameNode* node) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IBinArchiveFrame::IClient, 2, IRefObject);
};

//***************************************
// END
//***************************************

} // namespace libmedia
} // namespace UModSys

#endif // __UMODSYS_LIBMEDIA_ARCHIVE_H
