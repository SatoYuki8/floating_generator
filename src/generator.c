#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate.h"

#define ERROR -1
//int arg_check(int argc, char **argv, int ef[], FILE *fp){
FILE *arg_check(int argc, char **argv, int ef[]);
int FloatingAdder(int exp, int frac, int bit_width, FILE *fp);

int generator(int argc, char **argv){  
  int exp, frac, bit_width;
  FILE *fp;
  int ef[2];   //exp frac
  
  //if((arg_check(argc, argv, ef, fp)) == ERROR) return ERROR;
  fp = arg_check(argc, argv, ef);
  if(fp == ERROR) return ERROR;
  if(fp == 0) fp = stdout;
  
  exp = ef[0];
  frac = ef[1];
  bit_width = exp + frac + 1;

  int i;
  int module_flag=0;
  for(i=0; i<argc ;i++){
    if (!strcmp(argv[i], "Wrapper")) wrapper(bit_width, fp);
    if (!strcmp(argv[i], "wrapper")) wrapper(bit_width, fp);
    if (!strcmp(argv[i], "FPAdder")&module_flag==0){
      FloatingAdder(exp, frac, bit_width, fp); module_flag=1;	
    }else{
      
    }
  }
  //fclose(fp);
  if (module_flag==0) printf("Please Input Module name.\n");
  
  return 0;
}
