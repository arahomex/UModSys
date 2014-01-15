
//***************************************
// SWindow::
//***************************************

SWindow::SWindow(void) 
: wnd(NULL) 
{
  x = 20; y = 20; 
  dx = 640; dy = 480;
  flags = 0;
  caption = "SDL2";
}

SWindow::~SWindow(void) 
{ 
  close(); 
} 

void SWindow::close(void) 
{
  if(wnd!=NULL) { 
    SDL_DestroyWindow(wnd); 
    wnd = NULL; 
  }
}

bool SWindow::open(const SParameters& args) 
{
  close();
  //
  if(&args!=NULL) {
    args.get("caption", caption);
    args.get("x", x); args.get("y", y);
    args.get("dx", dx); args.get("dy", dy);
  }
  //
  wnd = SDL_CreateWindow(caption, x, y, dx, dy, flags);
  if(wnd==NULL) {
    close();
    return false;
  }
  return true;
}

//***************************************
// END
//***************************************
