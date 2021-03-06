#ifndef __UMODSYS_TL_CONTAINER_LIST2_H
#define __UMODSYS_TL_CONTAINER_LIST2_H 1
/*************************************************************/
// file: umodsys/tl/container/list2.h
// info: list two way
/*************************************************************/

#include <umodsys/common/stdtypedefs.h>

namespace UModSys {
namespace tl {

//***************************************

template<typename SNode>
struct TList2Node;

//***************************************

template<typename SNode>
struct TList2Node {
public:
  SNode *next, *prev;
  //
  inline void delme(void) { next->prev = prev; prev->next = next; next = prev = NULL; }
  inline static void s_append(SNode *x, SNode* r) { SNode *f = r->next; x->next = f; x->prev = r; f->prev = x; r->next = x; }
  inline static void s_makeroot(SNode* me) { me->next = me->prev = me; }
  inline void clear(void) { next = prev = NULL; }
  inline bool isempty(void) const { return next==prev; }
  inline bool isnull(void) const { return next==NULL || prev==NULL; }
public:
  inline explicit TList2Node(core::Void* vv) {}
  inline TList2Node(void) : next(NULL), prev(NULL) {}
};

//***************************************

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_CONTAINER_LIST2_H
