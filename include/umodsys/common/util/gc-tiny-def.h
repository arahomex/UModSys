//****************************************************************
// typedefs
//****************************************************************

#if TGC_POINTER_SIZE == 4
//#error "Pointer size 4")
typedef uint8_t TGC_Generation;
typedef uint8_t TGC_Flags;
typedef uint16_t TGC_Locks;
#elif TGC_POINTER_SIZE == 8
//#error "Pointer size 8"
typedef uint16_t TGC_Generation;
typedef uint16_t TGC_Flags;
typedef uint32_t TGC_Locks;
#else
//#error "Pointer size ??"
typedef uint8_t TGC_Generation;
typedef uint8_t TGC_Flags;
typedef uint16_t TGC_Locks;
#endif

template<typename Node> class TGC_Ptr;
template<typename Node> class TGC_Ref;
template<typename Node> class TGC_WeakRef;

class TGC_Master;
class TGC_State;
class TGC_RefBase;
class TGC_Node;
class TGC_NodeRoot;

struct TGC_ArrayPrecise;
struct TGC_ArrayFast;

template<typename Elem, typename Mix=TGC_ArrayPrecise> class TGC_ArrayZ;
template<typename Elem=char, typename Mix=TGC_ArrayPrecise> class TGC_StringZ;

typedef TGC_StringZ<char> TGC_String;
typedef TGC_StringZ<char, TGC_ArrayFast> TGC_StringBuf;

//****************************************************************
// interface TGC_Master
//****************************************************************

class TGC_Master {
protected:
  static void gc_node_drop(TGC_Node* x);
  static void gc_node_link(TGC_Node* r, TGC_Node* x);
  static void gc_list_generation(TGC_Node* r, TGC_Generation generation);
  static void gc_list_generation_locked(TGC_Node* r, TGC_Generation generation);
private: // hide operator=
  TGC_Master(const TGC_Master& R);
  void operator=(const TGC_Master& R);
public:
  virtual void link(TGC_Node* x) = 0;
  virtual void unlink(TGC_Node* x) = 0;
  virtual void die(TGC_Node* x) = 0;
  virtual void* born_ptr(size_t s) = 0;
  //
  virtual void* az_realloc(void* az_old, size_t sz_new) = 0;
  virtual size_t az_size(void* az) = 0;
  //
  virtual void collect(void) = 0;
public:
  virtual ~TGC_Master(void) {}
  TGC_Master(void) {}
  //
  TGC_State s(void);
  TGC_State a(void);
  TGC_State f(void);
  //
  operator TGC_State(void);
  TGC_State operator*(void);
  //
  template<typename XNode>
   TGC_Ptr<XNode> born(void);
  template<typename XNode, typename A1>
   TGC_Ptr<XNode> born(const A1& a1);
  template<typename XNode, typename A1, typename A2>
   TGC_Ptr<XNode> born(const A1& a1, const A2& a2);
  template<typename XNode, typename A1, typename A2, typename A3>
   TGC_Ptr<XNode> born(const A1& a1, const A2& a2, const A3& a3);
  template<typename XNode, typename A1, typename A2, typename A3, typename A4>
   TGC_Ptr<XNode> born(const A1& a1, const A2& a2, const A3& a3, const A4& a4);
  template<typename XNode, typename A1, typename A2, typename A3, typename A4, typename A5>
   TGC_Ptr<XNode> born(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5);
  //
  template<typename XNode>
   TGC_Ptr<XNode> bornx(size_t extra);
  template<typename XNode, typename A1>
   TGC_Ptr<XNode> bornx(size_t extra, const A1& a1);
  template<typename XNode, typename A1, typename A2>
   TGC_Ptr<XNode> bornx(size_t extra, const A1& a1, const A2& a2);
  template<typename XNode, typename A1, typename A2, typename A3>
   TGC_Ptr<XNode> bornx(size_t extra, const A1& a1, const A2& a2, const A3& a3);
  template<typename XNode, typename A1, typename A2, typename A3, typename A4>
   TGC_Ptr<XNode> bornx(size_t extra, const A1& a1, const A2& a2, const A3& a3, const A4& a4);
  template<typename XNode, typename A1, typename A2, typename A3, typename A4, typename A5>
   TGC_Ptr<XNode> bornx(size_t extra, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5);
};

