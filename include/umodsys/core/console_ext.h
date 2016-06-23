#ifndef __UMODSYS_BASE_CONSOLEEXT_H
#define __UMODSYS_BASE_CONSOLEEXT_H 1
/*************************************************************/
// file: umodsys/base/console_ext.h
// info: extended console object
/*************************************************************/

#include <umodsys/base/console.h>

namespace UModSys {
namespace base {

//***************************************
// FORWARDS
//***************************************

struct IConsoleHelper;              // console general helper
 struct IConsoleHelperHistory;      // console history helper
 struct IConsoleHelperInput;        // console input 
 struct IConsoleHelperOutput;       // console output helper
  struct IConsoleHelperOutputFCS;   // console output helper : FCS

struct CConsoleIFCS;                // FCS interface (non-root)
struct SConsoleIFCS;                // FCS internal data

//***************************************
// SConsoleIFCS
//***************************************

struct SConsoleIFCS {
  // this maintained by Console
  int prompt_color;   // prompted data color
  int vx, vy;         // virtial position [0-vsize<n>), 
  int vsizex, vsizey; // virtual size
  int cury, curx;     // prompt cursor
  int lastx;          // last x
  // this maintained by DisplayHelper
  int csizex, csizey; // size in chars
  //
  inline bool isvisiblex(int x) const { return x>=vx && x<vx+csizex; }
  inline bool isvisibley(int y) const { return y>=vy && y<vy+csizey; }
  inline void setvisiblex(int x) { if(x<vx) vx=x; else if(x>=vx+csizex) vx=x-(csizex-1); }
  inline void setvisibley(int y) { if(y<vy) vy=y; else if(y>=vy+csizey) vy=y-(csizey-1); }
  //
  inline void setvs(int sx, int sy) {
    vsizex = sx; vsizey = sy;
    if(vx>vsizex) vx = vsizex-csizex;
    if(vx<0) vx=0;
    if(vy>vsizey) vy = vsizey-csizey;
    if(vy<0) vy=0;
  }
  inline void clear(void) {
    csizex = csizey = 0;
    vx = vy = 0;
    vsizex = 0; vsizey = 0;
    prompt_color = 0;
    curx = cury = -1;
    lastx = 0;
  }
  inline void csize(int cx, int cy) {
    if(cx<csizex || cy<csizey) {
      bool x1 = isvisiblex(lastx), x2 = isvisiblex(0);
      bool y = isvisibley(vsizey-1);
      csizex = cx; csizey = cy;
      if(x1) setvisiblex(lastx); else if(x2) setvisiblex(0);
      if(y) setvisibley(vsizey-1);
    } else {
      csizex = cx; csizey = cy;
    }
  }
};

//***************************************
// CConsoleIFCS
//***************************************

/*
struct CConsoleIFCS {
  SThreadLock lock;   // MT lock
  SConsoleIFCS state;
  // use relative box inside a large box (vx, vy, vx+csizex, vy+csizey)
  virtual bool display_chars(int x, int y, int dx, int dy) = 0; // display all chars
};
*/

//***************************************
// IConsoleExt
//***************************************

struct IConsoleExt
: public IConsole
{
public:
  typedef int ColorInfoAtom;
  //
  struct SColorInfo {
    float fore[4], back[4]; // rgba
    int bold, italic, underline, striked;
    core::BCStr fontname, fontext;
    int fontmap, fontstyle;
  };
  //
  enum eConsoleHelperType {
    cht_None,
    cht_Input,
    cht_Output,
    cht_History,
    cht_Invalid
  };
  //
  enum eConsoleAsyncCommand {
                      // completed -> return 's'/'c', 
                      // no async -> return -1
                      // not complete -> return 0
    cac_Status,       // get current async state
    cac_Abort,        // abort current state, if complete - return 's'/'c'
    cac_Last
  };
  //
  enum eConsoleCommand {
    cc_Cursor,      // +dx +dy
    cc_CursorEnd,   // dirx, diry
    cc_CursorSel,   // +dx +dy
    cc_CursorSelEnd,// dirx, diry
    cc_Scroll,      // +dx, +dy 
    cc_ScrollTo,    // x, y
    cc_VisiblePoint,// x, y
    cc_Delete,      // num
    cc_Copy,        // is-cut
    cc_Paste,       // is-cut
    cc_Accept,      // accept=1/abort=0
    cc_CharC,       // char8
    cc_CharW,       // char16
    cc_CharU,       // char32
    cc_Last
  };

  //
public:
  // async
  virtual int async(eConsoleAsyncCommand cmd=cac_Status) =0; 
  virtual bool async_vget_c(int* ch, const char *fmt, va_list va) =0;
  virtual bool async_get_c(int* ch, const char *fmt, ...) =0;
  virtual bool async_vget_s(char* buf, size_t len, const char *fmt, va_list va) =0;
  virtual bool async_get_s(char* buf, size_t len, const char *fmt, ...) =0;
  //
  // helper functions
  virtual int command(eConsoleCommand cmd, int x, int y=0) =0;
  virtual bool set_colorinfo(ColorInfoAtom start, ColorInfoAtom count, const SColorInfo* inf) =0;
  virtual bool set_helper(IConsoleHelper* h, eConsoleHelperType cht) =0;
  virtual IConsoleHelper* get_helper(eConsoleHelperType cht) =0;
public:
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IConsoleExt, 1, IConsole);
};

//***************************************
// IConsoleHelper
//***************************************

struct IConsoleHelper : public core::IRefObject {
public:
  virtual bool link(IConsoleExt *cext) = 0;   
  // private parameters setup
  virtual bool set_parameters(const core::SParameters& params) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IConsoleHelper, 1, core::IRefObject);
};

