#ifndef __UMS_CORE_MEDIA_ARCHIVE_H
#define __UMS_CORE_MEDIA_ARCHIVE_H 1
//*****************************************************
// ums/core/media/media_archive.h: media library interface, archives
//*****************************************************

#include <ums/core/media/media.h>

#include <ums/core/types/tree_rb.h>

namespace UMS {
namespace core {
namespace media {

//***************************************

struct CMediaArchiveFrame;
struct IMediaArchiveFrameClient;

//***************************************

struct CMediaArchiveFrame : public IMediaArchive {
public:
  struct NameNode;
  typedef types::TTreeNodeRB<NameNode> NameNodeBase;
  typedef types::TTreeHoldRB<NameNode> NameNodes;
  //
  struct NameNode : public NameNodeBase {
    // node_flag1 -> rnode=0, wnode=1
    DHString name;
    int length; // ==-1 for directory only
    void *extra;
    //
    inline void* user_data(void) { return this+1; }
    //
  };
  //
  struct NameNodeCmpC {
    DHString name;
    //
    inline NameNodeCmpC(const DLString& s) 
      : name(s) {}
    //
    inline int operator()(const NameNode* node) const { return name.cmp(node->name); }
  };
  //
  struct NameNodeCmpUC {
    DHString name;
    //
    inline NameNodeCmpUC(const DLString& s) 
      : name(s, su::collate_nocase()) {}
    //
    inline int operator()(const NameNode* node) const { return name.cmp_any(node->name); }
  };
  //
public:
  virtual bool open(IMediaArchiveFrameClient* client, const SParameters* params) =0;
  virtual bool close(void) =0;
  //
  virtual NameNode* add_node_r(const DHString& name, size_t extra, SMem& memto) =0;
  virtual NameNode* add_node_w(const DHString& name, size_t extra) =0;
  virtual bool remove_node(NameNode* node) =0;
  virtual NameNode* find_node(const DHString& name) =0;
  virtual size_t node_size(const DHString &name, int extra) =0;
public:
  IMediaReader::P filer;
  IMediaWriter::P filew;
  IMediaReader::P filerw;
  bool ignore_case;
  int permission;
  int user_flags[8];
protected:
  UMS_ROOT_P_IMPLEMENT(core::media::CMediaArchiveFrame, IMediaArchive)
};

//***************************************

struct IMediaArchiveFrameClient : public IRefObject {
  //
  typedef CMediaArchiveFrame::NameNode SNameNode;
  //
  virtual bool arc_init(CMediaArchiveFrame* arch, const SParameters* params) =0;
  virtual bool arc_deinit(CMediaArchiveFrame* arch) =0;
  //
  virtual bool filer_pre_scan(CMediaArchiveFrame* arch, int &rspace, const SParameters* params) =0;
  virtual bool filer_scan(CMediaArchiveFrame* arch, SMem& memto, const SParameters* params) =0;
  //
  virtual bool filew_init(CMediaArchiveFrame* arch, const SParameters* params) =0;
  virtual bool filew_copy_data(CMediaArchiveFrame* arch, SNameNode* node) =0;
  virtual bool filew_writedir_start(CMediaArchiveFrame* arch) =0;
  virtual bool filew_writedir_entry(CMediaArchiveFrame* arch, SNameNode* node) =0;
  virtual bool filew_writedir_end(CMediaArchiveFrame* arch) =0;
  //
  virtual bool load_data(IMediaGroup* glib, CMediaArchiveFrame* arch, 
                         UMS_CSMem& mem, SNameNode* node, int flags) =0;
  virtual bool save_data(IMediaGroup* glib, CMediaArchiveFrame* arch, 
                         const SCMem& mem, const DHString& media_name, int flags) =0;
  virtual IMediaReader::P load_reader(IMediaGroup* glib, CMediaArchiveFrame* arch, 
                                      SNameNode* node, int flags) =0;
  virtual IMediaWriter::P save_writer(IMediaGroup* glib, CMediaArchiveFrame* arch, 
                                      const DHString& media_name, int flags) =0;
  //
  virtual bool entry_info(CMediaArchiveFrame *arc, SMediaFileInfo* info, SNameNode* node) = 0;
//  virtual bool entry_permission(SMediaArchiveEntryInfo* info, SNameNode* node) = 0;
protected:
  UMS_ROOT_P_IMPLEMENT(core::media::IMediaArchiveFrameClient, IRefObject)
};

//***************************************

//***************************************

} // namespace media
} // namespace core
} // namespace UMS

#endif
