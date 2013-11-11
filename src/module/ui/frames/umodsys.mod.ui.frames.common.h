//***************************************
// RCollectorTheme::
//***************************************

int RCollectorTheme::get_group_index(BCStr name)
{
  if(tl::su::seq(name, "general"))
    return ctg_General;
  if(tl::su::seq(name, "common"))
    return ctg_General;
  if(tl::su::seq(name, "text"))
    return ctg_Text;
  if(tl::su::seq(name, "edit"))
    return ctg_Edit;
  if(tl::su::seq(name, "button"))
    return ctg_Button;
  if(tl::su::seq(name, "list"))
    return ctg_List;
  if(tl::su::seq(name, "image") || tl::su::seq(name, "picture"))
    return ctg_Image;
  return 0;
}

int RCollectorTheme::get_elem_index(BCStr name)
{
  int rv = 0;
  //
  if(tl::su::sstr(name, "text")) rv |= cte_Text;
  else if(tl::su::sstr(name, "button")) rv |= cte_Button;
  else if(tl::su::sstr(name, "edge1")) rv |= cte_Edge1;
  else if(tl::su::sstr(name, "edge2")) rv |= cte_Edge2;
  else if(tl::su::sstr(name, "edges")) rv |= cte_Edges;
  else if(tl::su::sstr(name, "caret")) rv |= cte_Caret;
  else if(tl::su::sstr(name, "image") || tl::su::sstr(name, "picture")) rv |= cte_Image;
  //
  if(tl::su::sstr(name, "back")) rv |= cte_Back;
  if(tl::su::sstr(name, "fore")) rv |= cte_Fore;
  if(tl::su::sstr(name, "select")) rv |= cte_Selected;
  if(tl::su::sstr(name, "selected")) rv |= cte_Selected;
  if(tl::su::sstr(name, "focus")) rv |= cte_Focused;
  if(tl::su::sstr(name, "focused")) rv |= cte_Focused;
  if(tl::su::sstr(name, "active")) rv |= cte_Active;
  if(tl::su::sstr(name, "disabled")) rv |= cte_Disabled;
  //
  return rv;
}

RCollectorTheme::RCollectorTheme(ICollectorThemeTree *pv)
: ICollectorThemeTemplate(pv)
{
}

RCollectorTheme::~RCollectorTheme(void)
{
}

//***************************************
// ::
//***************************************
