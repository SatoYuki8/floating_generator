#include <stdio.h>
#include <stdlib.h>

int generator(int argc, char **argv){  
  int exp, frac, bit_width;
  
  if (argc < 3) return 1;

  exp = atoi(argv[1]);
  frac = atoi(argv[2]);
  bit_width = exp + frac + 1;  
  
  add_half(exp, frac, bit_width);
  
  return 0;
}
