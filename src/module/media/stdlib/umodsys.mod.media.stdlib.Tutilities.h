
//***************************************
// RUtilities::
//***************************************

bool RUtilities::buffer_free(SBuffer& B, const SSourceContext* sctx)
{
  if(B._hint()>0 && B._hint()<=~buffers) {
    Buffer& bb = buffers[B._hint()];
    if(bb.locked && B._allocated().data==bb.buf.All()) {
      bb.locked = false;
    }
    return true;
  }
  B._hint() = 0;
  B._allocated().clear();
  B.set(0, NULL);
  return false;
}

//***************************************

bool RUtilities::buffer_alloc(SBuffer& B, size_t sz, const SSourceContext* sctx)
{
  buffer_free(B, sctx);
  if(sz==0)
    return true;
  int found = -1;
  size_t foundsize = 0;
  for(int i=0; i<~buffers; i++) {
    size_t n = ~buffers(i).buf;
    if(sz>n)
      continue;
    if(sz==n) {
      foundsize = n;
      found = i;
      break;
    }
    if(found<0 || n<foundsize) {
      foundsize = n;
      found = i;
    }
  }
  if(found<0) {
    if(!buffers.Push() || buffers.Last().buf.Resize(sz))
      return false;
    found = buffers.LastIndex();
  }
  Buffer &b = buffers[found];
  B._allocated().set(b.buf.All(), ~b.buf);
  B._hint() = found;
  B.set(b.buf.All(), sz);
  b.locked = true;
  return true;
}

//***************************************

IStreamReader::P RUtilities::sub_reader(IStreamReader* master, DFilePosition pos, DFilePosition size)
{
  return NULL;
}

//***************************************

IStreamWriter::P RUtilities::sub_writer(IStreamWriter* master, DFilePosition pos, DFilePosition size)
{
  return NULL;
}

//***************************************

IStreamReader::P RUtilities::mem_reader(const SCMem& mem)
{
  return NULL;
}

//***************************************

IStreamReader::P RUtilities::mem_reader(const SCMemShared& mem)
{
  return NULL;
}

//***************************************

IStreamWriter::P RUtilities::mem_writer(const SMem* mem, IMemWriterCompleted* mw, IRefObject* mwh)
{
  return NULL;
}

//***************************************
//***************************************

RUtilities::RUtilities(DOwner *own, const SParameters& args)
: refs(own) 
{
}

RUtilities::~RUtilities(void)
{
}

//***************************************
// END
//***************************************
