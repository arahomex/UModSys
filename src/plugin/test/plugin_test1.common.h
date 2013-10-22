// RTest1_Shell::

void RTest1_Shell::dump_str(const char *s, size_t n)
{
  char bc[8];
  for(size_t i=0; i<n; i++) {
    unsigned char c = *s++;
    if(c<' ') {
      M.con().put(0, "\\x%02x", c);
    } else if(c=='\\') {
      M.con().put(0, "\\");
    } else if(c<0xc0) {
      M.con().put(0, "%c", c);
    } else if((c & 0xe0)==0xc0) {
      if(i+1<n) {
        bc[0] = c; 
        bc[1] = *s++;
        bc[2] = 0;
        M.con().put(0, "%s", bc);
      } else {
        M.con().put(0, "\\x%02x", c);
      }
      i++;
    } else if((c & 0xf0)==0xe0) {
      if(i+2<n) {
        bc[0] = c; 
        bc[1] = *s++;
        bc[2] = *s++;
        bc[3] = 0;
        M.con().put(0, "%s", bc);
      } else {
        M.con().put(0, "\\x%02x", c);
      }
      i+=2;
    } else if((c & 0xf8)==0xf0) {
      if(i+3<n) {
        bc[0] = c; 
        bc[1] = *s++;
        bc[2] = *s++;
        bc[3] = *s++;
        bc[4] = 0;
        M.con().put(0, "%s", bc);
      } else {
        M.con().put(0, "\\x%02x", c);
      }
      i+=3;
    } else if((c & 0xfc)==0xf8) {
      if(i+4<n) {
        bc[0] = c; 
        bc[1] = *s++;
        bc[2] = *s++;
        bc[3] = *s++;
        bc[4] = *s++;
        bc[5] = 0;
        M.con().put(0, "%s", bc);
      } else {
        M.con().put(0, "\\x%02x", c);
      }
      i+=4;
    } else {
      if(i+5<n) {
        bc[0] = c; 
        bc[1] = *s++;
        bc[2] = *s++;
        bc[3] = *s++;
        bc[4] = *s++;
        bc[5] = *s++;
        bc[6] = 0;
        M.con().put(0, "%s", bc);
      } else {
        M.con().put(0, "\\x%02x", c);
      }
      i+=5;
    }
  }
}

