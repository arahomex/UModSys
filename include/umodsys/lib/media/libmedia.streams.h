#ifndef __UMODSYS_LIBMEDIA_STREAMS_H
#define __UMODSYS_LIBMEDIA_STREAMS_H 1
/*************************************************************/
// file: umodsys/lib/libmedia/common.h
// info: media streams
/*************************************************************/

#include <umodsys/lib/media/libmedia.typedefs.h>


namespace UModSys {
namespace libmedia {

//***************************************
// INTERFACES
//***************************************

//***************************************
// IStreamReader::

struct IStreamReader
: public IRefObject
{
  virtual bool reader_seek(DFilePosition pos) =0;
  virtual bool reader_seekend(DFilePosition pos) =0;
  virtual bool reader_seekdelta(DFilePosition pos) =0;
  virtual DFilePosition reader_pos(void) =0;
  virtual DFilePosition reader_size(void) =0;
  virtual bool reader_read(void* data, size_t size) =0;
  virtual void reader_close(void) =0;
  //
  inline bool reader_read(const SMem& m) { return reader_read(m.data, m.size); }
  inline bool reader_read(const SMemChunk& m) { return reader_read(m.get_data(), m.get_size()); }
  inline bool reader_read(const SMemShared& m) { return reader_read(m.get_data(), m.get_size()); }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IStreamReader, 2, IRefObject);
};

//***************************************
// IStreamWriter::

struct IStreamWriter
: public IRefObject 
{
public:
  virtual bool writer_seek(DFilePosition pos) =0;
  virtual bool writer_seekend(DFilePosition pos) =0;
  virtual bool writer_seekdelta(DFilePosition pos) =0;
  virtual bool writer_setsize(DFilePosition pos) =0;
  virtual DFilePosition writer_pos(void) =0;
  virtual DFilePosition writer_size(void) =0;
  virtual bool writer_write(const void* data, size_t size) =0;
  virtual bool writer_copy(IStreamReader* from, DFilePosition count) =0;
  virtual bool writer_copy(IStreamReader* from, DFilePosition start, DFilePosition count) =0;
  virtual void writer_close(void) =0;
  virtual void writer_abort(void) =0;
  //
  inline bool writer_write(const SCMem& m) { return writer_write(m.data, m.size); }
  inline bool writer_write(const SMemChunk& m) { return writer_write(m.get_data(), m.get_size()); }
  inline bool writer_write(const SMemShared& m) { return writer_write(m.get_data(), m.get_size()); }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::IStreamWriter, 2, IRefObject);
};

//***************************************
// ITextReader::

struct ITextReader
: public IRefObject
{
  virtual bool reader_get(char* buf, size_t buflen, size_t *read, const char *delims, int numdelims) =0;
  virtual DFilePosition reader_pos(void) =0;
  virtual DFilePosition reader_size(void) =0;
  virtual void reader_close(void) =0;
  //
  inline bool reader_getln(char* buf, size_t buflen, size_t *read) 
    { return reader_get(buf, buflen, read, "\n", 1); }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ITextReader, 2, IRefObject);
};

//***************************************
// IStreamWriter::

struct ITextWriter
: public IRefObject 
{
public:
  virtual DFilePosition writer_pos(void) =0;
  virtual DFilePosition writer_size(void) =0;
  virtual bool writer_put(const char* buf, size_t buflen) =0;
  virtual void writer_close(void) =0;
  virtual void writer_abort(void) =0;
  //
  inline bool writer_putln(char* buf, size_t buflen) 
    { return writer_put(buf, buflen) && writer_put("\n", 1); }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libmedia::ITextWriter, 2, IRefObject);
};

//***************************************
// END
//***************************************

} // namespace libmedia
} // namespace UModSys

#endif // __UMODSYS_LIBMEDIA_STREAMS_H
