#ifndef __NANOUTL_TYPES_RES_LOCK_H
#define __NANOUTL_TYPES_RES_LOCK_H 1
//*****************************************************
// umodsys/tl/util/res_lock.h: Resource lock
//*****************************************************

#include <umodsys/common/stdtypedefs.h>

namespace UModSys {
namespace tl {

template<typename Res> struct TResLock;

template<typename Res>
struct TResLock<Res*> {
public:
  typedef Res* DRes;
public:
  DRes resource;
public:
  TResLock(DRes r) : resource(r) { resource->res_lock(); }
  ~TResLock(void) { resource->res_unlock(); }
  Res& operator()(void) const { return *resource; }
  Res& operator*(void) const { return *resource; }
  Res* operator->(void) const { return *resource; }
protected:
  TResLock(void);
  TResLock(const TResLock<Res>& R);
  void operator=(const TResLock<Res>& R);
};

template<typename Res>
struct TResLock<const Res> {
public:
  typedef const Res DRes;
public:
  DRes resource;
public:
  TResLock(DRes r) : resource(r) { resource.res_lock(); }
  ~TResLock(void) { resource.res_unlock(); }
  DRes& operator()(void) { return resource; }
  DRes& operator*(void) { return resource; }
  DRes* operator->(void) { return &resource; }
protected:
  TResLock(void);
  TResLock(const TResLock<Res>& R);
  void operator=(const TResLock<Res>& R);
};


} // namespace tl
} // namespace UModSys

#endif // __NANOUTL_TYPES_RES_LOCK_H
