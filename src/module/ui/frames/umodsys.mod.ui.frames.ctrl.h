//------------------------------------
// common controls
//------------------------------------

struct RFrame_Common : public IFrame {
  // controller functions
  bool set_controller(int context, IController *actrl, IRefObject *acc) { 
    return ctrl.set(context, actrl, acc) && controller_update(); 
  }
  IController* get_controller(void) { return ctrl; }
  int get_controller_context(void) { return ctrl; }
  bool controller_update(void) { return true; }
  // visual
  bool set_flag(int flagid, bool flag);
  bool get_flag(int flagid);
  const lib2d::DBox& get_box(void) { return size; }
  const SFrameSize& get_framesize(void) { return fs; }
  bool set_framesize(const SFrameSize& size) { fs = size; size_updated=true; return true; }
  lib2d::DPoint get_insize(void) { return size.size(); }
  bool set_insize(const lib2d::DPoint& size) { return false; }
  // get special interfaces
  ICollectorTheme* get_up_theme(void);
  bool configure(BCStr group, BCStr mode, const SParameters& params);
  // repeater
  bool set_repeater(int rid, int timemsec);
  // tree
  ICollector* get_collector(void);
  IFrame* get_parent(void);
  IFrame* get_sub(IFrame* cur, int skip);
  ICollectorTheme* get_theme(void) { return &theme; }
  IFrame* find_context(int context, IController *ctrl);
protected:
  friend struct RCollector;
  friend struct RDialog;
  friend struct SButton;
  //
  RCollectorTheme theme;
  lib2d::DBox size;
  SFrameSize fs;
  bool updated, visible, readonly, active, enabled, selected, size_updated, focused; 
  bool user_db, user_draw, use_mouse, use_keys, use_textinput;
  int mouse_inside;
  SController ctrl;
  RFrame_Common *up;
  RCollector *owner;
  RDialog* dialog;
  Frames frames;
  PFont font;
  //
  void mouse_seq(void);
  //
  virtual bool mouse_event(const SMouseInput* ms) { return false; }
  virtual bool key_event(const SKeyboardInputRaw& key) { return false; }
  virtual bool key_event(const SKeyboardInputText& key) { return false; }
  //
  virtual bool draw(const SDrawState& S);
  virtual bool draw_sub(const SDrawState& S);
  virtual bool draw_pre(const SDrawState& S) { return true; }
  virtual bool draw_post(const SDrawState& S) { return true; }
  virtual bool draw_focus(const SDrawState& S);
  //
  virtual bool button_cmd(SButton *bt, int cmd, int arg) { return true; }
  virtual bool button_draw(SButton *bt, const SDrawState& S, int flags) { return true; }
  //
  virtual bool set_focus(bool flag=true);
  //
  RFrame_Common* cont_prev(RFrame_Common* up_limit=NULL);
  RFrame_Common* cont_next(RFrame_Common* up_limit=NULL);
  //
  bool get_clr(const SParameters* args, lib2d::DColorAlphaf &color, BCStr prefix) { return lib2d::_util_getcolor(*args, color, prefix); }
  //
  lib2d::DColorAlphaf tclr(int g, int idx) { lib2d::DColorAlphaf rv(0.0); theme.get_property(g, idx, rv); return rv; }
  lib2d::DPointf tptr(int g, int idx) { lib2d::DPointf rv(0.0); theme.get_property(g, idx, rv); return rv; }
  //
  virtual bool set_fcp(const SFrameCreateParameters& fcp, const SParameters* args);
  virtual bool update_size(const lib2d::DBox& base);
  virtual bool is_enabled(void);
public:
  //
  RFrame_Common(RCollector *own, RFrame_Common *u);
  ~RFrame_Common(void);
  //
protected:
  UMODSYS_REFOBJECT_INTIMPLEMENT(UMODSYS_MODULE_NAME(ui,frames)::RFrame_Common, 2, IFrame);
};

