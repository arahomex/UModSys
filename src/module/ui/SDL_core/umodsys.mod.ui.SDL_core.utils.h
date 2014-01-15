
//***************************************
// ::
//***************************************

struct RTerminal;
struct RRenderDriver2D;

struct sdlRect : public SDL_Rect {
  inline sdlRect(int ax, int ay, int dx, int dy) {
    x = ax; y = ay;
    w = dx; h = dy;
  }
  inline sdlRect(const lib2d::DPoint& a, const lib2d::DPoint& b) {
    x = a(0); y = a(1);
    w = b(0)-a(0); h = b(1)-a(1);
  }
  inline sdlRect(const lib2d::DBox& a) {
    x = a.originx(); y = a.originy();
    w = a.sizex(); h = a.sizey();
  }
  inline operator SDL_Rect* (void) { return this; }
  inline operator const SDL_Rect* (void) const { return this; }
};

//***************************************
//***************************************

inline SDL_Color color(int r, int g, int b, int a) 
{
  SDL_Color rv;
  rv.r = r; rv.g = g; rv.b = b; rv.a = a;
  return rv;
}

inline SDL_Color color(const lib2d::DColorAlphaf& a) 
{
  SDL_Color rv;
  rv.r = a(0)*255; rv.g = a(1)*255; rv.b = a(2)*255; rv.a = a(3)*255;
  return rv;
}

inline SDL_Color color(const lib2d::DColorAlpha& a) 
{
  SDL_Color rv;
  rv.r = a(0); rv.g = a(1); rv.b = a(2); rv.a = a(3);
  return rv;
}

inline SDL_Point point(int x, int y) 
{
  SDL_Point rv;
  rv.x = x; rv.y = y;
  return rv;
}

inline SDL_Point point(const lib2d::DPoint& a) 
{
  SDL_Point rv;
  rv.x = a(0); rv.y = a(1);
  return rv;
}

//***************************************
//***************************************

template<typename T>
inline size_t t_count(const tl::TScatterArray<T>& arr) 
{
  size_t rv = 0;
  for(typename tl::TScatterArray<T>::CIter x = arr(); x; ++x) {
    if(!x->second.valid())
      continue;
    rv++;
  }
  return rv;
}

template<typename T>
inline IRefObject* t_get(const tl::TScatterArray<T>& arr, size_t id) 
{
  for(typename tl::TScatterArray<T>::CIter x = arr(); x; ++x, --id) {
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
  typename tl::TScatterArray<T>::CIter last = arr.Last();
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
  for(typename tl::TScatterArray<T>::CIter x = arr(); x; ++x) {
    if(!x->second.valid())
      continue;
    x->second->close();
  }
  return true;
}

//***************************************
// END
//***************************************
