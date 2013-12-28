//------------------------------------
// controls
//------------------------------------

// empty group (frame)

struct RFrame_Frame : public RFrame_Common {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RFrame_Frame, 2, RFrame_Common)
  UMODSYS_REFOBJECT_REFOTHER(RCollector)
  //
  // id functions (what interface is supported)
  DCString get_cids(void) { return "frame"; }
  int get_cidn(void) { return cidmask(cid_Frame); }
  // get special interfaces
  IFrameList *get_list(void) { return NULL; }
  IFrameData *get_data(void) { return NULL; }
  IControllerData *get_ctrldata(void) { return NULL; }
  IControllerCommand *get_ctrlcmd(void) { return NULL; }
  // frame special operation (FRAMES)
//  bool set_title(const DCString& title) { return false; }
//  bool get_title(DAString& title) { return false; }
  //
  RFrame_Frame(RCollector *own, RFrame_Common *u);
  ~RFrame_Frame(void);
};

// image/box

struct RFrame_Image : public RFrame_Common {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RFrame_Image, 2, RFrame_Common)
  UMODSYS_REFOBJECT_REFOTHER(RCollector)
  //
  // id functions (what interface is supported)
  DCString get_cids(void) { return "picture"; }
  int get_cidn(void) { return cidmask(cid_Picture); }
  // get special interfaces
  IFrameList *get_list(void) { return NULL; }
  IFrameData *get_data(void) { return NULL; }
  IControllerData *get_ctrldata(void) { return NULL; }
  IControllerCommand *get_ctrlcmd(void) { return NULL; }
  // frame special operation (FRAMES)
//  bool set_title(const DCString& title) { return false; }
//  bool get_title(DAString& title) { return false; }
  //
  RFrame_Image(RCollector *own, RFrame_Common *u);
  ~RFrame_Image(void);
protected:
  bool set_fcp(const SFrameCreateParameters& fcp, const SParameters* args);
  bool draw_pre(const SDrawState& S);
  //
  lib2d::IImage::P image;
};

// text

struct RFrame_Text : public RFrame_Common, public IFrameData {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RFrame_Text, 2, RFrame_Common)
  UMODSYS_REFOBJECT_REFOTHER(RCollector)
  //
  // id functions (what interface is supported)
  DCString get_cids(void) { return "text"; }
  int get_cidn(void) { return cidmask(cid_Text); }
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
  RFrame_Text(RCollector *pv, RFrame_Common *up);
  ~RFrame_Text(void);
protected:
  bool set_fcp(const SFrameCreateParameters& fcp, const SParameters* args);
  bool draw_post(const SDrawState& S);
  //
  DStringShared text;
};

//***************************************
// RFrame_Frame::
//***************************************

RFrame_Frame::RFrame_Frame(RCollector *own, RFrame_Common *u)
: RFrame_Common(own, u), refs(own) {
}

RFrame_Frame::~RFrame_Frame(void)
{
}

//***************************************
// RFrame_Text::
//***************************************

bool RFrame_Text::draw_post(const SDrawState& S)
{
  DCString text;
  if(user_db) {
    ctrl.db_get("text", text);
  } else {
    text = this->text;
  }
  BCharL line[max_text_length+1];
  int len = tl::su::utf_8to32(line, max_text_length, text, ~text, true);
  line[len] = 0;
  //
  int fd = is_enabled() ? 0 : cte_Disabled;
  S->setup_coloralpha( tclr(ctg_Text, cte_Text|cte_Fore|fd) );
  S->setup_font(S);
//  S->render_text(S.base.A, text, ~text, &tptr(ctg_Text, cte_Text|cte_Fore));
  S->render_textbox(S.base.A, S.base.B, line, len, 
                    lib2d::to_Left|lib2d::to_Left|lib2d::to_LF|lib2d::to_Wrap, 
                    tptr(ctg_Text, cte_Text|cte_Fore));
  return true;
}

//***************************************

bool RFrame_Text::set_fcp(const SFrameCreateParameters& fcp, const SParameters* args)
{
  if(!RFrame_Common::set_fcp(fcp, args))
    return false;
  text = fcp.text;
  if(args) {
    args->get("user_db", user_db);
  }
  return true;
}

/*
bool RFrame_Text::set_title(const DHString& title) 
{ 
  if(user_db)
    return false;
  text.alloc(title); 
  updated = true; 
  return true; 
}

bool RFrame_Text::get_title(DAString& title) 
{ 
  if(user_db)
    return false;
  title.alloc(title); 
  return true; 
}
*/

//***************************************

bool RFrame_Text::set_value(int kind, const SFrameDataIn& val)
{
  if(user_db || kind!=0)
    return false;
  if(val.type==fdt_String) {
    text = val.s();
    return true;
  }
  if(val.type==fdt_SString) {
    text = val.ss();
    return true;
  }
  return false;
}

bool RFrame_Text::get_value(int kind, const SFrameDataOut& val)
{
  if(user_db || kind!=0)
    return false;
  if(val.type==fdt_String) {
    val.stemp = text;
    val.s(val.stemp);
    return true;
  }
  if(val.type==fdt_SString) {
    val.ss(text);
    return true;
  }
  return false;
}

//***************************************

RFrame_Text::RFrame_Text(RCollector *pv, RFrame_Common *up)
: RFrame_Common(pv, up), refs(pv)
{
}

RFrame_Text::~RFrame_Text(void)
{
}

//***************************************
// RFrame_Image::
//***************************************

bool RFrame_Image::set_fcp(const SFrameCreateParameters& fcp, const SParameters* args)
{
  if(!RFrame_Common::set_fcp(fcp, args))
    return false;
  if(args) {
    IRefObject* ro;
    if(args->get("image", ro))
      ro->t_ref_get_other_interface(image);
  }
  return true;
}

bool RFrame_Image::draw_pre(const SDrawState& S)
{
  int fd = is_enabled() ? 0 : cte_Disabled;
  S->setup_coloralpha( tclr(ctg_Image, cte_Image|cte_Back|fd) );
  if(image) {
    lib2d::DPoint sz = image->get_info().size, bsz = S.base.size();
    lib2d::DPointf tsz(bsz(0)/(float)sz(0), bsz(1)/(float)sz(1));
/*
    float mr = tsz(0)>tsz(1) ? tsz(1) : tsz(0);
    if(mr<1) {
      tsz[0] *= 1/mr;
      tsz[1] *= 1/mr;
    }
*/
    S->render_box(image, S.base.A, S.base.B, lib2d::DPointf(0,0), tsz);
  } else {
    S->render_box(S.base.A, S.base.B);
  }
  return true;
}

//***************************************

RFrame_Image::RFrame_Image(RCollector *own, RFrame_Common *u)
: RFrame_Common(own, u), refs(own) {
}

RFrame_Image::~RFrame_Image(void)
{
}

