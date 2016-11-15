
enum TGC_LogLevel {
  lol_Fatal,
  lol_Error,
  lol_Warning,
  lol_Text,
  lol_Info,
  lol_Debug,
  lol_Dump,
};

enum TGC_LogType {
  log_GC                = 0x01,
  log_GCNode            = 0x02,
  log_GCUtil            = 0x03,
};

extern uint8_t z_loglev[0x200];

extern void z_brk(void);
// lev=0..7 key=0x00..0x1FF
extern void z_log(int key, int lev, const char *fmt, ...);
extern void z_log(int klev, const char *fmt, ...);


inline void z_logenable(int key, bool val)
{
  key &= 0xfff;
  int p=key>>3;
  uint8_t m=(1<<(key&7));
  if(!val) z_loglev[p] &= ~m; else z_loglev[p] |= m;
}

inline void z_logenable(int key, int lev, bool val)
{
  z_logenable((key<<3)+lev, val);
}

