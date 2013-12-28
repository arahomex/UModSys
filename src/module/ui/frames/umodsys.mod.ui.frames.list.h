//------------------------------------
// controls
//------------------------------------

// list

struct RFrame_List : public RFrame_Common, public IFrameList, public IFrameData {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RFrame_List, 2, RFrame_Common)
  UMODSYS_REFOBJECT_REFOTHER(RCollector)
  //
  // id functions (what interface is supported)
  DCString get_cids(void) { return "text"; }
  int get_cidn(void) { return cidmask(cid_Text); }
  // get special interfaces
  IFrameList *get_list(void) { return this; }
  IFrameData *get_data(void) { return this; }
  IControllerData *get_ctrldata(void) { return NULL; }
  IControllerCommand *get_ctrlcmd(void) { return NULL; }
  bool controller_update(void);
  //
  // data
  bool set_lcount(int count);
  int  get_lcount(void);
  bool set_lvalue(int index, int kind, const SFrameDataIn& val);
  bool get_lvalue(int index, int kind, const SFrameDataOut& val);
  bool set_value(int kind, const SFrameDataIn& val);
  bool get_value(int kind, const SFrameDataOut& val);
  //
public:
  RFrame_List(RCollector *pv, RFrame_Common *up);
  ~RFrame_List(void);
  //
  struct ListItem {
    DStringShared text;
    //
    ListItem(void) {}
    ~ListItem(void) {}
  };
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
  bool sselect(int id);
  //
  tl::TDynarrayDynamic<ListItem> list;
  SButton b_up, b_down, b_mid;
  //
  bool pressed;
  int selid, mselid, dy, count, count2, seltop, selwindow;
};

//***************************************
// RFrame_List::
//***************************************

bool RFrame_List::button_cmd(SButton *bt, int cmd, int arg)
{
  set_focus(true);
  switch(bt->id) {
    case 'u':
      if(cmd==cmd_Pressed && arg==-1) {
        if(seltop>0) seltop--;
      }
      break;
    case 'd':
      if(cmd==cmd_Pressed && arg==-1) {
        if(seltop+selwindow<count) seltop++;
      }
      break;
    case 'm':
      if(cmd==cmd_Pressed && arg==1) {
        seltop = count2<=0 ? 0 : (float)(bt->pressed_mouse_pos(1)-1)/(bt->size.sizey()-2) * count2 + 0.5;
      }
      break;
  }
  return true;
}

bool RFrame_List::button_draw(SButton *bt, const SDrawState& S, int flags)
{
  bt->draw_button(S, ctg_Button, flags);
  switch(bt->id) {
    case 'u':
      bt->draw_arrow(S, 90, ctg_Button, flags);
      break;
    case 'd':
      bt->draw_arrow(S, -90, ctg_Button, flags);
      break;
    case 'm':
      if(count2<=0) {
        bt->draw_progress(S, 0, -90, ctg_Button, flags);
      } else {
        bt->draw_progress(S, (float)seltop/count2, -90, ctg_Button, flags);
      }
      break;
  }
  return true;
}

//***************************************

bool RFrame_List::key_event(const SKeyboardInputRaw& key)
{
  if(key.status==bis_Pressed) {
    switch(key.code) {
      case k_up:
        sselect(selid-1);
        return true;
      case k_down:
        sselect(selid+1);
        return true;
      case k_pageup:
        sselect(selid-selwindow+1);
        return true;
      case k_pagedown:
        sselect(selid+selwindow-1);
        return true;
      case k_home:
        sselect(0);
        return true;
      case k_end:
        sselect(count);
        return true;
    }
  }
  return false;
}

bool RFrame_List::mouse_event(const SMouseInput* ms)
{
  if(ms && is_enabled()) {
    if(b_up.mouse(ms) || b_down.mouse(ms) || b_mid.mouse(ms))
      return true;
    if(ms->abs(0)>=size.sizex()-b_mid.size.sizex()) {
      return false;
    }
    //
    int id = ms->abs[1] / dy;
    if(id>=count) {
      if(ms->eventid & (me_Button | me_MoveButton)) {
        set_focus(true);
      }
      mouse_event(NULL);
      return false;
    }
    //    
    active = true;
    mselid = id+seltop;
    mouse_seq();
    //
    if(ms->eventid & (me_Button | me_MoveButton)) {
      set_focus(true);
      bool press2 = ms->buttons[0]==bis_Pressed;
      if(press2) {
        selid = mselid;
        // invoke command
        ctrl.command( cmd_Changed, selid );
        return true;
      }
    }
    //
  } else {
    b_up.mouse(ms);
    b_down.mouse(ms);
    b_mid.mouse(ms);
    //
    active = false;
    pressed = false;
  }
  return false;
}