//***************************************
// IConsoleHelperHistory
//***************************************

struct IConsoleHelperHistory : public IConsoleHelper {
public:
  virtual bool append(core::BCStr line) =0;
  virtual core::BCStr get(int index) =0;
  virtual int count(void) =0;
  virtual int find(core::BCStr line, bool partial=false) =0;
/*
  virtual bool load(IMediaReader* file, int changedsize=0) = 0;
  virtual bool save(IMediaWriter* file) = 0;
*/
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IConsoleHelperHistory, 1, IConsoleHelper);
}; 

//***************************************
// IConsoleHelperInput
//***************************************

struct IConsoleHelperInput : public IConsoleHelper {
public:
  virtual bool process(void) =0;               // process input
  virtual bool process_wait(void) =0;          // pause until input
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IConsoleHelperInput, 1, IConsoleHelper);
};

//***************************************
// IConsoleHelperOutput
//***************************************

struct IConsoleHelperOutput : public IConsoleHelper {
public:
  typedef IConsoleExt::ColorInfoAtom ColorInfoAtom;
  typedef IConsoleExt::SColorInfo SColorInfo;
public:
  // setup console host
  virtual bool update(void) =0;             // flag that console need to be redrawn
  virtual bool draw(void) =0;               // forced redraw
  // selection of color index, char index, return 0 if fails
  virtual bool map_colorinfo(ColorInfoAtom start, ColorInfoAtom count, const SColorInfo* inf) =0;
  // character/draw operation
  virtual bool put_chars(int x, int y, core::BCStrL cha, int count, const ColorInfoAtom* chc) =0;
  virtual bool put_chars(int x, int y, core::BCStrL cha, int count, ColorInfoAtom chc, int cursor=-1) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IConsoleHelperOutput, 1, IConsoleHelper);
};

//***************************************
// IConsoleHelperOutputFCS
//***************************************

struct IConsoleHelperOutputFCS : public IConsoleHelperOutput {
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::base::IConsoleHelperOutputFCS, 1, IConsoleHelperOutput);
};


//***************************************
// END
//***************************************

} // namespace base
} // namespace UModSys

#endif // __UMODSYS_BASE_CONSOLEEXT_H
