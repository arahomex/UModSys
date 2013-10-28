#ifndef __UMODSYS_LIBUI_KEYMAP_H
#define __UMODSYS_LIBUI_KEYMAP_H 1
/*************************************************************/
// file: umodsys/lib/libui/libui.keymap.h
// info: ui keyboard map
/*************************************************************/

namespace UModSys {
namespace libui {

//***************************************
// ENUM/CONST
//***************************************

enum eKeyCode {
  /* the keyboard syms have been cleverly chosen to map to ascii */
  k_backspace            = 8,
  k_tab                  = 9,
  k_clear                = 12,
  k_return               = 13,
  k_pause                = 19,
  k_escape               = 27,
  k_space                = 32,
  k_exclaim              = 33,
  k_quotedbl             = 34,
  k_hash                 = 35,
  k_dollar               = 36,
  k_ampersand            = 38,
  k_quote                = 39,
  k_leftparen            = 40,
  k_rightparen           = 41,
  k_asterisk             = 42,
  k_plus                 = 43,
  k_comma                = 44,
  k_minus                = 45,
  k_period               = 46,
  k_slash                = 47,
  k_0                    = 48,
  k_1                    = 49,
  k_2                    = 50,
  k_3                    = 51,
  k_4                    = 52,
  k_5                    = 53,
  k_6                    = 54,
  k_7                    = 55,
  k_8                    = 56,
  k_9                    = 57,
  k_colon                = 58,
  k_semicolon            = 59,
  k_less                 = 60,
  k_equals               = 61,
  k_greater              = 62,
  k_question             = 63,
  k_at                   = 64,
  k_leftbracket          = 91,
  k_backslash            = 92,
  k_rightbracket         = 93,
  k_caret                = 94,
  k_underscore           = 95,
  k_backquote            = 96,
  //
  k_a                    = 97,
  k_b                    = 98,
  k_c                    = 99,
  k_d                    = 100,
  k_e                    = 101,
  k_f                    = 102,
  k_g                    = 103,
  k_h                    = 104,
  k_i                    = 105,
  k_j                    = 106,
  k_k                    = 107,
  k_l                    = 108,
  k_m                    = 109,
  k_n                    = 110,
  k_o                    = 111,
  k_p                    = 112,
  k_q                    = 113,
  k_r                    = 114,
  k_s                    = 115,
  k_t                    = 116,
  k_u                    = 117,
  k_v                    = 118,
  k_w                    = 119,
  k_x                    = 120,
  k_y                    = 121,
  k_z                    = 122,
  //
  k_delete               = 127,
  // international
  k_world0               = 160,          /* 0xa0 */
    /* ... */
  k_world95              = 255,          /* 0xff */
  /* numeric keypad */
  k_kp0                  = 256,
  k_kp1                  = 257,
  k_kp2                  = 258,
  k_kp3                  = 259,
  k_kp4                  = 260,
  k_kp5                  = 261,
  k_kp6                  = 262,
  k_kp7                  = 263,
  k_kp8                  = 264,
  k_kp9                  = 265,
  k_kp_period            = 266,
  k_kp_divide            = 267,
  k_kp_multiply          = 268,
  k_kp_minus             = 269,
  k_kp_plus              = 270,
  k_kp_enter             = 271,
  k_kp_equals            = 272,
  /* arrows + home/end pad */
  k_up                   = 273,
  k_down                 = 274,
  k_right                = 275,
  k_left                 = 276,
  k_insert               = 277,
  k_home                 = 278,
  k_end                  = 279,
  k_pageup               = 280,
  k_pagedown             = 281,
  /* function keys */
  k_f1                   = 282,
  k_f2                   = 283,
  k_f3                   = 284,
  k_f4                   = 285,
  k_f5                   = 286,
  k_f6                   = 287,
  k_f7                   = 288,
  k_f8                   = 289,
  k_f9                   = 290,
  k_f10                  = 291,
  k_f11                  = 292,
  k_f12                  = 293,
  k_f13                  = 294,
  k_f14                  = 295,
  k_f15                  = 296,
  /* key state modifier keys */
  k_numlock              = 300,
  k_capslock             = 301,
  k_scrollock            = 302,
  k_rshift               = 303,
  k_lshift               = 304,
  k_rctrl                = 305,
  k_lctrl                = 306,
  k_ralt                 = 307,
  k_lalt                 = 308,
  k_rmeta                = 309,
  k_lmeta                = 310,
  k_lsuper               = 311,          /* left "windows" key */
  k_rsuper               = 312,          /* right "windows" key */
  k_mode                 = 313,          /* "alt gr" key */
  k_compose              = 314,          /* multi-key compose key */
  /* miscellaneous function keys */
  k_help                 = 315,
  k_print                = 316,
  k_sysreq               = 317,
  k_break                = 318,
  k_menu                 = 319,
  k_power                = 320,          /* power macintosh power key */
  k_euro                 = 321,          /* some european keyboards */
  k_undo                 = 322,          /* atari keyboard has undo */

  /* add any other keys here */

  k_last_used,
  k_max                  = 512           // no more than this
};

//***************************************

enum eKeyCodeModifier {
  kmi_l_shift           = 0,
  kmi_r_shift           = 1,
  kmi_l_ctrl            = 2,
  kmi_r_ctrl            = 3,
  kmi_l_alt             = 4,
  kmi_r_alt             = 5,
  kmi_l_super           = 6,
  kmi_r_super           = 7,
  //
  kmi_numlock           = 8,
  kmi_capslock          = 9,
  kmi_scrollock         = 10,
  //
  km_l_shift            = 1<<kmi_l_shift,
  km_r_shift            = 1<<kmi_r_shift,
  km_l_ctrl             = 1<<kmi_l_ctrl,
  km_r_ctrl             = 1<<kmi_r_ctrl,
  km_l_alt              = 1<<kmi_l_alt,
  km_r_alt              = 1<<kmi_r_alt,
  km_l_super            = 1<<kmi_l_super,
  km_r_super            = 1<<kmi_r_super,
  //
  km_numlock            = 1<<kmi_numlock,
  km_capslock           = 1<<kmi_capslock,
  km_scrollock          = 1<<kmi_scrollock,
  //
  km_shift              = km_l_shift | km_r_shift,
  km_ctrl               = km_l_ctrl | km_r_ctrl,
  km_alt                = km_l_alt | km_r_alt,
  km_super              = km_l_super | km_r_super,
  //
  km_none               = 0
};

//***************************************
// END
//***************************************

} // namespace libui
} // namespace UModSys

#endif // __UMODSYS_LIBUI_COMMON_H
