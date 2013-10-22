// RTest1_Shell::

libmedia::IBinArchive::P RTest1_Shell::media_arch_stdio(const DCString &path)
{
  if(TypeId found=M.t_firstobjname<libmedia::IBinArchive>("*::stdio::*")) {
    TParametersA<1024> params;
    params.add("pathname", path());
    //
    libmedia::IBinArchive::P arch;
    if(M.t_generate(arch, found, params)) {
      M.con().put(0, "  found sys archive {%s}: %s => %p\n", path(), found->name, arch());
      return arch;
    }
  }
  M.con().put(0, "  fail sys archive {%s}\n", path());
  return NULL;
}

libmedia::ILibraryBinTree::P RTest1_Shell::media_vfs(void)
{
  libmedia::ILibraryBinTree::P lib;
  if(TypeId found=M.t_firstobjname<libmedia::ILibraryBinTree>("*::std::*")) {
    TParametersA<1024> params;
    if(M.t_generate(lib, found, params)) {
      M.con().put(0, "  found lib(bin-tree): %s => %p\n", found->name, lib());
      return lib;
    }
  }
  M.con().put(0, "  fail lib(bin-tree)\n");
  return NULL;
}

libmedia::ILibraryObjFilter::P RTest1_Shell::media_flt(void)
{
  libmedia::ILibraryObjFilter::P lib;
  if(TypeId found=M.t_firstobjname<libmedia::ILibraryObjFilter>("*::std::*")) {
    TParametersA<1024> params;
    if(M.t_generate(lib, found, params)) {
      M.con().put(0, "  found lib(obj-filter): %s => %p\n", found->name, lib());
      return lib;
    }
  }
  M.con().put(0, "  fail lib(obj-filter)\n");
  return NULL;
}

libmedia::ILibraryLayered::P RTest1_Shell::media_lay(void)
{
  libmedia::ILibraryLayered::P lib;
  if(TypeId found=M.t_firstobjname<libmedia::ILibraryLayered>("*::std::*")) {
    TParametersA<1024> params;
    if(M.t_generate(lib, found, params)) {
      M.con().put(0, "  found lib(layered): %s => %p\n", found->name, lib());
      return lib;
    }
  }
  M.con().put(0, "  fail lib(layered)\n");
  return NULL;
}


void RTest1_Shell::test_op_file(bool f, const DCString &fname, SCMemShared& mem_block, bool isRead)
{
  if(isRead) {
    if(f) {
      M.con().put(0, "  load file {%s} size:%u {", fname(), int(~mem_block));
      dump_str(mem_block.get_tdata<char>(), ~mem_block);
      M.con().put(0, "}\n");
    } else {
      M.con().put(0, "  not load file {%s}\n", fname());
    }
  } else {
    if(f) {
      M.con().put(0, "  save file {%s} size:%u\n", fname(), ~mem_block);
    } else {
      M.con().put(0, "  not save file {%s}\n", fname());
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
  lib_vfs->mount_add(libmedia::ILibraryBinTree::SMountPoint(media_arch_stdio("."), libmedia::mp_Read), "/");
  lib_vfs->mount_add(libmedia::ILibraryBinTree::SMountPoint(media_arch_stdio("./write-dir"), libmedia::mp_RWL), "/", 1);
  //
  // test order
  SCMemShared mem_block;
  test_op_file(
    lib_vfs->bin_load(mem_block, "/read-test.txt"), 
    "/read-test.txt", mem_block, true
  );
  SCMemShared mem_block2(~mem_block/2);
  memcpy((void*)mem_block2.get_data(), mem_block.get_data(), mem_block2.get_size());
  test_op_file(
    lib_vfs->bin_save(mem_block2, "/write-test.txt", libmedia::DMediaFlags(libmedia::mf_safe::yes()) ),
    "/write-test.txt", mem_block2, false
  );
  test_op_file(
    lib_vfs->bin_load(mem_block, "/write-test.txt"), 
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
    lib_vfs->mount_add(libmedia::ILibraryBinTree::SMountPoint(media_arch_stdio("."), libmedia::mp_Read), "/");
    lib_vfs->mount_add(libmedia::ILibraryBinTree::SMountPoint(media_arch_stdio("./write-dir"), libmedia::mp_RWL), "/", 1);
    lib->layer_insert( libmedia::ILibraryLayered::SLayer(lib_vfs) );
  }
  //
  {
    libmedia::ILibraryObjFilter::P lib_flt = media_flt();
    if(!lib_flt.valid())
      return;
    TParametersA<1024> params;
    libmedia::IBinObjFilter::P flt = new RLines_Filter(refs.owner, params);
    lib_flt->filters_add(flt, &params, 0);
    lib->layer_insert( libmedia::ILibraryLayered::SLayer(lib_flt) );
  }
  //
  // test order
/*
  SCMemShared mem_block;
  test_op_file(
    lib_vfs->bin_load(mem_block, "/read-test.txt"), 
    "/read-test.txt", mem_block, true
  );
  SCMemShared mem_block2(~mem_block/2);
  memcpy((void*)mem_block2.get_data(), mem_block.get_data(), mem_block2.get_size());
  test_op_file(
    lib_vfs->bin_save(mem_block2, "/write-test.txt", libmedia::DMediaFlags(libmedia::mf_safe::yes()) ),
    "/write-test.txt", mem_block2, false
  );
  test_op_file(
    lib_vfs->bin_load(mem_block, "/write-test.txt"), 
    "/write-test.txt", mem_block, true
  );
*/
}
