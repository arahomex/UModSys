#define cidmask UMODSYS_UI_CID_MASK

//------------------------------------
// Collector
//------------------------------------

struct RCollector : public ICollector, public IKeyboardClient, public IMouseClient {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RCollector, 2, ICollector)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  IFrame* frame_create_n(int cid, const SFrameCreateParameters& fcp, const SParameters* args);
  IFrame* frame_create_s(const DCString& cid, const SFrameCreateParameters& fcp, const SParameters* args);
  bool frame_remove(IFrame* elem);
  bool shutdown(void);
  //
  int broadcast(int command, const SFrameDataIn& in, int ctx=0) { return broadcast(frames, command, in, ctx); }
  bool input_process(int quants);
  bool input_attach_m(IMouseController *m, int level);
  bool input_attach_k(IKeyboardController *m, int level);
  ICollectorTheme *get_theme(void) { return &theme; }
  //
  bool get_maxbox(lib2d::DBox& box);
  bool get_minsize(lib2d::DPoint &size);
  bool output_process(void);
  bool output_attach_2d(lib2d::IRenderDriver* drv, int level);
  bool output_attach_3d(lib3d::IRenderDriver* drv, int level);
  //
  IDialog* dialog_open(IFrame* root);
  IDialog* dialog_get(void);
  bool dialog_close(void);
  bool dialog_close_all(void);
  //
  //
  // IMouseClient
  bool mouse_event(const SMouseInput& ms);
  //
  // IKeyboardClient
  bool key_pressed(const SKeyboardInputRaw& key);
  bool key_text(const SKeyboardInputText& key);
  //
  // ICollectorThemeReturn
  ICollectorTheme* get_up_theme(void) { return NULL; }
  //
  RCollector(DOwner *pv, const SParameters& args);
  ~RCollector(void);
  //
  bool valid;
  bool validate_construction(void) { return valid; }
protected:
  friend struct RFrame_Common;
  friend struct RDialog;
  //
  RCollectorTheme theme;
  tl::TDynarrayDynamic< RDialog::SelfP > dialogs;
  PFont def_font;
  lib2d::IRenderDriver::P driver;
  lib2d::DBox size;
  lib2d::DPoint cached_start, cached_size;
  //
  ITerminal::P terminal;
  IKeyboardController::P ckeyboard;
  IMouseController::P cmouse;
  IFrame::P focused;
  int mouse_inside_seq;
  //
  bool update_size(void);
  bool update_size(const lib2d::DBox& base, const Frames& list, bool force);
  bool insert(RFrame_Common* master, RFrame_Common *slave);
  bool remove(RFrame_Common* master, RFrame_Common *slave);
  bool draw(const SDrawState& state, const Frames& list);
  int broadcast(const Frames& list, int command, const SFrameDataIn& in, int ctx);
  bool evmouse(const lib2d::DBox& base, const Frames& list, const SMouseInput& ms, RFrame_Common* root);
  bool evmouse(const Frames& list); // mouse lost
  bool set_focused(IFrame* fr);
  RFrame_Common* get_focused(void);
  //
  RFrame_Common* find_next_focused(RFrame_Common* root, RFrame_Common* focused, bool forward=true);
  RFrame_Common* cont_first(RFrame_Common* root);
  RFrame_Common* cont_last(RFrame_Common* root);
  //
  bool master_key_pressed(const SKeyboardInputRaw& key);
  //
  bool do_focused(RFrame_Common* fr, bool flag, bool tix) {
    if(ckeyboard)
      return ckeyboard->key_setvisible(flag && tix);
    return false;
  }
  //
  Frames frames;
};

//***************************************
// RCollector::
//***************************************

