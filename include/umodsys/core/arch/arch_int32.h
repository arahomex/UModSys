
typedef signed char Bsint8, BSI8;
typedef unsigned char Buint8, BUI8;

typedef signed short Bsint16, BSI16;
typedef unsigned short Buint16, BUI16;

typedef signed long Bsint32, BSI32;
typedef unsigned long Buint32, BUI32;

typedef signed long long Bsint64, BSI64;
typedef unsigned long long Buint64, BUI64;

typedef float BF32;
typedef double BF64;
typedef long double BF80;

#if defined(UMODSYS_ARCH_INTELTYPES)
  typedef signed __int64        sint64;
  typedef unsigned __int64      uint64;
  typedef signed __int32        sint32;
  typedef unsigned __int32      uint32;
  typedef signed __int16        sint16;
  typedef unsigned __int16      uint16;
  typedef signed __int8         sint8;
  typedef unsigned __int8       uint8;

  typedef float                 float32;
  typedef double                float64;
  typedef long double           float80;
#else
  typedef signed long long      sint64;
  typedef unsigned long long    uint64;
  typedef unsigned int          uint32;
  typedef signed int            sint32;
  typedef signed short          sint16;
  typedef unsigned short        uint16;
  typedef signed char           sint8;
  typedef unsigned char         uint8;
  typedef char                  cint8;

  typedef float                 float32;
  typedef double                float64;
#endif

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

typedef sint32                BCharL;
typedef uint32                BCharLIndex;
typedef const BCharL          *BCStrL, *BStrL;
typedef BCharL                *BWStrL, *BOStrL;

typedef unsigned char         BByte, byte;
typedef int                   Int;
typedef double                Real;

typedef uint64                OSTime;

enum {
  SIZE_sint8 = 1,
  SIZE_uint8 = 1,
  SIZE_sint16 = 2,
  SIZE_uint16 = 2,
  SIZE_sint32 = 4,
  SIZE_uint32 = 4,
  SIZE_sint64 = 8,
  SIZE_uint64 = 8,
  SIZE_float32 = 4,
  SIZE_float64 = 8,
  SIZE_float80 = 10,
  SIZE_byte = 1
};


const size_t mem_quant = sizeof(int)*2;

struct STimeMsec {
  BSI64 time;
  //
  inline STimeMsec(void) {}
  inline STimeMsec(BSI64 t) : time(t) {}
  inline STimeMsec(BSI64 t, long dt) : time(t+dt) {}
  //
  inline double fsec(void) const { return time*0.001; }
};

inline long operator-(const STimeMsec& A, const STimeMsec& B) { return A.time - B.time; }
inline STimeMsec operator+(const STimeMsec& A, long dt) { return STimeMsec(A.time, dt); }
inline STimeMsec operator+(const STimeMsec& A, float dt) { return STimeMsec(A.time, long(dt*1000)); }
inline STimeMsec operator+(const STimeMsec& A, double dt) { return STimeMsec(A.time, long(dt*1000)); }


