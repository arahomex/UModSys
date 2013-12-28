//------------------------------------
// * INLINES *
//------------------------------------

//***************************************
// RFrame_Common::
//***************************************

inline ICollectorTheme* RFrame_Common::get_up_theme(void) 
{ 
  return up ? &up->theme : &owner->theme; 
}

inline void RFrame_Common::mouse_seq(void) 
{ 
  mouse_inside = owner->mouse_inside_seq; 
}

ICollector* RFrame_Common::get_collector(void)
{
  return owner;
}

bool RFrame_Common::draw_sub(const SDrawState& S)
{
  return owner->draw(S, frames);
}

bool RFrame_Common::set_focus(bool flag)
{
  owner->do_focused(this, flag, use_textinput);
  RFrame_Common *p;
  for(p=this; p && p->dialog==NULL; p=p->up) {
  }
  if(p==NULL) {
    owner->set_focused(flag ? this : NULL);
    return true;
  }
  RDialog *dlg = p->dialog;
  dlg->set_focused(flag ? this : NULL);
  return true;
}

RFrame_Common* RFrame_Common::cont_prev(RFrame_Common* up_limit)
{
  RFrame_Common* x = this;
  int i;
  const Frames* fr;
  // use parent index
  do {
    fr = x->up ? &x->up->frames : &x->owner->frames;
    for(i=0; i<~*fr; i++) {
      if(fr->Get(i)==x) {
        if(i-1>=0) {
          x = fr->Get(i-1);
          while(~x->frames) {
            x = x->frames.Last();
          }
          return x;
        }
        if(x->up==NULL || x->up==up_limit)
          return NULL; // last one
        x = x->up;
        return x;
      }
    }
  } while(x!=NULL && i<~*fr);
  return NULL;
}

RFrame_Common* RFrame_Common::cont_next(RFrame_Common* up_limit)
{
  RFrame_Common* x = this;
  int i;
  const Frames* fr;
  // go thru subs
  if(~x->frames)
    return x->frames[0];
  // use parent index
  do {
    fr = x->up ? &x->up->frames : &x->owner->frames;
    for(i=0; i<~*fr; i++) {
      if(fr->Get(i)==x) {
        if(i+1<~*fr)
          return fr->Get(i+1);
        if(x->up==NULL || x->up==up_limit)
          return NULL; // last one
        x = x->up;
        break;
      }
    }
  } while(x!=NULL && i<~*fr);
  return NULL;
}

//***************************************
// RDialog::
//***************************************

bool RDialog::close(void)
{
  if(refs->dialog_get()==this)
    return refs->dialog_close();
  return false; // not first
}

bool RDialog::dopen(void)
{
  root->set_flag(ff_Disabled, false);
  root->set_flag(ff_Hidden, false);
  set_focused( refs->find_next_focused(root, NULL) );
  return true;
}


//***************************************
// SButton::
//***************************************

inline bool SButton::is_enabled(void) 
{
  return enabled && frame->is_enabled();
}

SButton::SButton(RFrame_Common *fframe, int i, const SFrameSize &afs)
: fs(afs), frame(fframe), size(0,0,0,0), id(i)
{
  enabled = true;
  active = false; 
  pressed = false;
  special = false;
}

SButton::~SButton(void)
{
}

inline bool SButton::update_size(const lib2d::DBox &parsize)
{
  libui::update_size(size, fs, parsize);
  return true;
}

bool SButton::mouse(const SMouseInput* ms2)
{
  if(ms2 && size.inside(ms2->abs)) {
    if(is_enabled()) {
      SMouseInput ms = *ms2;
      ms.abs -= size.A;
      //
      active = true;
      frame->mouse_seq();
      if(ms.eventid & (me_Button | me_MoveButton)) {
        bool press2 = ms.buttons[0]==bis_Pressed;
        pressed_mouse_pos = ms.abs;
        if(pressed && !press2) {
          // invoke command
          frame->button_cmd(this, cmd_Pressed, -1);
        }
        frame->button_cmd(this, cmd_Pressed, press2);
        pressed = press2;
      } else {
        if(pressed) {
          frame->button_cmd(this, cmd_Pressed, 0);
        }
        pressed = false;
      }
    } else {
      if(pressed) {
        frame->button_cmd(this, cmd_Pressed, 0);
      }
      active = false;
      pressed = false;
    }
    return true;
  } else {
    if(pressed) {
      frame->button_cmd(this, cmd_Pressed, 0);
    }
    active = false;
    pressed = false;
  }
  return false;
}

