#include <umodsys/common/syshlp.h>

#include <umodsys/common/platform/linux/syshlp_linux.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::core::syshlp;

//***************************************
// syshlp::
//***************************************

bool syshlp::opt_enable_colors = false;
const char* syshlp::opt_stdlog_file = "umodsys.log";

bool syshlp::validate_env(void)
{
  // check environment variables
  const char* val;
  if((val=getenv("UMS_COLORS"))!=NULL) {
    opt_enable_colors = atoi(val)!=0;
  }
  if((val=getenv("UMS_STDLOG"))!=NULL) {
    opt_stdlog_file = val;
  }
  //
  return true;
}

bool syshlp::validate_args(int &argc, char **&argv)
{
  // check arguments
  for(int i=1; i<argc; i++) {
    if(argv[i][0]!='-' || argv[i][1]!='-')
      break; // no more args
    if(strcmp(argv[i], "colors")==0) {
      opt_enable_colors  = true;
    } else if(strcmp(argv[i], "no-colors")==0) {
      opt_enable_colors  = false;
    } else {
      break;
    }
    argc--;
    if(i+1<argc)
      memmove(argv+i, argv+i+1, (argc-i-1)*sizeof(char*));
    i--;
  }
  return true;
}


//***************************************
// ::
//***************************************

