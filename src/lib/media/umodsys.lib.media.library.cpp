#include <umodsys/lib/media/libmedia.common.h>
#include <umodsys/core/syshlp.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::libmedia;

//***************************************
// ILibrary::
//***************************************

//***************************************
// general data functions

IStreamReader::P ILibrary::bin_reader(const DCString& media_name, const SFlags& flags)
{
  return NULL;
}

IStreamWriter::P ILibrary::bin_writer(const DCString& media_name, const SFlags& flags)
{
  return NULL;
}

// load binary data
bool ILibrary::bin_load(const DCString& media_name, SCMemShared& mem, const SFlags& flags)
{
  return false;
}

// save binary data
bool ILibrary::bin_save(const DCString& media_name, const SCMem& mem, const SFlags& flags)
{
  return false;
}

// determine attributes
bool ILibrary::bin_info(const DCString& media_name, SFileInfo& info, const SFlags& flags)
{
  return false;
}

// get full list
bool ILibrary::bin_info(const DCString& media_mask, DIFileInfoArray& info, const SFlags& flags)
{
  return false;
}

// get cache element
bool ILibrary::bin_get(const DCString& media_name, SCMemShared& mem, bool isinv)
{
  return false;
}

// put cache element
bool ILibrary::bin_put(const DCString& media_name, const SCMemShared* mem)
{
  return false;
}

//***************************************
// general object functions

bool ILibrary::obj_fget(const IBinObjFilter::SInfo& info, IRefObject::P& obj)
{
  return false;
}

bool ILibrary::obj_fload(const IBinObjFilter::SInfo& info, IRefObject* obj)
{
  return false;
}

bool ILibrary::obj_fsave(IBinObjFilter::SInfo& info, const IRefObject* obj)
{
  return false;
}

// get cache element
bool ILibrary::obj_cget(const DCString& media_name, IRefObject::P& obj, bool isinv) 
{
  return false;
}

// put cache element
bool ILibrary::obj_cput(const DCString& media_name, IRefObject* obj)
{
  return false;
}

//***************************************
// universal object functions

bool ILibrary::obj_get(const DCString& media_name, IRefObject::P& obj, const SObjOptions& opts)
{
  return false;
}

bool ILibrary::obj_load(const DCString& media_name, IRefObject* obj, const SObjOptions& opts)
{
  return false;
}

bool ILibrary::obj_save(const DCString& media_name, const IRefObject* obj, const SObjOptions& opts)
{
  return false;
}

//***************************************
// ::
//***************************************
