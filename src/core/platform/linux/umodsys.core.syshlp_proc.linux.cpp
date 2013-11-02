#include <umodsys/core/syshlp.h>

#include <umodsys/core/platform/linux/syshlp_linux.h>

using namespace UModSys;
using namespace UModSys::core;
using namespace UModSys::core::syshlp;

//***************************************
// syshlp::
//***************************************

#if 0
static bool create_pipe(HANDLE handles[3], DWORD size, DWORD maxwait, bool write)
{
  SECURITY_ATTRIBUTES sa;

  // Set up the security attributes struct.
  sa.nLength= sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle = TRUE;

   BOOL rv = CreatePipe(handles+0, handles+1, &sa, size);
   if(!rv)
     return false;
   if(!DuplicateHandle(GetCurrentProcess(), handles[write?0:1], 
                       GetCurrentProcess(), handles+2, 
                       0, FALSE, DUPLICATE_SAME_ACCESS)) {
     CloseHandle(handles[0]); handles[0] = INVALID_HANDLE_VALUE;
     CloseHandle(handles[1]); handles[1] = INVALID_HANDLE_VALUE;
     return false;
   } else {
//     CloseHandle(handles[write?0:1]);
//     handles[write?0:1] = INVALID_HANDLE_VALUE;
   }
   DWORD mode = PIPE_READMODE_BYTE|PIPE_WAIT;
   SetNamedPipeHandleState(handles[write?1:0], &mode, &size, &maxwait);
   mode = PIPE_READMODE_BYTE|PIPE_NOWAIT;
   SetNamedPipeHandleState(handles[2], &mode, &size, &maxwait);
   return true;
}

static bool destroy_pipe(HANDLE handles[3])
{
  CloseHandle(handles[0]); handles[0] = INVALID_HANDLE_VALUE;
  CloseHandle(handles[1]); handles[1] = INVALID_HANDLE_VALUE;
  CloseHandle(handles[2]); handles[2] = INVALID_HANDLE_VALUE;
  return true;
}

static bool prepare_pipe(HANDLE handles[3])
{
//  CloseHandle(handles[0]); handles[0] = INVALID_HANDLE_VALUE;
//  CloseHandle(handles[1]); handles[1] = INVALID_HANDLE_VALUE;
  return true;
}

static bool write_pipe(HANDLE handle, const char* &out, size_t &left, size_t block_size)
{
  static char err[1024];
  //
  DWORD rv;
  if(left) {
    size_t n = block_size<left ? block_size : left;
    if(WriteFile(handle, out, n, &rv, NULL)) {
      left -= rv;
      out += rv;
      return true;
    } else {
      sys_err(err, sizeof err, GetLastError());
    }
  }
  return false;
}

static bool read_pipe(HANDLE handle, char* &out, size_t &left, size_t block_size)
{
  static char err[1024];
  static bool ec;
  //
  DWORD rv;
  if(left) {
    size_t n = block_size<left ? block_size : left;
    if(ReadFile(handle, out, n, &rv, NULL)) {
      left -= rv;
      out += rv;
      return true;
    } else {
      ec = GetLastError() == ERROR_BROKEN_PIPE;
      sys_err(err, sizeof err, GetLastError());
    }
  }
  return false;
}

