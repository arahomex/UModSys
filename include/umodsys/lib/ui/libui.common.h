#ifndef __UMODSYS_LIBUI_COMMON_H
#define __UMODSYS_LIBUI_COMMON_H 1
/*************************************************************/
// file: umodsys/lib/libui/libui.common.h
// info: ui common imterfaces
/*************************************************************/

#include <umodsys/lib/2d/lib2d.common.h>
#include <umodsys/lib/ui/libui.keymap.h>
#include <umodsys/core/console_ext.h>

namespace UModSys {
namespace libui {

using namespace core;

//***************************************
// FORWARD/TYPEDEF
//***************************************

struct ITerminal;      // UI console have input & output included

struct IKeyboardClient;
struct IJoystickClient;
struct IMouseClient;
struct ITouchClient;

struct IKeyboardController;
struct IJoystickController;
struct IMouseController;
struct ITouchController;

struct SKeyboardInputRaw;
struct SKeyboardInputText;
struct SJoystickInput;
struct SMouseInput;
struct STouchInput;

//***************************************
// ENUM/CONST
//***************************************

enum eTerminalState {
  ts_Active,
  ts_Inactive,
  ts_Paused,
  ts_Offline,
  ts_SoftTerminate,
  ts_Terminated,
  ts_Keyboard,
  ts_Reset,
  ts_Unknown,
  ts_MaxStates = 16
};

enum eButtonInputStatus {
  bis_Modifier1     = 0x10000, // 16 modifiers allowed
  //
  bis_Pressed       = 0x0001,
  bis_Released      = 0x0002,
  bis_PressedTwice  = 0x0004,
  bis_ReleasedTwice = 0x0008,
  bis_DownShort     = 0x0010,
  bis_DownMedium    = 0x0020,
  bis_DownLong      = 0x0040,
  bis_DownVeryLong  = 0x0080,
  bis_ToggleOn      = 0x0100,
  bis_ToggleOff     = 0x0200,
  //
  bis_None          = 0x0000
};

enum eMouseEvent {
  me_Button         = 0x0001, // button state changed
  me_MoveAbs        = 0x0002, // move absolute
  me_MoveRelative   = 0x0004, // move relative
  me_MoveButton     = 0x0008, // drag
  me_None           = 0x0000
};

enum {
  max_mouse_buttons     = 16,
  max_key_modifiers     = 16,
  max_joystick_buttons  = 32,
  max_joystick_axes     = 16
};

//***************************************
// TYPES
//***************************************

//***************************************
// Keyboard input

struct IKeyboardClient {
  virtual bool key_pressed(const SKeyboardInputRaw& key) =0;
  virtual bool key_text(const SKeyboardInputText& key) =0;
};

struct IKeyboardController : public IRefObject {
  virtual bool key_unlink(int order) =0;
  virtual bool key_link(int order, IRefObject* combo, IKeyboardClient* target) =0;
  virtual bool key_reset(void) = 0;
  virtual bool key_setvisible(bool visible) = 0;
  virtual bool key_setmodifier(int modifier, int code, int statuson) =0;
  virtual bool key_setrepeat(float firsttime, float nexttime) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libui::IKeyboardController, 2, IRefObject);
};

//***************************************

struct SKeyboardInputRaw {
  Bsint16 code, status, os_code, special;
};

struct SKeyboardInputText {
  tl::su::utf32 text;
};

//***************************************
// Mouse input

struct IMouseClient {
  virtual bool mouse_event(const SMouseInput& ms) =0;
};

struct IMouseController : public IRefObject {
  virtual bool mouse_unlink(int order) =0;
  virtual bool mouse_link(int order, IRefObject* combo, IMouseClient* target, int memask) =0;
  virtual bool mouse_reset(void) = 0;
  virtual bool mouse_setvisible(bool visible) = 0;
  virtual bool mouse_setposition(int x, int y) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libui::IMouseController, 2, IRefObject);
};

//***************************************

struct SMouseInput {
  Bsint16 eventid, button_count;
  // coordinates
  lib2d::DPoint abs, rel;
  lib2d::DPoint aux; // eg. wheels
  // buttons states
  Bsint16 buttons[max_mouse_buttons];
  //
  inline unsigned get_buttonmask(eButtonInputStatus bis=bis_Pressed) const {
    unsigned rv = 0;
    for(int i=0; i<button_count; i++) {
      if(buttons[i] & bis)
        rv |= 1<<i;
    }
    return rv;
  }
};