IFrame* RCollector::frame_create_n(int cid, const SFrameCreateParameters& fcp, const SParameters* args)
{
  RFrame_Common::P rv = NULL;
  RFrame_Common *parent = NULL;
  if(fcp.parent) {
    if(!fcp.parent->t_root_get_other_interface(parent) || parent->owner!=this)
      return false;
  }
  switch(cid) {
    case cidmask(cid_Frame):
      rv = new RFrame_Frame(this, parent);
      break;
    case cidmask(cid_Text):
      rv = new RFrame_Text(this, parent);
      break;
    case cidmask(cid_Picture):
      rv = new RFrame_Image(this, parent);
      break;
    case cidmask(cid_Button):
      rv = new RFrame_Button(this, parent);
      break;
    case cidmask(cid_List):
      rv = new RFrame_List(this, parent);
      break;
    case cidmask(cid_Edit):
      rv = new RFrame_Edit(this, parent);
      break;
    default:
      return NULL;
  }
  if(!rv->set_fcp(fcp, args) || !insert(parent, rv)) {
    rv.clear();
  }
  return rv;
}

IFrame* RCollector::frame_create_s(const DCString& cid, const SFrameCreateParameters& fcp, const SParameters* args)
{
  RFrame_Common::P rv = NULL;
  RFrame_Common *parent = NULL;
  if(fcp.parent) {
    if(!fcp.parent->t_root_get_other_interface(parent) || parent->owner!=this)
      return false;
  }
  if(cid=="frame" || cid=="group" || cid=="") {
    rv = new RFrame_Frame(this, parent);
  } else if(cid=="text") {
    rv = new RFrame_Text(this, parent);
  } else if(cid=="image" || cid=="picture") {
    rv = new RFrame_Image(this, parent);
  } else if(cid=="list") {
    rv = new RFrame_List(this, parent);
  } else if(cid=="button") {
    rv = new RFrame_Button(this, parent);
  } else if(cid=="edit") {
    rv = new RFrame_Edit(this, parent);
  } else {
    return NULL;
  }
  if(!rv->set_fcp(fcp, args) || !insert(parent, rv)) {
    rv.clear();
  }
  return rv;
}

bool RCollector::frame_remove(IFrame* elem)
{
  return false;
}

//***************************************

bool RCollector::get_maxbox(lib2d::DBox& box)
{
  return false;
}

bool RCollector::get_minsize(lib2d::DPoint &size)
{
  return false;
}

bool RCollector::output_process(void)
{
  update_size();
  driver->setup_font(def_font);
  driver->setup_color(lib2d::DColorf(1,1,1));
  draw( SDrawState(def_font, driver, size), frames );
  for(int i=0, n=~dialogs; i<n; i++) {
    RDialog *d = dialogs[i];
    RFrame_Common *c = d->root;
    d->root->draw( SDrawState(c->font ? c->font : def_font, driver, c->size, d) );
  }
  return true;
}

bool RCollector::output_attach_2d(lib2d::IRenderDriver* drv, int level)
{
  driver = drv;
  return false;
}

bool RCollector::output_attach_3d(lib3d::IRenderDriver* drv, int level)
{
//  driver = drv;
  return false;
}

//***************************************

bool RCollector::input_process(int quants)
{
  if(terminal)
    terminal->poll_events(quants);
  return true;
}

bool RCollector::input_attach_m(IMouseController *m, int level)
{
  if(cmouse)
    cmouse->mouse_unlink(0);
  cmouse = m;
  if(cmouse) {
    cmouse->mouse_setvisible(true);
    cmouse->mouse_link(level, this, this, ~0);
  }
  return true;
}

bool RCollector::input_attach_k(IKeyboardController *m, int level)
{
  if(ckeyboard)
    ckeyboard->key_unlink(0);
  ckeyboard = m;
  if(ckeyboard)
    ckeyboard->key_link(level, this, this);
  return true;
}

//***************************************

bool RCollector::shutdown(void)
{
  input_attach_m(NULL, 0);
  input_attach_k(NULL, 0);
  output_attach_2d(NULL, 0);
  //
  dialogs.Clear();
  frames.Resize(0);
  driver.clear();
  def_font.clear();
  focused.clear();
  return true;
}

//***************************************
//***************************************

