
//***************************************
// RLibraryBinTree::
//***************************************

DMediaFlags RLibraryBinTree::auto_flags;

RLibraryBinTree::RLibraryBinTree(DOwner *own, const SParameters& args, DMediaFlags av)
: refs(own), flags(av)/*, cache(local_memory())*/ 
{
}

RLibraryBinTree::~RLibraryBinTree(void) 
{
}

//***************************************
//***************************************

DMediaFlags::eStates RLibraryBinTree::get_flag(int shift) const 
{ 
  return flags.get_s(shift); 
}

DMediaFlags RLibraryBinTree::get_flags_auto(void) const 
{ 
  return auto_flags; 
}

DMediaFlags::eStates RLibraryBinTree::set_flag(int shift, DMediaFlags::eStates flag) 
{ 
  return flags.getset_s(shift, flag); 
}

//***************************************
//***************************************

IStreamReader::P RLibraryBinTree::bin_reader(const DCString& media_name, const SFlags& flags) 
{
  for(DMounts::CIter x=mounts(); x; ++x) {
    const SMountInfo& mi = x->second;
    const DCString &mnt = mi.mount_name.get_s();
    if(~media_name<=~mnt || !tl::su::seq(*mnt, *media_name, ~mnt) 
       || !mi.archive.valid() || !(mi.pm & mp_Read))
      continue;
    IStreamReader::P rv = mi.archive->data_reader(media_name + ~mnt, flags);
    if(rv.valid())
      return rv;
  }
  return NULL;
}

IStreamWriter::P RLibraryBinTree::bin_writer(const DCString& media_name, const SFlags& flags) 
{
  for(DMounts::CIter x=mounts(); x; ++x) {
    const SMountInfo& mi = x->second;
    const DCString &mnt = mi.mount_name.get_s();
    if(~media_name<=~mnt || !tl::su::seq(*mnt, *media_name, ~mnt) 
       || !mi.archive.valid() || !(mi.pm & mp_Write))
      continue;
    IStreamWriter::P rv = mi.archive->data_writer(media_name + ~mnt, flags);
    if(rv.valid())
      return rv;
  }
  return NULL;
}

bool RLibraryBinTree::bin_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags) 
{
  mem.clear();
  for(DMounts::CIter x=mounts(); x; ++x) {
    const SMountInfo& mi = x->second;
    const DCString &mnt = mi.mount_name.get_s();
    if(~media_name<=~mnt || !tl::su::seq(*mnt, *media_name, ~mnt) 
       || !mi.archive.valid() || !(mi.pm & mp_Read))
      continue;
    if(mi.archive->data_load(media_name + ~mnt, mem, flags))
      return true;
  }
  return false;
}

bool RLibraryBinTree::bin_save(const DCString& media_name, const SCMem& mem, const SFlags& flags) 
{
  for(DMounts::CIter x=mounts(); x; ++x) {
    const SMountInfo& mi = x->second;
    const DCString &mnt = mi.mount_name.get_s();
    if(~media_name<=~mnt || !tl::su::seq(*mnt, *media_name, ~mnt) 
       || !mi.archive.valid() || !(mi.pm & mp_Read))
      continue;
    if(mi.archive->data_save(media_name + ~mnt, mem, flags))
      return true;
  }
  return false;
}

bool RLibraryBinTree::bin_info(const DCString& media_name, SFileInfo& info, const SFlags& flags) 
{
  for(DMounts::CIter x=mounts(); x; ++x) {
    const SMountInfo& mi = x->second;
    if(!mi.archive.valid() || !(mi.pm & mp_List))
      continue;
    const DCString &mnt = mi.mount_name.get_s();
    if(~media_name<=~mnt && !tl::su::seq(*mnt, *media_name, ~mnt))
      continue;
    if(mi.archive->data_info(media_name, ~mnt, info, flags))
      return true;
  }
  return false;
}

bool RLibraryBinTree::bin_list(const DCString& media_mask, DIFileInfoArray& info, const SFlags& flags) 
{
  bool rv = false;
  for(DMounts::CIter x=mounts(); x; ++x) {
    const SMountInfo& mi = x->second;
    if(!mi.archive.valid() || !(mi.pm & mp_List))
      continue;
    const DCString &mnt = mi.mount_name.get_s();
    if(~media_mask>=~mnt && tl::su::seq(*mnt, *media_mask, ~mnt)) {
      rv = mi.archive->data_list(media_mask, ~mnt, info, flags) || rv;
    } /*else if(tl::su::wildcmp(media_mask(), mnt())) {
      rv = mi.archive->data_list(media_mask, ~mnt, info, flags) || rv;
    }*/
  }
  return rv;
}

//***************************************
//***************************************

RLibraryBinTree::SPoint RLibraryBinTree::mount_get(const SId& id) const 
{
  const SMountInfo* x = mounts(id);
  if(x==NULL)
    return SPoint();
  return *x;
}

bool RLibraryBinTree::mount_add(const SId& id, const SPoint& mp) 
{
  SMountInfo* x = mounts(id);
  if(x==NULL) {
    DStringShared mn2 = id.mountpoint;
    x = mounts(SId(mn2.get_s(), id.order), void_null());
    if(x==NULL)
      return false;
    x->mount_name = mn2;
  } 
  *x = mp;
  return true;
}

bool RLibraryBinTree::mount_remove(const SId& id) 
{
  return mounts.Remove(id);
}

bool RLibraryBinTree::mount_clear(void) {
  mounts.Clear();
  return true;
}

//***************************************
// END
//***************************************
