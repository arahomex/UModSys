//------------------------------------
// controls
//------------------------------------

// edit

struct RFrame_Edit : public RFrame_Common, public IFrameData {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RFrame_Edit, 2, RFrame_Common)
  UMODSYS_REFOBJECT_REFOTHER(RCollector)
  //
  // id functions (what interface is supported)
  DCString get_cids(void) { return "edit"; }
  int get_cidn(void) { return cidmask(cid_Edit); }
  // get special interfaces
  IFrameList *get_list(void) { return NULL; }
  IFrameData *get_data(void) { return this; }
  IControllerData *get_ctrldata(void) { return NULL; }
  IControllerCommand *get_ctrlcmd(void) { return NULL; }
  // frame special operation (FRAMES)
  bool set_value(int kind, const SFrameDataIn& val);
  bool get_value(int kind, const SFrameDataOut& val);
  //
public:
  RFrame_Edit(RCollector *pv, RFrame_Common *up);
  ~RFrame_Edit(void);
protected:
  bool mouse_event(const SMouseInput* ms);
  bool key_event(const SKeyboardInputRaw& key);
  bool key_event(const SKeyboardInputText& key);
  bool set_fcp(const SFrameCreateParameters& fcp, const SParameters* args);
  bool draw_post(const SDrawState& S);
  //
  bool set_buf(BCStr str, int newlen);
  bool sselect(int pos);
  bool sdelete(int pos, int count);
  bool sinsert(int pos, BCStrL s, int count);
  bool sinsert(int pos, BCharL c) { return sinsert(pos, &c, 1); }
  bool update_last(void);
  //
  DStringBufShared last_text_buf;
  SMemShared text_buf;
  BWStrL text;
  int text_len, caret, max_len;
};

//***************************************
// RFrame_Edit::
//***************************************

bool RFrame_Edit::key_event(const SKeyboardInputRaw& key)
{
  if(key.status==bis_Pressed) {
    switch(key.code) {
      case k_left:
        sselect(caret-1);
        return true;
      case k_right:
        sselect(caret+1);
        return true;
      case k_home:
        sselect(0);
        return true;
      case k_end:
        sselect(text_len);
        return true;
      case k_delete:
        sdelete(caret, 1);
        return true;
      case k_backspace:
        sdelete(caret-1, 1);
        return true;
    }
  }
  return false;
}

bool RFrame_Edit::key_event(const SKeyboardInputText& key)
{
  sinsert(caret, key.text);
  return true;
}

bool RFrame_Edit::mouse_event(const SMouseInput* ms)
{
  if(ms && is_enabled()) {
    active = true;
    mouse_seq();
    if(ms->eventid & (me_Button | me_MoveButton)) {
      set_focus(true);
      bool press2 = ms->buttons[0]==bis_Pressed;
      if(press2) {
        // invoke command
//        ctrl.command( cmd_Changed, selid );
        return true;
      }
    }
  } else {
    //
    active = false;
  }
  return false;
}

//***************************************

bool RFrame_Edit::draw_post(const SDrawState& S)
{
  int fd = is_enabled() ? 0 : cte_Disabled;
//  lib2d::DBox tsize;
  //
  S->setup_coloralpha( tclr(ctg_Edit, cte_Back|fd) );
  S->render_box(S.base.A, S.base.B);
  //
  S->setup_coloralpha( tclr(ctg_Edit, cte_Text|cte_Fore|fd) );
  S->setup_font(S);
  lib2d::IRenderDriver::TextInfo inf(S.base.A, S.base.B, &tptr(ctg_Edit, cte_Text|cte_Fore));
  inf.set_option(lib2d::to_CalcCaretSize|lib2d::to_VCenter);
  inf.caret_pos = caret;
  S->render_text_advanced(inf, text, text_len);
  //
  S->setup_coloralpha( tclr(ctg_General, cte_Caret) );
  S->render_line(inf.calc_caret.get_point(0,0), inf.calc_caret.get_point(0,1));
//  S->render_text_size(tsize, S.base.A, text, caret, &tptr(ctg_Edit, cte_Text|cte_Fore));
//  S->render_line(tsize.get_point(true, false), tsize.B);
  //
  return true;
}

//***************************************

bool RFrame_Edit::set_fcp(const SFrameCreateParameters& fcp, const SParameters* args)
{
  if(!RFrame_Common::set_fcp(fcp, args))
    return false;
  set_buf(fcp.text, ~fcp.text);
  if(args) {
    args->get("max_len", max_len);
    set_buf(NULL, max_len+1);
  }
  return true;
}