bool RCollector::mouse_event(const SMouseInput& ms)
{
  mouse_inside_seq++;
  update_size();
  bool rv = false;
  if(~dialogs) {
    rv = evmouse(size, frames, ms, dialogs.Last()->root);
  } else {
    rv = evmouse(size, frames, ms, NULL);
  }
  evmouse(frames);
  return rv;
}

bool RCollector::key_pressed(const SKeyboardInputRaw& key)
{
  RFrame_Common* fr = get_focused();
  if(fr==NULL || !fr->use_keys)
    return master_key_pressed(key); 
  bool rv = fr->key_event(key);
  if(rv)
    return rv;
  return master_key_pressed(key); 
}

bool RCollector::key_text(const SKeyboardInputText& key)
{
  RFrame_Common* fr = get_focused();
  if(fr==NULL || !fr->use_keys)
    return false;
  return fr->key_event(key);
}

//***************************************
//***************************************

bool RCollector::insert(RFrame_Common* master, RFrame_Common *slave)
{
  if(slave==NULL)
    return false;
  Frames *fr = master ? &master->frames : &frames;
  if(!fr->Push())
    return false;
  fr->Last() = slave;
  return true;
}

bool RCollector::evmouse(const lib2d::DBox& base, const Frames& list, const SMouseInput& ms, RFrame_Common* root)
{
  for(int i=0, n=~list; i<n; i++) {
    RFrame_Common *c = list[i];
    lib2d::DBox b2(base, c->size);
    if(!b2.inside(ms.abs))
      continue;
    if(c->use_mouse && c->visible) {
      SMouseInput ms2 = ms;
      ms2.abs -= b2.A;
      c->mouse_inside = mouse_inside_seq;
      if(root==NULL || root==c)
        if(c->mouse_event(&ms2))
          return true;
    }
    if(~c->frames && c->visible)
      if(evmouse(b2, c->frames, ms, root==c ? NULL : root))
        return true;
  }
  return false;
}

bool RCollector::evmouse(const Frames& list)
{
  for(int i=0, n=~list; i<n; i++) {
    RFrame_Common *c = list[i];
    if(c->use_mouse && c->mouse_inside==mouse_inside_seq-1) {
      c->mouse_event(NULL);
    }
    if(~c->frames)
      evmouse(c->frames);
  }
  return true;
}

bool RCollector::draw(const SDrawState& state, const Frames& list)
{
  for(int i=0, n=~list; i<n; i++) {
    RFrame_Common *c = list[i];
    c->draw( SDrawState(state, c->size, c->font) );
  }
  return true;
}

int RCollector::broadcast(const Frames& list, int command, const SFrameDataIn& in, int ctx)
{
  int rv = 0;
  for(int i=0, n=~list; i<n; i++) {
    RFrame_Common *c = list[i];
    if(c->ctrl.valid() && (ctx==0 || ctx==c->ctrl.context))
      rv += c->ctrl.ctrl->command(c->ctrl, command, in);
    if(~c->frames)
      rv += broadcast(c->frames, command, in, ctx);
  }
  return rv;
}

bool RCollector::update_size(void)
{
  if(!driver.valid())
    return false;
  lib2d::DPoint size( driver->get_max_int("width"), driver->get_max_int("height") ),
                base( driver->get_max_int("xoffset"), driver->get_max_int("yoffset") );
  bool forced = cached_size!=size || cached_start!=base;
  if(forced) {
    this->size.A = base;
    this->size.B = base + size;
  }
  cached_size = size;
  cached_start = base;
  update_size(this->size, frames, forced);
  //
  for(int i=0, n=~dialogs; i<n; i++) {
    RFrame_Common *c = dialogs[i]->root;
    bool f = c->size_updated || forced;
    if(f)
      c->update_size(this->size);
    update_size(c->size, c->frames, f);
  }
  //
  return true;
}

bool RCollector::update_size(const lib2d::DBox& base, const Frames& list, bool force)
{
  for(int i=0, n=~list; i<n; i++) {
    RFrame_Common *c = list[i];
    if(c->dialog!=NULL)
      continue;
    bool f = c->size_updated || force;
    if(f)
      c->update_size(base);
    update_size(c->size, c->frames, f);
  }
  return true;
}

