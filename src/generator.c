#include <stdio.h>
#include <stdlib.h>

int generator(int argc, char **argv){  

  comment();

  comp_bit_head();
  comp_bit();
  
  exp_comp_head(atoi(argv[1]));
  exp_comp(atoi(argv[1]));

  /*
  sub_shift_head(atoi(argv[1]), atoi(argv[2]));  
  sub_shift(atoi(argv[1]), atoi(argv[2]));  

  exp_inc_right_shift_head(atoi(argv[1]), atoi(argv[2]));
  exp_inc_right_shift(atoi(argv[1]), atoi(argv[2]));

  add_or_sub_head(atoi(argv[2]));
  add_or_sub(atoi(argv[2]));

  read_zero_and_shift_head(atoi(argv[1]), atoi(argv[2]));
  read_zero_and_shift(atoi(argv[1]), atoi(argv[2]));
  */

  sub_shift_head(atoi(argv[1]), atoi(argv[2])+3);  
  sub_shift(atoi(argv[1]), atoi(argv[2])+3);  

  exp_inc_right_shift_head(atoi(argv[1]), atoi(argv[2])+3);
  exp_inc_right_shift(atoi(argv[1]), atoi(argv[2])+3);

  add_or_sub_head(atoi(argv[2])+3);
  add_or_sub(atoi(argv[2])+3);

  read_zero_and_shift_head(atoi(argv[1]), atoi(argv[2])+3);
  read_zero_and_shift(atoi(argv[1]), atoi(argv[2])+3);

  round_process_head(atoi(argv[2])+3);
  round_process(atoi(argv[2])+3);

  return 0;
}
