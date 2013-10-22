#ifndef __UMODSYS_CORE_STDINLINES_H
#define __UMODSYS_CORE_STDINLINES_H 1
/*************************************************************/
// file: umodsys/core/stdinlines.h
// info: core inlines
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>
#include <umodsys/core/source.h>

namespace UModSys {
namespace core {

//***************************************
// Types
//***************************************

struct Void {};
struct Empty {};

template<typename T>
struct TScalarCompare {
  static int compare(const T& a, const T& b);
};

template<typename T>
struct TObjectCompare {
  static int compare(const T& a, const T& b);
};

//***************************************
// Inlines
//***************************************

inline Void* void_null(void) { return NULL; }
inline Void& void_obj(void) { return *void_null(); }

template<typename Func> inline void lost_func(Func fn) { lost((void*)fn); }

//***************************************

template<typename T>
inline void t_set_array1(T *a, T value, size_t size) 
{
  while(size--) {
    *a++ = value;
  }
}

template<typename T>
inline void t_set_array(T* arr, size_t N, const T& v) 
{
  while(N--) {
    *arr++ = v;
  }
}

//***************************************

template<typename T> inline int ptr_compare(T a, T b) { return (a>b) - (a<b); }
template<typename T> inline int scalar_compare(T a, T b) { return (a>b) - (a<b); }

template<typename T1, typename T2>
inline int any_ptr_compare_null(const T1* a, const T2* b) 
{ 
  bool ba = a!=NULL, bb = b!=NULL; 
  return (ba>bb) - (ba<bb); 
}

template<typename T> inline T scalar_min(T a, T b) { return a>b ? b : a; }
template<typename T> inline T scalar_max(T a, T b) { return a<b ? b : a; }

template<typename T> inline T scalar_min(T a, T b, T c) { return a>b ? scalar_min(b,c) : scalar_min(a,c); }
template<typename T> inline T scalar_max(T a, T b, T c) { return a<b ? scalar_min(b,c) : scalar_min(a,c); }

template<typename T> inline T scalar_min(T a, T b, T c, T d) { return a>b ? scalar_min(b,c,d) : scalar_min(a,c,d); }
template<typename T> inline T scalar_max(T a, T b, T c, T d) { return a<b ? scalar_min(b,c,d) : scalar_min(a,c,d); }

template<typename T> inline int TScalarCompare<T>::compare(const T& a, const T& b) { return scalar_compare(a, b); }
template<typename T> inline int TObjectCompare<T>::compare(const T& a, const T& b) { return a.compare(b); }

//***************************************

inline int scalar_compare(const SSourceContext &p1, const SSourceContext &p2)
{
  int rv = scalar_compare(p1.file, p2.file);
  if(rv!=0)
    return rv;
  rv = scalar_compare(p1.function, p2.function);
  if(rv!=0)
    return rv;
  return scalar_compare(p1.line, p2.line);
}

inline int scalar_compare(const SSourceContext *p1, const SSourceContext *p2)
{
  if(p1==NULL)
    return p2!=NULL ? -1 : 0;
  if(p2==NULL)
    return 1;
  return scalar_compare(*p1, *p2);
}

//***************************************
// END
//***************************************

} // namespace core
} // namespace UModSys

#endif // __UMODSYS_CORE_STDINLINES_H