//***************************************

bool RCollector::set_focused(IFrame* fr)
{
  if(focused!=fr) {
    if(focused)
      focused->set_flag(ff_Focused|ff_Internal, false);
    focused = fr;
    if(focused)
      focused->set_flag(ff_Focused|ff_Internal, true);
  }
  return true;
}

RFrame_Common* RCollector::get_focused(void)
{
  IFrame *rv2 = ~dialogs ? dialogs.Last()->focused : focused;
  if(rv2==NULL)
    return NULL;
  RFrame_Common *rv;
  if(rv2->t_root_get_other_interface(rv))
    return rv;
  return NULL; // unknown type
}

//***************************************

RFrame_Common* RCollector::cont_first(RFrame_Common* root)
{
  const Frames* fr = root ? &root->frames : &frames;
  return ~*fr ? fr->Get(0) : NULL;
}

RFrame_Common* RCollector::cont_last(RFrame_Common* root)
{
  const Frames* fr = root ? &root->frames : &frames;
  RFrame_Common* rv = NULL;
  while(~*fr) {
    rv = fr->Last();
    fr = &rv->frames;
  }
  return rv;
}

RFrame_Common* RCollector::find_next_focused(RFrame_Common* root, RFrame_Common* focused, bool forward)
{
  if(forward) {
    if(focused!=NULL) {
      focused = focused->cont_next(root);
      while(focused) {
        if(focused->is_enabled() && focused->use_keys)
          return focused;
        focused = focused->cont_next(root);
      }
      focused = cont_first(root);
      while(focused) {
        if(focused->is_enabled() && focused->use_keys)
          return focused;
        focused = focused->cont_next(root);
      }
    } else {
      focused = cont_first(root);
      while(focused) {
        if(focused->is_enabled() && focused->use_keys)
          return focused;
        focused = focused->cont_next(root);
      }
    }
  } else {
    if(focused!=NULL) {
      focused = focused->cont_prev(root);
      while(focused) {
        if(focused->is_enabled() && focused->use_keys)
          return focused;
        focused = focused->cont_prev(root);
      }
      focused = cont_last(root);
      while(focused) {
        if(focused->is_enabled() && focused->use_keys)
          return focused;
        focused = focused->cont_prev(root);
      }
    } else {
      focused = cont_last(root);
      while(focused) {
        if(focused->is_enabled() && focused->use_keys)
          return focused;
        focused = focused->cont_prev(root);
      }
    }
  }
  return NULL;
}

//***************************************

bool RCollector::master_key_pressed(const SKeyboardInputRaw& key)
{
  RFrame_Common* fr = get_focused(), *fr2;
  if(key.status==bis_Pressed) {
    switch(key.code) {
      case k_tab:
        fr2 = find_next_focused(~dialogs ? dialogs.Last()->root : NULL, fr, (key.special & km_shift)==0 );
        if(fr2==NULL)
          return false;
        if(~dialogs) dialogs.Last()->set_focused(fr2); else set_focused(fr2);
        return true;
    }
  }
  return false; 
}

//***************************************

