#include <stdio.h>

int exp_inc_right_shift_head(int exp, int frac){
  FILE *fp;
  
  if((fp = fopen("exp_inc_right_shift.h", "w")) == NULL){
    fprintf(stderr, "File open error.\n");
    return 0;
  }

  fcomment(fp);

  fprintf(fp,
	  "declare exp_inc_right_shift{\n"
	  "input exp<%d>;\n"
	  "input frac<%d>;\n"
	  "output inc_exp<%d>;\n"
	  "output shifted_frac<%d>;\n"
	  "instrin do;\n\n"
	  "instr_arg do(exp, frac);\n"
	  "}\n"
	  ,exp, frac, exp, frac
	  );

  fclose(fp);
  return 0;
}
