
//***************************************
// ::
//***************************************

struct RTerminal_SDL_core;
struct RRenderDriver2D_SDL_core;

//***************************************
//***************************************

inline SDL_Rect rect(int x, int y, int dx, int dy) {
  SDL_Rect rv;
  rv.x = x; rv.y = y;
  rv.w = dx; rv.h = dy;
  return rv;
}

inline SDL_Rect rect(const lib2d::DPoint& a, const lib2d::DPoint& b) {
  SDL_Rect rv;
  rv.x = a(0); rv.y = a(1);
  rv.w = b(0)-a(0); rv.h = b(1)-a(1);
  return rv;
}

inline SDL_Rect rect(const lib2d::DBox& a) {
  SDL_Rect rv;
  rv.x = a.originx(); rv.y = a.originy();
  rv.w = a.sizex(); rv.h = a.sizey();
  return rv;
}

//***************************************
//***************************************

template<typename T>
inline size_t t_count(const tl::TScatterArray<T>& arr) 
{
  size_t rv = 0;
  for(tl::TScatterArray<T>::CIter x = arr(); x; ++x) {
    if(!x->second.valid())
      continue;
    rv++;
  }
  return rv;
}

template<typename T>
inline IRefObject* t_get(const tl::TScatterArray<T>& arr, size_t id) 
{
  for(tl::TScatterArray<T>::CIter x = arr(); x; ++x, --id) {
    if(!x->second.valid())
      continue;
    if(id==0)
      return x->second;
  }
  return NULL;
}

template<typename T>
inline IRefObject* t_get(const tl::TScatterArray<T>& arr, BCStr name) 
{
  return NULL;
}

template<typename T>
inline IRefObject* t_getfr(const tl::TScatterArray<T>& arr, IRefObject* obj) 
{
  return NULL;
}

template<typename T, typename Val>
inline bool t_add(tl::TScatterArray<T>& arr, Val* v) 
{
  int idx = 0;
  tl::TScatterArray<T>::CIter last = arr.Last();
  if(last)
    idx = last->first+1;
  T* value = arr(idx, void_null());
  if(value==NULL)
    return false;
  *value <<= v;
  return true;
}

template<typename T>
inline bool t_close(tl::TScatterArray<T>& arr) 
{
  for(tl::TScatterArray<T>::CIter x = arr(); x; ++x) {
    if(!x->second.valid())
      continue;
    x->second->close();
  }
  return true;
}

//***************************************
// END
//***************************************
