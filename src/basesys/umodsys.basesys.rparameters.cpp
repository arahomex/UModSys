#include "umodsys.basesys.rparameters.h"

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::base;
using namespace UModSys::base::rsystem;

//***************************************
// RParameters::
//***************************************

//***************************************
// service SParametersData

const RParameters::Elem* RParameters::find_elem(const void *data, int max, int type, int namehash, BCStr name) 
{
  if(data==NULL || max<sizeof(Elem))
    return NULL; // failed
  //
  for(const Elem* x = reinterpret_cast<const Elem*>(data); x->end_size(data)<=max; x = x->next()) {
    if(x->type==type && x->name_hash==namehash && strcmp(x->name, name)==0)
      return x; // found one
    if(x->end_size(data)==max)
      break;
  }
  return NULL;
}

RParameters::Elem* RParameters::get_last(void *data, int current, int max, int estimated_size) 
{
  if(data==NULL || estimated_size<=0 || current+estimated_size>max)
    return NULL; // failed
  //
  Elem* x;
  if(current==0) {
    x = reinterpret_cast<Elem*>(data);
  } else {
    for(x = reinterpret_cast<Elem*>(data); x->end_size(data)<current; x = x->next()) 
      ;
    if(x->end_size(data)!=current)
      return NULL; // error inside a data
    x = x->next(); // next is free one
  }
  //
  return x;
}

int RParameters::estimate_size(Elem& tmp, int namehash, BCStr name, int type, int typesizeext) 
{
  int typesize = 0;
  const int Q = sizeof(int);
  switch(type) {
    case et_Int: typesize = sizeof(int); break;
    case et_Double: typesize = sizeof(double); break;
    case et_RefObject: typesize = sizeof(IRefObject*); break;
    case et_String: typesize = sizeof(ElemSValue)+typesizeext; break;
    case et_Null: typesize = 0; break;
    default:
      return -1; // failed to determine this one
  }
  tmp.type = type;
  tmp.name_hash = namehash;
  tmp.name_len = ((strlen(name) + 1 + Q-1)/Q)*Q;
  tmp.full_len = ((typesize + sizeof(Elem) + tmp.name_len + Q-1)/Q)*Q;
  return tmp.full_len;
}

void RParameters::free_elems(void *data, int max) 
{
  if(data==NULL || max<sizeof(Elem))
    return; // nothing check
  //
  for(Elem* x = reinterpret_cast<Elem*>(data); x->end_size(data)<=max; x = x->next()) {
    if(x->type==et_RefObject) { // free it
      IRefObject* &obj = x->oval();
      tl::_Ref_remove(obj);
    }
    if(x->end_size(data)==max)
      break;
  }
}

void RParameters::alloc_elems(void *data, int max) 
{
  if(data==NULL || max<sizeof(Elem))
    return; // nothing check
  //
  for(Elem* x = reinterpret_cast<Elem*>(data); x->end_size(data)<=max; x = x->next()) {
    if(x->type==et_RefObject) { // free it
      IRefObject* obj = x->oval();
      if(obj) obj->ref_add();
    }
    if(x->end_size(data)==max)
      break;
  }
}

