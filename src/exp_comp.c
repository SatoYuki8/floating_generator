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

  fprintf(fp,"comp_bit ");

  for (i=1; i<exp+1; i++){
    fprintf(fp,"comp%d", i);

    if (i != exp) {
      fprintf(fp,", ");
    } else {
      fprintf(fp,";\n");
    }
  }
  
  fprintf(fp,"\n");
  
  fprintf(fp,"\ninstruct do par {\n");
  fprintf(fp,"alt {\n");

  for (i=1; i < exp + 1; i++) {
    fprintf(fp,"(comp%d.do(a<%d>, b<%d>).l == 0b1): l=0b1;\n", i, exp-i, exp-i);
    fprintf(fp,"(comp%d.s == 0b1): l=0b0;\n", i);

    if (i == exp){
      fprintf(fp,"else: l=0b0;\n");
    } else {
      fprintf(fp,"(comp%d.eq ==0b1): alt {\n", i);
    }
  }

  for (i=1; i < exp; i++) {
    fprintf(fp,"}\n");
  }
  
  fprintf(fp,"}\n");   // alt {
  fprintf(fp,"}\n");   // instruct do par {
  fprintf(fp,"}\n");   // module exp_comp{

  fclose(fp);
  
  return 0;
}