RCollector::RCollector(DOwner *pv, const SParameters& args)
: valid(false), size(0,0,0,0), theme(this), refs(pv)
{
  IRefObject *ro;
  if(!args.get("driver", ro) || ro==NULL || !ro->t_ref_get_other_interface(driver))
    return;
  if(!args.get("font", ro) || ro==NULL || !ro->t_ref_get_other_interface(def_font))
    return;
  //
  cached_start.set(0);
  cached_size.set(0);
  //
  // -- default theme init
  theme.set_property(ctg_General, cte_Focused|cte_Edges, lib2d::DColorAlphaf(1,1,1,0.5));
  theme.set_property(ctg_General, cte_Caret, lib2d::DColorAlphaf(1,1,1,1.0));
  //
  theme.set_property(ctg_Text, cte_Text|cte_Fore, lib2d::DColorAlphaf(1,1,1,1));
  theme.set_property(ctg_Text, cte_Text|cte_Fore, lib2d::DPointf(1));
  //
  theme.set_property(ctg_Image, cte_Image|cte_Back, lib2d::DColorAlphaf(0.5,0.5,0.5,1));
  //
  theme.set_property(ctg_Edit, cte_Text|cte_Fore, lib2d::DColorAlphaf(1,1,1,1));
  theme.set_property(ctg_Edit, cte_Text|cte_Fore, lib2d::DPointf(1));
  theme.set_property(ctg_Edit, cte_Back, lib2d::DColorAlphaf(0.5,0.5,0.5,1));
  //
  theme.set_property(ctg_List, cte_Text|cte_Fore, lib2d::DColorAlphaf(0.7f,0.7f,0.7f,1));
  theme.set_property(ctg_List, cte_Button|cte_Back, lib2d::DColorAlphaf(0.5,0.5,0.5,1));
  theme.set_property(ctg_List, cte_Text|cte_Fore, lib2d::DPointf(1));
  theme.set_property(ctg_List, cte_Text|cte_Fore|cte_Selected, lib2d::DColorAlphaf(1,1,1,1));
  theme.set_property(ctg_List, cte_Button|cte_Back|cte_Selected, lib2d::DColorAlphaf(0.7f,0.7f,0.7f,1));
  //
  theme.set_property(ctg_Button, cte_Text|cte_Fore, lib2d::DColorAlphaf(1,1,1,1));
  theme.set_property(ctg_Button, cte_Button|cte_Back, lib2d::DColorAlphaf(0.5f,0.5f,0.5f,1));
  theme.set_property(ctg_Button, cte_Button|cte_Back, lib2d::DColorAlphaf(0.5f,0.5f,0.5f,1));
  theme.set_property(ctg_Button, cte_Text|cte_Fore|cte_Pressed, lib2d::DColorAlphaf(1,1,1,1));
  theme.set_property(ctg_Button, cte_Button|cte_Back|cte_Pressed, lib2d::DColorAlphaf(0.0,0.0,0.0,1));
  theme.set_property(ctg_Button, cte_Text|cte_Fore|cte_Active, lib2d::DColorAlphaf(1,1,1,1));
  theme.set_property(ctg_Button, cte_Button|cte_Back|cte_Active, lib2d::DColorAlphaf(0.6f,0.6f,0.6f,1));
  theme.set_property(ctg_Button, cte_Text|cte_Fore, lib2d::DPointf(1));
  //
  mouse_inside_seq = 1;
  //
  valid = true;
}

RCollector::~RCollector(void)
{
}

IDialog* RCollector::dialog_open(IFrame* root)
{
  RDialog* odlg = ~dialogs ? dialogs.Last() : NULL;
  if(root==NULL || root->get_collector()!=this || !dialogs.Push())
    return NULL;
  RDialog::SelfP dlg = new RDialog(this, static_cast<RFrame_Common*>(root));
  if(dlg==NULL) {
    dialogs.Pop();
    return NULL;
  }
  if(odlg) odlg->dpause(true);
  dialogs.Last() = dlg;
  dlg->dopen();
  return dlg;
}

IDialog* RCollector::dialog_get(void)
{
  if(~dialogs==0)
    return NULL;
  return dialogs.Last();
}

bool RCollector::dialog_close(void)
{
  if(~dialogs==0)
    return false;
  RDialog::SelfP dlg = dialogs.Last();
  dialogs.Pop();
  dlg->dclose();
  RDialog* odlg = ~dialogs ? dialogs.Last() : NULL;
  if(odlg) odlg->dpause(false);
  return true;
}

bool RCollector::dialog_close_all(void)
{
  while(~dialogs) {
    RDialog::SelfP dlg = dialogs.Last();
    dialogs.Pop();
    dlg->dclose();
  }
  return true;
}

//***************************************
// ::
//***************************************
