#include <stdio.h>

int sub_shift_head(int exp, int frac){
  FILE *fp;

  if ((fp = fopen("sub_shift.h", "w")) == NULL){
    fprintf(stderr, "File open error.\n");
    return 0;
  }

  fcomment(fp);

  fprintf(fp,
	  "declare sub_shift{\n"
	  "input exp_a<%d>;\n"
	  "input exp_b<%d>;\n"
	  "input frac_b<%d>;\n"
	  "output shifted_frac_b<%d>;\n"
	  "output sticky;\n"
	  "instrin do;\n\n"
	  "instr_arg do(exp_a, exp_b, frac_b);\n"
	  "}\n"
	  , exp, exp, frac, frac
	  );

  fclose(fp);

  return 0;
}
