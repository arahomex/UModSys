// RTest1_Shell::

void RTest1_Shell::file_test1(void)
{
  M.con().put(0, "******** file test 1\n");
  libmedia::IStreamReader::P fr;
  libmedia::IStreamWriter::P fw;
  if(TypeId found=M.t_firstobjname<libmedia::IStreamReader>("*::stdio::*")) {
    TParametersA<1024> params;
    params.add("filename", "read-test.txt");
    M.t_generate(fr, found, params);
    M.con().put(0, "  found reader: %s => %p\n", found->name, fr());
  }
  if(TypeId found=M.t_firstobjname<libmedia::IStreamWriter>("*::stdio::*")) {
    TParametersA<1024> params;
    params.add("filename", "write-test.txt");
    params.add("safe", true);
    M.t_generate(fw, found, params);
    M.con().put(0, "  found writer: %s => %p\n", found->name, fw());
  }
  if(fr.valid()) {
    char line[1024];
    size_t req = core::scalar_min(sizeof(line)-1, size_t(fr->reader_size()));
    line[req] = 0;
    if(fr->reader_read(line, req)) {
      M.con().put(0, "  read: {");
      dump_str(line, req);
      M.con().put(0, "}\n", line);
    }
    if(fw.valid()) {
      fw->writer_copy(fr, 0, fr->reader_size());
      fw->writer_close();
      M.con().put(0, "  written\n");
    }
  }
}

void RTest1_Shell::file_test2(void)
{
  M.con().put(0, "******** file test 2\n");
  libmedia::IBinArchive::P arch = media_arch_stdio(".");
  if(!arch.valid())
    return;
  libmedia::IStreamReader::P fr = arch->data_reader("read-test.txt");
  M.con().put(0, "  gen reader: %p\n", fr());
  libmedia::IStreamWriter::P fw = arch->data_writer("write-test.txt", libmedia::SFlags(libmedia::mf_safe::yes()) );
  M.con().put(0, "  gen writer: %p\n", fw());
  if(!fr.valid())
    return;
  char line[1024];
  size_t req = core::scalar_min(sizeof(line)-1, size_t(fr->reader_size()));
  line[req] = 0;
  if(fr->reader_read(line, req)) {
    M.con().put(0, "  read: {");
    dump_str(line, req);
    M.con().put(0, "}\n");
  }
  if(!fw.valid())
    return;
  fw->writer_copy(fr, 0, fr->reader_size());
  fw->writer_close();
  M.con().put(0, "  written\n");
}

void RTest1_Shell::file_test3(void)
{
  M.con().put(0, "******** file test 3\n");
  libmedia::IBinArchive::P arch = media_arch_stdio(".");
  if(!arch.valid())
    return;
  SCMemShared mem_block;
  test_op_file(
    arch->data_load("read-test.txt", mem_block), 
    "read-test.txt", mem_block, true
  );
  test_op_file(
    arch->data_save("write-test.txt", mem_block, libmedia::SFlags(libmedia::mf_safe::yes()) ), 
    "write-test.txt", mem_block, false 
  );
}

void RTest1_Shell::file_test4(void)
{
  M.con().put(0, "******** file test 4\n");
  libmedia::ILibraryBinTree::P lib_vfs = media_vfs();
  if(!lib_vfs.valid())
    return;
  lib_vfs->mount_add(libmedia::ILibraryBinTree::SId("/"), libmedia::ILibraryBinTree::SPoint(media_arch_stdio("."), libmedia::mp_Read));
  lib_vfs->mount_add(libmedia::ILibraryBinTree::SId("/", 1), libmedia::ILibraryBinTree::SPoint(media_arch_stdio("./write-dir"), libmedia::mp_RWL));
  //
  // test order
  SCMemShared mem_block;
  test_op_file(
    lib_vfs->bin_load("/read-test.txt", mem_block), 
    "/read-test.txt", mem_block, true
  );
  SCMemShared mem_block2(~mem_block/2);
  memcpy((void*)mem_block2.get_data(), mem_block.get_data(), mem_block2.get_size());
  test_op_file(
    lib_vfs->bin_save("/write-test.txt", mem_block2, libmedia::DMediaFlags(libmedia::mf_safe::yes()) ),
    "/write-test.txt", mem_block2, false
  );
  test_op_file(
    lib_vfs->bin_load("/write-test.txt", mem_block), 
    "/write-test.txt", mem_block, true
  );
}