//***************************************
// Joystick input

struct IJoystickClient {
  virtual bool joystick_event(const SJoystickInput& ms) =0;
};

struct IJoystickController : public IRefObject {
  virtual bool joystick_unlink(int order) =0;
  virtual bool joystick_link(int order, IRefObject* combo, IJoystickClient* target, int memask) =0;
  virtual bool joystick_reset(void) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libui::IJoystickController, 2, IRefObject);
};

//***************************************

struct SJoystickInput {
  int eventid, button_count;
  // coordinates
  lib2d::DPoint axis[max_joystick_axes]; // trackpad, joy have 2 axis
  // buttons states
  sint8 buttons[max_joystick_buttons]; // hat have 2 buttons
};

//***************************************
// Terminal
//***************************************

struct ITerminal : public IRefObject {
  // special functions
  virtual bool get_terminal_state(eTerminalState state) = 0;
  virtual bool set_terminal_state(eTerminalState state, bool flag=true) = 0;
  //
  virtual bool poll_events(int maxcount=-1) = 0;
  virtual double get_clock(void) = 0;
  virtual bool wait(float sec) = 0;
  //
  virtual size_t get_count(TypeId tid) const =0;
  virtual IRefObject* get_handler(TypeId tid, size_t id) const =0;
  virtual IRefObject* get_handler(TypeId tid, BCStr name) const =0;
  virtual IRefObject* get_friend_handler(TypeId tid, IRefObject *obj) const =0; // map handlers
  virtual bool create_handler(TypeId tid, IRefObject::P &rv, BCStr name, const SParameters* params) =0;
public:
  template<typename CType>
  inline size_t t_get_count(void) const { 
    return get_count(CType::_root_get_interface_type()); 
  }
  template<typename CType>
  inline CType* t_get_handler(size_t id) const { 
    IRefObject *ao = get_handler(CType::_root_get_interface_type(), id); 
    CType* rv; 
    return ao!=NULL && ao->t_root_get_other_interface(&rv) ? rv : NULL; 
  }
  template<typename CType>
  inline CType* t_get_handler(BCStr name) const { 
    IRefObject *ao = get_handler(CType::_root_get_interface_type(), name); 
    CType* rv; 
    return ao!=NULL && ao->t_root_get_other_interface(&rv) ? rv : NULL; 
  }
  template<typename CType>
  inline CType* t_get_friend_handler(IRefObject *obj) const { 
    IRefObject *ao = get_friend_handler(CType::_root_get_interface_type(), obj); 
    CType* rv; 
    return ao!=NULL && ao->t_root_get_other_interface(&rv) ? rv : NULL; 
  }
  template<typename CType>
  inline bool t_create_handler(tl::TRefObject<CType> &rv, BCStr name, const SParameters* params) { 
    IRefObject::P ao; 
    return create_handler(CType::_root_get_interface_type(), ao, name, params) 
        && ao->t_ref_get_other_interface(rv); 
  }
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libui::ITerminal, 2, IRefObject);
};

//***************************************
// Terminal Connection

template<typename CType>
struct TTerminalConnection {
public:
  typedef IRoot::TypeId TypeId;
protected:
  ITerminal::P term;
public:
  inline TTerminalConnection(ITerminal* t) : term(t) {}
  inline ~TTerminalConnection(void) {}
  //
  inline operator ITerminal*(void) const { return term; }
  inline ITerminal* operator ->(void) const { return term; }
  inline bool valid(void) const { return term.valid(); }
  //
  inline size_t get_count(void) const { return term->t_get_count<CType>(); }
  inline CType* get_handler(size_t id) const { return term->t_get_handler<CType>(id); }
  inline CType* get_handler(BCStr name) const { return term->t_get_handler<CType>(name); }
  inline CType* get_friend_handler(IRefObject *obj) const { return term->t_get_friend_handler<CType>(obj); }
  inline bool create_handler(tl::TRefObject<CType> &rv, BCStr name, const SParameters* params) const { return term->t_create_handler(rv, name, params); }
};

//***************************************
// Re-tracers
//***************************************

struct IConsoleHelperInput_UIKey : public base::IConsoleHelperInput, public IKeyboardClient {
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libui::IConsoleHelperInput_UIKey, 2, IConsoleHelperInput);
};


//***************************************
// INLINES/OUTLINES
//***************************************

//***************************************
// END
//***************************************

} // namespace libui
} // namespace UModSys

#endif // __UMODSYS_LIBUI_COMMON_H
