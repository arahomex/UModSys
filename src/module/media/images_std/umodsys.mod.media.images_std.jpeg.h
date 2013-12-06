
//***************************************
// RMediaFilter::
//***************************************

struct JpegErrorManager : public jpeg_error_mgr {
  bool cenabled;
  jmp_buf jjb;
};

static void error_exit (j_common_ptr cinfo)
{
  JpegErrorManager* err = (JpegErrorManager*)cinfo->err;
  /* Always display the message */
  if(err->cenabled)
    (*cinfo->err->output_message) (cinfo);

  /* Let the memory manager delete any temp files before we die */
  jpeg_destroy(cinfo);

  longjmp(err->jjb, 5);
}

static void init_source (j_decompress_ptr cinfo) 
{
  // DO NOTHING
}

static boolean fill_input_buffer (j_decompress_ptr cinfo) 
{
  // DO NOTHING
  return 1;
}

static void skip_input_data (j_decompress_ptr cinfo, long count) 
{
  jpeg_source_mgr * src = cinfo->src;
  if(count > 0) {
    src->bytes_in_buffer -= count;
    src->next_input_byte += count;
  }
}

static void resync_to_restart (j_decompress_ptr cinfo, long desired) 
{
  // DO NOTHING
}

static void term_source (j_decompress_ptr cinfo) 
{
  // DO NOTHING
}

bool RMediaFilter::loader_jpeg(IImage *img, const SInfo& info, const SParameters& fp, int pk, BCStr hint, bool falpha) 
{
        falpha = falpha || pk>=0;

        struct jpeg_decompress_struct cinfo;
        // allocate and initialize JPEG decompression object
        JpegErrorManager jerr;

        //We have to set up the error handler first, in case the initialization
        //step fails.  (Unlikely, but it could happen if you are out of memory.)
        //This routine fills in the contents of struct jerr, and returns jerr's
        //address which we place into the link field in cinfo.
        
        if(setjmp(jerr.jjb))
          return false;

        cinfo.err = jpeg_std_error(&jerr);
        cinfo.err->error_exit = error_exit;
        jerr.cenabled = false;
        // Now we can initialize the JPEG decompression object. 
        jpeg_create_decompress(&cinfo);

        // specify data source
        jpeg_source_mgr jsrc;

        // Set up data pointer
        jsrc.bytes_in_buffer = info.binary.get_size();
        jsrc.next_input_byte = (JOCTET*)info.binary.get_data();
        cinfo.src = &jsrc;

        jsrc.init_source = init_source;
        jsrc.fill_input_buffer = fill_input_buffer;
        jsrc.skip_input_data = skip_input_data;
        jsrc.resync_to_restart = jpeg_resync_to_restart;
        jsrc.term_source = term_source;

        // Decodes JPG input from whatever source
        // Does everything AFTER jpeg_create_decompress
        // and BEFORE jpeg_destroy_decompress
        // Caller is responsible for arranging these + setting up cinfo

        // read file parameters with jpeg_read_header() 
        jpeg_read_header(&cinfo, TRUE);
        // Start decompressor
        jerr.cenabled = true;
        jpeg_start_decompress(&cinfo);

        // Get image data
        unsigned rowspan = cinfo.image_width * cinfo.num_components;
        unsigned width = cinfo.image_width;
        unsigned height = cinfo.image_height;

        bool greyscale;

        if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
                greyscale = true;
        else
                greyscale = false;

        if( greyscale) {
          jpeg_destroy_decompress(&cinfo);
          return false; // unsupported jpeg
        }

        int RN = width*3; 
        int N = RN*height;
        //
        BByte* rp[4096]; // row pointers
        if(height>4096) { // too much rows
          jpeg_destroy_decompress(&cinfo);
          return false;
        }
        // Allocate memory for buffer
        
        SMemShared mem;
        if(!mem.alloc(N, UMODSYS_SOURCEINFO)) {
          jpeg_destroy_decompress(&cinfo); // Clean up memory 
          return false;
        }
        // Here we use the library's state variable cinfo.output_scanline as the
        // loop counter, so that we don't have to keep track ourselves.
        // Create array of row pointers for lib
        BByte *data; mem.t_data(data);
        for( unsigned i = 0; i < height; i++ )
           rp[i] = &data[i * RN];
        //
        unsigned nread = 0;
        while( cinfo.output_scanline < cinfo.output_height )
                nread += jpeg_read_scanlines( &cinfo, &rp[nread], cinfo.output_height-nread);
        //
        // Finish decompression
        jpeg_finish_decompress(&cinfo);
        // Release JPEG decompression object
        // This is an important step since it will release a good deal of memory.
        jpeg_destroy_decompress(&cinfo);
        //
        eImageType fmt, ifmt;
        fmt = it_R8G8B8;
        ifmt = falpha ? it_R8G8B8A8 : it_R8G8B8;
        //
        if(!img->set_info(ifmt, width, height))
          return false;
        if(!img->set_data(SImagePatchInfo(fmt, width, height), mem.get()))
          return false;
        //
        return true;
}

bool RMediaFilter::saver_jpeg(const IImage *im, SInfo& info, const SParameters& fp, BCStr hint, bool falpha)
{
  return false;
}

//***************************************
// END
//***************************************
