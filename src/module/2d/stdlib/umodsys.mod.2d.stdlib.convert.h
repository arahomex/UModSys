//------------------------------------
// 
//------------------------------------

inline bool ConvertSameSize(eImageType type, const DPoint &size,
                            const DPoint &pout, const DPoint& sout, const SMem& bout, 
                            const DPoint &pin, const DPoint& sin, const SCMem& bin)
{
  size_t sz = GetPixelSize(type);
  size_t stride_in = sz * sin(0), stride_out = sz * sout(0), q_line = sz * size(0);
  register const BByte* p_in = bin.get_data( pin(0)*sz + pin(1) * stride_in );
  register BByte* p_out = bout.get_data( pout(0)*sz + pout(1) * stride_out );
  for(register int i=0, n=size(1); i<n; i++, p_out += stride_out, p_in += stride_in)
    memcpy(p_out, p_in, q_line);
  return true;
}

inline bool ConvertSame(eImageType type, 
                        const DBox &rout, const DPoint& sout, const SMem& bout, 
                        const DBox &rin, const DPoint& sin, const SCMem& bin)
{
  if(rout.size()==rin.size())
    return ConvertSameSize(type, rout.size(), rout.origin(), sout, bout, rin.origin(), sin, bin);
  return false;
}


inline bool Convert(const DBox &rout, const SImageInfo& iout, const SMem& bout, 
                    const DBox &rin, const SImageInfo& iin, const SCMem& bin)
{
  if(rin.A(0)<0 || rin.A(1)<0 || rin.B(0)>iin.size(0) || rin.B(1)>iin.size(1))
    return false;
  if(rout.A(0)<0 || rout.A(1)<0 || rout.B(0)>iout.size(0) || rout.B(1)>iout.size(1))
    return false;
  if(iin.type==iout.type)
    return ConvertSame(iout.type, rout, iout.size, bout, rin, iin.size, bin);
  return false;
}

//------------------------------------
// 
//------------------------------------
