#ifndef __UMODSYS_TL_CONTAINER_ISOCKETS_H
#define __UMODSYS_TL_CONTAINER_ISOCKETS_H 1
/*************************************************************/
// file: umodsys/tl/container/isockets.h
// info: sockets to composite objects
/*************************************************************/

#include <umodsys/core/stdtypedefs.h>

namespace UModSys {
namespace tl {

using core::array_index_none;

/*************************************************************/

template<typename SNode> struct TIStackSocket; // socket to connect to
template<typename SNode, typename Stack> struct TRStackSocket; // socket to connect to

template<typename SNode> struct TIArraySocket; // socket to connect to
template<typename SNode, typename Array> struct TRArraySocket; // socket to connect to

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

template<typename SNode> 
struct TIArraySocket {
public:
  virtual size_t getcount(void) const =0;
  virtual const SNode& getelem(size_t idx) const =0;
  virtual SNode& getelem(size_t idx) =0;
  //
  inline size_t operator~(void) const { return getcount(); }
  inline const SNode& operator[](size_t idx) const { return getelem(idx); }
  inline SNode& operator[](size_t idx) { return getelem(idx); }
  inline const SNode& operator()(size_t idx) const { return getelem(idx); }
  inline SNode& operator()(size_t idx) { return getelem(idx); }
};

template<typename SNode, typename Array> 
struct TRArraySocket : public TIArraySocket<SNode> {
public:
  Array& refs;
  //
  inline TRArraySocket(Array& r) : refs(r) {}
  inline ~TRArraySocket(void) {}
public:
  inline size_t getcount(void) const { return ~refs; }
  inline const SNode& getelem(size_t idx) const { return refs(idx); }
  inline SNode& getelem(size_t idx) { return refs(idx); }
};

/*************************************************************/

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_CONTAINER_ISOCKETS_H
