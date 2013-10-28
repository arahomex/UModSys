#ifndef __UMODSYS_LIBUI_COMMON_H
#define __UMODSYS_LIBUI_COMMON_H 1
/*************************************************************/
// file: umodsys/lib/libui/libui.common.h
// info: ui common imterfaces
/*************************************************************/

#include <umodsys/lib/2d/lib2d.common.h>
#include <umodsys/lib/ui/libui.keymap.h>
#include <umodsys/base/console_ext.h>

namespace UModSys {

namespace lib3d {
  struct IRenderDriver;
} // namespace lib3d

namespace lib2d {
  struct IRenderDriver;
} // namespace lib3d

namespace libui {

using namespace core;

//***************************************
// FORWARD/TYPEDEF
//***************************************

struct ITerminal;      // UI console have input & output included

struct IKeyboardClient;
struct IJoystickClient;
struct IMouseClient;

struct IKeyboardController;
struct IJoystickController;
struct IMouseController;

struct SKeyboardInputRaw;
struct SKeyboardInputC;
struct SKeyboardInputW;
struct SKeyboardInputU;
struct SMouseInput;
struct SJoystickInput;

//***************************************
// ENUM/CONST
//***************************************

enum eTerminalState {
  ts_Active,
  ts_Inactive,
  ts_Paused,
  ts_Offline,
  ts_Terminated,
  ts_Unknown
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
  max_joystick_buttons  = 16,
  max_joystick_axes     = 16
};

//***************************************
// TYPES
//***************************************

//***************************************
// Keyboard input
//***************************************

struct IKeyboardClient {
  virtual bool key_pressed(const SKeyboardInputRaw& key) =0;
  virtual bool key_charc(const SKeyboardInputC& key) =0;
  virtual bool key_charw(const SKeyboardInputW& key) =0;
  virtual bool key_charu(const SKeyboardInputU& key) =0;
};

struct IKeyboardController : public IRefObject {
  virtual bool key_unlink(int order) =0;
  virtual bool key_link(int order, IRefObject* combo, IKeyboardClient* target) =0;
  virtual bool key_setmodifier(int modifier, int code, int statuson) =0;
  virtual bool key_setrepeat(float firsttime, float nexttime) =0;
  virtual bool key_reset(void) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libui::IKeyboardController, 2, IRefObject);
};

//***************************************

struct SKeyboardInputRaw {
  int code, status, os_code, special;
};

struct SKeyboardInputC {
  BByte len;
  BChar keyc[su::utf_max8len+1];
};

struct SKeyboardInputW {
  BByte len;
  BCharW keyw[su::utf_max16len+1];
};

struct SKeyboardInputU {
  BCharU keyu;
};

//***************************************
// Mouse input
//***************************************

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
  int eventid, button_count;
  // coordinates
  lib2d::DPoint abs, rel;
  lib2d::DPoint secondary; // eg. wheels
  // buttons states
  int buttons[max_mouse_buttons];
};

//***************************************
// Joystick input
//***************************************

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
  lib2d::DPoint axis[max_joystick_axes];
  // buttons states
  int buttons[max_joystick_buttons];
};

//***************************************
// Terminal Connection
//***************************************

template<typename CType>
struct TTerminalConnection {
  virtual int get_count(void) =0;
  virtual CType* get_handler(int id) =0;
  virtual CType* get_handler(BCStr name) =0;
  virtual TRefObject<CType> create_handler(BCStr name, const SParameters* params) =0;
};

//***************************************
// Terminal
//***************************************

struct ITerminal : public IRefObject {
  // special functions
  virtual bool poll_events(int maxcount=-1) = 0;
  virtual eTerminalState get_terminal_state(void) = 0;
  virtual bool set_terminal_state(eTerminalState state) = 0;
  virtual bool set_terminal_caption(BCStr caption) = 0;
  virtual double get_clock(void) = 0;
  virtual bool wait(float sec) = 0;
  virtual bool terminal_reset(void) = 0;
  // get or create inputs
  virtual TTerminalConnection<IKeyboardController>& get_input_keyboard(void) = 0;
  virtual TTerminalConnection<IMouseController>& get_input_mouse(void) = 0;
  virtual TTerminalConnection<IJoystickController>& get_input_joystick(void) = 0;
  // get or create outputs
  virtual TTerminalConnection<lib3d::IRenderDriver>& get_output_3d(void) = 0;
  virtual TTerminalConnection<lib2d::IRenderDriver>& get_output_2d(void) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libui::ITerminal, 2, IRefObject);
};

//***************************************
// Re-tracers
//***************************************

struct IConsoleHelperInput_UIKey : public IConsoleHelperInput, public IKeyboardClient {
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