void RTest1_Shell::file_test5(void)
{
  M.con().put(0, "******** file test 5\n");
  libmedia::ILibraryLayered::P lib = media_lay();
  if(!lib.valid())
    return;
  //
  {
    libmedia::ILibraryBinTree::P lib_vfs = media_vfs();
    if(!lib_vfs.valid())
      return;
    lib_vfs->mount_add("/", media_arch_stdio("."), libmedia::mp_Read);
    lib_vfs->mount_add("/", 1, media_arch_stdio("./write-dir"), libmedia::mp_RWL);
    lib->layer_push( lib_vfs );
  }
  //
  {
    libmedia::ILibraryObjFilter::P lib_flt = media_flt();
    if(!lib_flt.valid())
      return;
    TParametersA<1024> params;
    libmedia::IBinObjFilter::P flt = new RLines_Filter(refs.owner, params);
    lib_flt->filters_add(flt, &params, 0);
    lib->layer_push( lib_flt );
  }
  {
    lib->layer_push( media_cache(false) );
    lib->layer_push( media_cache(true) );
  }
  //
  // test order
  SCMemShared mem_block;
  test_op_file(
    lib->bin_load("/read-test.txt", mem_block), 
    "/read-test.txt", mem_block, true
  );
  SCMemShared mem_block2(~mem_block/2);
  memcpy((void*)mem_block2.get_data(), mem_block.get_data(), mem_block2.get_size());
  test_op_file(
    lib->bin_save("/write-test.txt", mem_block2, libmedia::DMediaFlags(libmedia::mf_safe::yes()) ),
    "/write-test.txt", mem_block2, false
  );
  test_op_file(
    lib->bin_load("/write-test.txt", mem_block), 
    "/write-test.txt", mem_block, true
  );
  //
  lib->set_flag(libmedia::mf_objects::base_shift, libmedia::DMediaFlags::Yes);
  lib->set_flag(libmedia::mf_objsave::base_shift, libmedia::DMediaFlags::Yes);
  //
  // load lines
  for(int ii=0; ii<2; ii++) {
    ILines::P lines;
    test_op_file(
      lib->t_obj_get("/read-test.txt", lines), 
      "/read-test.txt", "new object [ILines]"
    );
    if(lines.valid()) {
      M.con().put(0, "  ILines: %p {\n", lines());
      for(size_t i=0; i<~lines->lines; i++) {
        M.con().put(0, "    line %u: %s\n", (int)i, lines->lines(i)());
      }
      M.con().put(0, "  } ILines: %p \n", lines());
      test_op_file(
        lib->obj_save("/write-test-2.txt", lines), 
        "/write-test-2.txt", "save object [ILines]"
      );
    }
  }
}