//****************************************************************
// structure TGC_State
//****************************************************************

class TGC_State {
public:
  enum eFlags {
    fStatic = 0x01,
    fFree   = 0x02,
    fRoot   = 0x06,
    fAuto   = 0x00
  };
public:
  mutable TGC_Flags flags;
  mutable TGC_Generation generation;
  mutable TGC_Locks locks;
  TGC_Master* master;
public:
  TGC_State(TGC_Master* m, TGC_Flags ff=fAuto) 
  : master(m), generation(0), flags(ff), locks(0) {}
  //
  inline bool is_free(void) const { return (flags & fFree)!=0; }
  inline bool is_static(void) const { return (flags & fStatic)!=0; }
  inline bool is_gen(TGC_Generation g) const { return g==generation; }
  //
  inline void set_gen(TGC_Generation g) const { generation = g; }
  inline void link(TGC_Node* x) { if(!is_free()) master->link(x); }
  inline void unlink(TGC_Node* x) { if(is_static()) master->unlink(x); }
  //
  inline void lock(void) const { ++locks; }
  inline void unlock(void) const { --locks; }
  //
  inline TGC_Master* operator->(void) const { return master; }
  inline operator TGC_Master*(void) const { return master; }
};

//****************************************************************
// struct TGC_RefBase
//****************************************************************

struct TGC_RefBase {
protected:
  inline void operator=(const TGC_RefBase& r) {
//fprintf(stderr, "--@%p op= [ptr:%p %p %p]\n", this, ptr, next, prev);
    ptr = r.ptr;
    next = r.next;
    prev = r.prev;
//fprintf(stderr, "--@%p now [ptr:%p %p %p]\n", this, ptr, next, prev);
  }
public:
  TGC_Node *ptr;
  TGC_RefBase *next, *prev;
  //
  void loop(void);
  void zero(void);
  void unlink(void);
  void link(TGC_Node *p, TGC_RefBase* r);
  void unlink_ref(void);
  void link_ref(TGC_Node *p, TGC_RefBase* r);
  //
  void link(TGC_Node *p);
  void link_ref(TGC_Node *p);
  void free_chain(void);
  void dump_state(const char* key);
};

//****************************************************************
// class TGC_Node
//****************************************************************

class TGC_Node {
public:
  TGC_State gc_state;
  TGC_Node *gc_next, *gc_prev;
  TGC_RefBase gc_refroot;
public:
  static void gc_alive_p(const TGC_Node* n, TGC_Generation gen);
//  static void gc_alive_p(const TGC_Node* n, TGC_Generation gen);
  template<typename N> static void gc_alive_pa(N n[], size_t sz, TGC_Generation gen);
  template<typename A> static void gc_alive_pa(const A& arr, TGC_Generation gen);
  template<typename N> static void gc_alive_xa(N n[], size_t sz, TGC_Generation gen);
  template<typename A> static void gc_alive_xa(const A& arr, TGC_Generation gen);
  //
  template<typename N> static void gc_clear_p(N* &n);
  template<typename N> static void gc_clear_pa(N* n[], size_t sz);
  template<typename A> static void gc_clear_pa(A& arr);
  template<typename A> static void gc_clear_a(A& arr);
private: // hide operator=
  TGC_Node(const TGC_Node& R);
  void operator=(const TGC_Node& R);
public:
  inline void gc_init(void) {}
  virtual void gc_clear(void);
  virtual void gc_alive(TGC_Generation generation) const;
  virtual ~TGC_Node(void);
  TGC_Node(TGC_State st);
  TGC_Node(TGC_State s, TGC_NodeRoot* rr);
};


//****************************************************************
// end
//****************************************************************
