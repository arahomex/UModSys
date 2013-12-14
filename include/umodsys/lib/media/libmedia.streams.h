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
// TTextStreamReader::
// TTextStreamReaderBuf::

template<typename Char>
struct TTextStreamReader
{
public:
  typedef Char DChar;
public:
  inline TTextStreamReader(IStreamReader* s, Char* b, size_t l) 
    : stream(s), buf(b), buflen(l), len(0), p(0) {}
  inline ~TTextStreamReader(void)
    {}
  //
  inline bool eof(void) { return !check(1); }
  inline Char get_c(void) { if(!check(1)) return 0; return buf[p++]; }
  inline size_t get_nz(Char *b, size_t nb) { size_t rv = get_n(b, nb-1); b[rv] = 0; return rv; }
  inline size_t get_nzd(Char *b, size_t nb, const Char* delim, size_t ndelim) { size_t rv = get_nd(b, nb-1, delim, ndelim); b[rv] = 0; return rv; }
  inline size_t get_nzd(Char *b, size_t nb, Char delim) { size_t rv = get_nd(b, nb-1, delim); b[rv] = 0; return rv; }
  //
  inline size_t get_n(Char *b, size_t nb) 
  {
    size_t rv = 0;
    while(!eof() && nb) {
      *b++ = buf[p++];
      nb--; rv++;
    }
    return rv;
  }
  inline size_t get_nd(Char *b, size_t nb, const Char* delim, size_t ndelim) {
    size_t rv = 0;
    while(!eof() && nb) {
      Char c = buf[p++];
      if(tl::su::schr(delim, c, ndelim)!=NULL)
        return rv;
      *b++ = c;
      nb--; rv++;
    }
    return rv;
  }
  inline size_t get_nd(Char *b, size_t nb, Char delim) {
    size_t rv = 0;
    while(!eof() && nb) {
      Char c = buf[p++];
      if(c==delim)
        return rv;
      *b++ = c;
      nb--; rv++;
    }
    return rv;
  }
protected:
  inline Char get1(void) { return ; }
  //
  inline bool check(size_t n)
  {  
    if(p+n<=len)
      return true;
    if(p!=0 && p!=len) {
      tl::su::smemmove(buf, buf+p, len-p);
    }
    len -= p; p = 0;
    if(!stream.valid())
      return false;
    DFilePosition left = stream->reader_size() - stream->reader_pos();
    if(left==0)
      return false; // EOF
    size_t left_buf = buflen-len;
    if(left<left_buf)
      left_buf = size_t(left);
    if(!stream->reader_read(buf + len, left_buf))
      return false;
    len += left_buf;
    return n<len;
  }
  //
  Char* buf;
  size_t buflen, p, len;
public:
  IStreamReader::P stream;
};

template<typename Char, size_t BufLength>
struct TTextStreamReaderBuf : public TTextStreamReader<Char>
{
  inline TTextStreamReaderBuf(IStreamReader* s) 
    : TTextStreamReader<Char>(s, fbuf, sizeof(fbuf)) {}
  inline ~TTextStreamReaderBuf(void)
    {}
protected:
  Char fbuf[BufLength];
};

//***************************************
// TTextStreamWriter::
// TTextStreamWriterBuf::

template<typename Char>
struct TTextStreamWriter
{
public:
  typedef Char DChar;
public:
  inline TTextStreamWriter(IStreamWriter* s, Char* b, size_t l) 
    : stream(s), buf(b), buflen(l), p(0) {}
  inline ~TTextStreamWriter(void)
    { put_buffer(); }
  //
  inline bool flush(void) { return put_buffer(); }
  inline bool put_c(Char b) { return put_chars(&b, 1); }
  inline bool put_n(const Char* b, size_t n) { return put_chars(b, n); }
  inline bool put_n(const Char* b1, size_t n1, const Char* b2, size_t n2) { return put_chars(b1, n1) && put_chars(b2, n2); }
  inline bool put_z(const Char* b) { return put_chars(b, tl::su::slen(b)); }
  inline bool put_z(const Char* b1, const Char* b2) { return put_chars(b1, tl::su::slen(b1)) && put_chars(b2, tl::su::slen(b2)); }
public:
  inline bool put_chars(const Char* b, size_t n)
  {
    while(n) {
      if(n<buflen-p) {
        tl::su::scpy(buf+p, b, n);
        p += n;
        return true;
      }
      if(p<buflen) {
        size_t r = buflen-p;
        tl::su::scpy(buf+p, b, r);
        p += r;
        b += r; n -= r;
      }
      if(!put_buffer())
        return false;
    }
    return true;
  }
  inline bool put_buffer(void)
  {
    if(!stream.valid())
      return false;
    if(!stream->writer_write(buf, p))
      return false;
    p = 0;
    return true;
  }
  //
  Char* buf;
  size_t buflen, p;
public:
  IStreamWriter::P stream;
};

template<typename Char, size_t BufLength>
struct TTextStreamWriterBuf : public TTextStreamWriter<Char>
{
  inline TTextStreamWriterBuf(IStreamWriter* s) 
    : TTextStreamWriter<Char>(s, fbuf, sizeof(fbuf)) {}
  inline ~TTextStreamWriterBuf(void)
    {}
protected:
  Char fbuf[BufLength];
};

//***************************************
// END
//***************************************

} // namespace libmedia
} // namespace UModSys

#endif // __UMODSYS_LIBMEDIA_STREAMS_H
