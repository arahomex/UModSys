// RTest1_Shell::

lib2d::IImage::P RTest1_Shell::new_mem_image(void)
{
  TParametersA<1024> params;
  return generate_type<lib2d::IImage>("*::stdlib::*Image_Memory", params);
}

lib2d::IImageFactory::P RTest1_Shell::new_mem_imagefactory(void)
{
  TParametersA<1024> params;
  return generate_type<lib2d::IImageFactory>("*::stdlib::*ImageFactory_Memory", params);
}

lib2d::IMultiImage::P RTest1_Shell::new_mem_multiimage(lib2d::IImageFactory* imgf)
{
  TParametersA<1024> params;
  params.add("factory", imgf);
  return generate_type<lib2d::IMultiImage>("*::stdlib::*MultiImage", params);
}

libmedia::IBinArchive::P RTest1_Shell::media_arch_stdio(const DCString &path)
{
  TParametersA<1024> params;
  params.add("filepath", path());
  return generate_type<libmedia::IBinArchive>("*::stdio::*", params, path);
}

libmedia::IBinArchive::P RTest1_Shell::media_arch_zip(libmedia::ILibrary* root, libmedia::IUtilities* utl, const DCString &path, bool rw)
{
  TParametersA<1024> params;
  libmedia::IBinArchiveFrame::IClient::P cli = generate_type<libmedia::IBinArchiveFrame::IClient>("*_ZIP", params);
  if(!cli.valid())
    return NULL;
  params.add("write", rw);
  params.add("client", cli);
  params.add("filename", path);
  params.add("library", root);
  libmedia::IBinArchiveFrame::P rv = generate_type<libmedia::IBinArchiveFrame>("*::stdlib::*", params, path)();
  if(!rv.valid())
    return NULL;
  rv->utils = utl;
  return rv();
}

libmedia::IBinObjFilter::P RTest1_Shell::media_filter_new(const DCString &mask, const SParameters& args)
{
  TParametersA<1024> params;
  return generate_type<libmedia::IBinObjFilter>(NULL, params);
}

libmedia::IUtilities::P RTest1_Shell::media_utils(void)
{
  TParametersA<1024> params;
  return generate_type<libmedia::IUtilities>("*::stdlib::*", params);
}

libmedia::ILibraryBinTree::P RTest1_Shell::media_vfs(void)
{
  TParametersA<1024> params;
  return generate_type<libmedia::ILibraryBinTree>("*::stdlib::*", params);
}

libmedia::ILibraryObjFilter::P RTest1_Shell::media_flt(void)
{
  TParametersA<1024> params;
  return generate_type<libmedia::ILibraryObjFilter>("*::stdlib::*", params);
}

libmedia::ILibraryLayered::P RTest1_Shell::media_lay(void)
{
  TParametersA<1024> params;
  return generate_type<libmedia::ILibraryLayered>("*::stdlib::*", params);
}

libmedia::ILibrary::P RTest1_Shell::media_cache(bool isobj)
{
  TParametersA<1024> params;
  if(isobj)
    return generate_type<libmedia::ILibraryObjCache>("*::stdlib::*", params)();
  return generate_type<libmedia::ILibraryBinCache>("*::stdlib::*", params)();
}

void RTest1_Shell::test_op_file(bool f, const DCString &fname, const DCString &operation)
{
  if(f) {
    M.con().put(0, "  %s file {%s} ok\n", operation(), fname());
  } else {
    M.con().put(0, "  ERROR %s file {%s}\n", operation(), fname());
  }
}

void RTest1_Shell::test_op_file(bool f, const DCString &fname, SCMemShared& mem_block, bool isRead)
{
  if(isRead) {
    if(f) {
      M.con().put(0, "  load file {%s} ok, size:%u {", fname(), int(~mem_block));
      dump_str(mem_block.get_tdata<char>(), ~mem_block);
      M.con().put(0, "}\n");
    } else {
      M.con().put(0, "  ERROR load file {%s}\n", fname());
    }
  } else {
    if(f) {
      M.con().put(0, "  save file {%s} ok, size:%u\n", fname(), ~mem_block);
    } else {
      M.con().put(0, "  ERROR save file {%s}\n", fname());
    }
  }
}

