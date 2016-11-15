
//****************************************************************
// class TGC_NodeRoot
//****************************************************************

class TGC_NodeRoot : public TGC_Node {
public:
  TGC_NodeRoot(TGC_Master* gcm);
};

//****************************************************************
// template struct TGC_WeakRef
//****************************************************************

template<typename Node>
class TGC_WeakRef {
public:
  typedef TGC_WeakRef<Node> Self;
protected:
  TGC_RefBase bp;
  //
  inline void set(const TGC_RefBase& rr) {
    set(static_cast<Node*>(rr.ptr));
  }
  inline void init(const TGC_RefBase& rr) {
    bp.link(static_cast<Node*>(rr.ptr));
  }
  inline void init(Node *p) {
    bp.link(p);
  }
public:
  inline void clear(void) { 
    bp.unlink(); 
  }
  inline void set(Node* rr) {
    if(rr!=bp.ptr) {
      bp.unlink();
      bp.link(rr);
    }
  }
public:
  inline ~TGC_WeakRef(void) { clear(); }
  inline TGC_WeakRef(void) { bp.zero(); }
  //
  inline TGC_WeakRef(const Self& p) { init(p.bp); }
  inline const Self& operator=(const Self& p) { set(p.bp); return *this; }
  //
  inline TGC_WeakRef(Node* p) { init(p); }
  inline const Self& operator=(Node* p) { set(p); return *this; }
  //
  template<typename N2>
  inline TGC_WeakRef(const TGC_WeakRef<N2>& p) { init(p.get()); }
  template<typename N2>
  inline const Self& operator=(const TGC_WeakRef<N2>& p) { set(p.get()); return *this; }
  //
  inline Node* operator->(void) const { return static_cast<Node*>(bp.ptr); }
  inline operator Node*(void) const { return static_cast<Node*>(bp.ptr); }
  inline Node* get(void) const { return static_cast<Node*>(bp.ptr); }
  inline Node* operator()(void) const { return static_cast<Node*>(bp.ptr); }
  inline Node* operator~(void) const { return static_cast<Node*>(bp.ptr); }
};

//****************************************************************
// template struct TGC_Ref
//****************************************************************

template<typename Node>
class TGC_Ref {
public:
  typedef TGC_WeakRef<Node> Weak;
  typedef TGC_Ref<Node> Self;
public:
  TGC_RefBase bp;
  //
  inline void set(const TGC_RefBase& rr) {
    set(static_cast<Node*>(rr.ptr));
  }
  inline void init(const TGC_RefBase& rr) {
    bp.link_ref(static_cast<Node*>(rr.ptr));
  }
  inline void init(Node *p) {
    bp.link_ref(p);
  }
public:
  inline void clear(void) { 
    bp.unlink_ref(); 
  }
  inline void set(Node* rr) {
    if(rr!=bp.ptr) {
      bp.unlink_ref();
      bp.link_ref(rr);
    }
  }
public:
  inline ~TGC_Ref(void) { clear(); }
  inline TGC_Ref(void) { bp.zero(); }
  //
  inline TGC_Ref(const Self& p) { init(p.bp); }
  inline const Self& operator=(const Self& p) { set(p.bp); return *this; }
  //
  inline TGC_Ref(Node* p) { init(p); }
  inline const Self& operator=(Node* p) { set(p); return *this; }
  //
  template<typename N2>
  inline TGC_Ref(const TGC_Ref<N2>& p) { init(p.get()); }
  template<typename N2>
  inline const Self& operator=(const TGC_Ref<N2>& p) { set(p.get()); return *this; }
  //
  template<typename N2>
  inline TGC_Ref(const TGC_WeakRef<N2>& p) { init(p.get()); }
  template<typename N2>
  inline const Self& operator=(const TGC_WeakRef<N2>& p) { set(p.get()); return *this; }
  //
  inline Node* operator->(void) const { return static_cast<Node*>(bp.ptr); }
  inline operator Node*(void) const { return static_cast<Node*>(bp.ptr); }
  inline Node* get(void) const { return static_cast<Node*>(bp.ptr); }
  inline Node* operator()(void) const { return static_cast<Node*>(bp.ptr); }
  inline Node* operator~(void) const { return static_cast<Node*>(bp.ptr); }
};

