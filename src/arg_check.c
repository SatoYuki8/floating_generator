#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -1

//int arg_check(int argc, char **argv, int ef[], FILE *fp){
FILE *arg_check(int argc, char **argv, int ef[]){
  FILE *fp;
  int i;
  char output[64];
  
  if (argc == 1){
    printf("SFL Floating Point Arithmetic Gnerator.\n");
    printf("argmentたりてへん\n");
    return ERROR;
  }

  ef[0]=0; ef[1]=0;
  int filename_flag=0;
  for(i=0; i<argc ;i++){
    if(!strncmp(argv[i], "--help", 6)){
      help();
      return 0;
    }
    
    if (!strncmp(argv[i], "filename=", 9)){
      strcpy(output, argv[i]+9);
      if ((fp = fopen(output, "w")) == NULL){
	fprintf(stderr, "File open error.\n");
	return ERROR;
      }
      filename_flag=1;
    }
    if(!strncmp(argv[i], "exp=", 4)) ef[0] = atoi(argv[i]+4);
    if(!strncmp(argv[i], "frac=", 5)) ef[1] = atoi(argv[i]+5);
  }

  if ((ef[0] == 0) | (ef[1] == 0)){
    printf("Exp or Frac is irregularity.\n");
    return ERROR;
  }
  
  if(filename_flag == 0)     return 0;
  
  return fp;
}