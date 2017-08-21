#include <stdio.h>

int exp_comp_head(int exp){
  FILE *fp;

  if ((fp = fopen("exp_comp.h", "w")) == NULL){
    fprintf(stderr, "File open error.\n");
    return 0;
  }

  fcomment(fp);

  fprintf(fp,
	  "declare exp_comp{\n"
	  "input a<%d>;\n"
	  "input b<%d>;\n"
	  "output l;\n"
	  "instrin do;\n\n"
	  "instr_arg do(a, b);\n"
	  "}\n"
	  , exp, exp
	  );

  fclose(fp);
  return 0;
}