bool SButton::draw(const SDrawState& S2)
{
  SDrawState S(S2, size, NULL);
  int flg = 0;
  int fd = is_enabled() ? 0 : cte_Disabled;
  if(active && fd==0)
    flg |= cte_Active;
/*
  if(pressed && fd==0)
    flg |= cte_Pressed;
*/
  if(special && fd==0)
    flg |= cte_Selected;
  //
  flg |= fd;
  frame->button_draw(this, S, flg);
  return true;
}

//***************************************

inline bool SButton::draw_button(const SDrawState& S, int group, int addflag)
{
  S->setup_coloralpha( frame->tclr(group, cte_Button|cte_Back|addflag));
  S->render_box(S.base.A, S.base.B);
  return true;
}

inline bool SButton::draw_text(const SDrawState& S, const DCString& text, int group, int addflag)
{
  BCharL line[max_line_length+1];
  int len = tl::su::utf_8to32(line, max_line_length, text, ~text, true);
  line[len]=0;
  //
  S->setup_coloralpha( frame->tclr(group, cte_Text|cte_Fore|addflag) );
  S->setup_font(S);
  S->render_textbox(S.base.A, S.base.B, line, len, 
                    lib2d::to_Center|lib2d::to_VCenter|lib2d::to_LF, 
                    frame->tptr(group, cte_Text|cte_Fore));
  return true;
}

bool SButton::draw_arrow(const SDrawState& S, int angle, int group, int addflag)
{
  lib2d::DPoint A, B, C;
  switch(angle) {
    case 0: // right
      A = S.base.A;
      B.set( S.base.A(0), S.base.B(1) );
      C.set( S.base.B(0), (S.base.A(1)+S.base.B(1))/2 );
      break;
    case 180: // left
    case -180: // left
      A = S.base.B;
      B.set( S.base.B(0), S.base.B(1) );
      C.set( S.base.A(0), (S.base.A(1)+S.base.B(1))/2 );
      break;
    case 90: // up
    case -270: // up
      A.set( S.base.A(0), S.base.B(1) );
      B = S.base.B;
      C.set( (S.base.A(0)+S.base.B(0))/2, S.base.A(1) );
      break;
    case -90: // down
    case 270: // down
      A.set( S.base.B(0), S.base.A(1) );
      B = S.base.A;
      C.set( (S.base.A(0)+S.base.B(0))/2, S.base.B(1) );
      break;
    default:
      return false;
  }
  S->setup_coloralpha( frame->tclr(group, cte_Text|cte_Fore|addflag) );
  S->render_tri(A, B, C);
  return true;
}

bool SButton::draw_progress(const SDrawState& S, float ratio, int angle, int group, int addflag)
{
  lib2d::DPoint A, B;
  if(ratio<0) ratio = 0;
  else if(ratio>1) ratio = 1;
  switch(angle) {
    case 0: // right
      A.set( S.base.A(0)+1, S.base.A(1)+1 );
      B.set( S.base.A(0)+1 + ratio*(S.base.sizex()-2), S.base.B(1)-1 );
      break;
    case 180: // left
    case -180: // left
      A.set( S.base.B(0)-1 - ratio*(S.base.sizex()-2), S.base.B(1)-1 );
      B.set( S.base.B(0)-1, S.base.B(1)+1 );
      break;
    case 90: // up
    case -270: // up
      A.set( S.base.A(0)+1, S.base.B(1)-1 - ratio*(S.base.sizey()-2) );
      B.set( S.base.B(0)+1, S.base.B(1)+1 );
      break;
    case -90: // down
    case 270: // down
      A.set( S.base.A(0)+1, S.base.A(1)+1 );
      B.set( S.base.B(0)+1, S.base.A(1)+1 + ratio*(S.base.sizey()-2) );
      break;
    default:
      return false;
  }
  S->setup_coloralpha( frame->tclr(group, cte_Text|cte_Fore|addflag));
  S->render_box(A, B);
  return true;
}