int syshlp::execute(const char *options, const char* cmd, const char *hstdin, char *hstdout, size_t nstdout, char *hstderr, size_t nstderr)
{
  ui::U16String<4096> cmdw(cmd);
  bool fhandles = hstdin && *hstdin;
  HANDLE handles[3][3];
  DWORD flags = 0;
  DWORD exitcode;
  STARTUPINFOW si;
  bool fuser = false;
  size_t nstdin = fhandles ? strlen(hstdin) : 0;
  size_t block_size = 4096;

  memset(&si, 0, sizeof si);
  si.cb = sizeof(si);
  si.dwFlags = 0;
  //
  if(strstr(options, "(cmd)")!=NULL) {
    ui::U16String<4096> cmdw2;
    wcscpy(cmdw2, L"cmd.exe /c ");
    wcscpy(cmdw2, cmdw);
    cmdw = cmdw2;
  }
  if(strstr(options, "(events)")!=NULL) {
    fuser = true;
  }
  if(strstr(options, "(nowindow)")!=NULL) {
    flags |= CREATE_NO_WINDOW;
  }
  if(strstr(options, "(detach)")!=NULL) {
    flags |= DETACHED_PROCESS;
  }
  if(strstr(options, "(new_console)")!=NULL) {
    flags |= CREATE_NEW_CONSOLE;
  }
  if(strstr(options, "(min)")!=NULL) {
    si.dwFlags |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_MINIMIZE;
  }
  if(strstr(options, "(max)")!=NULL) {
    si.dwFlags |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_MAXIMIZE;
  }
  if(strstr(options, "(hide)")!=NULL) {
    si.dwFlags |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
  }
  if(strstr(options, "(show)")!=NULL) {
    si.dwFlags |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;
  }
  //
  if(fhandles) {
    if(!create_pipe(handles[0], block_size, 1000, false))
      return false;
    if(!create_pipe(handles[1], block_size, 1000, true)) {
      destroy_pipe(handles[0]);
      return false;
    }
    if(!create_pipe(handles[2], block_size, 1000, true)) {
      destroy_pipe(handles[0]);
      destroy_pipe(handles[1]);
      return false;
    }
    //
    si.hStdInput = handles[0][0];
    si.hStdOutput = handles[1][1];
    si.hStdError = handles[2][1];
    si.dwFlags |= STARTF_USESTDHANDLES;
  }
  //
  //wcscpy(cmdw, L"cmd /c dir");
//  si.dwFlags &= ~STARTF_USESHOWWINDOW;
//  flags &= ~CREATE_NO_WINDOW;
//  flags |= CREATE_SUSPENDED;
  //
  PROCESS_INFORMATION pi;
  if(!CreateProcessW(NULL, cmdw, NULL, NULL, fhandles?TRUE:FALSE, flags, NULL, NULL, &si, &pi)) {
    if(fhandles) {
      destroy_pipe(handles[0]);
      destroy_pipe(handles[1]);
      destroy_pipe(handles[2]);
    }
    return -1;
  }
  prepare_pipe(handles[0]);
  prepare_pipe(handles[1]);
  prepare_pipe(handles[2]);
  //
  while(GetExitCodeProcess(pi.hProcess, &exitcode)) {
    if(fuser) {
      ui::PumpEvents();
    }
    Sleep(10);
    //
    if(fhandles) {
      bool repeat = false;
      do {
        repeat = false;
        repeat = write_pipe(handles[0][2], hstdin, nstdin, block_size) || repeat;
        repeat = read_pipe(handles[1][2], hstdout, nstdout, block_size) || repeat;
        repeat = read_pipe(handles[2][2], hstderr, nstderr, block_size) || repeat;
      } while(repeat);
    }
    //
    if(exitcode!=STILL_ACTIVE)
      break;
  }
  //
  write_pipe(handles[0][2], hstdin, nstdin, block_size);
  read_pipe(handles[1][2], hstdout, nstdout, block_size);
  read_pipe(handles[2][2], hstderr, nstderr, block_size);
  //
  if(hstderr) {
    if(nstderr) *hstderr = 0; else hstderr[-1] = 0;
  }
  if(hstdout) {
    if(nstdout) *hstdout = 0; else hstdout[-1] = 0;
  }
  //
  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);
  //
  if(fhandles) {
    if(fhandles) {
      destroy_pipe(handles[0]);
      destroy_pipe(handles[1]);
      destroy_pipe(handles[2]);
    }
  }
  //
  return 0;
}
#endif

//***************************************
// ::
//***************************************

