#include <umodsys/common/syshlp.h>

#include <umodsys/common/platform/win32/syshlp_win32.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::core::syshlp;

//***************************************
// syshlp::
//***************************************

static bool get_reg_string(HKEY root, const char* keyname, const char* valuename, char* value, int value_size)
{
  syshlp::U16String<> keynamew(keyname), valuenamew(valuename);
  HKEY sub;
  if(RegOpenKeyExW(root, keynamew, 0, KEY_READ, &sub)!=ERROR_SUCCESS)
    return false;
  syshlp::U16String<4096> valuew;
  DWORD type=REG_SZ, size=sizeof(valuew.buf);
  bool rv = RegQueryValueExW(sub, valuenamew, NULL, &type, (LPBYTE)&valuew.buf, &size)==ERROR_SUCCESS;
  if(type!=REG_SZ && type!=REG_EXPAND_SZ)
    rv = false;
  RegCloseKey(sub);
  if(rv) {
    gracial_convert(value, value_size, valuew, wcslen(valuew));
  }
  return rv;
}

static const char* local_shell_folders[][2]={
  {"path:My Documents", "Personal"},
  {"path:My Desktop", "Desktop"},
  {"path:My Favorites", "Favorites"},
  {"path:My Recent", "Recent"},
  {"path:My Start Menu", "Start Menu"},
  {"path:My SendTo", "SendTo"},
  {"path:My Startup", "Startup"},
  {"path:My Music", "My Music"},
  {"path:My Pictures", "My Pictures"},
  {"path:My Video", "My Video"},
  {"path:My Local AppData", "Local AppData"},
  {"path:My Local Settings", "Local Settings"},
  {"path:My Templates", "Templates"},
  {NULL, NULL}
};

int syshlp::getvar(const char *name, char* value, int valuelen)
{
  for(int i=0; local_shell_folders[i][0]!=NULL; i++) {
    if(STREQ(local_shell_folders[i][0], name)) {
      if(get_reg_string(
        HKEY_CURRENT_USER, 
        "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
        local_shell_folders[i][1], 
        value,
        valuelen
        )) {
        //
        return 1;
      }
    }
  }
  return 0;
}


//***************************************
// ::
//***************************************

