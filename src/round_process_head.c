#include <stdio.h>

int round_process_head(int frac){
  FILE *fp;
  int i=0;
  
  if ((fp = fopen("round_process.h", "w")) == NULL){
    fprintf(stderr, "File open error.\n");
    return 0;
  }
  
  fcomment(fp);
  
  fprintf(fp,
	  "declare round_process{\n"
	  "input frac<%d>;\n",frac
	  );
  fprintf(fp,
	  "input sticky;\n"
	  "output result<%d>;\n", frac
	  );
  fprintf(fp,
	  "output cout;\n"
	  "instrin do;\n"
	  "\n"
	  "instr_arg do(frac, sticky);\n"
	  "}\n"
	  );

  return 0;
}