void RTest1_Shell::file_test6(void)
{
  M.con().put(0, "******** file test 6\n");
  libmedia::IUtilities::P utils = media_utils();
  libmedia::ILibraryLayered::P lib = media_lay();
  if(!utils.valid() || !lib.valid())
    return;
  //
  {
    libmedia::ILibraryBinTree::P lib_vfs = media_vfs();
    if(!lib_vfs.valid())
      return;
    lib_vfs->mount_add("/", media_arch_stdio("."), libmedia::mp_Read);
    lib_vfs->mount_add("/", 1, media_arch_stdio("./write-dir"), libmedia::mp_RWL);
    lib->layer_push( lib_vfs );
  }
  //
  {
    libmedia::ILibraryObjFilter::P lib_flt = media_flt();
    if(!lib_flt.valid())
      return;
    TParametersA<1024> params;
    libmedia::IBinObjFilter::P flt = media_filter_new("*::images_std::*", params);
    lib_flt->filters_add(flt, &params, 0);
    lib->layer_push( lib_flt );
  }
  {
    lib->layer_push( media_cache(false) );
    lib->layer_push( media_cache(true) );
  }
  //
  {
    libmedia::ISerializeWriter::P wr;
    TParametersA<1024> params;
    params.add("stream", lib->bin_writer("/test.json"));
    wr = generate_type<libmedia::ISerializeWriter>("*json*", params);
    if(wr.invalid())
      return;
    wr->writer_begin(NULL, libmedia::SSerializeValue::t_Array);
    wr->writer_value(NULL, 1.0f);
    wr->writer_value(NULL, 12);
    wr->writer_value(NULL, wr->s_true());
    wr->writer_value(NULL, wr->s_false());
    wr->writer_value(NULL, wr->s_null());
    wr->writer_value(NULL, "12 \r \t \\ \"[]\" \x03");
    {
      wr->writer_begin(NULL, libmedia::SSerializeValue::t_Hash);
      wr->writer_key(NULL, "a");
      wr->writer_value(NULL, 15);
      wr->writer_value(NULL, 33.2);
      wr->writer_end();
    }
    {
      wr->writer_begin(NULL, libmedia::SSerializeValue::t_Hash, libmedia::sero_Compact);
      wr->writer_key(NULL, "a", libmedia::sero_Compact);
      wr->writer_value(NULL, 15, libmedia::sero_Compact);
      wr->writer_value(NULL, 33.2, libmedia::sero_Compact);
      wr->writer_end();
    }
    wr->writer_end();
  }
  {
    libmedia::ISerializeWriter::P wr;
    {
      TParametersA<1024> params;
      params.add("stream", lib->bin_writer("/test2.json"));
      wr = generate_type<libmedia::ISerializeWriter>("*json*", params);
      if(wr.invalid())
        return;
    }
    libmedia::ISerializeReader::P rd;
    {
      TParametersA<1024> params;
      params.add("stream", lib->bin_reader("/test.json"));
      rd = generate_type<libmedia::ISerializeReader>("*json*", params);
      if(rd.invalid())
        return;
    }
    if(!rd->reader_parse(wr)) {
      M.con().put(0, "parse error: %s\n", rd->reader_lasterror());
    } else {
      M.con().put(0, "configuration parsed\n");
    }
  }
  //
  lib2d::IImage::P img = new_mem_image();
  bool rv = lib->obj_load("/minecraft_textures.png", img, "lib2d::IImage");
  M.con().put(0, "loaded %d\n", rv);
  rv = lib->obj_save("/minecraft_textures_new.png", img, "lib2d::IImage");
  M.con().put(0, "saved %d\n", rv);
}

