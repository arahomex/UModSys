#ifndef __UMODSYS_LIBUI_FRAMES_H
#define __UMODSYS_LIBUI_FRAMES_H 1
/*************************************************************/
// file: umodsys/lib/libui/libui.frames.h
// info: ui frames interface
/*************************************************************/

#include <umodsys/lib/ui/libui.common.h>
#include <umodsys/lib/2d/lib2d.typedefs.h>
#include <umodsys/lib/3d/lib3d.typedefs.h>

namespace UModSys {
namespace libui {

//***************************************
// FORWARD/TYPEDEF
//***************************************

struct SController;
struct SFrameSize;

struct SFrameDataIn;
struct SFrameDataOut;

struct IControllerCommand;
struct IControllerData;
struct IController;
struct ICollector;

struct IAsyncController;
struct IAsyncCollector;
struct IAsyncCollectorCreator;

struct ICollectorThemeTree;
struct ICollectorTheme;

struct IFrame;
struct IDialog;

struct IFrameData;
struct IFrameList;

//***************************************
// ENUM/CONST
//***************************************

enum eFrameFlag {
  ff_Active,
  ff_Hidden,
  ff_Disabled,
  ff_Indicator,
  ff_Selected,
  ff_Focused,
  //
  ff_Internal = 0x0100,
  //
  ff_Last
};

enum eClassId {
  cid_None,             // NULL
  cid_Frame,            // general group
  cid_Menu,             // menu
  cid_MenuItem,         // menu line
  cid_Button,           // press button
  cid_Text,             // static text
  cid_Picture,          // picture
  cid_UserDraw,         // user draw element
  cid_Edit,             // editable text line
  cid_EditBox,          // editable text multiline
  cid_RadioGroup,       // group of one-selection
  cid_CheckGroup,       // group of bit-selection
  cid_List,             // normal list
  cid_DropList,         // droppable list
  cid_TabControl,       // tab control
  cid_TabItem,          // tab item
  cid_Unknown
};

#define UMODSYS_UI_CID_MASK(cid) (1<<(cid))
inline int cidmask(eClassId cid) { return 1<<cid; }

enum eFrameValueId {
  fvi_Null,
  fvi_Default,
  fvi_NumberId,
  fvi_Text,
  fvi_Value,
  fvi_RadioValue,
  fvi_CheckValue,
  fvi_ListSelection,
  fvi_ListCurrent, // int
  fvi_ButtonText,
  //
  fvi_Last
};

enum eFrameCommands {
  cmd_Null,       // null (dummy) command
  cmd_Enter,      // gain/lost focus
  cmd_HotLight,   // hotlight element (mouse over)
  cmd_Pressed,    // usualy buttons
  cmd_Changed,    // edit control char, checkbox press, etc.
  cmd_HideFrame,  // frame is closed/hidden
  cmd_ListDropped,// drop list opened/closed
  cmd_Repeater,   // repeater (timer)
  //
  cmd_Last = 0xff
};

enum eFrameDataType {
  fdt_Null,        // no command
  fdt_Integer,     // integer type
  fdt_Real,        // float type
  fdt_String,      // string type
  fdt_SString,     // shared string type
  fdt_Parameters,  // parameter list type
  fdt_Unknown,     // unrecognized type
  fdt_Last
};

//***************************************
// TYPES
//***************************************

//***************************************
// SFrameDataIn, SFrameDataOut

struct SFrameDataIn {
  eFrameDataType type;
  //
  union Data {
    const DStringShared *ssval;
    const DCString *sval;
    const SParameters* pval;
    const int *ival;
    const double *rval;
  } data;
  //
  SFrameDataIn(const DCString& v) : type(fdt_String) { data.sval=&v; }
  SFrameDataIn(const DStringShared &v) : type(fdt_SString) { data.ssval=&v; }
  SFrameDataIn(const int& v) : type(fdt_Integer) { data.ival = &v; }
  SFrameDataIn(const double& v) : type(fdt_Real) { data.rval = &v; }
  SFrameDataIn(const SParameters& v) : type(fdt_Parameters) { data.pval = &v; }
  SFrameDataIn(void) : type(fdt_Null) {}
  //
  const Data* operator->(void) const { return &data; }
  //
  inline int i(void) const { return *data.ival; }
  inline const double& r(void) const { return *data.rval; }
  inline const SParameters& p(void) const { return *data.pval; }
  inline const DCString& s(void) const { return *data.sval; }
  inline const DStringShared& ss(void) const { return *data.ssval; }
};

struct SFrameDataOut {
  eFrameDataType type;
  //
  union Data {
    DStringShared *ssval;
    DCString *sval;
    SParameters* pval;
    int *ival;
    double *rval;
  } data;
  //
  inline SFrameDataOut(DCString& v) : type(fdt_String) { data.sval = &v; }
  inline SFrameDataOut(DStringShared& v) : type(fdt_SString) { data.ssval = &v; }
  inline SFrameDataOut(int& v) : type(fdt_Integer) { data.ival = &v; }
  inline SFrameDataOut(double& v) : type(fdt_Real) { data.rval = &v; }
  inline SFrameDataOut(SParameters& v) : type(fdt_Parameters) { data.pval = &v; }
  inline SFrameDataOut(void) : type(fdt_Null) {}
  //
  inline const Data* operator->(void) const { return &data; }
  //
  inline void i(int v) const { *data.ival = v; }
  inline void r(const double& v) const { *data.rval = v; }
  inline SParameters& p(void) const { return *data.pval; }
  inline void s(const DCString& v) const { *data.sval = v; }
  inline void ss(const DStringShared& v) const { *data.ssval = v; }
  inline void s(const BCStr v) const { *data.sval = v; }
  inline void ss(const BCStr v) const { *data.ssval = v; }
};

//***************************************
// IControllerCommand, IControllerData

struct IControllerCommand {
  virtual bool command(const SController& ci, int command, const SFrameDataIn& in) =0;
};

struct IControllerData {
  virtual bool db_get(const SController& ci, const BCStr hint, const SFrameDataOut& out) = 0;
  virtual bool db_get(const SController& ci, const BCStr hint, int sid, const SFrameDataOut& out) = 0;
public:
public:
};

//***************************************
// IController, SController

struct IController : public IRefObject, public IControllerCommand, public IControllerData {
public:
  virtual bool command_draw(const SController& ci, lib2d::IRenderDriver *drv, const lib2d::DBox &bounds) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libui::IController, 2, IRefObject);
};

struct SController {
  int context;
  IFrame *source;
  tl::TRefObject<IController> ctrl;
  //
  inline SController(IFrame *s) : source(s), context(0) {}
  inline SController(IFrame *s, int ctx, IController *c) : source(s), context(ctx), ctrl(c) {}
  inline ~SController(void) {}
  //
  inline bool set(int cx2, IController *c2) { ctrl = c2; context = cx2; return true; }
  inline operator IController*(void) const { return ctrl; }
  inline operator IFrame*(void) const { return source; }
  inline operator int(void) const { return context; }
  inline bool valid(void) { return ctrl.valid(); }
  inline IFrame* operator->(void) const { return source; }
  //
  bool command(int cmd, const SFrameDataIn& in);
  bool db_get(const BCStr hint, const SFrameDataOut& out);
  bool db_get(const BCStr hint, int sid, const SFrameDataOut& out);
  bool draw(lib2d::IRenderDriver *drv, const lib2d::DBox &bounds);
  //
  inline int db_get_i(const BCStr hint, int sid) {
    int rv;
    if(db_get(hint, sid, rv))
      return rv;
    return 0;
  }
  inline int db_get_i(const BCStr hint) {
    int rv;
    if(db_get(hint, rv))
      return rv;
    return 0;
  }
  //
  inline DCString db_get_s(const BCStr hint, int sid) {
    DCString rv;
    if(db_get(hint, sid, rv))
      return rv;
    return 0;
  }
  inline DCString db_get_s(const BCStr hint) {
    DCString rv;
    if(db_get(hint, rv))
      return rv;
    return 0;
  }
};

//***************************************
// ICollectorTheme, ICollectorThemeTree

struct ICollectorTheme {
  virtual int get_group_index(BCStr name) =0;
  virtual int get_elem_index(BCStr name) =0;
  //
  // get propeties
  //
  // color properties
  virtual bool get_property(int group_idx, int elem_idx, lib2d::DColorAlphaf& elem) =0; 
  virtual bool get_property(int group_idx, int elem_idx, lib2d::DColorAlpha& elem) =0; 
  // size property
  virtual bool get_property(int group_idx, int elem_idx, lib2d::DPointf& elem) =0;
  virtual bool get_property(int group_idx, int elem_idx, lib2d::DPoint& elem) =0;
  // flags property
  virtual bool get_property(int group_idx, int elem_idx, int& elem) =0; // flags
  //
  // set propeties
  //
  // color properties
  virtual bool set_property(int group_idx, int elem_idx, const lib2d::DColorAlphaf& elem) =0; 
  virtual bool set_property(int group_idx, int elem_idx, const lib2d::DColorAlpha& elem) =0; 
  // size property
  virtual bool set_property(int group_idx, int elem_idx, const lib2d::DPointf& elem) =0;
  virtual bool set_property(int group_idx, int elem_idx, const lib2d::DPoint& elem) =0;
  // flags property
  virtual bool set_property(int group_idx, int elem_idx, int elem) =0; // flags
  //
  //
  //
  inline bool set_byname(BCStr gname, BCStr ename, const lib2d::DColorAlphaf& elem) {
    int i1 = get_group_index(gname);
    int i2 = get_elem_index(ename);
    return set_property(i1, i2, elem);
  }
  inline bool set_byname(BCStr gname, BCStr ename, const lib2d::DPointf& elem) {
    int i1 = get_group_index(gname);
    int i2 = get_elem_index(ename);
    return set_property(i1, i2, elem);
  }
  inline bool set_byname(BCStr gname, BCStr ename, int elem) {
    int i1 = get_group_index(gname);
    int i2 = get_elem_index(ename);
    return set_property(i1, i2, elem);
  }
};

struct ICollectorThemeTree {
  virtual ICollectorTheme* get_up_theme(void) =0;
};

//***************************************
// SFrameSize. SFrameCreateParameters

struct SFrameSize {
  lib2d::eAlign h, v;
  lib2d::DPoint start, size;
  //
  inline SFrameSize(void) 
  : start(0,0), size(0,0), h(lib2d::a_Left), v(lib2d::a_Left) {
  }
  inline SFrameSize(const lib2d::DPoint &a, const lib2d::DPoint &b, lib2d::eAlign ah, lib2d::eAlign av) 
  : start(a), size(b), h(ah), v(av) {
  }
};

struct SFrameCreateParameters {
  IFrame* parent;
  DCString text;
  int ctrl_context;
  IController *ctrl;
  SFrameSize size;
  //
  inline SFrameCreateParameters(void) 
  : parent(NULL), ctrl(NULL), ctrl_context(0) {
  }
};

//***************************************
// ICollector

struct ICollector : public IRefObject, public ICollectorThemeTree {
public:
  // general
  virtual bool shutdown(void) = 0; 
  virtual ICollectorTheme *get_theme(void) =0;
  //
  // frame processing
  virtual IFrame* frame_create_n(int cid, const SFrameCreateParameters& fcp, const SParameters* args) =0;
  virtual IFrame* frame_create_s(const DCString& cid, const SFrameCreateParameters& fcp, const SParameters* args) =0;
  virtual bool frame_remove(IFrame* elem) =0; // remove frame from it's parent
  //
  // dialog processing
  virtual IDialog* dialog_open(IFrame* root) =0; // create new dialog in stack
  virtual IDialog* dialog_get(void) =0; // get top dialog
  virtual bool dialog_close(void) =0; // close dialog
  virtual bool dialog_close_all(void) =0; // close all dialogs
  //
  // output
  virtual bool output_process(void) = 0;
  virtual bool output_attach(ITerminal* term, BCStr name, const SParameters* params, int level) =0;
  virtual bool output_attach_2d(lib2d::IRenderDriver* drv) =0;
  virtual bool output_attach_3d(lib3d::IRenderDriver* drv) =0;
  virtual bool get_maxbox(lib2d::DBox& box) =0;
  virtual bool get_minsize(lib2d::DPoint &size) =0;
  //
  // input
  virtual bool input_attach(ITerminal* term, BCStr name, const SParameters* params, int level) =0;
  virtual bool input_process(int quants=1) =0;
  virtual int broadcast(int command, const SFrameDataIn& in, int ctx=0) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libui::ICollector, 2, IRefObject);
};