bool RFrame_List::draw_post(const SDrawState& S)
{
  b_up.draw(S);
  b_down.draw(S);
  b_mid.draw(S);
  //
  int fd = is_enabled() ? 0 : cte_Disabled;
  S->setup_coloralpha( tclr(ctg_List, cte_Text|cte_Fore|fd) );
  S->setup_font(S);
  lib2d::DPointf scale = tptr(ctg_List, cte_Text|cte_Fore);
  lib2d::DPoint p = S.base.A;
  for(int j=0; j<selwindow; j++) {
    int i = seltop + j;
    if(i>=count)
      break;
    DCString text;
    if(user_db) {
      ctrl.db_get("text", i, text);
    } else {
      const ListItem& li = list[i];
      text = li.text;
    }
    BCharL line[max_line_length+1];
    int len = tl::su::utf_8to32(line, max_line_length, text, ~text, true);
    line[len] = 0;
    //
    if(selid==i) {
      S->setup_coloralpha( tclr(ctg_List, cte_Button|cte_Back|cte_Selected) );
      S->render_box(p, lib2d::DPoint(S.base.B(0)-b_mid.size.sizex(), p(1)+dy));
      S->setup_coloralpha( tclr(ctg_List, cte_Text|cte_Fore|cte_Selected) );
      S->render_text(p, line, len, &scale);
      S->setup_coloralpha( tclr(ctg_List, cte_Text|cte_Fore|fd) );
    } else if(active && mselid==i) {
      S->setup_coloralpha( tclr(ctg_List, cte_Button|cte_Back|fd) );
      S->render_box(p, lib2d::DPoint(S.base.B(0)-b_mid.size.sizex(), p(1)+dy));
      S->setup_coloralpha( tclr(ctg_List, cte_Text|cte_Fore|fd) );
      S->render_text(p, line, len, &scale);
      S->setup_coloralpha( tclr(ctg_List, cte_Text|cte_Fore|fd) );
    } else {
      S->render_text(p, line, len, &scale);
    }
    p[1] += dy;
  }
  return true;
}

bool RFrame_List::update_size(const lib2d::DBox& base)
{
  RFrame_Common::update_size(base);
  b_up.update_size(size);
  b_down.update_size(size);
  b_mid.update_size(size);
  return true;
}

//***************************************

bool RFrame_List::set_fcp(const SFrameCreateParameters& fcp, const SParameters* args)
{
  if(!RFrame_Common::set_fcp(fcp, args))
    return false;                                          
  if(args) {
    args->get("user_db", user_db);
    args->get("dy", dy);
    int scrollx=8, scrolly=8;
    if(args->get("scrollx", scrollx)) {
      b_up.fs.size[0] = scrollx;
      b_mid.fs.size[0] = scrollx;
      b_down.fs.size[0] = scrollx;
      size_updated = true;
    }
    if(args->get("scrolly", scrolly)) {
      b_up.fs.size[1] = scrolly;
      b_mid.fs.size[1] = scrolly;
      b_mid.fs.start[1] = scrolly;
      b_down.fs.size[1] = scrolly;
      size_updated = true;
    }
  }
  controller_update();
  return true;
}

bool RFrame_List::controller_update(void)
{
  if(user_db) {
    count = ctrl.db_get_i("count");
    dy = ctrl.db_get_i("dy");
  } else {
    count = ~list;
  }
  if(dy<=0) dy = 1;
  selwindow = size.sizey()/dy;
  count2 = count - selwindow;
  if(count2<0) count2 = 0;
  if(count2==0) {
    b_up.enabled = false;
    b_down.enabled = false;
    b_mid.enabled = false;
  } else {
    b_up.enabled = true;
    b_down.enabled = true;
    b_mid.enabled = true;
  }
  if(selid>=0) {
    if(selid<seltop)
      seltop = selid;
    else if(selid>=seltop+selwindow)
      seltop = selid-selwindow+1;
  }
  if(count && seltop>count) 
    seltop = count-1;
  return true;
}


//***************************************

bool RFrame_List::set_lcount(int count)
{
  if(user_db)
    return false;
  if(!list.Resize(count))
    return false;
  controller_update();
  return true;
}

int RFrame_List::get_lcount(void)
{
  if(user_db)
    return false;
  return ~list;
}

bool RFrame_List::set_lvalue(int index, int kind, const SFrameDataIn& val)
{
  if(user_db)
    return false;
  if(kind!=0 || index<0 || index>=~list)
    return false;
  if(val.type==fdt_String) {
    list[index].text = val.s();
    return true;
  }
  if(val.type==fdt_SString) {
    list[index].text = val.ss();
    return true;
  }
  return false;
}

bool RFrame_List::get_lvalue(int index, int kind, const SFrameDataOut& val)
{
  if(user_db)
    return false;
  if(kind!=0 || index<0 || index>=~list)
    return false;
  if(val.type==fdt_String) {
    val.stemp = list[index].text;
    val.s(val.stemp);
    return true;
  }
  if(val.type==fdt_SString) {
    val.ss( list[index].text );
    return true;
  }
  return false;
}

bool RFrame_List::set_value(int kind, const SFrameDataIn& val)
{
  if(user_db)
    return false;
  if(val.type!=fdt_Integer || kind!=0)
    return false;
  selid = val.i();
  return true;
}

bool RFrame_List::get_value(int kind, const SFrameDataOut& val)
{
  if(user_db)
    return false;
  if(val.type!=fdt_Integer || kind!=0)
    return false;
  val.i( selid );
  return true;
}

//***************************************

bool RFrame_List::sselect(int id)
{
  if(id>=count) {
    id = count-1;
  } else if(id<0) {
    id = 0;
  }
  selid = id;
  controller_update();
  //
  ctrl.command( cmd_Changed, selid );
  return true;
}

//***************************************

RFrame_List::RFrame_List(RCollector *pv, RFrame_Common *up)
: RFrame_Common(pv, up),
  b_up(this, 'u', SFrameSize(lib2d::DPoint(0,0), lib2d::DPoint(8,8), lib2d::a_Right, lib2d::a_Left) ), 
  b_down(this, 'd', SFrameSize(lib2d::DPoint(0,0), lib2d::DPoint(8,8), lib2d::a_Right, lib2d::a_Right) ), 
  b_mid(this, 'm', SFrameSize(lib2d::DPoint(0,8), lib2d::DPoint(8,8), lib2d::a_Right, lib2d::a_Justify) ),
  refs(pv)
{
  pressed = false;
  use_mouse = use_keys = true;
  selid = -1;
  mselid = -1;
  dy = 20;
  seltop = 0;
}

RFrame_List::~RFrame_List(void)
{
}

//***************************************
// ::
//***************************************