void RTest1_Shell::file_test7(void)
{
  typedef tl::TDynarrayDynamic<libmedia::SFileInfo> FileList;
  //
  M.con().put(0, "******** file test 6\n");
  libmedia::IUtilities::P utils = media_utils();
  libmedia::ILibraryLayered::P lib = media_lay();
  if(!utils.valid() || !lib.valid())
    return;
  //
  {
    libmedia::ILibraryBinTree::P lib_vfs = media_vfs();
    if(!lib_vfs.valid())
      return;
    lib_vfs->mount_add("/", media_arch_stdio("."), libmedia::mp_Read);
    lib_vfs->mount_add("/", 1, media_arch_stdio("./write-dir"), libmedia::mp_RWL);
    lib->layer_push( lib_vfs );
    //
    lib_vfs->mount_add("/mc/", 5, media_arch_zip(lib, utils, "/mctest.zip"), libmedia::mp_RL);
  }
  //
  {
    libmedia::ILibraryObjFilter::P lib_flt = media_flt();
    if(!lib_flt.valid())
      return;
    TParametersA<1024> params;
    libmedia::IBinObjFilter::P flt = media_filter_new("*::images_std::*", params);
    lib_flt->filters_add(flt, &params, 0);
    lib->layer_push( lib_flt );
  }
  {
    lib->layer_push( media_cache(false) );
    lib->layer_push( media_cache(true) );
  }
  //
  if(0) {
    FileList lst;
    tl::TRStackSocket<libmedia::SFileInfo,FileList> slst(lst);
    bool rv = lib->bin_list("/mc/*.bin", slst);
    M.con().put(0, "find %d : %d elems\n", rv, ~lst);
    for(size_t i=0; i<~lst; i++) {
      M.con().put(0, "  [%u] : %s\n", i, lst[i].name());
    }
  }
  //
  lib2d::IImageFactory::P factory = new_mem_imagefactory();
  lib2d::IMultiImage::P atlas = new_mem_multiimage(factory);
  if(!atlas->setup_fixed_cell(16, 16)
    || !atlas->set_layer_count(3))
    return;
  if(!atlas->get_layer(0)->set_info(lib2d::it_R8G8B8A8, 512, 256))
    return;
  if(!atlas->get_layer(1)->set_info(lib2d::it_R8G8B8A8, 128, 512))
    return;
  if(!atlas->get_layer(2)->set_info(lib2d::it_R8G8B8A8, 32, 1024))
    return;
  FileList lst;
  {
    tl::TRStackSocket<libmedia::SFileInfo,FileList> slst(lst);
    bool rv = lib->bin_list("/mc/textures/blocks/*.png", slst);
  }
  lib2d::IImage::P src = factory->image_new(), dst;
  char buf[0x40000];
  static const char* names[]={
    "/atlas1.png",
    "/atlas2.png",
    "/atlas3.png",
    NULL
  };
  for(int key=0; key<3; key++) {
    int loaded = 0, filled = 0;
    int row_dy = 0;
    int x = 0, y = 0;
    dst = atlas->get_layer(key);
    const lib2d::SImageInfo& di = dst->get_info();
    SMem membuf(buf, sizeof(buf));
    for(size_t i=0; i<~lst; i++) {
      if(lib->obj_load(lst[i].name.s(), src, "lib2d::IImage")) {
//        M.con().put(0, "...loaded at %d : '%s'\n", i, lst[i].name());
        const lib2d::SImageInfo& si = src->get_info();
        int key2 = si.size(1)==16 ? 0 
                 : si.size(1)<=512 ? 1 
                 : 2;
        if(key2 != key)
          continue; // skipped
        if(key!=0) {
          M.con().put(0, "...loaded at %d : '%s' size (%d, %d)\n", i, lst[i].name(), si.size(0), si.size(1));
        }
        lib2d::SImageCellInfo ici(lib2d::if_Rect, 0, x, y, src->get_info().size(0), src->get_info().size(1));
        if(ici.start(0)+ici.size(0)>di.size(0)) {
          x = 0;
          y += row_dy;
          row_dy = 0;
        }
        if(y + ici.size(1) > di.size(1)) {
          M.con().put(0, "...overflow at %d : '%s'\n", i, lst[i].name());
          break; // overflow
        }
        if(row_dy<ici.size(1))
          row_dy = ici.size(1);
        loaded++;
        x += ici.size(0);
        if(atlas->setup_variable_cell(&ici, 1, i)) {
          membuf.size = src->get_info().getbinsize();
          if(membuf.size>sizeof(buf)) {
            M.con().put(0, "...buf overflow at %d : '%s'\n", i, lst[i].name());
            break; // overflow
          }
          if(src->get_data(lib2d::SImagePatchInfo(src->get_info(), lib2d::DPoint(0,0)), membuf)) {
            if(dst->set_data(lib2d::SImagePatchInfo(src->get_info(), ici.start), membuf)) {
              filled++;
            }
          }
        }
      }
    }
    M.con().put(0, "loaded %d/%d images\n", loaded, ~lst);
    bool rv = lib->obj_save(names[key], dst, "lib2d::IImage");
    M.con().put(0, "saved %d\n", rv);
  }
//  bool rv = lib->obj_load("/mc/textures/font/ascii.png", img, "lib2d::IImage");
//  M.con().put(0, "loaded %d\n", rv);
  //
}
