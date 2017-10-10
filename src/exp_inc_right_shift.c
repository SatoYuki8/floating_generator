#include <stdio.h>

int exp_inc_right_shift(int exp, int frac){
  int i;
  FILE *fp;
  char *file = "exp_inc_right_shift.sfl";
  
  if ((fp = fopen(file, "w")) == NULL){
    fprintf(stderr, "File open error\n");
    return 0;
  }

  fprintf(fp, "circuit exp_inc_right_shift{\n");
  fprintf(fp, "input exp<%d>;\n", exp);
  fprintf(fp, "input frac<%d>;\n", frac);
  fprintf(fp, "output inc_exp<%d>;\n", exp);
  fprintf(fp, "output shifted_frac<%d>;\n", frac);
  fprintf(fp, "instrin do;\n\n");

  fprintf(fp, "instruct do par{\n");
  fprintf(fp, "inc_exp = exp + 0b");

  for (i=0; i<exp; i++){
    if (i == exp -1) fputs("1", fp);
    else fputs("0", fp);
  }

  fputs(";\n", fp);
  fputs("shifted_frac = frac >> 0b1;\n", fp);

  fputs("}\n", fp);    // instruct do par {
  fputs("}\n", fp);    // circuit exp_inc_right_shift{
  
  fclose(fp);
  
  return 0;
}
