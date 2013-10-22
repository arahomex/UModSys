#ifndef __UMODSYS_LIBMEDIA_UTILS_H
#define __UMODSYS_LIBMEDIA_UTILS_H 1
/*************************************************************/
// file: umodsys/lib/libmedia/common.h
// info: media common structures
/*************************************************************/

#include <umodsys/lib/media/libmedia.common.h>


namespace UModSys {
namespace libmedia {

//***************************************
// INLINES
//***************************************

inline bool archive_load_data(IBinArchive& a, const DCString& media_name, SCMemShared& mem, const SFlags& flags) 
{
  mem.clear();
  IStreamReader::P rv = a.data_reader(media_name, flags);
  DFilePosition sz = rv->reader_size();
  if(sz>mem_max_allocation) {
    rv->reader_close();
    return false; // max size not allowed
  }
  size_t msize = size_t(sz);
  if(msize==0)
    return true; // ok to load zero size file
  SMemShared m2(shared_memory(), msize);
  if(!m2.valid() || m2.get_size()!=msize) {
    rv->reader_close();
    return false; // max size not allowed
  }
  if(!rv->reader_read(m2.get_data(), m2.get_size())) {
    rv->reader_close();
    return false; // max size not allowed
  }
  mem = m2;
  return true;
}

inline bool archive_save_data(IBinArchive& a, const DCString& media_name, const SCMem& mem, const SFlags& flags) 
{
  IStreamWriter::P rv = a.data_writer(media_name, flags);
  if(!rv->writer_write(mem.data, mem.size)) {
    rv->writer_abort();
    return false;
  }
  rv->writer_close();
  return true;
}

//***************************************
// ILibrary::

inline bool ILibrary::obj_std_get(IRefObject::P& obj, const DCString& media_name, const SObjOptions& opts)
{
  return false;
}

inline bool ILibrary::obj_std_load(IRefObject* obj, const DCString& media_name, const SObjOptions& opts)
{
  return false;
}

inline bool ILibrary::obj_std_save(IRefObject* obj, const DCString& media_name, const SObjOptions& opts)
{
  return false;
}

//***************************************
// END
//***************************************

} // namespace libmedia
} // namespace UModSys

#endif // __UMODSYS_LIBMEDIA_UTILS_H
