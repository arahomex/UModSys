
//***************************************
// RMediaFilter::
//***************************************

void RMediaFilter::png_err(png_structp png_ptr, png_const_charp msg) 
{
   if(png_ptr) { 
     M.con().put(0, "PNG FATAL ERROR: png error - %s\n",msg);
   } 
}

void RMediaFilter::png_read(png_structp png_ptr, png_bytep data, png_size_t length) 
{
   SCMem *vdata = static_cast<SCMem *>(png_get_io_ptr(png_ptr));
   if(vdata->size<size_t(length))
     png_error(png_ptr, "Read Error"); // not enougth size
   //
   memcpy(data, vdata->data, length);
   vdata->move(length);
}

void RMediaFilter::png_write(png_structp png_ptr, png_bytep data, png_size_t length) 
{
   SMem *vdata = static_cast<SMem *>(png_get_io_ptr(png_ptr));
   if(vdata->size<size_t(length))
     png_error(png_ptr, "Write Error"); // not enougth size
   //
   memcpy(vdata->data, data, length);
   vdata->move(length);
}

bool RMediaFilter::loader_png(IImage *img, const SInfo& info, const SParameters& fp, int pk, BCStr hint, bool falpha) 
{
  falpha = falpha || pk>=0;
  if(info.binary.get_size()<=8)
    return false; // bad size
  png_bytep data = static_cast<png_bytep>(const_cast<void*>(info.binary.get_data()));
  SCMem vdata = info.binary;
  //
  if(!png_check_sig(data, 8))
    return false; // bad PNG sig
  //
  png_structp png_ptr; 
  png_infop info_ptr; 
  png_uint_32 width, height;
  bool alpha;
  int bitdepth, colortype, interlace, compression, filter;
//  char png_load_buffer[0x10000];
  //
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, png_err, NULL); 
  if (!png_ptr) 
    return false; // error create internal struct
  info_ptr = png_create_info_struct(png_ptr); 
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, 0, 0); 
    return false; // error create internal info struct
  } 
  //
  png_set_read_fn(png_ptr, &vdata, png_read); 
  png_set_sig_bytes(png_ptr, 8);   // Tell png that we read the signature 
  vdata.move(8);
  png_read_info(png_ptr, info_ptr);   // Read the info section of the png file 
  png_get_IHDR(png_ptr, info_ptr, &width, &height, 
               &bitdepth, &colortype, 
               &interlace, &compression, &filter);   // Extract info 
  //
  if ( bitdepth != 8) {
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);   // Clean up memory 
    return false;
  }
  //        
  if(colortype==PNG_COLOR_TYPE_RGBA) alpha = true; 
  else if(colortype==PNG_COLOR_TYPE_RGB)  alpha = false; 
  else {
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);   // Clean up memory 
    return false;
  }
  //        
  if(interlace!=PNG_INTERLACE_NONE) {
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);   // Clean up memory 
    return false;
  }
  // Update the changes 
  png_read_update_info(png_ptr, info_ptr); 
  png_get_IHDR(png_ptr, info_ptr, &width, &height, 
               &bitdepth, &colortype, 
               &interlace, &compression, &filter);   // Extract info 
  //now read the png stucture into a simple bitmap array 
  SMemShared mem;
  int RN = alpha ? width*4 : width*3; 
  int N = RN*height;
  if(!mem.alloc(N, UMODSYS_SOURCEINFO)) {
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);   // Clean up memory 
    return false;
  }
  //
  eImageType fmt, ifmt;
  fmt = alpha ? it_R8G8B8A8 : it_R8G8B8;
  ifmt = falpha || alpha ? it_R8G8B8A8 : it_R8G8B8;
  //
  png_bytep rows; mem.t_data(rows);
  for(int y = 0; y < height; y++, rows+=RN) {
    png_read_row(png_ptr, rows, 0);
  }
  png_destroy_read_struct(&png_ptr, &info_ptr, 0);   // Clean up memory 
  //
  if(!img->set_info(ifmt, width, height))
    return false;
  if(!img->set_data(SImagePatchInfo(fmt, width, height), mem.get()))
    return false;
  //
  return true;
}

