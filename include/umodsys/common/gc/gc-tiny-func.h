
//****************************************************************
// AC_RefBase::
//****************************************************************

inline void TGC_RefBase::link(TGC_Node *p)
{
  if(p==NULL) zero(); else link(p, &p->gc_refroot);
}

inline void TGC_RefBase::link_ref(TGC_Node *p)
{
  if(p==NULL) zero(); else link_ref(p, &p->gc_refroot);
}

inline void TGC_RefBase::loop(void) 
{
//  dump_state("+loop");
  ptr = NULL;
  next = prev = this;
  dump_state("-loop");
}

inline void TGC_RefBase::zero(void) 
{ 
//  dump_state("+zero");
  ptr = NULL;
  next = prev = NULL; 
  dump_state("-zero");
}

inline void TGC_RefBase::unlink(void) 
{
  dump_state("+unlink");
  if(ptr!=NULL) {
    next->prev = prev;
    prev->next = next;
    zero();
  }
  dump_state("-unlink");
}

inline void TGC_RefBase::link(TGC_Node *p, TGC_RefBase* r) 
{
//  dump_state("+link");
  r->dump_state("+link-base");
  ptr = p;
  prev = r;
  next = r->next;
  r->next->prev = this;
  r->next = this;
  r->dump_state("-link-base");
  dump_state("-link");
}

inline void TGC_RefBase::unlink_ref(void) 
{
  dump_state("+unlink_ref");
  if(ptr!=NULL) {
    ptr->gc_state.unlock();
    next->prev = prev;
    prev->next = next;
    zero();
  }
  dump_state("-unlink_ref");
}

inline void TGC_RefBase::link_ref(TGC_Node *p, TGC_RefBase* r) 
{
//  dump_state("+link_ref");
  r->dump_state("+link-base");
  ptr = p;
  p->gc_state.lock();
  prev = r;
  next = r->next;
  r->next->prev = this;
  r->next = this;
  r->dump_state("-link-base");
  dump_state("-link_ref");
}


//****************************************************************
// TGC_Master::
//****************************************************************

inline TGC_State TGC_Master::a(void) 
{ 
  return TGC_State(this, TGC_State::fAuto); 
}

inline TGC_State TGC_Master::s(void) 
{ 
  return TGC_State(this, TGC_State::fStatic); 
}

inline TGC_State TGC_Master::f(void) 
{ 
  return TGC_State(this, TGC_State::fFree); 
}

inline TGC_Master::operator TGC_State(void)
{
  return TGC_State(this, TGC_State::fStatic); 
}

inline TGC_State TGC_Master::operator*(void)
{
  return TGC_State(this, TGC_State::fStatic); 
}

inline void TGC_Master::gc_node_drop(TGC_Node* x) 
{
  x->gc_next->gc_prev = x->gc_prev;
  x->gc_prev->gc_next = x->gc_next;
  x->gc_next = x->gc_prev = NULL;
}

inline void TGC_Master::gc_node_link(TGC_Node* r, TGC_Node* x)
{
  x->gc_prev = r;
  x->gc_next = r->gc_next;
  r->gc_next->gc_prev = x;
  r->gc_next = x;
}

//****************************************************************

template<typename XNode>
inline TGC_Ptr<XNode> TGC_Master::born(void) 
{
  TGC_Ptr<XNode> rv ( new(born_ptr(sizeof(XNode))) XNode(a()) );
  rv->gc_init();
  return rv;
}

template<typename XNode, typename A1>
inline TGC_Ptr<XNode> TGC_Master::born(const A1& a1) 
{
  TGC_Ptr<XNode> rv ( new(born_ptr(sizeof(XNode))) XNode(a()) );
  rv->gc_init(a1);
  return rv;
}

template<typename XNode, typename A1, typename A2>
inline TGC_Ptr<XNode> TGC_Master::born(const A1& a1, const A2& a2) 
{
  TGC_Ptr<XNode> rv ( new(born_ptr(sizeof(XNode))) XNode(a()) );
  rv->gc_init(a1, a2);
  return rv;
}

template<typename XNode, typename A1, typename A2, typename A3>
inline TGC_Ptr<XNode> TGC_Master::born(const A1& a1, const A2& a2, const A3& a3) 
{
  TGC_Ptr<XNode> rv ( new(born_ptr(sizeof(XNode))) XNode(a()) );
  rv->gc_init(a1, a2, a3);
  return rv;
}

template<typename XNode, typename A1, typename A2, typename A3, typename A4>
inline TGC_Ptr<XNode> TGC_Master::born(const A1& a1, const A2& a2, const A3& a3, const A4& a4) 
{
  TGC_Ptr<XNode> rv ( new(born_ptr(sizeof(XNode))) XNode(a()) );
  rv->gc_init(a1, a2, a3, a4);
  return rv;
}

