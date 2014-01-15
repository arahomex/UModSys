// 
// RMediaFilter class
// 
struct RSDLMediaFilter : public IMediaFilter {
  UMS_REFOBJECT_RIMPLEMENT_DEF(RSDLMediaFilter, IMediaFilter, const RGenerator)
  //
  //
  bool load(IRefObject::P& obj, const SMediaFilter& info, const SParameters& fp) {
    return false;
  }
  bool load(IRefObject* obj, const SMediaFilter& info, const SParameters& filter_params) {
    return false;
  }
  bool save(IRefObject* obj, SMediaFilter& info, const SParameters& filter_params) {
    return false;
  }
  bool set_paramb(const DHString &kind, const DHString &name, const SCMem& memory) {
    return false;
  }
  bool set_params(const DHString &kind, const SParameters* filter_params) {
    return false;
  }
  //
  RSDLMediaFilter(const RGenerator* pv, const SParameters& args) {
    rc_init(pv);
  }
};

// 
// RMediaFilter::
// 
UMS_REFOBJECT_RIMPLEMENT_BODY(RSDLMediaFilter)


// 
// RGenerator_Terminal class
// 

RGenerator::RGenerator(void) 
: term(this) 
{
  rc_init();
}

int RGenerator::get_generated_types(DListStr& list, const DHString& type) const 
{
  int rv = IMediaFilter::_get_interface_types(list);
  return rv + UI::ITerminal::_get_interface_types(list);
}

int RGenerator::get_generated_names(DListStr& list) const
{
  list << "RSDLTerminal";
  list << "RSDLMediaFilter";
#if defined(SDLV13)
  list << "RSDL13Terminal";
  list << "RSDL13MediaFilter";
#elif defined(SDLV12)
  list << "RSDL12Terminal";
  list << "RSDL12MediaFilter";
#endif
  return ~list;
}

bool RGenerator::generate(IRefObject::P& obj, const DHString& type, const SParameters& args) 
{
  if(RSDLMediaFilter::_is_interface_supported(type)) {
    obj = new RSDLMediaFilter(this, args);
    return obj;
  }
  if(
#if defined(SDLV13)
  type=="RSDL13Terminal" 
#elif defined(SDLV12)
  type=="RSDL12Terminal"
#endif
    || RSDLTerminal::_is_interface_supported(type)) {
    term.terminal_reset();
    int flags = 0, x;
    if(args.get("all", x) && x) {
      flags |= SDL_INIT_EVERYTHING;
    }
    if(args.get("video", x) && x) {
      flags |= SDL_INIT_VIDEO;
    }
    if(args.get("audio", x) && x) {
      flags |= SDL_INIT_AUDIO;
    }
    if(args.get("timer", x) && x) {
      flags |= SDL_INIT_TIMER;
    }
    if(args.get("cdrom", x) && x) {
      flags |= SDL_INIT_CDROM;
    }
    if(args.get("joystick", x) && x) {
      flags |= SDL_INIT_JOYSTICK;
    }
    if(args.get("keyboard", x) && x) {
      // always set
    }
    if(args.get("mouse", x) && x) {
      // always set
    }
    if(args.get("eventmaster", x) && x) {
//      flags |= SDL_INIT_EVENTTHREAD;
    }
    if(args.get("parachute", x) && x==0) {
      flags |= SDL_INIT_NOPARACHUTE;
    }
    if(flags & SDL_INIT_VIDEO) {
      BCStr videodriver = NULL;
      if(SDL_Init(flags & ~SDL_INIT_VIDEO))
        return false;
      if(args.get("videodriver", videodriver)) {
      }
      if(term.open(videodriver, flags)<0)
        return false;
      SDL_EnableUNICODE(1);
    } else {
      int rv = SDL_Init(flags);
      SDL_EnableUNICODE(1);
      if(rv)
        return false;
    }
    obj = static_cast<UI::ITerminal*>(&term);
    return true;
  }
  return false;
}

