#ifndef __UMODSYS_TYPES_TYPE_CONSTRUCTOR_H
#define __UMODSYS_TYPES_TYPE_CONSTRUCTOR_H 1
/*************************************************************/
// nanoutl/types/type_constructor.h: type constructor template
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>
#include <umodsys/tl/util/fast.h>

namespace UModSys {
namespace tl {

/*************************************************************/

template<typename Node>
struct TNodeDeleter0 {
  inline TNodeDeleter0(void) {}
  //
  int operator()(Node* node) const { delete node; return 1; }
  //
  static void* op_new(void) { return ::operator new(sizeof(Node)); }
  static void* op_new(size_t add) { return ::operator new(sizeof(Node)+add); }
};

template<typename Node, typename MemAlloc>
struct TNodeDeleter : public MemAlloc {
  const MemAlloc& mema;
  //
  inline TNodeDeleter(void) : mema(MemAlloc::get_default()) {}
  inline TNodeDeleter(const TNodeDeleter& R) : mema(R.mema) {}
  inline TNodeDeleter(const MemAlloc& aa) : mema(aa) {}
  static TNodeDeleter get_default(void) { return TNodeDeleter(); }
  //
  int operator()(Node* node) const { node->~Node(); mema.mem_free(node); return 1; }
  //
  void* op_new(void) const { return mema.mem_alloc(sizeof(Node)); }
  void* op_new(size_t add) const { return mema.mem_alloc(sizeof(Node)+add); }
};

template<typename Node, typename MemAlloc>
struct TNodeDeleterExtra : public MemAlloc {
  const MemAlloc& mema;
  size_t extra;
  //
  inline TNodeDeleterExtra(void) : mema(MemAlloc::get_default()) {}
  inline TNodeDeleterExtra(const MemAlloc& aa, size_t x) : mema(aa), extra(x) {}
  //
  int operator()(Node* node) const { node->~Node(); mema.mem_free(node); return 1; }
  //
  void* op_new(void) const { return mema.mem_alloc(sizeof(Node)+extra); }
  void* op_new(size_t add) const { return mema.mem_alloc(sizeof(Node)+add+extra); }
};

/*************************************************************/

template<typename T>
struct TTypeStaticHolder {
  typedef T Type;
  //
  T* pointer;
  char binary[sizeof(T)];
  bool finit;
  //
  inline TTypeStaticHolder(void) : finit(false) { pointer=reinterpret_cast<T*>(binary); }
  inline ~TTypeStaticHolder(void) { clear(); }
  //
  inline T* operator->(void) { return finit ? pointer : NULL; }
  inline const T* operator->(void) const { return finit ? pointer : NULL; }
  inline operator T*(void) { return finit ? pointer : NULL; }
  inline operator const T*(void) const { return finit ? pointer : NULL; }
  //
  inline bool valid(void) const { return finit; }
  inline bool invalid(void) const { return !finit; }
  //
  inline void _construct(void) { new(pointer) T(); }
  inline void _destruct(void) { pointer->~T(); }
  //
  inline void clear(void) { deinit(); }
  inline T* init(void) { if(!finit) { _construct(); finit=true; } return pointer; }
  inline void deinit(void) { if(finit) { _destruct(); finit=false; } }
};

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TYPES_TYPE_CONSTRUCTOR_H
