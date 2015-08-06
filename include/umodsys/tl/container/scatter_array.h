#ifndef __UMODSYS_TL_SCATTER_ARRAY_H
#define __UMODSYS_TL_SCATTER_ARRAY_H 1
/*************************************************************/
// file: umodsys/tl/container/tree_rb.h
// info: scatter array type based on tree-rb
/*************************************************************/

#include <umodsys/core/stdinlines.h>

#include <umodsys/tl/container/tree_rb.h>
#include <umodsys/tl/container/pair.h>
#include <umodsys/tl/alloc/allocator.h>
#include <umodsys/tl/util/node_deleter.h>
#include <umodsys/tl/metastl/throws.h>

namespace UModSys {
namespace tl {

//***************************************

template<typename TData, typename TIndex=int>
struct TScatterArrayNode;

template<typename TData, typename TIndex=int, 
         typename TComparer = core::TScalarCompare< TIndex >,
         typename TNodeDeleter = TNodeDeleter0< TScatterArrayNode<TData,TIndex> > >
struct TScatterArray;

template<typename TData, typename TIndex=int, 
         typename TComparer = core::TScalarCompare< TIndex >,
         typename MemAlloc = core::SIMemAlloc >
struct TScatterArrayD1;

//***************************************

template<typename Data, typename Index>
struct TScatterArrayNode 
: public TTreeNodeRB< TScatterArrayNode<Data, Index> >,
  public TPair<Index, Data>
{
  typedef TScatterArrayNode<Data, Index> Self;
  typedef TPair<Index, Data> Pair;
  //
  inline TScatterArrayNode(const Index& id) : Pair(id, Data()) {}
  inline TScatterArrayNode(const Index& id, const Data& v) : Pair(id, v) {}
  inline ~TScatterArrayNode(void) {}
};

//***************************************

template<typename TData, typename TIndex, typename Comparer, typename TNodeDeleter>
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
  typedef const TPairM<TIndex, TData> Pair;
  typedef const TPair<TIndex, TData> CPair;
  //
  // stl typedefs
  typedef Index key_type;
  typedef Value mapped_type;
  typedef Pair value_type;
  typedef Pair& reference;
  typedef const Pair& const_reference;
  typedef Pair* pointer;
  typedef const Pair* const_pointer;
//  typedef ptrdiff_t difference_type;
//  typedef size_t size_type;
  //
  typedef TTreeIterRB<Node, Pair> Iter;
  typedef TTreeIterRB<const Node, CPair> CIter;
  //
  typedef Iter iterator;
  typedef ReverseIterator<iterator, value_type, core::Void> reverse_iterator;
  typedef CIter const_iterator;
  typedef ReverseIterator<const_iterator, const value_type, core::Void> const_reverse_iterator;
  //
  static Node* None(void) UMODSYS_NOTHROW() { return NULL; }
  //
  struct Cmp {
    const Index& index;
    //
    inline Cmp(const Index& x) : index(x) {}
    inline int operator()(const Node *r) const { return Comparer::compare(index, r->first); }
  };
  //
  template<typename T>
  struct CmpT {
    const T& index;
    //
    inline CmpT(const T& x) : index(x) {}
    inline int operator()(const Node *r) const { return Comparer::compare(index, r->first); }
  };
  //
  struct Gen {
    const Value &value;
    //
    inline Gen(const Value &d0) : value(d0) {}
    inline Node* operator()(const Cmp& c, const Holder& h) const { return new(h.node_del.op_new()) Node(c.index, value); }
  };
  //
  struct Gen0 {
    inline Gen0(void) {}
    inline Node* operator()(const Cmp& c, const Holder& h) const { return new(h.node_del.op_new()) Node(c.index); }
  };
  //
  template<typename X>
  struct NodeProc {
    const X& proc;
    //
    inline NodeProc(const X& p) : proc(p) {}
    inline bool operator()(Node* n) const { return proc(n->second); }
    inline bool operator()(const Node* n) const { return proc(n->second); }
  };
public:
  