//***************************************
// IAsyncController, IAsyncCollector

struct IAsyncController : public IRefObject {
  // context<0 force control to be cleared
  virtual int remote_process(int context, ICollector *col) =0; // for async create/destroy
  virtual bool control_cmd(const DCString& cmd, const SParameters* args) = 0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libui::IAsyncController, 2, IRefObject);
};

struct IAsyncCollector : public IRefObject {
public:
  virtual bool start_remote(IAsyncController *cr, int context) =0;
  virtual bool finish_remote(IAsyncController *cr, int context, int *res, bool waitinfinite=false) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libui::IAsyncCollector, 2, IRefObject);
};

//***************************************
// IFrameList, IFrameData, IFrame

struct IFrameList {
  // list special operation (LISTS)
  virtual bool set_lcount(int count) =0;
  virtual int  get_lcount(void) =0;
  virtual bool set_lvalue(int index, int kind, const SFrameDataIn& val) =0;
  virtual bool get_lvalue(int index, int kind, const SFrameDataOut& val) =0;
};

struct IFrameData {
  // value functions
  virtual bool set_value(int kind, const SFrameDataIn& val) =0;
  virtual bool get_value(int kind, const SFrameDataOut& val) =0;
};

struct IFrame : public IRefObject, public ICollectorThemeTree {
public:
  // id functions (what interface is supported)
  virtual DCString get_cids(void) =0;
  virtual int get_cidn(void) =0;
  // get special interfaces
  virtual IFrameList *get_list(void) = 0; // get list interface (valid only for some frames)
  virtual IFrameData *get_data(void) = 0; // get data interface (valid only for some frames)
  virtual IControllerData *get_ctrldata(void) = 0; // get control data interface (valid only for some frames)
  virtual IControllerCommand *get_ctrlcmd(void) = 0; // get control command interface (valid only for some frames)
  virtual bool configure(BCStr group, BCStr mode, const SParameters& params) =0;
  // controller functions
  virtual bool set_controller(int context, IController *ctrl) =0;
  virtual IController* get_controller(void) =0;
  virtual int get_controller_context(void) =0;
  virtual bool controller_update(void) =0; // update data caches
  // visual
  virtual bool set_flag(int flagid, bool flag) =0;
  virtual bool get_flag(int flagid) =0;
  virtual const lib2d::DBox& get_box(void) =0;
  virtual const SFrameSize& get_framesize(void) =0;
  virtual bool set_framesize(const SFrameSize& size) =0;
  virtual lib2d::DPoint get_insize(void) =0;
  virtual bool set_insize(const lib2d::DPoint& size) =0;
  // repeater
  virtual bool set_repeater(int rid, int timemsec=0) =0; // repeater (timer) command, =0 to turn off
  // tree
  virtual ICollector* get_collector(void) =0;
  virtual IFrame* get_parent(void) =0;
  virtual IFrame* get_sub(IFrame* cur=NULL, int skip=1) =0;
  virtual ICollectorTheme* get_theme(void) =0;
  virtual IFrame* find_context(int context, IController *ctrl=NULL) =0;
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libui::IFrame, 2, IRefObject);
};

