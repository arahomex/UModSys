#ifndef __UMODSYS_TL_STREAMS_H
#define __UMODSYS_TL_STREAMS_H 1
/*************************************************************/
// file: umodsys/tl/codec/streams.h
// info: streams
/*************************************************************/

namespace UModSys {
namespace tl {

//***************************************
// TTextStreamReader::
// TTextStreamReaderBuf::

template<typename Char, typename BinReader>
struct TTextStreamReader {
public:
  typedef Char DChar;
  typedef BinReader DBinReader;
  typedef typename BinReader::P DBinReaderP;
public:
  inline TTextStreamReader(BinReader* s, Char* b, size_t l) 
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
    if(!valid(stream))
      return false;
    size_t left_buf = buflen-len;
    if(!stream->reader_read(buf + len, &left_buf))
      return false;
    if(left_buf==0)
      return false; // EOF
    len += left_buf;
    return n<len;
  }
  //
  Char* buf;
  size_t buflen, p, len;
public:
  DBinReaderP stream;
};

template<typename Char, size_t BufLength, typename BinReader>
struct TTextStreamReaderBuf : public TTextStreamReader<Char, BinReader>
{
  inline TTextStreamReaderBuf(BinReader* s) 
    : TTextStreamReader<Char, BinReader>(s, fbuf, BufLength) {}
  inline ~TTextStreamReaderBuf(void)
    {}
protected:
  Char fbuf[BufLength];
};

//***************************************
// TTextStreamWriter::
// TTextStreamWriterBuf::

template<typename Char, typename BinWriter>
struct TTextStreamWriter
{
public:
  typedef Char DChar;
  typedef BinWriter DBinWriter;
  typedef typename BinWriter::P DBinWriterP;
public:
  inline TTextStreamWriter(BinWriter* s, Char* b, size_t l) 
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
  DBinWriterP stream;
};

template<typename Char, size_t BufLength, typename BinWriter>
struct TTextStreamWriterBuf : public TTextStreamWriter<Char, BinWriter>
{
  inline TTextStreamWriterBuf(BinWriter* s) 
    : TTextStreamWriter<Char, BinWriter>(s, fbuf, BufLength) {}
  inline ~TTextStreamWriterBuf(void)
    {}
protected:
  Char fbuf[BufLength];
};

//***************************************
// END
//***************************************

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_STREAMS_H
