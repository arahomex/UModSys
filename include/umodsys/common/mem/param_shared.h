#ifndef __UMS_CORE_MEDIA_PARAMETERS_TYPE2_H
#define __UMS_CORE_MEDIA_PARAMETERS_TYPE2_H 1
//*****************************************************
// ums/common/media/parameter_shared.h: universal memory parameter
//*****************************************************

namespace UMS {
namespace core {
namespace media {

/*
struct SFixedParameters;
struct SFixedParametersData;
struct SFixedParametersBase;
struct SFixedParametersBaseData;
*/
/*
template<typename Data>
struct SFixedParameterRelPtr;

typedef SFixedParameterRelPtr<BWStr>      SFixedParameterRelPtr_Str;
typedef SFixedParameterRelPtr<int*>          SFixedParameterRelPtr_Int;
typedef SFixedParameterRelPtr<double*>       SFixedParameterRelPtr_Double;
typedef SFixedParameterRelPtr<IRefObject**>  SFixedParameterRelPtr_Object;
*/

//***************************************

struct IParametersShared : public core::IRefObject {
  // SFixedBaseParameters
  virtual void p_deinit(SFixedParametersBaseData *p) =0;
  virtual void p_init(SFixedParametersBaseData *p, int defallocn, int defallocv) =0;
  virtual void p_init(SFixedParametersBaseData *p, const SFixedParametersBaseData& r) =0;
  virtual void p_copy(SFixedParametersBaseData *p, const SFixedParametersBaseData& r) =0;
  //
  virtual int  add(SFixedParametersBaseData *p, BCStr name, BCStr value, int maxlen, int arrlen) =0;
  virtual int  add(SFixedParametersBaseData *p, BCStr name, int value, int arrlen) =0;
  virtual int  add(SFixedParametersBaseData *p, BCStr name, const double &value, int arrlen) =0;
  virtual int  add(SFixedParametersBaseData *p, BCStr name, IRefObject *value, int arrlen) =0;
  //
  virtual int  addv(SFixedParametersBaseData *p, BCStr name, const BCStr *value, int maxlen, int arrlen) =0;
  virtual int  addv(SFixedParametersBaseData *p, BCStr name, const int *value, int arrlen) =0;
  virtual int  addv(SFixedParametersBaseData *p, BCStr name, const double *value, int arrlen) =0;
  virtual int  addv(SFixedParametersBaseData *p, BCStr name, IRefObject* const* value, int arrlen) =0;
  //
  virtual int  get_rp_int(const SFixedParametersBaseData *p, BCStr name) =0;
  virtual int  get_rp_double(const SFixedParametersBaseData *p, BCStr name) =0;
  virtual int  get_rp_str(const SFixedParametersBaseData *p, BCStr name) =0;
  virtual int  get_rp_object(const SFixedParametersBaseData *p, BCStr name) =0;
  //
  virtual bool ready(SFixedParametersBaseData *p) =0;
  //
  // SFixedParametersData
  virtual void p_deinit(SFixedParametersData *p) =0;
  virtual void p_init(SFixedParametersData *p, SFixedParametersBaseData* pp) =0;
  virtual void p_init(SFixedParametersData *p, const SFixedParametersData& r) =0;
  virtual void p_copy(SFixedParametersData *p, const SFixedParametersData& r) =0;
  //
  virtual bool set(const SFixedParametersData *p, BCStr name, BCStr value, int si) =0;
  virtual bool set(const SFixedParametersData *p, BCStr name, int value, int si) =0;
  virtual bool set(const SFixedParametersData *p, BCStr name, const double &value, int si) =0;
  virtual bool set(const SFixedParametersData *p, BCStr name, IRefObject *value, int si) =0;
  //
  virtual bool get(const SFixedParametersData *p, BCStr name, BCStr &value, int si) =0;
  virtual bool get(const SFixedParametersData *p, BCStr name, int &value, int si) =0;
  virtual bool get(const SFixedParametersData *p, BCStr name, double &value, int si) =0;
  virtual bool get(const SFixedParametersData *p, BCStr name, IRefObject* &value, int si) =0;
  //
  virtual bool set(const SFixedParametersData *p, int h, BCStr value, int si) =0;
  virtual bool set(const SFixedParametersData *p, int h, int value, int si) =0;
  virtual bool set(const SFixedParametersData *p, int h, const double &value, int si) =0;
  virtual bool set(const SFixedParametersData *p, int h, IRefObject *value, int si) =0;
  //
  virtual bool get(const SFixedParametersData *p, int h, BCStr &value, int si) =0;
  virtual bool get(const SFixedParametersData *p, int h, int &value, int si) =0;
  virtual bool get(const SFixedParametersData *p, int h, double &value, int si) =0;
  virtual bool get(const SFixedParametersData *p, int h, IRefObject* &value, int si) =0;
  //
  virtual int*          get_p_int(const SFixedParametersData *p, int h, int si) =0;
  virtual double*       get_p_double(const SFixedParametersData *p, int h, int si) =0;
  virtual IRefObject**  get_p_object(const SFixedParametersData *p, int h, int si) =0;
  virtual BWStr         get_p_str(const SFixedParametersData *p, int h, int si) =0;
  virtual int           get_size_str(const SFixedParametersData *p, int h) =0;
  virtual int           get_count(const SFixedParametersData *p, int h) =0;
  //
  virtual int*          get_p_int(const SFixedParametersData *p, BCStr name, int si) =0;
  virtual double*       get_p_double(const SFixedParametersData *p, BCStr name, int si) =0;
  virtual IRefObject**  get_p_object(const SFixedParametersData *p, BCStr name, int si) =0;
  virtual BWStr      get_p_str(const SFixedParametersData *p, BCStr name, int si) =0;
  virtual int           get_size_str(const SFixedParametersData *p, BCStr name) =0;
  virtual int           get_count(const SFixedParametersData *p, BCStr name) =0;
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::core::IParametersShared, 2, IRefObject);
};

//***************************************

template<typename Data>
struct SFixedParameterRelPtr {
  //
  explicit SFixedParameterRelPtr(int s=-1) {
    offs = s;
  }
  //
  void assign(int v) { offs = v; }
  void operator=(int v) { offs = v; }
  void clear(void) { offs = -1; }
  bool valid(void) const { return offs!=-1; }
  //
  Data operator()(const SFixedParameters& fp) const;
  Data get(const SFixedParameters& fp) const;
protected:
  int offs;
};

//***************************************

struct SFixedParametersBaseData {
  IParameters* worker;
  SMemChunk mem_registry;
  SMemChunk mem_init;
  int mem_used, last, rc_count;
  //
  inline SFixedParametersBaseData(void) : worker(NULL), last(0), rc_count(0) {}
};

struct SFixedParametersBase {
  inline ~SFixedParametersBase(void)
    { if(data.worker) data.worker->p_deinit(&data); }
  inline SFixedParametersBase(int defallocn=1024, int defallocv=1024)
    { get_paramworker()->p_init(&data, defallocn, defallocv); }
  inline SFixedParametersBase(const SFixedParametersBase& r)
    { if(r.data.worker) r.data.worker->p_init(&data, r.data); }
  inline const SFixedParametersBase& operator=(const SFixedParametersBase& r)
    { data.worker->p_copy(&data, r.data); return *this; }
  //
  inline int add(BCStr name, BCStr value, int maxv, int arr=1)
    { return data.worker ? data.worker->add(&data, name, value, maxv, arr) : -1; }
  inline int add(BCStr name, int value, int arr=1)
    { return data.worker ? data.worker->add(&data, name, value, arr) : -1; }
  inline int add(BCStr name, const double &value, int arr=1)
    { return data.worker ? data.worker->add(&data, name, value, arr) : -1; }
  inline int add(BCStr name, IRefObject *value, int arr=1)
    { return data.worker ? data.worker->add(&data, name, value, arr) : -1; }
  template<typename IRef> inline int add(BCStr name, const TRefObject<IRef>& value, int arr=1) 
    { return data.worker ? data.worker->add(&data, name, value(), arr) : -1; }
  //
  inline int addv(BCStr name, const BCStr *value, int maxv, int arr=1)
    { return data.worker ? data.worker->addv(&data, name, value, maxv, arr) : -1; }
  inline int addv(BCStr name, const int *value, int arr=1)
    { return data.worker ? data.worker->addv(&data, name, value, arr) : -1; }
  inline int addv(BCStr name, const double *value, int arr=1)
    { return data.worker ? data.worker->addv(&data, name, value, arr) : -1; }
  inline int addv(BCStr name, IRefObject* const* value, int arr=1)
    { return data.worker ? data.worker->addv(&data, name, value, arr) : -1; }
  template<typename IRef> inline int addv(BCStr name, const TRefObject<IRef>& value, int arr=1) 
    { return data.worker ? data.worker->addv(&data, name, value(), arr) : -1; }
  //
  inline SFixedParameterRelPtr_Str get_rp_str(BCStr name)
    { return SFixedParameterRelPtr_Str(data.worker ? data.worker->get_rp_str(&data, name) : -1); }
  inline SFixedParameterRelPtr_Int get_rp_int(BCStr name)
    { return SFixedParameterRelPtr_Int(data.worker ? data.worker->get_rp_int(&data, name) : -1); }
  inline SFixedParameterRelPtr_Double get_rp_double(BCStr name)
    { return SFixedParameterRelPtr_Double(data.worker ? data.worker->get_rp_double(&data, name) : -1); }
  inline SFixedParameterRelPtr_Object get_rp_object(BCStr name)
    { return SFixedParameterRelPtr_Object(data.worker ? data.worker->get_rp_object(&data, name) : -1); }
  //
  inline bool add(SFixedParameterRelPtr_Str& rv, BCStr name, BCStr value, int maxv, int arr=1) {
    if(data.worker) {
      data.worker->add(&data, name, value, maxv, arr);
      rv = data.worker->get_rp_str(&data, name);
      return rv.valid();
    }
    rv.clear(); return false;
  }
  inline bool add(SFixedParameterRelPtr_Int& rv, BCStr name, int value, int arr=1) {
    if(data.worker) {
      data.worker->add(&data, name, value, arr);
      rv = data.worker->get_rp_int(&data, name);
      return rv.valid();
    }
    rv.clear(); return false;
  }
  inline int add(SFixedParameterRelPtr_Double& rv, BCStr name, const double &value, int arr=1) {
    if(data.worker) {
      data.worker->add(&data, name, value, arr);
      rv = data.worker->get_rp_double(&data, name);
      return rv.valid();
    }
    rv.clear(); return false;
  }
  inline bool add(SFixedParameterRelPtr_Object& rv, BCStr name, IRefObject *value, int arr=1) {
    if(data.worker) {
      data.worker->add(&data, name, value, arr);
      rv = data.worker->get_rp_object(&data, name);
      return rv.valid();
    }
    rv.clear(); return false;
  }
  //
  inline bool addv(SFixedParameterRelPtr_Int& rv, BCStr name, const int *value, int arr=1) {
    if(data.worker) {
      data.worker->addv(&data, name, value, arr);
      rv = data.worker->get_rp_int(&data, name);
      return rv.valid();
    }
    rv.clear(); return false;
  }
  inline bool addv(SFixedParameterRelPtr_Double& rv, BCStr name, const double *value, int arr=1) {
    if(data.worker) {
      data.worker->addv(&data, name, value, arr);
      rv = data.worker->get_rp_double(&data, name);
      return rv.valid();
    }
    rv.clear(); return false;
  }
  inline bool addv(SFixedParameterRelPtr_Object& rv, BCStr name, IRefObject *const *value, int arr=1) {
    if(data.worker) {
      data.worker->addv(&data, name, value, arr);
      rv = data.worker->get_rp_object(&data, name);
      return rv.valid();
    }
    rv.clear(); return false;
  }
  inline bool addv(SFixedParameterRelPtr_Str& rv, BCStr name, const BCStr *value, int maxv, int arr=1) {
    if(data.worker) {
      data.worker->addv(&data, name, value, maxv, arr);
      rv = data.worker->get_rp_str(&data, name);
      return rv.valid();
    }
    rv.clear(); return false;
  }
  //
  inline bool ready(void)
    { return data.worker ? data.worker->ready(&data) : false; }
protected:
  SFixedParametersBaseData data;
//  friend struct SFixedBaseParameters;
  friend struct SFixedParameters;
};

//***************************************

struct SFixedParametersData {
  SFixedParametersBaseData *base;
  IParameters* worker;
  SMemChunk mem;
  //
  inline SFixedParametersData(void) : base(NULL), worker(NULL) {}
};

struct SFixedParameters {
public:
protected:
  SFixedParametersData data;
public:
  inline void* get_data_block(void) const { return data.mem.get_data(); }
  //
  inline ~SFixedParameters(void)
    { if(data.worker) data.worker->p_deinit(&data); }
  inline SFixedParameters(SFixedParametersBase* base)
    { base->data.worker->p_init(&data, &base->data); }
  inline SFixedParameters(SFixedParametersBase& base)
    { base.data.worker->p_init(&data, &base.data); }
  inline SFixedParameters(const SFixedParameters& r)
    { if(r.data.worker) r.data.worker->p_init(&data, r.data); }
  inline const SFixedParameters& operator=(const SFixedParameters& r)
    { data.worker->p_copy(&data, r.data); return *this; }
  //
  inline int get_count(int h) const
    { return data.worker ? data.worker->get_count(&data, h) : 0; }
  inline int get_size_str(int h) const
    { return data.worker ? data.worker->get_size_str(&data, h) : 0; }
  inline BWStr p_str(int h, int subid=0) const
    { return data.worker ? data.worker->get_p_str(&data, h, subid) : NULL; }
  inline int* p_int(int h, int subid=0) const
    { return data.worker ? data.worker->get_p_int(&data, h, subid) : NULL; }
  inline double* p_double(int h, int subid=0) const
    { return data.worker ? data.worker->get_p_double(&data, h, subid) : NULL; }
  inline IRefObject** p_object(int h, int subid=0) const
    { return data.worker ? data.worker->get_p_object(&data, h, subid) : NULL; }
  //
  inline int get_count(BCStr name) const
    { return data.worker ? data.worker->get_count(&data, name) : 0; }
  inline int get_size_str(BCStr name) const
    { return data.worker ? data.worker->get_size_str(&data, name) : 0; }
  inline BWStr p_str(BCStr name, int subid=0) const
    { return data.worker ? data.worker->get_p_str(&data, name, subid) : NULL; }
  inline int* p_int(BCStr name, int subid=0) const
    { return data.worker ? data.worker->get_p_int(&data, name, subid) : NULL; }
  inline double* p_double(BCStr name, int subid=0) const
    { return data.worker ? data.worker->get_p_double(&data, name, subid) : NULL; }
  inline IRefObject** p_object(BCStr name, int subid=0) const
    { return data.worker ? data.worker->get_p_object(&data, name, subid) : NULL; }
  //
  inline bool get(int h, BCStr &value, int subid=0) const
    { return data.worker ? data.worker->get(&data, h, value, subid) : false; }
  inline bool get(int h, int &value, int subid=0) const
    { return data.worker ? data.worker->get(&data, h, value, subid) : false; }
  inline bool get(int h, double &value, int subid=0) const
    { return data.worker ? data.worker->get(&data, h, value, subid) : false; }
  inline bool get(int h, IRefObject* &value, int subid=0) const
    { return data.worker ? data.worker->get(&data, h, value, subid) : false; }
  //
  inline bool set(int h, BCStr value, int subid=0) const
    { return data.worker ? data.worker->set(&data, h, value, subid) : false; }
  inline bool set(int h, int value, int subid=0) const
    { return data.worker ? data.worker->set(&data, h, value, subid) : false; }
  inline bool set(int h, const double &value, int subid=0) const
    { return data.worker ? data.worker->set(&data, h, value, subid) : false; }
  inline bool set(int h, IRefObject* value, int subid=0) const
    { return data.worker ? data.worker->set(&data, h, value, subid) : false; }
  //
  inline bool get(BCStr name, BCStr &value, int subid=0) const
    { return data.worker ? data.worker->get(&data, name, value, subid) : false; }
  inline bool get(BCStr name, int &value, int subid=0) const
    { return data.worker ? data.worker->get(&data, name, value, subid) : false; }
  inline bool get(BCStr name, double &value, int subid=0) const
    { return data.worker ? data.worker->get(&data, name, value, subid) : false; }
  inline bool get(BCStr name, IRefObject* &value, int subid=0) const
    { return data.worker ? data.worker->get(&data, name, value, subid) : false; }
  //
  inline bool set(BCStr name, BCStr value, int subid=0) const
    { return data.worker ? data.worker->set(&data, name, value, subid) : false; }
  inline bool set(BCStr name, int value, int subid=0) const
    { return data.worker ? data.worker->set(&data, name, value, subid) : false; }
  inline bool set(BCStr name, const double &value, int subid=0) const
    { return data.worker ? data.worker->set(&data, name, value, subid) : false; }
  inline bool set(BCStr name, IRefObject* value, int subid=0) const
    { return data.worker ? data.worker->set(&data, name, value, subid) : false; }
};

//***************************************
// INLINES
//***************************************

template<typename Data>
inline Data SFixedParameterRelPtr<Data>::operator()(const SFixedParameters& fp) const {
    return reinterpret_cast<Data>(
      reinterpret_cast<byte*>(fp.get_data_block())
      + offs
    );
}

template<typename Data>
inline Data SFixedParameterRelPtr<Data>::get(const SFixedParameters& fp) const {
    return reinterpret_cast<Data>(
      reinterpret_cast<byte*>(fp.get_data_block())
      + offs
    );
}

//***************************************
// END
//***************************************

} // namespace media
} // namespace core
} // namespace UMS

#endif
