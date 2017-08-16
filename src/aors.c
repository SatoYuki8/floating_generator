#include <stdio.h>

int add_or_sub(int frac){
  FILE *fp;

  if ((fp = fopen("add_or_sub.sfl", "w")) == NULL){
    fprintf(stderr, "File open error\n");
    return 0;
  }

  fcomment(fp);

  fputs("circuit add_or_sub{\n", fp);
  fprintf(fp, "input a<%d>;\n", frac);
  fprintf(fp, "input b<%d>;\n", frac);
  fprintf(fp, "output result<%d>;\n", frac);
  fputs("output cout;\n", fp);
  fputs("instrin add;\n", fp);
  fputs("instrin sub;\n\n", fp);

  fprintf(fp, "sel_v tmp_a<%d>;\n", frac+1);
  fprintf(fp, "sel_v tmp_b<%d>;\n", frac+1);
  fprintf(fp, "sel_v tmp_res<%d>;\n\n", frac+1);  

  fputs("instruct add par{\n", fp);
  fputs("tmp_a = 0b0 || a;\n", fp);
  fputs("tmp_b = 0b0 || b;\n", fp);
  fputs("tmp_res = tmp_a + tmp_b;\n", fp);
  fprintf(fp, "result = tmp_res<%d:0>;\n", frac-1);
  fprintf(fp, "cout = tmp_res<%d>;\n", frac);
  fputs("}\n\n", fp);   //instruct add par{

  fputs("instruct sub par{\n", fp);
  fputs("result = a + (^b + 0b1);\n", fp);
  fputs("cout = 0b0;\n", fp);
  fputs("}\n", fp);  //instruct sub par{

  fputs("}\n", fp);  //circuit add_or_sub{

  fclose(fp);
  
  return 0;
}
