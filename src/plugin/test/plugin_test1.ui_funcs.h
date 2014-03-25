// RTest1_Shell::

libui::ICollector::P RTest1_Shell::ui_newframes(const DCString &mask, const SParameters& args)
{
  return generate_type<libui::ICollector>(mask, args, "ICollector");
}

libui::ITerminal::P RTest1_Shell::ui_newterm(const DCString &mask, const SParameters& args)
{
  return generate_type<libui::ITerminal>(mask, args, "ITerminal");
}

lib2d::IMultiImage::P RTest1_Shell::ui_newfont(const DCString &mask, const SParameters& args)
{
  return generate_type<lib2d::IMultiImage>(mask, args, "IMultiImage");
}


bool RTest1_Shell::key_pressed(const libui::SKeyboardInputRaw& key)
{
  s_dbg.put(d_UI, cl_Debug, 
    "[KEY.KEY %d,%d,%d,%d]\n",
    key.code, key.status, key.os_code, key.special
  );
  if(key.code==libui::k_escape) {
    f_quit = true;
  }
  return true;
}

bool RTest1_Shell::key_text(const libui::SKeyboardInputText& key)
{
  tl::su::utf8 text[8];
  size_t n = tl::su::utf_32to8(text, 8, &key.text, 1, true);
  text[n] = 0;
  s_dbg.put(d_UI, cl_Debug, 
    "[KEY.TEXT %s]\n",
    text
  );
  return true;
}

bool RTest1_Shell::mouse_event(const libui::SMouseInput& ms)
{
  s_dbg.put(d_UI, cl_Debug, 
    "[MOUSE %x B%x:%d A(%d,%d) R(%d,%d) X(%d,%d)]\n",
    ms.eventid, ms.get_buttonmask(), ms.button_count,
    ms.abs(0), ms.abs(1), ms.rel(0), ms.rel(1), ms.aux(0), ms.aux(1)
  );
  return true;
}

bool RTest1_Shell::command(const libui::SController& ci, int command, const libui::SFrameDataIn& in)
{
  s_dbg.put(d_UI, cl_Debug, 
    "[COMMAND %d@%p C%d D%d]\n",
    ci.context, ci.source, command, 
    in.safe_i()
  );
  return true;
}

bool RTest1_Shell::db_get(const libui::SController& ci, const BCStr hint, const libui::SFrameDataOut& out)
{
  if(ci.context==6) {
    if(out.type==libui::fdt_Integer) {
      if(tl::su::seq(hint, "count")) {
        out.i(50);
        return true;
      } else if(tl::su::seq(hint, "dy")) {
        out.i(16);
        return true;
      }
    }
    return false;
  }
  return false;
}

bool RTest1_Shell::db_get(const libui::SController& ci, const BCStr hint, int sid, const libui::SFrameDataOut& out)
{
  if(ci.context==6) {
    if(out.type==libui::fdt_String) {
      if(tl::su::seq(hint, "text")) {
        char buf[32];
        sprintf(buf, "Item #%d", sid);
        out.s(buf);
        return true;
      }
    }
  }
  return false;
}

bool RTest1_Shell::command_draw(const libui::SController& ci, lib2d::IRenderDriver *drv, const lib2d::DBox &bounds)
{
  return false;
}

