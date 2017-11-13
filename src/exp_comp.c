#include <stdio.h>
#include "generate.h"

int exp_comp(int exp){
  //int main(){
  int i;
  FILE *fp;
  char *file = "exp_comp.sfl";

  if ((fp = fopen(file, "w")) == NULL){
    fprintf(stderr, "It was error to open exp_comp.sfl\n");
    return 0;
  }
  
  fcomment(fp);
  
  fprintf(fp,"/******************/\n");
  fprintf(fp,"/* if (a > b) l=1 */\n");
  fprintf(fp,"/* else l=0;      */\n");
  fprintf(fp,"/******************/\n\n");

  fputs("%i \"comp_bit.h\"\n", fp);

  fputs("module exp_comp{\n", fp);

  fprintf(fp,"input a<%d>;\n",exp);
  fprintf(fp,"input b<%d>;\n",exp);
  fputs("output l;\n", fp);
  fputs("instrin do;\n\n", fp);

  fprintf(fp,"sel_v ");

  for (i=0; i<exp-1; i++){
    fprintf(fp,"f%d", i);

    if (i != exp-2) {
      fprintf(fp,", ");
    } else {
      fprintf(fp,";\n");
    }
  }


  fprintf(fp,"comp_bit ");

  for (i=0; i<exp-1; i++){
    fprintf(fp,"comp%d", i);

    if (i != exp-2) {
      fprintf(fp,", ");
    } else {
      fprintf(fp,";\n");
    }
  }
  
  fprintf(fp,"\n\n");
  
  fprintf(fp,"instruct do par {\n");

  fprintf(fp, "f0 = a<0>&^b<0>;\n");
  for (i=1; i<exp-1; i++){
    fprintf(fp,"f%d = comp%d.do(a<%d>, b<%d>, f%d).fout;\n", i, i-1, i, i, i-1);
  }
  
  fprintf(fp, "l = comp%d.do(a<%d>, b<%d>, f%d).fout;\n", exp-2, exp-1, exp-1, exp-2);

  fprintf(fp,"}\n");   // instruct do par {
  fprintf(fp,"}\n");   // module exp_comp{

  fclose(fp);
  
  return 0;
}
