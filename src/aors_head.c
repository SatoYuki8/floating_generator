#include <stdio.h>

int add_or_sub_head(int frac){
  FILE *fp;
  
  if((fp = fopen("add_or_sub.h", "w")) == NULL){
    fprintf(stderr, "File open error.\n");
    return 0;
  }

  fcomment(fp);

  fprintf(fp,
	  "declare add_or_sub{\n"
	  "input a<%d>;\n"
	  "input b<%d>;\n"
	  "output result<%d>;\n"
	  "output cout;\n"
	  "instrin add;\n"
	  "instrin sub;\n\n"
	  "instr_arg add(a, b);\n"
	  "instr_arg sub(a, b);\n"
	  "}\n"
	  ,frac, frac, frac
	  );

  fclose(fp);
  return 0;
}
