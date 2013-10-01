#ifndef __UMODSYS_TL_QUANT_ARRAY_H
#define __UMODSYS_TL_QUANT_ARRAY_H 1
/*************************************************************/
// file: umodsys/tl/composite/quant_array.h
// info: array of elements with quant allocation
/*************************************************************/

#include <umodsys/tl/composite/tree_rb.h>

namespace UModSys {
namespace tl {

//***************************************

template<typename TData, size_t Quant,
         typename TMemAlloc = core::SMemAlloc_Malloc >
struct TQuantArray;

template<typename TData, size_t Quant>
struct TQuantArrayNode;

//***************************************

template<typename Data, size_t Quant>
struct TQuantArrayNode : public TTreeNodeRB< TQuantArrayNode<Data, Quant> > {
  size_t used;
  Data elems[Quant];
  //
  inline TQuantArrayNode(void) : used(0) {}
  inline ~TQuantArrayNode(void) {}
};

//***************************************

template<typename TData, size_t Quant, typename TMemAlloc>
struct TQuantArrayUnique {
public:
  enum { quant = Quant };
  //
  typedef TData Value, Elem;
  typedef TQuantArrayNode<TData, Quant> Node;
  typedef tl::TNodeDeleter< Node, TMemAlloc > NodeDeleter;
  typedef TTreeHoldRBD<Node, NodeDeleter> Holder;
  //
public:
  inline TQuantArrayUnique(void) : cur_size(0) {}
  inline TQuantArrayUnique(const TMemAlloc& ma) : hold(NodeDeleter(ma)), cur_size(0) {}
  inline ~TQuantArrayUnique(void) {}
  //
  inline size_t count(void) const { return cur_size; }
  inline size_t len(void) const { return cur_size; }
  inline size_t operator~(void) const { return cur_size; }
  //
  const TData* find(const TData& val);
  TData* add(const TData& val);
  bool remove(const TData* it);
  //
  inline bool nonempty(void) const { return hold.nonempty(); }
  inline bool empty(void) const { return hold.empty(); }
protected:
  Holder hold;
  size_t cur_size;
};


template<typename TData, size_t Quant, typename TMemAlloc>
const TData* TQuantArrayUnique<TData, Quant, TMemAlloc>::find(const TData& val)
{
  TQuantArrayNode<TData, Quant> *x;
  for(x=hold.min_node(); x!=NULL; x=hold.next_node(x)) {
    for(size_t i=0; i<x->used; i++) {
      if(core::scalar_compare(x->elems[i], val)==0)
        return &x->elems[i];
    }
  }
  return NULL;
}

template<typename TData, size_t Quant, typename TMemAlloc>
TData* TQuantArrayUnique<TData, Quant, TMemAlloc>::add(const TData& val)
{
  TQuantArrayNode<TData, Quant> *x;
  for(x=hold.min_node(); x!=NULL; x=hold.next_node(x)) {
    if(x->used==Quant)
      continue;
    TData* rv = &x->elems[x->used++];
    *rv = val;
    return rv;
  }
  x = new(hold.node_del.op_new()) TQuantArrayNode<TData, Quant>();
  if(x==NULL)
    return NULL;
  TData* rv = &x->elems[x->used++];
  *rv = val;
  return rv;
}

template<typename TData, size_t Quant, typename TMemAlloc>
bool TQuantArrayUnique<TData, Quant, TMemAlloc>::remove(const TData* it)
{
  return false;
}

//***************************************

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_QUANT_ARRAY_H