template<typename XNode, typename A1, typename A2, typename A3, typename A4, typename A5>
inline TGC_Ptr<XNode> TGC_Master::born(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5) 
{
  TGC_Ptr<XNode> rv ( new(born_ptr(sizeof(XNode))) XNode(a()) );
  rv->gc_init(a1, a2, a3, a4, a5);
  return rv;
}

//****************************************************************

template<typename XNode>
inline TGC_Ptr<XNode> TGC_Master::bornx(size_t x) 
{
  TGC_Ptr<XNode> rv ( new(born_ptr(sizeof(XNode)+x)) XNode(a()) );
  rv->gc_init();
  return rv;
}

template<typename XNode, typename A1>
inline TGC_Ptr<XNode> TGC_Master::bornx(size_t x, const A1& a1) 
{
  TGC_Ptr<XNode> rv ( new(born_ptr(sizeof(XNode)+x)) XNode(a()) );
  rv->gc_init(a1);
  return rv;
}

template<typename XNode, typename A1, typename A2>
inline TGC_Ptr<XNode> TGC_Master::bornx(size_t x, const A1& a1, const A2& a2) 
{
  TGC_Ptr<XNode> rv ( new(born_ptr(sizeof(XNode)+x)) XNode(a()) );
  rv->gc_init(a1, a2);
  return rv;
}

template<typename XNode, typename A1, typename A2, typename A3>
inline TGC_Ptr<XNode> TGC_Master::bornx(size_t x, const A1& a1, const A2& a2, const A3& a3) 
{
  TGC_Ptr<XNode> rv ( new(born_ptr(sizeof(XNode)+x)) XNode(a()) );
  rv->gc_init(a1, a2, a3);
  return rv;
}

template<typename XNode, typename A1, typename A2, typename A3, typename A4>
inline TGC_Ptr<XNode> TGC_Master::bornx(size_t x, const A1& a1, const A2& a2, const A3& a3, const A4& a4) 
{
  TGC_Ptr<XNode> rv ( new(born_ptr(sizeof(XNode)+x)) XNode(a()) );
  rv->gc_init(a1, a2, a3, a4);
  return rv;
}

template<typename XNode, typename A1, typename A2, typename A3, typename A4, typename A5>
inline TGC_Ptr<XNode> TGC_Master::bornx(size_t x, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5) 
{
  TGC_Ptr<XNode> rv ( new(born_ptr(sizeof(XNode)+x)) XNode(a()) );
  rv->gc_init(a1, a2, a3, a4, a5);
  return rv;
}

//****************************************************************
// TGC_Node::
//****************************************************************

inline void TGC_Node::gc_alive_p(const TGC_Node* n, TGC_Generation gen) 
{
  if(n!=NULL && !n->gc_state.is_gen(gen)) {
    n->gc_state.set_gen(gen);
    n->gc_alive(gen); 
  }
}

template<typename N>
inline void TGC_Node::gc_alive_pa(N n[], size_t sz, TGC_Generation gen) 
{
  for(size_t k=0; k<sz; k++) {
    gc_alive_p(n[k], gen);
  }
}

template<typename A>
inline void TGC_Node::gc_alive_pa(const A& arr, TGC_Generation gen) 
{
  for(typename A::const_iterator x=arr.begin(), e=arr.end(); x!=e; ++x) {
    gc_alive_p(*x, gen);
  }
}

template<typename N> 
inline void TGC_Node::gc_alive_xa(N n[], size_t sz, TGC_Generation gen)
{
  for(size_t k=0; k<sz; k++) {
    n[k].gc_alive(gen);
  }
}

template<typename A>
inline void TGC_Node::gc_alive_xa(const A& arr, TGC_Generation gen) 
{
  for(typename A::const_iterator x=arr.begin(), e=arr.end(); x!=e; ++x) {
    (*x).gc_alive(gen);
  }
}

template<typename N> inline void TGC_Node::gc_clear_p(N* &n)
{
  n = NULL;
}

template<typename N> 
inline void TGC_Node::gc_clear_pa(N* n[], size_t sz)
{
  if(sz)
    memset(n, 0, sizeof(N*)*sz);
}

template<typename A>
inline void TGC_Node::gc_clear_pa(A& arr)
{
  for(typename A::iterator x=arr.begin(), e=arr.end(); x!=e; ++x) {
    gc_clear_p(*x);
  }
}

template<typename A>
inline void TGC_Node::gc_clear_a(A& arr)
{
  arr.clear();
}

//****************************************************************
// shared inlines
//****************************************************************

template<typename T> 
inline unsigned push(TGC_ArrayZ<T>& a, const T& v) 
{ 
  a.push_back(v); return a.size()-1; 
}

template<typename T> 
size_t find(const TGC_ArrayZ<T>& a, const T& v) 
{ 
  for(size_t i=0, n=~a; i<n; ++i) {
    if(a[i] == v)
      return i;
  }
  return size_t(-1);
}

//****************************************************************
// end
//****************************************************************
