#ifndef GEN_H
#define GEN_H


#include <stdio.h>
#include <stdlib.h>


#define P_IF if (flag->pipeline_flag == 1){
#define P_ELSE }else{
#define P_END }


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

int Manage_FPAdder_Generator(int argc, char **argv);

FILE *fpadd_arg_check(int argc, char **argv, int ef[], flags_t flag, char *top_module_name);
int FPAdder_Generator(int exp, int frac, int bit_width, FILE *fp, flags_t flag, char *module_name);
int wrapper_adder(int bit_width, FILE *fp, char *module_name);
int fpadder_step_measurement(int exp, int frac, int width, FILE *fp, flags_t flag, char *top_module_name);

int BarrelShiftDec(int exp, int frac, int bit_width, FILE *fp);
int BarrelShift(int exp, int frac, int bit_width, FILE *fp);
int LeadingZeroShiftDec(int exp, int frac, int bit_width, FILE *fp);
int LeadingZeroShift(int exp, int frac, int bit_width, FILE *fp);
int MantissaAdderDec(int bit_width, FILE *fp);  
int MantissaAdder(int bit_width, FILE *fp);
int IncreaseFracDec(int frac,  FILE *fp);
int IncreaseFrac(int frac, FILE *fp);
 
int FPAdder_Main(int exp, int frac, int bit_width, FILE *fp, flags_t flag, char *module_name);
int fpadd_exp_comparison_declaration(FILE *fp, int exp, int width, char *sel, char *sel_or_reg);
int fpadd_preshift_declaration(FILE *fp, int exp, int frac_bit, char *sel, char *sel_or_reg);
int fpadd_mantissa_add_declaration(FILE *fp, int exp, int frac_bit, char *sel, char *sel_or_reg);
int fpadd_leadingzeroshift_declaration(FILE *fp, int exp, int frac, char *sel_or_reg);
int fpadd_round_and_finish_declaration(FILE *fp, int frac, int width, char *sel, char *sel_or_reg);

int fpadd_exp_comparison(FILE *fp, int exp, int frac, int width, flags_t flag);
int fpadd_preshift(FILE *fp, int frac, int width, flags_t flag);
int fpadd_mantissa_add(FILE *fp, int frac_bit, flags_t flag);
int fpadd_leadingzeroshift(FILE *fp, int frac_bit, flags_t flag);
int fpadd_round(FILE *fp, int frac, int width, flags_t flag);


#endif
