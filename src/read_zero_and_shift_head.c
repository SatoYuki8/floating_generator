#include <stdio.h>

int read_zero_and_shift_head(int exp, int frac){
  FILE *fp;

  if ((fp = fopen("read_zero_and_shift.h", "w")) == NULL){
    fprintf(stderr, "File open error.\n");
    return 0;
  }

  fcomment(fp);

  fprintf(fp,
	  "declare read_zero_and_shift{\n"
	  "input frac<%d>;\n"
	  "input exp<%d>;\n"
	  "output shifted_frac<%d>;\n"
	  "output sub_exo<%d>;\n"
	  "instrin do;\n\n"
	  "instr_arg do(frac, exp);\n"
	  "}\n"
	  , frac, exp, frac, exp
	  );

  return 0;
}