/*
bool RFrame_Edit::set_title(const DHString& title) 
{ 
  if(user_db)
    return false;
  text.alloc(title); 
  updated = true; 
  return true; 
}

bool RFrame_Edit::get_title(DAString& title) 
{ 
  if(user_db)
    return false;
  title.alloc(title); 
  return true; 
}
*/

//***************************************

bool RFrame_Edit::set_value(int kind, const SFrameDataIn& val)
{
  if(user_db || kind!=0)
    return false;
  if(val.type==fdt_String) {
    set_buf( val.s(), ~val.s());
    caret = text_len;
    return true;
  }
  if(val.type==fdt_SString) {
    set_buf( val.ss(), ~val.ss());
    caret = text_len;
    return true;
  }
  return false;
}

bool RFrame_Edit::get_value(int kind, const SFrameDataOut& val)
{
  if(user_db || kind!=0)
    return false;
  DCString st;
  if(val.type==fdt_String) {
    if(!update_last())
      return false;
    st = last_text_buf;
    val.s( st );
    return true;
  }
  if(val.type==fdt_SString) {
    if(!update_last())
      return false;
/******** !!!!
    val.ss( last_text_buf );
*/
    return true;
  }
  return false;
}

//***************************************

bool RFrame_Edit::update_last(void)
{
  int len = tl::su::utf_32to8_len(text, text_len);
  if(!last_text_buf.info_alloc(len))
    return false;
  if(tl::su::utf_32to8(last_text_buf.get_buftext(), last_text_buf.get_bufmax(), text, text_len)!=len)
    return false;
  last_text_buf.set_length(len);
  return true;
}

bool RFrame_Edit::set_buf(BCStr str, int newlen)
{
  int str_len = str ? tl::su::slen(str, newlen) : 0;
  int new_ulen = str ? tl::su::utf_8to32_len(str, str_len) : newlen;
  int new_binlen = sizeof(BCharL) * new_ulen;
  if(new_binlen>text_buf.get_size()) {
    max_len = new_binlen;
    SMemShared newbuf;
    if(!newbuf.alloc(new_binlen, UMODSYS_SOURCEINFO))
      return false;
    BWStrL t = newbuf.get_tdata<BCharL>();
    if(str!=NULL) {
      tl::su::utf_8to32(t, newlen, str, new_ulen, true);
      text_len = newlen;
    } else if(text_len) {
      tl::su::smemcpy(t, text, text_len);
    }
    text_buf = newbuf;
    text = t;
  } else {
    if(str!=NULL) {
      tl::su::utf_8to32(text, newlen, str, new_ulen, true);
      text_len = newlen;
      if(caret>text_len)
        caret = text_len;
    }
  }
  return true;
}

bool RFrame_Edit::sselect(int pos)
{
  if(pos<0) {
    pos = 0;
  } else if(pos>text_len) {
    pos = text_len;
  }
  caret = pos;
  return true;
}

bool RFrame_Edit::sdelete(int pos, int count)
{
  if(pos<0 || pos+count>text_len)
    return false; // range
  int L = text_len-(pos+count);
  if(L) {
    tl::su::smemmove(text+pos, text+(pos+count), L);
  }
  text_len -= count;
  if(caret>pos && caret<=pos+count)
    caret = pos;
//  ctrl.command( cmd_Changed, DHString(text, text_len, 0) );
  ctrl.command( cmd_Changed, SFrameDataIn() );
  return true;
}

bool RFrame_Edit::sinsert(int pos, BCStrL s, int count)
{
  if(text_len+count>=text_buf.get_size()/sizeof(BCharL))
    return false; // memory
  if(pos<0 || pos>text_len)
    return false; // range
  int L = text_len-pos;
  if(L) {
    tl::su::smemmove(text+(pos+count), text+pos, L);
  }
  tl::su::smemcpy(text+pos, s, count);
  text_len += count;
  if(caret==pos)
    caret += count;
  ctrl.command( cmd_Changed, SFrameDataIn() );
//  ctrl.command( cmd_Changed, DHString(text, text_len, 0) );
  return true;
}

//***************************************

RFrame_Edit::RFrame_Edit(RCollector *pv, RFrame_Common *up)
: RFrame_Common(pv, up), refs(pv)
{
  use_mouse = use_keys = use_textinput = true;
  caret = text_len = 0;
  text = NULL;
}

RFrame_Edit::~RFrame_Edit(void)
{
}

//***************************************
// ::
//***************************************