bool RParameters::add_last(SParametersData *p, BCStr name, const void *value, int type) 
{
  if(name==NULL || *name==0)
    return false; // nothing added with this args
  //
  int hash = tl::su::shash(name);
  const Elem* found = find_elem(p->mem.data, p->mem_used, type, hash, name);
  if(found!=NULL) {
    // duplicate ones, erase old one for strings, replace for others
    switch(type) {
      case et_Int: 
        const_cast<Elem*>(found)->ival() = *reinterpret_cast<const int*>(value);
        return true;
      case et_Double: 
        const_cast<Elem*>(found)->dval() = *reinterpret_cast<const double*>(value);
        return true;
      case et_RefObject:
        tl::_Ref_set(const_cast<Elem*>(found)->oval(), *reinterpret_cast<IRefObject*const*>(value));
        return true;
      case et_String:
        const_cast<Elem*>(found)->type = et_Null; 
        // now this one is skipped ! place string value again !
        break;
      case et_Null: 
        break;
      default:
        return false; // failed to determine this one
    }
  }
  //
  int slen = 0;
  if(type==et_String) {
    if(value==NULL) value = "";
    else slen=strlen(reinterpret_cast<BCStr>(value));
  }
  Elem hdr;
  int S = estimate_size(hdr, hash, name, type, slen);
  Elem* last = get_last(p->mem.data, p->mem_used, p->mem.size, S);
  if(last==NULL) 
    return false; // can't fit
  //
  // fit values to place
  *last = hdr;
  strcpy(last->name, name);
  switch(type) {
    case et_Int: 
      last->ival() = *reinterpret_cast<const int*>(value);
      break;
    case et_Double: 
      last->dval() = *reinterpret_cast<const double*>(value);
      break;
    case et_RefObject: {
      IRefObject* obj = *reinterpret_cast<IRefObject*const*>(value);
      last->oval() = obj;
      if(obj)
        obj->ref_add();
    } break;
    case et_String: {
      ElemSValue& v = last->sval();
      v.len = slen;
      strcpy(v.value, reinterpret_cast<BCStr>(value));
    } break;
    case et_Null: 
      break;
    default:
      return false; // failed to determine this one
  }
  //
  p->mem_used += hdr.full_len;
  return true;
}

//***************************************
// SParametersData

void RParameters::p_deinit(SParametersData *p) 
{
  free_elems(p->mem.data, p->mem_used);
  p->amem.t_free(*sys->get_sharemem(), UMODSYS_SOURCEINFO);
  p->mem.clear();
  p->mem_used = 0;
  tl::_Ref_remove(p->worker);
}

void RParameters::p_init(SParametersData *p, int defalloc) 
{
  p->mem_used = 0;
  p->amem.t_alloc(*sys->get_sharemem(), defalloc, UMODSYS_SOURCEINFO);
  p->mem = p->amem.get();
  tl::_Ref_init(p->worker, this);
}

void RParameters::p_init(SParametersData *p, const SParametersData& r) 
{
  p->mem_used = 0;
  p->amem.t_alloc(*sys->get_sharemem(), r.mem.size, UMODSYS_SOURCEINFO);
  if(p->amem.get_size()!=r.mem.size)
    return; // error allocate data, parameters not initalized (valid) !!!
  p->mem = p->amem.get();
  //
  p->mem_used = r.mem_used;
  if(p->mem.data!=NULL && r.mem_used>0) {
    memcpy(p->mem.data, r.mem.data, r.mem_used);
    alloc_elems(p->mem.data, p->mem_used);
  }
  tl::_Ref_init(p->worker, this);
}

void RParameters::p_copy(SParametersData *p, const SParametersData& r) 
{
  if(p->worker!=r.worker)
    return; // CAN COPY ONLY SAME WORKERS !
  //
  if(r.mem_used>p->mem.size) { // need to reallocate more data
    if(p->amem.get_data()!=p->mem.data) // fixed allocation
      return; // NOT ENOUGH STATIC MEMORY
    //
    SMemChunk m;
    m.t_alloc(*sys->get_sharemem(), r.mem.size, UMODSYS_SOURCEINFO);
    if(m.get_size()!=r.mem.size)
      return; // fail: not enough memory
    //
    free_elems(p->mem.data, p->mem_used);
    p->amem.t_free(*sys->get_sharemem(), UMODSYS_SOURCEINFO);
    p->amem = m;
    p->mem = p->amem.get();
  } else {
    free_elems(p->mem.data, p->mem_used);
    p->mem_used = 0;
  }
  //
  p->mem_used = r.mem_used;
  if(r.mem_used>0)
    memcpy(p->mem.data, r.mem.data, r.mem_used);
  alloc_elems(p->mem.data, p->mem_used);
}

void RParameters::p_init_s(SParametersData *p, void* buffer, int nalloc) 
{ // static
  p->mem_used = 0;
  p->mem.assign(buffer, nalloc);
  tl::_Ref_init(p->worker, this);
}

void RParameters::p_init_s(SParametersData *p, const SParametersData& r, void *buffer, int nalloc) 
{ // static
  p->mem_used = 0;
  p->mem.assign(buffer, nalloc);
  if(p->mem.size<r.mem.size)
    return; // not enough space for copy
  //
  p->mem_used = r.mem_used;
  if(p->mem.data!=NULL && r.mem_used>0) {
    memcpy(p->mem.data, r.mem.data, r.mem_used);
    alloc_elems(p->mem.data, p->mem_used);
  }
  tl::_Ref_init(p->worker, this);
}

