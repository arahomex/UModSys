#ifndef __UMODSYS_TL_UTIL_NODEDELETER_H
#define __UMODSYS_TL_UTIL_NODEDELETER_H 1
/*************************************************************/
// file: umodsys/tl/util/node_deleter.h
// info: fixed array of elements
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>

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
  int operator()(Node* node) const { node->~Node(); mema.mem_free(node, UMODSYS_SOURCEINFO); return 1; }
  //
  void* op_new(void) const { return mema.mem_alloc(sizeof(Node), UMODSYS_SOURCEINFO); }
  void* op_new(size_t add) const { return mema.mem_alloc(sizeof(Node)+add, UMODSYS_SOURCEINFO); }
};

template<typename Node, typename MemAlloc>
struct TNodeDeleterExtra : public MemAlloc {
  const MemAlloc& mema;
  size_t extra;
  //
  inline TNodeDeleterExtra(void) : mema(MemAlloc::get_default()) {}
  inline TNodeDeleterExtra(const MemAlloc& aa, size_t x) : mema(aa), extra(x) {}
  //
  int operator()(Node* node) const { node->~Node(); mema.mem_free(node, UMODSYS_SOURCEINFO); return 1; }
  //
  void* op_new(void) const { return mema.mem_alloc(sizeof(Node)+extra, UMODSYS_SOURCEINFO); }
  void* op_new(size_t add) const { return mema.mem_alloc(sizeof(Node)+add+extra, UMODSYS_SOURCEINFO); }
};

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_UTIL_NODEDELETER_H
