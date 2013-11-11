//------------------------------------
// CollectorTheme
//------------------------------------

enum eCT_Groups {
  ctg_General,
  ctg_Text,
  ctg_Edit,
  ctg_Image,
  ctg_Button,
  ctg_List,
  ctg_Unknown
};

enum eCT_Elems {
  cte_FMask      = 0x00ff,
  cte_Back       = 0x0001,
  cte_Fore       = 0x0002,
  cte_Selected   = 0x0004,
  cte_Focused    = 0x0008,
  cte_Active     = 0x0010,
  cte_Disabled   = 0x0020,
  cte_Pressed    = 0x0040,
  //
  cte_KMask      = 0xff00,
  cte_Default    = 0x0000,
  cte_Text       = 0x0100,
  cte_Image      = 0x0200,
  cte_Button     = 0x0300,
  cte_Edge1      = 0x0400,
  cte_Edge2      = 0x0500,
  cte_Edges      = 0x0600,
  cte_Caret      = 0x0700,
  //
  cte_Unknown    = 0
};

const int max_line_length = 0x01000;
const int max_text_length = 0x10000;

//------------------------------------
// struct button
//------------------------------------

struct SDrawState {
  const SDrawState* parent;
  lib2d::DBox base;
  lib2d::IMultiImage* font;
  lib2d::IRenderDriver *driver;
  RDialog* dialog;
  //
  inline SDrawState(lib2d::IMultiImage* f, lib2d::IRenderDriver *d, const lib2d::DBox& main, RDialog* dlg=NULL)
    : base(main), driver(d), font(f), parent(NULL), dialog(dlg) {}   
  inline SDrawState(const SDrawState& R, const lib2d::DBox& sub, lib2d::IMultiImage* f)
    : base(R.base, sub), driver(R.driver), font(f ? f : R.font), parent(&R), dialog(NULL) {}
  //
  inline lib2d::IRenderDriver* operator->(void) const { return driver; }
  inline operator lib2d::IMultiImage*(void) const { return font; }
};

struct SButton {
  RFrame_Common *frame;
  lib2d::DBox size;
  SFrameSize fs;
  int id;
  bool enabled, active, pressed, special;
  lib2d::DPoint pressed_mouse_pos;
  //
  SButton(RFrame_Common *frame, int id, const SFrameSize &fs);
  ~SButton(void);
  //
  bool is_enabled(void);
  bool update_size(const lib2d::DBox &parsize);
  bool mouse(const SMouseInput* ms);
  bool draw(const SDrawState& S);
  //
  bool draw_button(const SDrawState& S, int group, int addflag);
  bool draw_text(const SDrawState& S, const DCString& text, int group, int addflag);
  bool draw_arrow(const SDrawState& S, int angle, int group, int addflag);
  bool draw_progress(const SDrawState& S, float ratio, int angle, int group, int addflag);
};

//------------------------------------
// CollectorTheme
//------------------------------------

struct RCollectorTheme : public ICollectorThemeTemplate {
  int get_group_index(BCStr name);
  int get_elem_index(BCStr name);
  //
  RCollectorTheme(ICollectorThemeTree *pv);
  ~RCollectorTheme(void);
};


//------------------------------------
// 
//------------------------------------
