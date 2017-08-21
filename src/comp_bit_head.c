#include <stdio.h>

int comp_bit_head(){
  FILE *fp;

  if((fp = fopen("comp_bit.h", "w")) == NULL){
    fprintf(stderr, "File open error.\n");
    return 0;
  }

  fcomment(fp);

  fprintf(fp,
	  "declare comp_bit{\n"
	  "input a, b;\n"
	  "output l;\n"
	  "output s; \n"
	  "output eq;\n"
	  "instrin do;\n\n"
	  
	  "instr_arg do(a, b);\n"
	  "}\n"
	  );

  fclose(fp);
  return 0;
}
