#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate.h"

#define FPERROR (FILE*)-1

void first_message(){
  fprintf(stdout,
	  "\n"
	  "***************************************************************************\n"
	  "Generator of Floating Point Adder written in SFL\n\n"
	  "  ./gen FPAdder exp=<n> frac=<m> \n"
	  "  -->Output 1+n+m bits FPAdder\n\n"
	  "------Oprtion------\n"
	  "filename=<string> : SFL is output in the <string> (default : standard out)\n"
	  "pipeline          : Generate 5steps pipeline version\n"
	  "wrapper           : Wrapper module for delay measurement is created\n"
	  "\n"
	  "***************************************************************************\n"	  
	  "\n"
	  );
}

//int arg_check(int argc, char **argv, int ef[], FILE *fp){
FILE *arg_check(int argc, char **argv, int ef[], flags_t flag, char *top_module_name){
  FILE *fp;
  int i;
  char output[64];
  
  if (argc == 1){
    first_message();
    return FPERROR;
  }

  first_message();
  
  ef[0]=0; ef[1]=0;
  int filename_flag=0;
  for(i=0; i<argc ;i++){
    if (!strcmp(argv[i], "Wrapper")) flag->wrapper_flag = 1;
    if (!strcmp(argv[i], "wrapper")) flag->wrapper_flag = 1;
    if (!strcmp(argv[i], "FPAdder")) flag->module_flag=1;	
    if (!strcmp(argv[i], "fpadder")) flag->module_flag=1;
    if (!strcmp(argv[i], "pipeline"))flag->pipeline_flag = 1;
    if (!strcmp(argv[i], "step1")) flag->step1_flag = 1;
    if (!strcmp(argv[i], "step2")) flag->step2_flag = 1;
    if (!strcmp(argv[i], "step3")) flag->step3_flag = 1;
    if (!strcmp(argv[i], "step4")) flag->step4_flag = 1;
    if (!strcmp(argv[i], "step5")) flag->step5_flag = 1;
    
    if (!strncmp(argv[i], "filename=", 9)){
      strcpy(output, argv[i]+9);
      if ((fp = fopen(output, "w")) == NULL){
	fprintf(stderr, "File open error.\n");
	return FPERROR;
      }
      filename_flag=1;
    }
    if(!strncmp(argv[i], "exp=", 4)) ef[0] = atoi(argv[i]+4);
    if(!strncmp(argv[i], "frac=", 5)) ef[1] = atoi(argv[i]+5);
  }

  if ((ef[0] == 0) | (ef[1] == 0)){
    printf("Exp or Frac is irregularity.\n");
    return FPERROR;
  }

  char main_module_name[16];
  if(flag->module_flag == 1){
    strcpy(main_module_name, "FPAdder");
  }else if(flag->step1_flag == 1){
    strcpy(main_module_name, "step1");
  }else if(flag->step2_flag == 1){
    strcpy(main_module_name, "step2");
  }else if(flag->step3_flag == 1){
    strcpy(main_module_name, "step3");
  }else if(flag->step4_flag == 1){
    strcpy(main_module_name, "step4");
  }else if(flag->step5_flag == 1){
    strcpy(main_module_name, "step5");    
  }else{
    fprintf(stdout, "Please Input Module name.\n");
    return FPERROR;
  }

  if(flag->pipeline_flag == 1){
    sprintf(top_module_name, "%s_%d_%d_%d", main_module_name,  ef[0], ef[1], ef[0]+ef[1]+1);
  }else{
    sprintf(top_module_name, "%s_%d_%d_%d_comb", main_module_name,  ef[0], ef[1], ef[0]+ef[1]+1);
  }
  
  if(filename_flag == 0){
    sprintf(output, "%s.sfl", top_module_name);
    if ((fp = fopen(output, "w")) == NULL){
      fprintf(stderr, "File open error.\n");
      return FPERROR;
    }
  }

  fprintf(stdout, "Output in %s\n", output);
  
  return fp;
}
