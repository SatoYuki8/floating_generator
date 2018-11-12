#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate.h"

#define FPERROR (FILE*)-1
#define ERROR -1

//int arg_check(int argc, char **argv, int ef[], FILE *fp){
FILE *arg_check(int argc, char **argv, int ef[]);
int FloatingAdder(int exp, int frac, int bit_width, FILE *fp, int pipe);
int wrapper(int bit_width, FILE *fp);

int generator(int argc, char **argv){  
  int exp, frac, bit_width;
  FILE *fp;
  int ef[2];   //exp frac
  flags_t flag;

  flags_init(flag);
  
  //if((arg_check(argc, argv, ef, fp)) == ERROR) return ERROR;
  fp = arg_check(argc, argv, ef);
  if(fp == FPERROR) return ERROR;
  if(fp == 0) fp = stdout;
  
  exp = ef[0];
  frac = ef[1];
  bit_width = exp + frac + 1;

  int i;
  for(i=0; i<argc ;i++){
    if (!strcmp(argv[i], "Wrapper")) flag->wrapper_flag = 1;
    if (!strcmp(argv[i], "wrapper")) flag->wrapper_flag = 1;
    if (!strcmp(argv[i], "FPAdder")) flag->module_flag=1;	
    if (!strcmp(argv[i], "fpadder")) flag->module_flag=1;
    if (!strcmp(argv[i], "pipeline"))flag->pipeline_flag = 1;
  }
  
  
  if (flag->module_flag == 1){
    FloatingAdder(exp, frac, bit_width, fp, flag->pipeline_flag);
  }else{
    printf("Please Input Module name.\n");
    return 0;
  }
  if (flag->wrapper_flag == 1)  wrapper(bit_width, fp);
  
  //fclose(fp);
  
  return 0;
}
