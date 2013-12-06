//------------------------------------
// Dialog
//------------------------------------

struct RDialog : public IDialog {
  UMODSYS_REFOBJECT_IMPLEMENT1(U_MOD::RDialog, 2, IDialog)
  UMODSYS_REFOBJECT_REFOTHER(RCollector)
  //
  bool close(void);
  IFrame* get_root(void);
  IFrame* get_focused(void);
  bool set_focused(IFrame* fr);
  //
  RFrame_Common::P root;
  IFrame::P focused;
  //
  RDialog(RCollector* c, RFrame_Common* r);
  ~RDialog(void);
  //
  bool dopen(void);
  bool dclose(void);
  bool dpause(bool paused);
};

//***************************************
// RDialog::
//***************************************

IFrame* RDialog::get_root(void)
{
  return root;
}

IFrame* RDialog::get_focused(void)
{
  return focused;
}

bool RDialog::set_focused(IFrame* fr)
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

//***************************************

bool RDialog::dclose(void)
{
  set_focused(NULL);
  root->set_flag(ff_Hidden, true);
  return true;
}

bool RDialog::dpause(bool paused)
{
  root->set_flag(ff_Disabled, paused);
  return true;
}

//***************************************

RDialog::RDialog(RCollector* c, RFrame_Common* r)
: root(r), refs(c)
{
  root->dialog = this;
}

RDialog::~RDialog(void)
{
  root->dialog = NULL;
}

//***************************************
// ::
//***************************************