//****************************************************************
// template struct TGC_Ptr
//****************************************************************

template<typename Node>
class TGC_Ptr {
public:
  typedef TGC_Ptr<Node> Self;
protected:
  Node* ptr;
  //
  inline void init(Node *p) {
    ptr = p;
    if(p!=NULL)
      p->gc_state.lock();
  }
public:
  inline void clear(void) { 
    if(ptr!=NULL) {
      ptr->gc_state.unlock();
      ptr = NULL;
    }
  }
  inline void set(Node* rr) {
    if(rr!=ptr) {
      if(rr!=NULL)
        rr->gc_state.lock();
      if(ptr!=NULL)
        ptr->gc_state.unlock();
      ptr = rr;
    }
  }
public:
  inline ~TGC_Ptr(void) { clear(); }
  inline TGC_Ptr(void) : ptr(NULL) {}
  //
  inline TGC_Ptr(const Self& p) { init(p.ptr); }
  inline const Self& operator=(const Self& p) { set(p.ptr); return *this; }
  //
  inline TGC_Ptr(Node* p) { init(p); }
  inline const Self& operator=(Node* p) { set(p); return *this; }
  //
  template<typename N2>
  inline TGC_Ptr(const TGC_Ptr<N2>& p) { init(p.get()); }
  template<typename N2>
  inline const Self& operator=(const TGC_Ptr<N2>& p) { set(p.get()); return *this; }
  //
  template<typename N2>
  inline TGC_Ptr(const TGC_Ref<N2>& p) { init(p.get()); }
  template<typename N2>
  inline const Self& operator=(const TGC_Ref<N2>& p) { set(p.get()); return *this; }
  //
  template<typename N2>
  inline TGC_Ptr(const TGC_WeakRef<N2>& p) { init(p.get()); }
  template<typename N2>
  inline const Self& operator=(const TGC_WeakRef<N2>& p) { set(p.get()); return *this; }
  //
  inline Node* operator->(void) const { return ptr; }
  inline operator Node*(void) const { return ptr; }
  inline Node* get(void) const { return ptr; }
  inline Node* operator()(void) const { return ptr; }
  inline Node* operator~(void) const { return ptr; }
};

//****************************************************************
// mixing struct TGC_ArrayPrecise
//****************************************************************

struct TGC_ArrayPrecise {
  static size_t calc_size(size_t n) { return n; }
  static void set_size(size_t n) {}
};

//****************************************************************
// mixing struct TGC_ArrayFast
//****************************************************************

struct TGC_ArrayFast {
  size_t allocated;
  //
  size_t calc_size(size_t n) const {
    if(allocated>=n)
      return allocated;
    if(n==0)
      return 0;
    if(n<8)
      return 8;
    size_t rv = allocated ? allocated : 8;
    while(rv<n) {
      rv *= 2;
    }
    return rv;
  }
  void set_size(size_t n) { allocated = n; }
};

//****************************************************************
// template struct TGC_ArrayZ
//****************************************************************

