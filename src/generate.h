#ifndef GEN_H
#define GEN_H


#include <stdio.h>
#include <stdlib.h>

typedef struct flags{
  int module_flag;
  int wrapper_flag;
  int pipeline_flag;
  int step1_flag;
  int step2_flag;
  int step3_flag;
  int step4_flag;
  int step5_flag;
} _flags_t;

typedef _flags_t flags_t[1];

#define GET_VAR_NAME(VAR) (#VAR)//変数VARの変数名を文字列として取得

int comment();
int fcomment(FILE *fp);
void flags_init(flags_t flag);
int DecToBi(int dec, int dig, FILE *fp);
char ZeroStr(int N, char in[]);
char OneStr(int N, char in[]);
int help();


FILE *arg_check(int argc, char **argv, int ef[], flags_t flag);
int FloatingAdder(int exp, int frac, int bit_width, FILE *fp, int pipe);
int wrapper(int bit_width, FILE *fp);
int fpadder_step_measurement(int exp, int frac, int width, FILE *fp, flags_t flag);

int BarrelShiftDec(int exp, int frac, int bit_width, FILE *fp);
int BarrelShift(int exp, int frac, int bit_width, FILE *fp);
int LeadingZeroShiftDec(int exp, int frac, int bit_width, FILE *fp);
int LeadingZeroShift(int exp, int frac, int bit_width, FILE *fp);
int MantissaAdderDec(int bit_width, FILE *fp);  
int MantissaAdder(int bit_width, FILE *fp);
int IncreaseFracDec(int frac,  FILE *fp);
int IncreaseFrac(int frac, FILE *fp);
int add_float(int exp, int frac, int bit_width, FILE *fp, int pipe);


#endif
