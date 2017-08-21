#include <stdio.h>

int comp_bit(){
  FILE *fp;

  fp = fopen("comp_bit.sfl", "w");
  
  fcomment(fp);
  
  fputs("/* 1bit comperision module */\n\n\n", fp);
  
  fputs("module comp_bit{\n", fp);
  
  fputs("input a, b;\n", fp);
  fputs("output l;\n", fp);
  fputs("output s;\n", fp);
  fputs("output eq;\n", fp);
  fputs("instrin do;\n", fp);

  fputs("instruct do par {\n", fp);
  fputs("l = a&(^b);\n", fp);
  fputs("eq = ^(a@b);\n", fp);
  fputs("a = (^a)&b;\n", fp);

  fputs("}\n", fp);     // instrin do par {
  fputs("}\n", fp);     // module comp_bit{

  fclose(fp);
  
  return 0;
}
