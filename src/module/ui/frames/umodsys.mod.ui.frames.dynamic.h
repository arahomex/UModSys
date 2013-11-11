//------------------------------------
// controls
//------------------------------------

// button

struct RFrame_Button : public RFrame_Common {
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(ui,frames)::RFrame_Button, 2, RFrame_Common)
  UMODSYS_REFOBJECT_REFOTHER(RCollector)
  //
  // id functions (what interface is supported)
  DCString get_cids(void) { return "text"; }
  int get_cidn(void) { return cidmask(cid_Text); }
  // get special interfaces
  IFrameList *get_list(void) { return NULL; }
  IFrameData *get_data(void) { return NULL; }
  IControllerData *get_ctrldata(void) { return NULL; }
  IControllerCommand *get_ctrlcmd(void) { return NULL; }
  // frame special operation (FRAMES)
  //
public:
  RFrame_Button(RCollector *pv, RFrame_Common *up);
  ~RFrame_Button(void);
protected:
  bool key_event(const SKeyboardInputRaw& key);
  bool mouse_event(const SMouseInput* ms);
  //
  bool set_fcp(const SFrameCreateParameters& fcp, const SParameters* args);
  bool draw_post(const SDrawState& S);
  bool update_size(const lib2d::DBox& base);
  //
  bool button_cmd(SButton *bt, int cmd, int arg);
  bool button_draw(SButton *bt, const SDrawState& S, int flags);
  //
  DStringShared text;
  SButton button;
};

//***************************************
// RFrame_Button::
//***************************************

bool RFrame_Button::button_cmd(SButton *bt, int cmd, int arg)
{
  set_focus(true);
  return ctrl.command(cmd, arg);
}

bool RFrame_Button::button_draw(SButton *bt, const SDrawState& S, int flags)
{
  DCString text;
  if(user_db) {
    ctrl.db_get("text", text);
  } else {
    text = this->text;
  }
  //
  if(selected && !(flags & cte_Disabled))
    flags |= cte_Selected;
  //
  bt->draw_button(S, ctg_Button, flags);
  bt->draw_text(S, text, ctg_Button, flags);
  return true;
}

//***************************************

bool RFrame_Button::key_event(const SKeyboardInputRaw& key)
{
  if(key.status==bis_Pressed) {
    switch(key.code) {
      case k_return:
      case k_space:
        return ctrl.command(cmd_Pressed, -1);
    }
  }
  return false;
}

bool RFrame_Button::mouse_event(const SMouseInput* ms)
{
  if(button.mouse(ms))
    return true;
  return false;
}

bool RFrame_Button::draw_post(const SDrawState& S)
{
  button.draw(S);
  return true;
}

bool RFrame_Button::update_size(const lib2d::DBox& base)
{
  RFrame_Common::update_size(base);
  button.update_size(size);
  return true;
}

//***************************************

bool RFrame_Button::set_fcp(const SFrameCreateParameters& fcp, const SParameters* args)
{
  if(!RFrame_Common::set_fcp(fcp, args))
    return false;
  text = fcp.text;
  if(args) {
    args->get("user_db", user_db);
  }
  return true;
}

//***************************************

//UMS_REFOBJECT_RIMPLEMENT_BODY(UI::RFrame_Button, RCollector)

RFrame_Button::RFrame_Button(RCollector *pv, RFrame_Common *up)
: RFrame_Common(pv, up), refs(pv),
  button(this, 0, SFrameSize(0, 0, lib2d::a_Justify, lib2d::a_Justify))
{
  use_mouse = use_keys = true;
}

RFrame_Button::~RFrame_Button(void)
{
}

//***************************************
// ::
//***************************************