template<typename Elem, typename Mix>
class TGC_ArrayZ : public Mix {
public:
  typedef Elem Value;
  typedef TGC_ArrayZ<Elem> Self;
  typedef Value* iterator;
  typedef const Value* const_iterator;
protected:
  TGC_Master* master;
  Elem* values;
  size_t count;
private: // forbid copy
  void operator=(const TGC_ArrayZ&) {};
  TGC_ArrayZ(const TGC_ArrayZ&) {};
public:
  ~TGC_ArrayZ(void) {
    clear();
  }
  TGC_ArrayZ(TGC_Master* m, size_t sz=0) 
  : master(m), values(NULL), count(0) {
    Mix::set_size(0);
    if(sz) resize(sz);
  }
  TGC_ArrayZ(TGC_Master* m, const Value* p, size_t sz) 
  : master(m), values(NULL), count(0) {
    Mix::set_size(0);
    if(sz) clone(sz, p);
  }
  //
  void resize(size_t sz) {
    size_t nsz = Mix::calc_size(sz);
    values = reinterpret_cast<Value*>(master->az_realloc(values, sizeof(Value)*nsz ));
    count = sz; Mix::set_size(nsz);
z_log(log_GCUtil, lol_Debug, "values=%p count=%d\n", values, count);
  }
  void clone(size_t sz, const Value* p) {
    size_t nsz = Mix::calc_size(sz);
    values = reinterpret_cast<Value*>(master->az_realloc(values, sizeof(Value)*sz));
    count = sz; Mix::set_size(nsz);
    if(p!=NULL) 
      memcpy(values, p, sizeof(Value)*sz); 
z_log(log_GCUtil, lol_Debug, "values=%p count=%d\n", values, count);
  }
  void clear(void) {
z_log(log_GCUtil, lol_Debug, "values=%p count=%d\n", values, count);
    if(values!=NULL) {
      master->az_realloc(values, 0);
      values = NULL;
      count = 0;
      Mix::set_size(0);
    }
  }
  //
  size_t size(void) const { return count; }
  size_t operator~(void) const { return count; }
  //
  Value& get(size_t idx) const { return values[idx]; }
  Value& operator[](size_t idx) const { return values[idx]; }
  //
  Value& last(void) const { return values[count-1]; }
  size_t last_index(void) const { return count-1; }
  //
  Value* get(void) const { return values; }
  Value* operator()(void) const { return values; }
  //
  void push_back(const Value& v) { resize(count+1); values[count-1] = v; }
  void push_back(void) { resize(count+1); }
  ///
  size_t push(const Value& v) { resize(count+1); values[count-1] = v; return count-1; }
  Value& push(void) { resize(count+1); return values[count-1]; }
  void pop(void) { resize(count-1); }
  //
  template<typename It>
  void append(It a, It b) { 
    size_t pos = count;
    resize(count+(b-a));
    for(; a!=b; ++a) {
      values[pos++] = *a;
    }
  }
  template<typename It>
  void replace(It a, It b) { 
    size_t pos = 0;
    resize(b-a);
    for(; a!=b; ++a) {
      values[pos++] = *a;
    }
  }
  //
  template<typename Arr>
  void replace(const Arr& arr) {
    replace(arr.begin(), arr.end()); 
  }
  //
  void remove_at(size_t idx) {
    if(idx<count-1) {
      memmove(values+idx, values+idx+1, sizeof(Value)*(count-idx-1));
    }
    resize(count-1);
  }
  //
  iterator begin(void) const { return values; }
  iterator end(void) const { return values+count; }
  iterator cbegin(void) const { return values; }
  iterator cend(void) const { return values+count; }
public:
};

//****************************************************************
// template struct TGC_StringZ
//****************************************************************

template<typename Elem, typename Mix>
class TGC_StringZ : public TGC_ArrayZ<Elem> {
public:
  typedef Elem Char;
  typedef const Elem* CP;
  typedef TGC_ArrayZ<Elem> Super;
  typedef TGC_StringZ<Elem> Self;
public:
  TGC_StringZ(const Self& R) : Super(R.master, R.values, R.count) {}
  const Self& operator=(const Self& R) { clone(R.count, R.values); return *this; }
  //
  TGC_StringZ(TGC_Master* m, size_t sz=0) : Super(m, sz) {}
  TGC_StringZ(TGC_Master* m, CP p) : Super(m, p, p ? strlen(p)+1 : 0) {}
  TGC_StringZ(TGC_Master* m, CP p, size_t sz) : Super(m, p, sz) {}
  //
  inline const Self& operator=(const Char* p) { set(p); return *this; }
  inline void set(const Char* p) { Super::replace(p, p + (p ? strlen(p)+1 : 0)); }
  //
  const Elem* c_str(void) const { return Super::get(); }
  size_t len(void) const { return Super::count?Super::count-1:0; }
  size_t operator~(void) const { return len(); }
  //
  inline bool eq(const Char* text) const { 
    size_t l = strlen(text)+1; 
    if(l!=Super::count) return false; 
    return l==0 ? true : memcmp(Super::get(), text, sizeof(Char)*l)==0;
  }
};

//****************************************************************
// end
//****************************************************************