  inline TScatterArray(void) {}
  inline TScatterArray(const NodeDeleter& nd) : hold(nd) {}
  inline ~TScatterArray(void) {}
  //
  inline size_t Count(void) const { return hold.process_nodes(STreeProcCounter()); }
  inline size_t Len(void) const { return Count(); }
  inline size_t operator~(void) const { return Count(); }
  //
  inline Value* operator()(const Index& index, const Value& v) {
    Node *n = hold.genx_node(Cmp(index), Gen(v));
    return n==NULL ? NULL : &n->second;
  }
  inline Value* operator()(const Index& index, core::Void* v) {
    Node *n = hold.genx_node(Cmp(index), Gen0());
    return n==NULL ? NULL : &n->second;
  }
  inline Value* operator()(const Index& index) const {
    Node *n = hold.find_node_u(Cmp(index));
    return n==NULL ? NULL : &n->second;
  }
  //
  inline Value* Get(const Index& index) const {
    Node *n = hold.find_node_u(Cmp(index));
    return n==NULL ? NULL : &n->second;
  }
  template<typename T>
  inline Value* GetT(const T& index) const {
    Node *n = hold.find_node_u(CmpT<T>(index));
    return n==NULL ? NULL : &n->second;
  }
  //
  inline Iter operator()(void) { return hold.min_node(); }
  inline CIter operator()(void) const { return hold.min_node(); }
  //
  inline Iter First(void) { return hold.min_node(); }
  inline CIter First(void) const { return hold.min_node(); }
  //
  inline Iter Last(void) { return hold.max_node(); }
  inline CIter Last(void) const { return hold.max_node(); }
  //
  inline bool Remove(const Index& index) {
    Node *n = hold.find_node_u(Cmp(index));
    if(n==NULL) return false;
    hold.delete_node(n);
    return true;
  }
  inline bool Clear(void) {
    hold.delete_all_nodes();
    return true;
  }
  //
  inline bool First(Index &index) const {
    Node *n = hold.min_node();
    if(n==NULL)
      return false;
    index = n->index;
    return true;
  }
  inline bool Next(Index &index) const {
    Node *n = hold.first_node_u(Cmp(index+1));
    if(n==NULL)
      return false;
    index = n->index;
    return true;
  }
  //
  inline bool First(Index &index, const Value* &d) const {
    Node *n = hold.min_node();
    if(n==NULL)
      return false;
    index = n->index;
    d = &n->value;
    return true;
  }
  inline bool Next(Index &index, const Value* &d) const {
    Node *n = hold.first_node_u(Cmp(index+1));
    if(n==NULL)
      return false;
    index = n->index;
    d = &n->value;
    return true;
  }
  //
  inline bool First(Index &index, Value* &d) const {
    Node *n = hold.min_node();
    if(n==NULL)
      return false;
    index = n->index;
    d = &n->value;
    return true;
  }
  inline bool Next(Index &index, Value* &d) const {
    Node *n = hold.first_node_u(Cmp(index+1));
    if(n==NULL)
      return false;
    index = n->index;
    d = &n->value;
    return true;
  }
  //
  inline bool First(Index &index, Value &d) const {
    Node *n = hold.min_node();
    if(n==NULL)
      return false;
    index = n->index;
    d = n->value;
    return true;
  }
  inline bool Next(Index &index, Value &d) const {
    Node *n = hold.first_node_u(Cmp(index+1));
    if(n==NULL)
      return false;
    index = n->index;
    d = n->value;
    return true;
  }
  //
  inline bool NonEmpty(void) const { return hold.nonempty(); }
  inline bool Empty(void) const { return hold.empty(); }
  //
  template<typename tProcessor>
  inline int t_process_items(const tProcessor& proc) {
    return hold.process_nodes(NodeProc<tProcessor>(proc));
  }
public:
  // stl typedefs
  inline iterator begin(void) UMODSYS_NOTHROW() { return hold.min_node(); }
  inline iterator end(void) UMODSYS_NOTHROW() { return NULL; }
  inline const_iterator begin(void) const UMODSYS_NOTHROW() { return hold.min_node(); }
  inline const_iterator end(void) const UMODSYS_NOTHROW() { return NULL; }
  inline const_iterator cbegin(void) const UMODSYS_NOTHROW() { return hold.min_node(); }
  inline const_iterator cend(void) const UMODSYS_NOTHROW() { return NULL; }
  //
  inline reverse_iterator rbegin(void) UMODSYS_NOTHROW() { return reverse_iterator(hold.max_node()); }
  inline reverse_iterator rend(void) UMODSYS_NOTHROW() { return reverse_iterator(NULL); }
  inline const_reverse_iterator rbegin(void) const UMODSYS_NOTHROW() { return reverse_iterator(hold.max_node()); }
  inline const_reverse_iterator rend(void) const UMODSYS_NOTHROW() { return reverse_iterator(NULL); }
  inline const_reverse_iterator crbegin(void) const UMODSYS_NOTHROW() { return reverse_iterator(hold.max_node()); }
  inline const_reverse_iterator crend(void) const UMODSYS_NOTHROW() { return reverse_iterator(NULL); }
  //
  inline bool empty(void) const UMODSYS_NOTHROW() { return hold.empty(); }
  inline void clear(void) UMODSYS_NOTHROW() { hold.delete_all_nodes(); }
  //
  inline iterator find(const key_type& n) UMODSYS_NOTHROW() { return hold.find_node_u(Cmp(n)); }
  inline const_iterator find(const key_type& n) const UMODSYS_NOTHROW() { return hold.find_node_u(Cmp(n)); }
  //
//  inline void insert(iterator pos, const_reference v) { size_t p = pos - items; if(!InsertAt(p)) throw_memoryerror(UMODSYS_SOURCEINFO); items[p] = v; }
//  inline void insert(iterator position, const value_type& val) { size_t p = pos - items; if(!InsertAt(p, n)) throw_memoryerror(UMODSYS_SOURCEINFO); TC::acopy1(items+p, n, v); }
//  template<typename InputIterator> inline void insert(iterator pos, InputIterator first, InputIterator last) { size_t p = pos - items, n = last - first; if(!InsertAt(p, n)) throw_memoryerror(UMODSYS_SOURCEINFO); TC::atcopy(items+p, n, first); }
  //
  inline void erase(iterator pos) { hold.delete_node(pos.node); }
  inline void erase(const key_type& k) { Node *n = hold.find_node_u(Cmp(index)); if(n==NULL) throw_memoryerror(UMODSYS_SOURCEINFO); hold.delete_node(n); }
  inline void erase(iterator first, iterator last) { for(iterator x=first; x!=last;) { x++; hold.delete_node(first.node); first=x; } }
  //
  inline Value& operator[](const key_type& index) { Node *n = hold.genx_node(Cmp(index), Gen0()); if(n==NULL) throw_memoryerror(UMODSYS_SOURCEINFO); return n->second; }
  inline const Value& operator[](const key_type& index) const { Node *n = hold.find_node_u(Cmp(index)); if(n==NULL) throw_memoryerror(UMODSYS_SOURCEINFO); return n->second; }
  inline Value& at(const key_type& index) { Node *n = hold.find_node_u(Cmp(index)); if(n==NULL) throw_memoryerror(UMODSYS_SOURCEINFO); return n->second; }
  inline const Value& at(const key_type& index) const { Node *n = hold.find_node_u(Cmp(index)); if(n==NULL) throw_memoryerror(UMODSYS_SOURCEINFO); return n->second; }
protected:
  Holder hold;
};

//***************************************

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_SCATTER_ARRAY_H
