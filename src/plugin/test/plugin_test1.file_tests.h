// RTest1_Shell::

lib2d::IImage::P RTest1_Shell::new_mem_image(void)
{
  TParametersA<1024> params;
  return generate_type<lib2d::IImage>("*::stdlib::*Image_Memory", params);
}

libmedia::IBinArchive::P RTest1_Shell::media_arch_stdio(const DCString &path)
{
  TParametersA<1024> params;
  params.add("pathname", path());
  return generate_type<libmedia::IBinArchive>("*::stdio::*", params, path);
}

libmedia::IBinObjFilter::P RTest1_Shell::media_filter_new(const DCString &mask, const SParameters& args)
{
  TParametersA<1024> params;
  return generate_type<libmedia::IBinObjFilter>(NULL, params);
}

libmedia::ILibraryBinTree::P RTest1_Shell::media_vfs(void)
{
  TParametersA<1024> params;
  return generate_type<libmedia::ILibraryBinTree>("*::std::*", params);
}

libmedia::ILibraryObjFilter::P RTest1_Shell::media_flt(void)
{
  TParametersA<1024> params;
  return generate_type<libmedia::ILibraryObjFilter>("*::std::*", params);
}

libmedia::ILibraryLayered::P RTest1_Shell::media_lay(void)
{
  TParametersA<1024> params;
  return generate_type<libmedia::ILibraryLayered>("*::std::*", params);
}

libmedia::ILibrary::P RTest1_Shell::media_cache(bool isobj)
{
  TParametersA<1024> params;
  if(isobj)
    return generate_type<libmedia::ILibraryObjCache>("*::std::*", params)();
  return generate_type<libmedia::ILibraryBinCache>("*::std::*", params)();
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
