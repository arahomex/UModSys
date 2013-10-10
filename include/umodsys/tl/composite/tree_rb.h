#ifndef __UMODSYS_TL_TREE_RB_H
#define __UMODSYS_TL_TREE_RB_H 1
/*************************************************************/
// file: umodsys/tl/composite/tree_rb.h
// info: Tree red-black
/*************************************************************/

#include <umodsys/core/stdcore.h>

#include <umodsys/tl/util/node_deleter.h>
#include <umodsys/tl/alloc/allocator.h>

namespace UModSys {
namespace tl {

//***************************************

template<typename SNode>
struct TTreeNodeRB;

template<typename Node>
struct TTreeHoldRB;

template<typename Node>
struct TNodeDeleter0;

template<typename Node, typename MemAlloc=core::SMemAlloc_Malloc>
struct TNodeDeleter;

template<typename Node, typename MemAlloc=core::SMemAlloc_Malloc>
struct TNodeDeleterExtra;

template<typename Node, typename NodeDeleter=TNodeDeleter0<Node> >
struct TTreeHoldRBD;

template<typename Node>
struct TTreeIterRB;

//***************************************
//***************************************

struct STreeNodeColor {
  enum RBColor {
    _node_black = 0,
    _node_red = 1
  };
  //
  char _node_color, _node_flag1, _node_flag2, _node_flag3;
};

//***************************************

struct STreeProcCounter {
  inline STreeProcCounter(void) {}
  //
  template<typename tNode>
  inline int operator()(tNode *node) const { return 1; }
};

//***************************************

template<typename SNode>
struct TTreeNodeRB : public STreeNodeColor {
public:
  typedef TTreeNodeRB<SNode> NodeBase;
  typedef SNode Node;
public:
  SNode *_node_left, *_node_right, *_node_parent;
  //
  static SNode* node_get_next(SNode* x);
  static SNode* node_get_prev(SNode* x);
  static const SNode* node_get_next(const SNode* x);
  static const SNode* node_get_prev(const SNode* x);
  //
public:
  inline TTreeNodeRB(void) : _node_left(NULL), _node_right(NULL), _node_parent(NULL) { _node_color = _node_black; }
};

//***************************************

template<typename tNode>
struct TTreeHoldRB {
protected:
  tNode *root;
  //
  inline void rotate_left(tNode *n) {
    tNode *tmp = n->_node_right;
    if((n->_node_right=tmp->_node_left)!=NULL) {
      tmp->_node_left->_node_parent = n;
    }
    tmp->_node_left = n;
    if((tmp->_node_parent=n->_node_parent)!=NULL) {
      (n==n->_node_parent->_node_left ? n->_node_parent->_node_left : n->_node_parent->_node_right) = tmp;
    } else {
      root = tmp;
    }
    n->_node_parent = tmp;
  }
  //
  inline void rotate_right(tNode *n) {
    tNode *tmp = n->_node_left;
    if((n->_node_left=tmp->_node_right)!=NULL) {
      tmp->_node_right->_node_parent = n;
    }
    tmp->_node_right = n;
    if((tmp->_node_parent=n->_node_parent)!=NULL) {
      (n==n->_node_parent->_node_left ? n->_node_parent->_node_left : n->_node_parent->_node_right) = tmp;
    } else {
      root = tmp;
    }
    n->_node_parent = tmp;
  }
  //
  void insert_color(tNode *n);
  void remove_color(tNode *p, tNode *n);
  //
  template<typename tProcessor>
  static int process_nodes_r(tNode *n, const tProcessor& proc) {
    int rv = 0;
    if(n->_node_left)
      rv += process_nodes_r(n->_node_left, proc);
    tNode *r = n->_node_right;
    rv += proc(n);
    if(r)
      rv += process_nodes_r(r, proc);
    return rv;
  }
  template<typename tCmp, typename tProcessor>
  static int process_filter_r(const tCmp &cmp, tNode *n, const tProcessor& proc) {
    int rv = 0;
    int c = cmp(n);
    if(c<=0 && n->_node_left)
      rv += process_filter_r(cmp, n->_node_left, proc);
    tNode *r = n->_node_right;
    if(c==0)
      rv += proc(n);
    if(c>=0 && r)
      rv += process_filter_r(cmp, r, proc);
    return rv;
  }
  //
  template<typename tProcessor>
  static int process_cnodes_r(const tNode *n, const tProcessor& proc) {
    int rv = 0;
    if(n->_node_left)
      rv += process_cnodes_r(n->_node_left, proc);
    const tNode *r = n->_node_right;
    rv += proc(n);
    if(r)
      rv += process_cnodes_r(r, proc);
    return rv;
  }
  template<typename tCmp, typename tProcessor>
  static int process_cfilter_r(const tCmp &cmp, const tNode *n, const tProcessor& proc) {
    int rv = 0;
    int c = cmp(n);
    if(c<=0 && n->_node_left)
      rv += process_cfilter_r(cmp, n->_node_left, proc);
    const tNode *r = n->_node_right;
    if(c==0)
      rv += proc(n);
    if(c>=0 && r)
      rv += process_cfilter_r(cmp, r, proc);
    return rv;
  }
public:
  inline TTreeHoldRB(void) : root(NULL) {}
  //
  inline bool nonempty(void) const { return root!=NULL; }
  inline bool empty(void) const { return root==NULL; }
  inline void clear_all_nodes(void) { root=NULL; }
  //
  template<typename tProcessor>
  inline int process_nodes(const tProcessor& proc) {
    return root ? process_nodes_r(root, proc) : 0;
  }
  template<typename tCmp, typename tProcessor>
  inline int process_filter(const tCmp &cmp, const tProcessor& proc) {
    return root ? process_filter_r(cmp, root, proc) : 0;
  }
  //
  template<typename tProcessor>
  inline int process_nodes(const tProcessor& proc) const {
    return root ? process_cnodes_r(root, proc) : 0;
  }
  template<typename tCmp, typename tProcessor>
  inline int process_filter(const tCmp &cmp, const tProcessor& proc) const {
    return root ? process_cfilter_r(cmp, root, proc) : 0;
  }
  //
  template<typename tCmp, typename tProcessor>
  inline int process_filter_seq(const tCmp &cmp, const tProcessor& proc) {
    tNode *x = min_node(), *y;
    int num = 0;
    while(x) {
      y = next_node(x);
      if(cmp(x)) {
        num += proc(x);
      }
      x = y;
    }
    return num;
  }
  //
  template<typename tCmp, typename tProcessor>
  inline int remove_filter_seq(const tCmp &cmp, const tProcessor& postproc) {
    tNode *x = min_node(), *y;
    int num = 0;
    while(x) {
      y = next_node(x);
      if(cmp(x)) {
        remove_node(x);
        postproc(x);
        num++;
      }
      x = y;
    }
    return num;
  }
  //
  template<typename tCmp, typename tProcessor>
  inline int remove_filterc_seq(const tCmp &cmp, const tProcessor& postproc) {
    tNode *x = min_node(), *y;
    int num = 0;
    while(x) {
      y = next_node(x);
      if(cmp(x)==0) {
        remove_node(x);
        postproc(x);
        num++;
      }
      x = y;
    }
    return num;
  }
  //
  template<typename tProcessor>
  inline int remove_seq(tProcessor postproc) {
    tNode *x = min_node(), *y;
    int num = 0;
    while(x) {
      y = next_node(x);
      remove_node(x);
      postproc(x);
      num++;
      x = y;
    }
    return num;
  }
  //
  inline void remove_node(tNode *n) {
    tNode *c, *p, *old, *tmp;
    STreeNodeColor::RBColor clr;
    if(n->_node_left==NULL) {
      c = n->_node_right;
    } else if(n->_node_right==NULL){
      c = n->_node_left;
    } else {
      old = n;
      n = n->_node_right;
      while((tmp=n->_node_left)!=NULL)
        n = tmp;
      c = n->_node_right;
      p = n->_node_parent;
      clr = STreeNodeColor::RBColor(n->_node_color);
      if(c)
        c->_node_parent = p;
      if(p) {
        (p->_node_left==n ? p->_node_left : p->_node_right) = c;
      } else {
        root = c;
      }
      if(n->_node_parent==old)
        p = n;
      n->_node_parent = old->_node_parent;
      n->_node_left = old->_node_left;
      n->_node_right = old->_node_right;
      n->_node_color = old->_node_color;
      if(old->_node_parent) {
        (old->_node_parent->_node_left==old ? old->_node_parent->_node_left : old->_node_parent->_node_right) = n;
      } else {
        root = n;
      }
      old->_node_left->_node_parent = n;
      if(old->_node_right)
        old->_node_right->_node_parent = n;
      if(clr==tNode::_node_black)
        remove_color(p, c);
      return;
    }
    //
    p = n->_node_parent;
    clr = STreeNodeColor::RBColor(n->_node_color);
    if(c)
      c->_node_parent = p;
    if(p) {
      (p->_node_left==n ? p->_node_left : p->_node_right) = c;
    } else {
      root = c;
    }
    if(clr==tNode::_node_black)
      remove_color(p, c);
  }
  //
  inline void sub_insert_node(tNode *p, tNode *n, int compare) {
    n->_node_parent = p;
    n->_node_color = tNode::_node_red;
    //
    if(p!=NULL) {
      if(compare<0) {
        n->_node_right = NULL;
        n->_node_left = p->_node_left;
        p->_node_left = n;
        if(n->_node_left!=NULL)
          n->_node_left->_node_parent = n;
      } else {
        n->_node_left = NULL;
        n->_node_right = p->_node_right;
        p->_node_right = n;
        if(n->_node_right!=NULL)
          n->_node_right->_node_parent = n;
      }
    } else {
      root = n;
      n->_node_left = n->_node_right = NULL;
    }
    insert_color(n);
  }
  //
  // ---------------- Insert existing
  //
  template<typename tCmp>
  tNode* insert_node_u(tNode *n, tCmp cmp) {
    tNode *tmp, *p = NULL;
    int compare = 0;
    tmp = root;
    while(tmp) {
      p = tmp;
      compare = cmp(n, p);
      if(compare<0)
        tmp = tmp->_node_left;
      else if(compare>0)
        tmp = tmp->_node_right;
      else
        return tmp; // dup found
    }
    //
    sub_insert_node(p, n, compare);
    return NULL; // insert node ok
  }
  //
  template<typename tCmp>
  tNode* insert_node_m(tNode *n, tCmp cmp) {
    tNode *tmp, *p = NULL;
    int compare = 0;
    tmp = root;
    while(tmp) {
      p = tmp;
      compare = cmp(n, p);
      if(compare<0)
        tmp = tmp->_node_left;
      else if(compare>0)
        tmp = tmp->_node_right;
      else
        break; // insert at any dup found
    }
    //
    sub_insert_node(p, n, compare);
    return NULL; // insert node ok
  }
  //
  // ---------------- Generate new node
  //
  template<typename tCmp, typename tGen>
  tNode* gen_node(tCmp cmp, const tGen& gen, tNode** oldn=NULL, tNode **newn=NULL, bool multi=false) {
    tNode *tmp, *p = NULL;
    int compare = 0;
    tmp = root;
    if(oldn!=NULL)
      *oldn = NULL;
    while(tmp) {
      p = tmp;
      compare = cmp(p);
      if(compare<0)
        tmp = tmp->_node_left;
      else if(compare>0)
        tmp = tmp->_node_right;
      else { // dup found
        if(oldn!=NULL)
          *oldn = tmp;
        if(!multi) {
          if(newn!=NULL)
            *newn = NULL;
          return tmp; 
        }
      }
    }
    //
    tNode* n = gen(cmp); // generate node
    if(n==NULL)
      return NULL; // ok to insert NULL
    if(newn!=NULL)
      *newn = n;
    //
    sub_insert_node(p, n, compare);
    return n; // insert node ok
  }
  //
  // ---------------- Find node
  //
  template<typename tCmp>
  tNode* find_node(tNode *n, tCmp cmp) const {
    tNode *tmp = root, *p;
    while(tmp) {
      p = tmp;
      int compare = cmp(n, p);
      if(compare<0)
        tmp = tmp->_node_left;
      else if(compare>0)
        tmp = tmp->_node_right;
      else
        return tmp; // found
    }
    return NULL; // not found
  }
  //
  template<typename tCmp>
  tNode* find_node_u(tCmp cmp) const {
    tNode *tmp = root, *p;
    while(tmp) {
      p = tmp;
      int compare = cmp(p);
      if(compare<0)
        tmp = tmp->_node_left;
      else if(compare>0)
        tmp = tmp->_node_right;
      else
        return tmp; // found
    }
    return NULL; // not found
  }
  //
  inline static tNode* next_node(tNode *n) { return tNode::node_get_next(n); }
  inline static tNode* prev_node(tNode *n) { return tNode::node_get_prev(n); }
  inline static const tNode* next_node(const tNode *n) { return tNode::node_get_next(n); }
  inline static const tNode* prev_node(const tNode *n) { return tNode::node_get_prev(n); }
  //
  tNode* min_node(void) const {
    tNode *tmp = root;
    tNode *p = NULL;
    while(tmp) {
      p = tmp;
      tmp = tmp->_node_left;
    }
    return p;
  }
  //
  tNode* max_node(void) const {
    tNode *tmp = root;
    tNode *p = NULL;
    while(tmp) {
      p = tmp;
      tmp = tmp->_node_right;
    }
    return p;
  }
  //
  template<typename tCmp>
  tNode* find_node_minu(tCmp cmp) const {
    tNode *rv = find_node_u(cmp);
    if(rv==NULL)
      return NULL; // no found
    do {
      tNode *tmp = prev_node(rv);
      if(!tmp || cmp(tmp))
        return rv;
      rv = tmp;
    } while(true);
    return NULL;
  }
  //
  template<typename tCmp>
  tNode* prefirst_node_u(tCmp cmp) const { // < value
    if(root==NULL)
      return NULL; // empty list
    tNode *tmp = root, *p;
    while(tmp) {
      p = tmp;
      int compare = cmp(p);
      if(compare<0)
        tmp = tmp->_node_left;
      else if(compare>0)
        tmp = tmp->_node_right;
      else
        break; // found
    }
    if(tmp!=NULL || cmp(p)<0) { // scan for minimal one if found GE value
      for(;;) {
        tmp = prev_node(p);
        if(!tmp || cmp(tmp)>0)
          break;
        p = tmp;
      }
      return tmp; // minimal node
    }
    return p; // no nodes at _node_right
  }
  //
  template<typename tCmp>
  tNode* first_node_u(tCmp cmp) const { // <= value
    if(root==NULL)
      return NULL; // empty list
    tNode *tmp = root, *p;
    while(tmp) {
      p = tmp;
      int compare = cmp(p);
      if(compare<0)
        tmp = tmp->_node_left;
      else if(compare>0)
        tmp = tmp->_node_right;
      else
        break; // found
    }
    if(tmp!=NULL || cmp(p)<0) { // scan for minimal one if found GE value
      for(;;) {
        tmp = prev_node(p);
        if(!tmp || cmp(tmp)>0)
          break;
        p = tmp;
      }
      return p; // minimal node
    } else {
      for(;;) {
        tmp = next_node(p);
        if(!tmp || cmp(tmp)<=0)
          break;
        p = tmp;
      }
      return tmp; // minimal node
    }
  }
};


//***************************************
// TTreeNodeRB::
//***************************************

template<typename SNode>
inline SNode* TTreeNodeRB<SNode>::node_get_next(SNode* n)
{
  if(n->_node_right) {
          /* If we have a _node_right-hand child, go down and then _node_left as far as we can. */
    n = n->_node_right;
    while(n->_node_left)
      n = n->_node_left;
  } else {
    /* No _node_right-hand children.  Everything down and _node_left is
             smaller than us, so any 'next' node must be in the general
             direction of our _node_parent. Go up the tree; any time the
             ancestor is a _node_right-hand child of its _node_parent, keep going
             up. First time it's a _node_left-hand child of its _node_parent, said
             _node_parent is our 'next' node. */
    while (n->_node_parent && n == n->_node_parent->_node_right)
                  n = n->_node_parent;
    n = n->_node_parent;
  }
  return n;
}

template<typename SNode>
inline const SNode* TTreeNodeRB<SNode>::node_get_next(const SNode* n)
{
  if(n->_node_right) {
          /* If we have a _node_right-hand child, go down and then _node_left as far as we can. */
    n = n->_node_right;
    while(n->_node_left)
      n = n->_node_left;
  } else {
    /* No _node_right-hand children.  Everything down and _node_left is
             smaller than us, so any 'next' node must be in the general
             direction of our _node_parent. Go up the tree; any time the
             ancestor is a _node_right-hand child of its _node_parent, keep going
             up. First time it's a _node_left-hand child of its _node_parent, said
             _node_parent is our 'next' node. */
    while (n->_node_parent && n == n->_node_parent->_node_right)
                  n = n->_node_parent;
    n = n->_node_parent;
  }
  return n;
}

template<typename SNode>
inline SNode* TTreeNodeRB<SNode>::node_get_prev(SNode* n) 
{
  if(n->_node_left) {
    /* If we have a _node_left-hand child, go down and then _node_right as far as we can. */
    n = n->_node_left;
    while(n->_node_right)
      n = n->_node_right;
  } else {
    /* No _node_left-hand children. Go up till we find an ancestor which
       is a _node_right-hand child of its _node_parent */
    while (n->_node_parent && n == n->_node_parent->_node_left)
                  n = n->_node_parent;
    n = n->_node_parent;
  }
  return n;
}

template<typename SNode>
inline const SNode* TTreeNodeRB<SNode>::node_get_prev(const SNode* n)
{
  if(n->_node_left) {
    /* If we have a _node_left-hand child, go down and then _node_right as far as we can. */
    n = n->_node_left;
    while(n->_node_right)
      n = n->_node_right;
  } else {
    /* No _node_left-hand children. Go up till we find an ancestor which
       is a _node_right-hand child of its _node_parent */
    while (n->_node_parent && n == n->_node_parent->_node_left)
                  n = n->_node_parent;
    n = n->_node_parent;
  }
  return n;
}

//***************************************
// TTreeHoldRB::
//***************************************

template<typename tNode>
void TTreeHoldRB<tNode>::insert_color(tNode *n) {
    tNode *p, *gp, *tmp;
    while((p=n->_node_parent) && p->_node_color==tNode::_node_red) {
      gp = p->_node_parent;
      if(p==gp->_node_left) {
        tmp = gp->_node_right;
        if(tmp && tmp->_node_color==tNode::_node_red) {
          tmp->_node_color = tNode::_node_black;
          p->_node_color = tNode::_node_black;
          gp->_node_color = tNode::_node_red;
          n = gp;
          continue;
        }
        if(p->_node_right==n) {
          rotate_left(p);
          tmp = p;
          p = n;
          n = tmp;
        }
        p->_node_color = tNode::_node_black;
        gp->_node_color = tNode::_node_red;
        rotate_right(gp);
      } else {
        tmp = gp->_node_left;
        if(tmp && tmp->_node_color==tNode::_node_red) {
          tmp->_node_color = tNode::_node_black;
          p->_node_color = tNode::_node_black;
          gp->_node_color = tNode::_node_red;
          n = gp;
          continue;
        }
        if(p->_node_left==n) {
          rotate_right(p);
          tmp = p;
          p = n;
          n = tmp;
        }
        p->_node_color = tNode::_node_black;
        gp->_node_color = tNode::_node_red;
        rotate_left(gp);
      }
    }
    root->_node_color = tNode::_node_black;
}

//***************************************

template<typename tNode>
void TTreeHoldRB<tNode>::remove_color(tNode *p, tNode *n) {
    tNode *tmp, *tmp2;
    while((n==NULL || n->_node_color==tNode::_node_black) && n!=root) {
      if(p->_node_left==n) {
        tmp = p->_node_right;
        if(tmp->_node_color==tNode::_node_red) {
          tmp->_node_color = tNode::_node_black;
          p->_node_color = tNode::_node_red;
          rotate_left(p);
          tmp = p->_node_right;
        }
        if( (tmp->_node_left==NULL || tmp->_node_left->_node_color==tNode::_node_black)
         && (tmp->_node_right==NULL || tmp->_node_right->_node_color==tNode::_node_black)) {
          tmp->_node_color = tNode::_node_red;
          n = p;
          p = n->_node_parent;
        } else {
          if(tmp->_node_right==NULL || tmp->_node_right->_node_color==tNode::_node_black) {
            tmp2 = tmp->_node_left;
            if(tmp2)
              tmp2->_node_color = tNode::_node_black;
            tmp->_node_color = tNode::_node_red;
            rotate_right(tmp);
            tmp = p->_node_right;
          }
          tmp->_node_color = p->_node_color;
          p->_node_color = tNode::_node_black;
          if(tmp->_node_right)
            tmp->_node_right->_node_color = tNode::_node_black;
          rotate_left(p);
          n = root;
          break;
        }
      } else {
        tmp = p->_node_left;
        if(tmp->_node_color==tNode::_node_red) {
          tmp->_node_color = tNode::_node_black;
          p->_node_color = tNode::_node_red;
          rotate_right(p);
          tmp = p->_node_left;
        }
        if( (tmp->_node_left==NULL || tmp->_node_left->_node_color==tNode::_node_black)
         && (tmp->_node_right==NULL || tmp->_node_right->_node_color==tNode::_node_black)) {
          tmp->_node_color = tNode::_node_red;
          n = p;
          p = n->_node_parent;
        } else {
          if(tmp->_node_left==NULL || tmp->_node_left->_node_color==tNode::_node_black) {
            tmp2 = tmp->_node_right;
            if(tmp2)
              tmp2->_node_color = tNode::_node_black;
            tmp->_node_color = tNode::_node_red;
            rotate_left(tmp);
            tmp = p->_node_left;
          }
          tmp->_node_color = p->_node_color;
          p->_node_color = tNode::_node_black;
          if(tmp->_node_left)
            tmp->_node_left->_node_color = tNode::_node_black;
          rotate_right(p);
          n = root;
          break;
        }
      }
    }
    if(n)
      n->_node_color = tNode::_node_black;
}

//***************************************

template<typename Node, typename NodeDeleter>
struct TTreeHoldRBD : public TTreeHoldRB<Node> {
public:
  typedef TTreeHoldRB<Node> Hold;
  //
  NodeDeleter node_del;
public:
  inline ~TTreeHoldRBD(void) { clear(); Hold::root = NULL; }
  TTreeHoldRBD(void);
  TTreeHoldRBD(const NodeDeleter& nd);
  //
  inline void clear(void) { process_nodes(node_del); Hold::root=NULL; }
  inline void delete_all_nodes(void) { process_nodes(node_del); Hold::root=NULL; }
  inline void delete_node(Node *n) { remove_node(n); node_del(n); }
  //
  template<typename tCmp>
  inline void delete_filterc(const tCmp &cmp) { remove_filterc_seq(cmp, node_del); }
  //
  template<typename tCmp>
  inline void delete_filter(const tCmp &cmp) { remove_filter_seq(cmp, node_del); }
  //
  template<typename tCmp, typename tGen>
  Node* genx_node(tCmp cmp, const tGen& gen, Node** oldn=NULL, Node **newn=NULL, bool multi=false) {
    Node *tmp, *p = NULL;
    int compare = 0;
    tmp = Hold::root;
    if(oldn!=NULL)
      *oldn = NULL;
    while(tmp) {
      p = tmp;
      compare = cmp(p);
      if(compare<0)
        tmp = tmp->_node_left;
      else if(compare>0)
        tmp = tmp->_node_right;
      else { // dup found
        if(oldn!=NULL)
          *oldn = tmp;
        if(!multi) {
          if(newn!=NULL)
            *newn = NULL;
          return tmp; 
        }
      }
    }
    //
    Node* n = gen(cmp, *this); // generate node
    if(n==NULL)
      return NULL; // ok to insert NULL
    if(newn!=NULL)
      *newn = n;
    //
    sub_insert_node(p, n, compare);
    return n; // insert node ok
  }
};

template<typename Node, typename NodeDeleter>
inline TTreeHoldRBD<Node,NodeDeleter>::TTreeHoldRBD(void) {}

template<typename Node, typename NodeDeleter>
inline TTreeHoldRBD<Node,NodeDeleter>::TTreeHoldRBD(const NodeDeleter& nd) 
: TTreeHoldRB<Node>(), node_del(nd) {}

//***************************************

template<typename TNode>
struct TTreeIterRB {
public:
  typedef TNode Node;
  typedef TTreeHoldRB<Node> Hold;
protected:
  Node* node;
public:
  //
  inline Node* operator->(void) const { return node; }
  inline Node* operator()(void) const { return node; }
  inline Node& operator*(void) const { return *node; }
  inline bool valid(void) const { return node!=NULL; }
  inline Node& ref(void) const { return *node; }
  inline Node* ptr(void) const { return node; }
  //
  inline void next(void) { if(node!=NULL) node = Hold::next_node(node); }
  inline void operator++(void) { next(); }
  inline void operator++(int) { next(); }
  //
  inline void prev(void) { if(node!=NULL) node = Hold::prev_node(node); }
  inline void operator--(void) { prev(); }
  inline void operator--(int) { prev(); }
  //
  inline TTreeIterRB(Node* n=NULL) : node(n) {}
  inline TTreeIterRB(const Hold& h, bool left=true) : node(left ? h.min_node() : h.max_node()) {}
};

//***************************************

} // namespace tl
} // namespace UModSys

#endif // __UMODSYS_TL_TREE_RB_H
