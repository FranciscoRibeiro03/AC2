/*  1 */ 
  char inkey(void);
  
/*  2 */ 
  char getChar(void);
  
/*  3 */ 
  void putChar(char ch);
  
/*  4 */ 
  unsigned int readInt(int base);
  
/*  5 */ 
  int readInt10(void);
  
/*  6 */ 
  void printInt(unsigned int val,int base);
  
/*  7 */ 
  void printInt10(int val);
  
/*  8 */ 
  void printStr(char *s);
  
/*  9 */ 
  void readStr(char *buffer,unsigned char nc);  // "nc" does not include '\0' terminador
  
/* 10 */ 
  void exit(int return_code) __attribute__((__noreturn__));
  
/* 11 */ 
  unsigned int readCoreTimer(void);
  
/* 12 */ 
  void resetCoreTimer(void);