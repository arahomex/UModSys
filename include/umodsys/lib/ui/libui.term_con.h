#ifndef __UMODSYS_LIBUI_TERMCON_H
#define __UMODSYS_LIBUI_TERMCON_H 1
/*************************************************************/
// file: umodsys/lib/libui/libui.term_con.h
// info: terminal connections
/*************************************************************/

#include <umodsys/lib/ui/libui.common.h>

namespace UModSys {
namespace libui {

//***************************************
// TYPES
//***************************************

//***************************************
// TRTerminalConnectionNull

template<typename CType>
struct TRTerminalConnectionNull : public TITerminalConnection<CType> {
  inline size_t get_count(void) const { return 0; }
  inline CType* get_handler(size_t id) const { return NULL; }
  inline CType* get_handler(BCStr name) const { return NULL; }
  inline CType* get_friend_handler(IRefObject *obj) const { return NULL; }
  inline bool create_handler(tl::TRefObject<CType> &rv, BCStr name, const SParameters* params) { return false; }
  //
  inline TRTerminalConnectionNull(void) {}
};

//***************************************
// TRTerminalConnectionComposite

template<typename CType, typename CTerminal>
struct TRTerminalConnectionComposite : public TITerminalConnection<CType> {
  CTerminal *link;
  //
  inline static CType* vp(void) { return NULL; }
  //
  size_t get_count(void) const;
  CType* get_handler(size_t id) const;
  CType* get_handler(BCStr name) const;
  CType* get_friend_handler(IRefObject *obj) const;
  bool create_handler(tl::TRefObject<CType> &rv, BCStr name, const SParameters* params);
  //
  inline TRTerminalConnectionComposite(CTerminal *pv) : link(pv) {}
  //
  inline void operator=(CTerminal *pv) { link=pv; }
  inline operator TITerminalConnection<CType>*(void) { return this; }
  inline TITerminalConnection<CType>* operator*(void) { return this; }
};

template<typename CType, typename CTerminal>
inline size_t TRTerminalConnectionComposite<CType, CTerminal>::get_count(void) const
{ 
  return link->connection_get_count(vp); 
}

template<typename CType, typename CTerminal>
inline CType* TRTerminalConnectionComposite<CType, CTerminal>::get_handler(size_t id) const
{ 
  return link->connection_get_handler_i(vp, id); 
}

template<typename CType, typename CTerminal>
inline CType* TRTerminalConnectionComposite<CType, CTerminal>::get_handler(BCStr name) const
{ 
  return link->connection_get_handler_s(vp, name); 
}

template<typename CType, typename CTerminal>
inline CType* TRTerminalConnectionComposite<CType, CTerminal>::get_friend_handler(IRefObject *obj) const
{
  return link->connection_get_handler_fr(vp, obj); 
}

template<typename CType, typename CTerminal>
inline bool TRTerminalConnectionComposite<CType, CTerminal>::create_handler(tl::TRefObject<CType> &rv, BCStr name, const SParameters* params)
{ 
  return link->connection_create_handler(rv, vp, name, params);
}

//***************************************
// INLINES/OUTLINES
//***************************************

//***************************************
// END
//***************************************

} // namespace libui
} // namespace UModSys

#endif // __UMODSYS_LIBUI_TERMCON_H
