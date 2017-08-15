#include <stdio.h>
#include <stdlib.h>

int generator(int argc, char **argv){  

  comp_bit();
  
  exp_comp(atoi(argv[1]));

  sub_shift(atoi(argv[1]), atoi(argv[2]));
  
  return 0;
}
