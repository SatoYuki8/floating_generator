#include <stdio.h>
#include "generate.h"

int exp_comp(int exp){
  //int main(){
  int i;
  
  comment();
  
  printf("/******************/\n");
  printf("/* if (a > b) l=1 */\n");
  printf("/* else l=0;      */\n");
  printf("/******************/\n\n");

  puts("%i \"comp_bit.h\"");

  puts("module exp_comp{");

  printf("input a<%d>;\n",exp);
  printf("input b<%d>;\n",exp);
  puts("output l;");
  puts("instrin do;\n");

  printf("comp_bit ");

  for (i=1; i<exp+1; i++){
    printf("comp%d", i);

    if (i != exp) {
      printf(", ");
    } else {
      printf(";\n");
    }
  }
  
  printf("\n");
  
  printf("\ninstruct do par {\n");
  printf("alt {\n");

  for (i=1; i < exp + 1; i++) {
    printf("(comp%d.do(a<%d>, b<%d>).l == 0b1): l=0b1;\n", i, exp-i, exp-i);
    printf("(comp%d.s == 0b1): l=0b0;\n", i);

    if (i == exp){
      printf("else: l=0b0;\n");
    } else {
      printf("(comp%d.eq ==0b1): alt {\n", i);
    }
  }

  for (i=1; i < exp; i++) {
    printf("}\n");
  }
  
  printf("}\n");   // alt {
  printf("}\n");   // instruct do par {
  printf("}\n");   // module exp_comp{
  
  return 0;
}
