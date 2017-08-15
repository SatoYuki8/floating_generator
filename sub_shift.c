#include <stdio.h>

int sub_shift(int exp, int frac){

  printf("input exp_a<%d>;\n", exp);
  printf("input exp_b<%d>;\n", exp);
  printf("input frac_b<%d>;\n);", frac);

  printf("output shifted_frac_b<%d>;\n", frac);
  puts("sticky;");
  puts("instrin do;\n");

  printf("sel_v exp_sub<%d>;\n", exp);
  printf("sel_v tmp_shift<%d>;\n", frac*3);

  puts("instruct do par {");
  
  return 0;
}