bool RParameters::next(const SParametersData *p, BCStr &name) 
{
  if(name==NULL || *name==0) {
    if(p->mem_used==0)
      return false; // empty
    name = reinterpret_cast<const Elem*>(p->mem.data)->name;
    return true;
  }
  //
  int h = tl::su::shash(name);
  for(const Elem* x = reinterpret_cast<const Elem*>(p->mem.data); 
      x->end_size(p->mem.data)<=p->mem_used; 
      ) {
    if(x->end_size(p->mem.data)==p->mem_used)
      break;
    x = x->next();
    if(x->name_hash==h && strcmp(x->name, name)==0) {
      name = reinterpret_cast<const Elem*>(p->mem.data)->name;
      return true; // found one
    }
  }
  //
  name = NULL; // end
  return false;
}

bool RParameters::next(const SParametersData *p, DCString& name) 
{
  if(name.text==NULL || name.length==0) {
    if(p->mem_used==0)
      return false; // empty
    name = reinterpret_cast<const Elem*>(p->mem.data)->name;
    return true;
  }
  //
  for(const Elem* x = reinterpret_cast<const Elem*>(p->mem.data); 
      x->end_size(p->mem.data)<=p->mem_used; 
      ) {
    if(x->end_size(p->mem.data)==p->mem_used)
      break;
    x = x->next();
    if(x->name_hash==name.hash && strcmp(x->name, name.text)==0) {
      name = reinterpret_cast<const Elem*>(p->mem.data)->name;
      return true; // found one
    }
  }
  //
  name.clear(); // end
  return false;
}

//***************************************

bool RParameters::add(SParametersData *p, BCStr name, BCStr value) 
{
  return add_last(p, name, value, et_String);
}

bool RParameters::add(SParametersData *p, BCStr name, int value) 
{
  return add_last(p, name, &value, et_Int);
}

bool RParameters::add(SParametersData *p, BCStr name, const double &value) 
{
  return add_last(p, name, &value, et_Double);
}

bool RParameters::add(SParametersData *p, BCStr name, IRefObject *value) 
{
  return add_last(p, name, &value, et_RefObject);
}

//***************************************

bool RParameters::get(const SParametersData *p, BCStr name, BCStr &value) 
{
  if(name==NULL || *name==0 || p==NULL || p->mem_used==0)
    return false; // nothing found
  //
  int hash = tl::su::shash(name);
  const Elem* found = find_elem(p->mem.data, p->mem_used, et_String, hash, name);
  if(!found) return false;
  value = found->sval().value;
  return true;
}

bool RParameters::get(const SParametersData *p, BCStr name, int &value) 
{
  if(name==NULL || *name==0 || p==NULL || p->mem_used==0)
    return false; // nothing found
  //
  int hash = tl::su::shash(name);
  const Elem* found = find_elem(p->mem.data, p->mem_used, et_Int, hash, name);
  if(!found) return false;
  value = found->ival();
  return true;
}

bool RParameters::get(const SParametersData *p, BCStr name, double &value) 
{
  if(name==NULL || *name==0 || p==NULL || p->mem_used==0)
    return false; // nothing found
  //
  int hash = tl::su::shash(name);
  const Elem* found = find_elem(p->mem.data, p->mem_used, et_Double, hash, name);
  if(!found) return false;
  value = found->dval();
  return true;
}

bool RParameters::get(const SParametersData *p, BCStr name, IRefObject* &value) {
  if(name==NULL || *name==0 || p==NULL || p->mem_used==0)
    return false; // nothing found
  //
  int hash = tl::su::shash(name);
  const Elem* found = find_elem(p->mem.data, p->mem_used, et_RefObject, hash, name);
  if(!found) return false;
  value = found->oval();
  return true;
}

//***************************************
//***************************************

RParameters::RParameters(ISystem* s)
: refs(M), sys(s), strict(true) 
{
}

RParameters::~RParameters(void)
{
}

//***************************************
// ::
//***************************************
