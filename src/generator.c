#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate.h"

#define FPERROR (FILE*)-1
#define ERROR -1

int generator(int argc, char **argv){  
  int exp, frac, bit_width;
  FILE *fp;
  int ef[2];   //exp frac
  flags_t flag;

  flags_init(flag);
  
  //if((arg_check(argc, argv, ef, fp)) == ERROR) return ERROR;
  fp = arg_check(argc, argv, ef, flag);
  if(fp == FPERROR) return ERROR;
  
  exp = ef[0];
  frac = ef[1];
  bit_width = exp + frac + 1;
  
  if (flag->module_flag == 1){           /* When it generates FPadder module  */
    FloatingAdder(exp, frac, bit_width, fp, flag->pipeline_flag);
    fprintf(stdout, "Generate FPAdder (exp=%d, frac=%d)\n", exp, frac);
    if(flag->pipeline_flag==1) fprintf(stdout, "  -->Pipeline Version\n");
    
  }else if((flag->step1_flag == 1) |     /* When it measure step latency */
	   (flag->step2_flag == 1) |
	   (flag->step3_flag == 1) |
	   (flag->step4_flag == 1) |
	   (flag->step5_flag == 1) 
	   ){
    fpadder_step_measurement(exp, frac, bit_width, fp, flag);
  }else{
    printf("Please Input Module name.\n");
    return 0;
  }
  
  if (flag->wrapper_flag == 1){
    wrapper(bit_width, fp);
    fprintf(stdout, "Generate Wrapper Module\n");
  }  
  
  fclose(fp);
  
  return 0;
}
