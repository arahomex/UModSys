
//***************************************
// RMediaFilter::
//***************************************

struct RMediaFilter : public IBinObjFilter  {
  UMODSYS_REFOBJECT_IMPLEMENT1(UMODSYS_MODULE_NAME(media,images_std)::RMediaFilter, 1, IBinObjFilter)
  UMODSYS_REFOBJECT_REFMODULE()
public:
  bool loader_jpeg(IImage *im, const SInfo& info, const SParameters& fp, int pk, BCStr hint, bool falpha);
  bool loader_png(IImage *im, const SInfo& info, const SParameters& fp, int pk, BCStr hint, bool falpha);
  //
  bool saver_jpeg(const IImage *im, const SInfo& info, const SParameters& fp, BCStr hint, bool falpha);
  bool saver_png(const IImage *im, const SInfo& info, const SParameters& fp, BCStr hint, bool falpha);
protected:
  static void png_err(png_structp png_ptr, png_const_charp msg);
  static void png_read(png_structp png_ptr, png_bytep data, png_size_t length);
  static void png_write(png_structp png_ptr, png_const_bytep data, png_size_t length);
public:
  // SFlags::ISetter
  DMediaFlags::eStates get_flag(int shift) const { return flags.get_s(shift); }
  DMediaFlags get_flags_auto(void) const { return auto_flags; }
  DMediaFlags::eStates set_flag(int shift, DMediaFlags::eStates flag) { return flags.getset_s(shift, flag); }
  //
  // IBinObjFilter::
  bool filter_load(const SInfo& info, IRefObject::P& obj);
  bool filter_load(const SInfo& info, IRefObject* obj);
  bool filter_save(SInfo& info, const IRefObject* obj);
protected:
  SParameters suffix_replace;
  SParameters fp;
  DMediaFlags flags;
  static DMediaFlags auto_flags;
public:
  RMediaFilter(DOwner *own, const SParameters& args) : refs(own) {
  }
  ~RMediaFilter(void) {
  }
  //
  inline bool validate_construction(void) const { return true; }
public:
};

DMediaFlags RMediaFilter::auto_flags;

bool RMediaFilter::filter_load(const SInfo& info, IRefObject::P& obj)
{
  return false;
}

bool RMediaFilter::filter_load(const SInfo& info, IRefObject* obj)
{
  if(obj==NULL)
    return false;
  //
  if(info.common_type=="lib2d::IImage") { // load image
    IImage *im = obj->t_root_get_other_interface<IImage>();
    if(im==NULL)
      return false; // bad type
    int pk = -1; // palette key
    BCStr hint = NULL;
    bool falpha = false;
    fp.get("image_palette_alphakey", pk);
    fp.get("image_hint", hint);
    fp.get("image_alpha", falpha);
    if(info.params) {
      info.params->get("image_palette_alphakey", pk);
      info.params->get("image_hint", hint);
      info.params->get("image_alpha", falpha);
    }
    //
    BCStr name=NULL;
    BCStr suffix = info.media_name;
    while(suffix_replace.enum_names(name)) {
      if(strstr(info.media_name, name)) {
        suffix_replace.get(name, suffix);
      }
    }
    //
    if(is_extension_nocase(suffix, ".jpg") || is_extension_nocase(suffix, ".jpeg"))
      return loader_jpeg(im, info, fp, pk, hint, falpha);
    if(is_extension_nocase(info.media_name, ".png"))
      return loader_png(im, info, fp, pk, hint, falpha);
  }
  return false;
}

bool RMediaFilter::filter_save(SInfo& info, const IRefObject* obj)
{
  if(info.common_type=="lib2d::IImage") { // load image
    const IImage *im = obj->t_root_get_other_interface<IImage>();
    if(im==NULL)
      return false; // bad type
    //
    BCStr hint = NULL;
    bool falpha = false;
    fp.get("image_alpha", falpha);
    fp.get("image_hint", hint);
    if(info.params) {
      info.params->get("image_hint", hint);
      info.params->get("image_alpha", falpha);
    }
    //
    BCStr name=NULL;
    BCStr suffix = info.media_name;
    while(suffix_replace.enum_names(name)) {
      if(strstr(info.media_name, name)) {
        suffix_replace.get(name, suffix);
      }
    }
    //
    if(is_extension_nocase(suffix, ".jpg") || is_extension_nocase(suffix, ".jpeg"))
      return saver_jpeg(im, info, fp, hint, falpha);
    if(is_extension_nocase(info.media_name, ".png"))
      return saver_png(im, info, fp, hint, falpha);
  }
  return false;
}

/*
bool RMediaFilter::set_params(const DHString &kind, const SParameters* filter_params) 
{
  if(kind=="suffix_replace" && filter_params) {
    suffix_replace = *filter_params;
    return true;
  }
  return false;
}
*/

//***************************************
// END
//***************************************