//***************************************
// IDialog

struct IDialog : public IRefObject {
  virtual bool close(void) =0; // close dialog
  virtual IFrame* get_root(void) = 0; // get root dialog frame
  virtual IFrame* get_focused(void) = 0; // get kbd&mouse focused frame
  virtual bool set_focused(IFrame* fr) = 0; // set kbd&mouse focused frame
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UModSys::libui::IDialog, 2, IRefObject);
};

//***************************************
// INLINES/OUTLINES
//***************************************

inline bool update_size(lib2d::DBox &size, const SFrameSize &fs, const lib2d::DBox& base)
{
  int c, s;
  switch(fs.h) {
    case lib2d::a_Left:
      size.A[0] = fs.start(0);
      size.B[0] = fs.start(0) + fs.size(0) - 1;
      break;
    case lib2d::a_Right:
      s = base.sizex();
      size.A[0] = s - fs.start(0) - fs.size(0);
      size.B[0] = s - fs.start(0) - 1;
      break;
    case lib2d::a_Center:
      s = fs.size(0);
      c = (base.sizex() - s)/2;
      size.A[0] = c + fs.start(0);
      size.B[0] = c + fs.start(0) + s - 1;
      break;
    case lib2d::a_Justify:
      size.A[0] = fs.start(0);
      size.B[0] = base.sizex() - fs.size(0) - 1;
      break;
  }
  switch(fs.v) {
    case lib2d::a_Left:
      size.A[1] = fs.start(1);
      size.B[1] = fs.start(1) + fs.size(1) - 1;
      break;
    case lib2d::a_Right:
      s = base.sizey();
      size.A[1] = s - fs.start(1) - fs.size(1);
      size.B[1] = s - fs.start(1) - 1;
      break;
    case lib2d::a_Center:
      s = fs.size(1);
      c = (base.sizey() - s)/2;
      size.A[1] = c + fs.start(1);
      size.B[1] = c + fs.start(1) + s - 1;
      break;
    case lib2d::a_Justify:
      size.A[1] = fs.start(1);
      size.B[1] = base.sizey() - fs.size(1) - 1;
      break;
  }
  return true;
}

//***************************************
// END
//***************************************

} // namespace libui
} // namespace UModSys

#endif // __UMODSYS_LIBUI_FRAMES_H
