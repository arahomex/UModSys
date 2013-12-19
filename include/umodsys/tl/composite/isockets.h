#ifndef __UMODSYS_TL_COMPOSITE_ISOCKETS_H
#define __UMODSYS_TL_COMPOSITE_ISOCKETS_H 1
/*************************************************************/
// file: umodsys/tl/composite/isockets.h
// info: sockets to composite objects
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>

namespace UModSys {
namespace tl {

using core::array_index_none;

/*************************************************************/

template<typename SNode> struct TIStackSocket; // socket to connect to
template<typename SNode, typename Stack> struct TRStackSocket; // socket to connect to

/*************************************************************/

template<typename SNode> 
struct TIStackSocket {
public:
  virtual bool isempty(void) const =0;
  virtual bool isfull(void) const =0;
  virtual bool push(const SNode& item) =0;
  virtual bool pop(SNode& item) =0;
};

template<typename SNode, typename Stack> 
struct TRStackSocket : public TIStackSocket<SNode> {
public:
  Stack& refs;
  //
  inline TRStackSocket(Stack& r) : refs(r) {}
  inline ~TRStackSocket(void) {}
public:
  inline bool isempty(void) const { return refs.IsEmpty(); }
  inline bool isfull(void) const { return refs.IsFull(); }
  inline bool push(const SNode& item) { return refs.Push(item); }
  inline bool pop(SNode& item) { return refs.Pop(item); }
};

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_COMPOSITE_ISOCKETS_H
