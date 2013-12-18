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
// TYPES
//***************************************

//***************************************
// SBuffer::

struct SBuffer : public SMem {
public:
  inline ~SBuffer(void) { 
    clear(UMODSYS_SOURCEINFO); 
  }
  inline SBuffer(IUtilities* U) 
  : utils(U), hint(0) {
  }
  inline SBuffer(ILibObject* lo) 
  : utils(lo ? lo->utils : NULL), hint(0) {
  }
  inline SBuffer(const SBuffer& R) 
  : utils(R.utils), hint(0) {
    SMem::operator=(R);
  }
  inline void operator=(const SBuffer& R) {
    SMem::operator=(R);
  }
  inline void operator=(const SMem& R) {
    SMem::operator=(R);
  }
  //
  inline void clear(const SSourceContext* sctx) {
    if(allocated.data) {
      if(utils.valid()) {
        utils->buffer_free(*this, sctx);
      }
      allocated.clear();
      hint = 0;
    }
    SMem::clear();
  }
  inline bool alloc(size_t sz, const SSourceContext* sctx) {
    if(!utils.valid())
      return false;
    return utils->buffer_alloc(*this, sz, sctx);
  }
  //
  inline SMem& _allocated(void) { return allocated; }
  inline int& _hint(void) { return hint; }
protected:
  SMem allocated;
  int hint;
  IUtilities::P utils;
};

//***************************************
// INLINES
//***************************************

inline bool archive_load_data(IBinArchive& a, const DCString& media_name, SCMemShared& mem, const SFlags& flags) 
{
  mem.clear();
  IStreamReader::P rv = a.data_reader(media_name, flags);
  if(rv.invalid())
    return false;
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
  if(rv.invalid())
    return false;
  if(!rv->writer_write(mem.data, mem.size)) {
    rv->writer_abort();
    return false;
  }
  rv->writer_close();
  return true;
}

//***************************************
// END
//***************************************

} // namespace libmedia
} // namespace UModSys

#endif // __UMODSYS_LIBMEDIA_UTILS_H
