#ifndef __UMODSYS_TL_SCATTER_ARRAY_H
#define __UMODSYS_TL_SCATTER_ARRAY_H 1
/*************************************************************/
// file: umodsys/tl/composite/tree_rb.h
// info: scatter array type based on tree-rb
/*************************************************************/

#include <umodsys/tl/composite/tree_rb.h>

namespace UModSys {
namespace tl {

//***************************************

template<typename TData, typename TIndex=int>
struct TScatterArrayNode;

template<typename TData, typename TIndex=int, 
         typename TNodeDeleter = TNodeDeleter0< TScatterArrayNode<TData,TIndex> > >
struct TScatterArray;

template<typename TData, typename TIndex=int, 
         typename MemAlloc = core::SMemAlloc_Malloc >
struct TScatterArrayD1;

//***************************************

template<typename Data, typename Index>
struct TScatterArrayNode : public TTreeNodeRB< TScatterArrayNode<Data, Index> > {
  Index index;
  Data value;
  //
  inline TScatterArrayNode(Index id) : index(id) {}
  inline TScatterArrayNode(Index id, const Data& v) : index(id), value(v) {}
  inline ~TScatterArrayNode(void) {}
};

//***************************************

template<typename TData, typename TIndex, typename TNodeDeleter>
struct TScatterArray {
public:
  struct Cmp;
  struct Gen;
  struct Gen0;
  //
  typedef TData Value, Elem;
  typedef TIndex Index;
  typedef TScatterArrayNode<TData, TIndex> Node;
  typedef TNodeDeleter NodeDeleter;
  typedef TTreeHoldRBD<Node, NodeDeleter> Holder;
  //
  struct Cmp {
    const Index& index;
    //
    inline Cmp(const Index& x) : index(x) {}
    inline int operator()(const Node *r) const { return core::scalar_compare(index, r->index); }
  };
  //
  struct Gen {
    const Value &value;
    //
    inline Gen(const Value &d0) : value(d0) {}
    inline Node* operator()(const Cmp& c, const Holder& h) const 
      { return new(h.node_del.op_new()) Node(c.index, value); }
  };
  //
  struct Gen0 {
    inline Gen0(void) {}
    inline Node* operator()(const Cmp& c, const Holder& h) const 
      { return new(h.node_del.op_new()) Node(c.index); }
  };
  //
  template<typename X>
  struct NodeProc {
    const X& proc;
    //
    inline NodeProc(const X& p) : proc(p) {}
    inline bool operator()(Node* n) const { return proc(n->value); }
    inline bool operator()(const Node* n) const { return proc(n->value); }
  };
public:
  
  inline TScatterArray(void) {}
  inline TScatterArray(const NodeDeleter& nd) : hold(nd) {}
  inline ~TScatterArray(void) {}
  //
  inline size_t count(void) const { return hold.process_nodes(STreeProcCounter()); }
  inline size_t len(void) const { return count(); }
  inline size_t operator~(void) const { return count(); }
  //
  inline Value* operator()(const Index& index, const Value& v) {
    Node *n = hold.genx_node(Cmp(index), Gen(v));
    return n==NULL ? NULL : &n->value;
  }
  inline Value* operator[](const Index& index) {
    Node *n = hold.genx_node(Cmp(index), Gen0());
    return n==NULL ? NULL : &n->value;
  }
  //
  inline Value* operator[](const Index& index) const {
    Node *n = hold.find_node_u(Cmp(index));
    return n==NULL ? NULL : &n->value;
  }
  inline Value* operator()(const Index& index) const {
    Node *n = hold.find_node_u(Cmp(index));
    return n==NULL ? NULL : &n->value;
  }
  //
  inline bool remove(const Index& index) {
    Node *n = hold.find_node_u(Cmp(index));
    if(n==NULL) return false;
    hold.delete_node(n);
    return true;
  }
  inline bool clear(void) {
    hold.delete_all_nodes();
    return true;
  }
  //
  inline bool first(Index &index) const {
    Node *n = hold.min_node();
    if(n==NULL)
      return false;
    index = n->index;
    return true;
  }
  inline bool next(Index &index) const {
    Node *n = hold.first_node_u(Cmp(index+1));
    if(n==NULL)
      return false;
    index = n->index;
    return true;
  }
  //
  inline bool first(Index &index, const Value* &d) const {
    Node *n = hold.min_node();
    if(n==NULL)
      return false;
    index = n->index;
    d = &n->value;
    return true;
  }
  inline bool next(Index &index, const Value* &d) const {
    Node *n = hold.first_node_u(Cmp(index+1));
    if(n==NULL)
      return false;
    index = n->index;
    d = &n->value;
    return true;
  }
  //
  inline bool first(Index &index, Value* &d) const {
    Node *n = hold.min_node();
    if(n==NULL)
      return false;
    index = n->index;
    d = &n->value;
    return true;
  }
  inline bool next(Index &index, Value* &d) const {
    Node *n = hold.first_node_u(Cmp(index+1));
    if(n==NULL)
      return false;
    index = n->index;
    d = &n->value;
    return true;
  }
  //
  inline bool first(Index &index, Value &d) const {
    Node *n = hold.min_node();
    if(n==NULL)
      return false;
    index = n->index;
    d = n->value;
    return true;
  }
  inline bool next(Index &index, Value &d) const {
    Node *n = hold.first_node_u(Cmp(index+1));
    if(n==NULL)
      return false;
    index = n->index;
    d = n->value;
    return true;
  }
  //
  inline bool nonempty(void) const { return hold.nonempty(); }
  inline bool empty(void) const { return hold.empty(); }
  //
  template<typename tProcessor>
  inline int process_items(const tProcessor& proc) {
    return hold.process_nodes(NodeProc<tProcessor>(proc));
  }
protected:
  Holder hold;
};

//***************************************

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_SCATTER_ARRAY_H