bool RMediaFilter::saver_png(const IImage *im, SInfo& info, const SParameters& fp, BCStr hint, bool falpha)
{
  const SImageInfo& iinf = im->get_info();
  if(iinf.width()<=0 || iinf.height()<=0)
    return false;
  int bits = 8, ctype;
  switch(iinf.type) {
    case it_R8G8B8:
      ctype = PNG_COLOR_TYPE_RGB;
      break;
    case it_R8G8B8A8:
      ctype = PNG_COLOR_TYPE_RGB_ALPHA;
      break;
    case it_L8:
      ctype = PNG_COLOR_TYPE_GRAY;
      break;
    case it_L8A8:
      ctype = PNG_COLOR_TYPE_GRAY_ALPHA;
      break;
    default:
      return false;
  }
  //
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, png_err, NULL); 
  if (!png_ptr) 
    return false; // error create internal struct
  SMemShared mem(iinf.getbinsize()*2+1024, UMODSYS_SOURCEINFO);
  if(mem.get_size()==0)
    return false;
  SMem vdata = mem;
  png_set_write_fn(png_ptr, &vdata, png_write, NULL); 
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr) {
    png_destroy_write_struct(&png_ptr, NULL);
    return false;
  }
  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return false;
  }
  png_set_IHDR(
    png_ptr, info_ptr, iinf.width(), iinf.height(), bits, ctype, 
    PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT
  );
  /*
  png_set_PLTE(png_ptr, info_ptr, palette, num_palette);
  png_set_gAMA(png_ptr, info_ptr, gamma);
  png_set_sRGB(png_ptr, info_ptr, srgb_intent);
  png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);
  png_set_iCCP(png_ptr, info_ptr, name, compression_type, profile, proflen);
  png_set_sBIT(png_ptr, info_ptr, sig_bit);
  png_set_tRNS(png_ptr, info_ptr, trans, num_trans, trans_values);
  png_set_hIST(png_ptr, info_ptr, hist);
  png_set_tIME(png_ptr, info_ptr, mod_time);
  png_set_bKGD(png_ptr, info_ptr, background);
  png_set_text(png_ptr, info_ptr, text_ptr, num_text);
  png_set_sPLT(png_ptr, info_ptr, &palette_ptr, num_spalettes);
  png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit_type);
  png_set_pHYs(png_ptr, info_ptr, res_x, res_y, unit_type);
  png_set_sCAL(png_ptr, info_ptr, unit, width, height)
  png_set_sCAL_s(png_ptr, info_ptr, unit, width, height)
  png_set_unknown_chunks(png_ptr, info_ptr, &unknowns, num_unknowns)
  */
  png_write_info(png_ptr, info_ptr);
  //
  SMem bin(NULL, iinf.width()*iinf.pixelsize());
  png_bytep row = static_cast<png_bytep>(alloca(bin.size));
  bin.data = row;
  if(row==NULL) {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return false;
  }
  SImagePatchInfo pix(iinf);
  pix.size[1] = 1;
  for(int i=0; i<iinf.height(); i++) {
    pix.start[1] = i;
    if(!im->get_data(pix, bin)) {
      png_destroy_write_struct(&png_ptr, &info_ptr);
      return false;
    }
    png_write_rows(png_ptr, &row, 1);
  }
  //
  png_write_end(png_ptr, NULL);
  png_destroy_write_struct(&png_ptr, &info_ptr);   // Clean up memory 
  //
  size_t written = mem.get_size()-vdata.size;
  SMemShared mem2(SCMem(mem.get_data(), written), UMODSYS_SOURCEINFO);
  if(mem2.get_size()==0)
    return false;
  info.binary = mem2;
  return true;
}

//***************************************
// END
//***************************************
