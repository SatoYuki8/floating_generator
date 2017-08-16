#include <stdio.h>

int sub_shift(int exp, int frac){
  int i;
  FILE *fp;
  
  if ((fp = fopen("sub_shift.sfl", "w")) == NULL){
    fprintf(stderr, "File open error\n");
    return 0;
  }

  fcomment(fp);

  fputs("/********************************/\n", fp);
  fputs("/* exp_a - exp_b -> shit_amount */\n", fp);
  fputs("/* frac_b >> shift_amont        */\n", fp);
  fputs("/********************************/\n\n", fp);
  
  fputs("circuit sub_shift{\n\n", fp);
  
  fprintf(fp, "input exp_a<%d>;\n", exp);
  fprintf(fp, "input exp_b<%d>;\n", exp);
  fprintf(fp, "input frac_b<%d>;\n", frac);
  
  fprintf(fp, "output shifted_frac_b<%d>;\n", frac);
  fputs("output sticky;\n", fp);
  fputs("instrin do;\n\n", fp);
  
  fprintf(fp, "sel_v exp_sub<%d>;\n", exp);
  fprintf(fp, "sel_v tmp_shift<%d>;\n\n", frac*3);

  fputs("instruct do par {\n", fp);
  fprintf(fp, "exp_sub = exp_a + (^exp_b + 0b");

  for (i=0; i<exp; i++){
    if (i == exp - 1) fprintf(fp, "1");
    else fprintf(fp, "0");
  }
  fputs(");\n", fp);

  fputs("alt {\n", fp);
  fputs("(exp_sub == 0b", fp);
  for (i=0; i<exp; i++) fputs("0", fp);
  fputs("): par{\n", fp);

  fputs("shifted_frac_b = frac_b;\n", fp);
  fputs("sticky = 0b0;\n", fp);
  fputs("}\n", fp);  //par {

  fputs("else: par {\n", fp);
  fputs("shifted_frac_b = frac_b >> exp_sub;\n", fp);
  fputs("tmp_shift = (frac_b || 0b", fp);

  for (i=0; i<frac*3; i++){
    fputs("0", fp);
  }
  fputs(") >> exp_sub;\n", fp);
  fprintf(fp, "sticky = /|(tmp_shift<%d:0>);\n", frac*2 - 1);

  fputs("}\n", fp);  //else par{
  fputs("}\n", fp);  //alt {
  fputs("}\n", fp);  //instruct do par{
  fputs("}\n", fp);

  fclose(fp);
  return 0;
}
