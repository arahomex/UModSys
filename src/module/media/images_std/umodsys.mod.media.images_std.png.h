
//***************************************
// RMediaFilter::
//***************************************

void RMediaFilter::png_err(png_structp png_ptr, png_const_charp msg) {
   if(png_ptr) { 
     M.con().put(0, "PNG FATAL ERROR: png error - %s\n",msg);
   } 
}

void RMediaFilter::png_read(png_structp png_ptr, png_bytep data, png_size_t length) {
   SCMem *vdata = static_cast<SCMem *>(png_get_io_ptr(png_ptr));
   if(vdata->size<size_t(length))
     png_error(png_ptr, "Read Error"); // not enougth size
   //
   memcpy(data, vdata->data, length);
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
  if(!img->set_reallocate(ifmt, width, height))
    return false;
  if(!img->set_linear(SImagePatchInfo(fmt, width, height), mem.get()))
    return false;
  //
  return true;
}

//***************************************
// END
//***************************************