//***************************************
// RFrame_Common::
//***************************************

bool RFrame_Common::set_flag(int flagid, bool flag)
{
  switch(flagid) {
    case ff_Active: active = flag; break;
    case ff_Hidden: visible = !flag; break;
    case ff_Disabled: enabled = !flag; break;
    case ff_Indicator: readonly = flag; break;
    case ff_Selected: selected = flag; break;
    case ff_Focused: set_focus(flag); break;
    case ff_Focused|ff_Internal: focused = flag; break;
    default:
      return false;
  }
  updated = true;
  return true;
}

bool RFrame_Common::get_flag(int flagid)
{
  switch(flagid) {
    case ff_Active: return active;
    case ff_Hidden: return !visible;
    case ff_Disabled: return !enabled;
    case ff_Indicator: return readonly;
    case ff_Selected: return selected;
    case ff_Focused: return focused;
  }
  return false;
}

bool RFrame_Common::set_repeater(int rid, int timemsec)
{
  return false;
}

IFrame* RFrame_Common::get_parent(void)
{
  return up;
}

IFrame* RFrame_Common::get_sub(IFrame* cur, int skip)
{
  return NULL;
}

IFrame* RFrame_Common::find_context(int context, IController *cp)
{
  if(ctrl.context==context && (cp==NULL || ctrl.ctrl==cp))
    return this;
  for(int i=0, n=~frames; i<n; i++) {
    IFrame *rv = frames[i]->find_context(context, cp);
    if(rv)
      return rv;
  }
  return NULL;
}

bool RFrame_Common::configure(BCStr group, BCStr mode, const SParameters& params)
{
  if(tl::su::seq(group, "view")) {
    if(tl::su::seq(mode, "size")) {
      int tmp;
      if(params.get("valign", tmp)) {
        fs.v = lib2d::eAlign(tmp);
        size_updated=true;
      }
      if(params.get("halign", tmp)) {
        fs.h = lib2d::eAlign(tmp);
        size_updated=true;
      }
      return true;
    }
    return false;
  }
  return false;
}

//***************************************

bool RFrame_Common::draw(const SDrawState& S)
{
  if(!visible || dialog!=S.dialog)
    return true;
  return draw_pre(S) && draw_sub(S) && draw_post(S) && (focused ? draw_focus(S) : true);
}

bool RFrame_Common::draw_focus(const SDrawState& S)
{
  S.driver->setup_coloralpha( tclr(ctg_General, cte_Focused|cte_Edges) );
  S.driver->render_rect(S.base.A - lib2d::DPoint(1,1), S.base.B + lib2d::DPoint(1,1));
  return true;
}

//***************************************

//***************************************

//***************************************

bool RFrame_Common::update_size(const lib2d::DBox& base)
{
  libui::update_size(size, fs, base);
  controller_update();
  size_updated = false;
  return true;
}

bool RFrame_Common::set_fcp(const SFrameCreateParameters& fcp, const SParameters* args)
{
  fs = fcp.size;
  size_updated = true;
  //
  ctrl.set(fcp);
  //
  IRefObject *ro;
  if(args && args->get("font", ro) && ro!=NULL)
    ro->t_ref_get_other_interface(font);
  //
  return true;
}

bool RFrame_Common::is_enabled(void)
{
  return enabled && visible && (up==NULL || dialog!=NULL || up->is_enabled());
}

//***************************************

RFrame_Common::RFrame_Common(RCollector *own, RFrame_Common *u)
: size(0,0, 0,0), ctrl(this), up(u), owner(own), theme(this)
{
  updated = true;
  visible = true;
  active = false;
  readonly = false;
  enabled = true;
  selected = false;
  focused = false;
  //
  user_db = false;
  user_draw = false;
  mouse_inside = 0;
  //
  use_mouse = false;
  use_keys = false;
  use_textinput = false;
  size_updated = true;
  //
  dialog = NULL;
}

RFrame_Common::~RFrame_Common(void)
{
}

