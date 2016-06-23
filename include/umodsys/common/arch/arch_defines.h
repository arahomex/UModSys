
typedef arch_types::sint8 Bsint8, BSI8;
typedef arch_types::uint8 Buint8, BUI8;

typedef arch_types::sint16 Bsint16, BSI16;
typedef arch_types::uint16 Buint16, BUI16;

typedef arch_types::sint32 Bsint32, BSI32;
typedef arch_types::uint32 Buint32, BUI32;

#ifdef UMODSYS_HAS_INT64
typedef arch_types::sint64 Bsint64, BSI64;
typedef arch_types::uint64 Buint64, BUI64;
#endif

typedef arch_types::float32 Bfloat32, BF32;
typedef arch_types::float64 Bfloat64, BF64;
typedef arch_types::float80 Bfloat80, BF80;

#if defined(UMODSYSL_ARCH_NONALIGNED)
  #define UMODSYS_ARCH_SIZE_CORRECT(size) (((size)+(sizeof(int)-1))&~(sizeof(int)-1))
  #define UMODSYS_ALIGNED_SIZE(_x) (_x, 3)
#else // !defined(UMODSYS_ARCH_NONALIGNED)
  #define UMODSYS_ARCH_SIZE_CORRECT(size) (((size)+(sizeof(int)-1))&~(sizeof(int)-1))
  #define UMODSYS_ALIGNED_SIZE(_x) (_x, 4)
#endif // defined(UMODSYS_ARCH_NONALIGNED)

typedef char                  BChar;
typedef unsigned char         BCharIndex;
typedef const BChar           *BCStr, *BStr;
typedef BChar                 *BWStr, *BOStr;

typedef wchar_t               BCharW;
typedef unsigned short        BCharWIndex;
typedef const BCharW          *BCStrW, *BStrW;
typedef BCharW                *BWStrW, *BOStrW;

typedef arch_types::sint32    BCharL;
typedef arch_types::uint32    BCharLIndex;
typedef const BCharL          *BCStrL, *BStrL;
typedef BCharL                *BWStrL, *BOStrL;

typedef unsigned char         BByte, byte;
typedef int                   Int;
typedef double                Real;

const size_t mem_quant = sizeof(int)*2;

struct STimeMsec {
  arch_types::OSTimeMsec time;
  //
  inline STimeMsec(void) {}
  inline STimeMsec(arch_types::OSTimeMsec t) : time(t) {}
  inline STimeMsec(arch_types::OSTimeMsec t, long dt) : time(t+dt) {}
  //
  inline double fsec(void) const { return time*0.001; }
};

inline long operator-(const STimeMsec& A, const STimeMsec& B) { return A.time - B.time; }
inline STimeMsec operator+(const STimeMsec& A, long dt) { return STimeMsec(A.time, dt); }
inline STimeMsec operator+(const STimeMsec& A, float dt) { return STimeMsec(A.time, long(dt*1000)); }
inline STimeMsec operator+(const STimeMsec& A, double dt) { return STimeMsec(A.time, long(dt*1000)); }


