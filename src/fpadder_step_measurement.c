#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate.h"

#define ERROR -1

int isDelimiter(char p, char delim){
  return p == delim;
}

int split(char *dst[], char *src, char delim){
  int count = 0;
  char *tmp = (char*)malloc(sizeof(char)*strlen(src));
  
  strcpy(tmp, src);
  
  for(;;) {
    while (isDelimiter(*tmp, delim)){
      tmp++;
    }

    if (*tmp == '\0') break;

    dst[count++] = tmp;

    while (*tmp && !isDelimiter(*tmp, delim)) {
      tmp++;
    }

    if (*tmp == '\0') break;

    *tmp++ = '\0';
  }
  return count;
}

int fpadder_step_measurement(int exp, int frac, int width, FILE *fp, flags_t flag){
  int step1 = flag->step1_flag;
  int step2 = flag->step2_flag;
  int step3 = flag->step3_flag;
  int step4 = flag->step4_flag;
  int step5 = flag->step5_flag;

  char module_name[8];
  char input_name[64];
  char input_reg_name[64];  
  char output_name[64];
  char output_reg_name[64];
  char in_argment[32];
  char in_reg_argment[32];
  char out_argment[32];
  char out_reg_argment[32];
  
  if(step1 == 1){
    /**************************** step1 *******************************/
    strcpy(module_name, GET_VAR_NAME(step1));
    sprintf(input_name, "a<%d>, b<%d>", width, width);
    sprintf(in_argment, "a, b");
    sprintf(output_name, "exp_diff<%d>, Aa<%d>, Ab<%d>", exp, width, width);
    sprintf(out_argment, "exp_diff, Aa, Ab");
    fprintf(fp,
	    "circuit %s{\n"
	    "input %s;\n"
	    "output %s;\n"
	    "instrin do;\n"
	    "sel wdiff<%d>;\n",
	    module_name,
	    input_name,
	    output_name,
	    exp+1
	    );
    fprintf(fp,
	    "instruct do par{\n"
	    "wdiff = (0b0||a<%d:%d>) + (^(0b0||b<%d:%d>) + 0b1);\n"
	    "alt{\n"
	    "(wdiff<%d>): par{\n"
	    "exp_diff = ^wdiff<%d:0> + 0b1;\n"
	    "Aa = b;\n"
	    "Ab = a;\n"
	    "}\n"
	    "else: par{\n"
	    "exp_diff = wdiff<%d:0>;\n"
	    "Aa = a; Ab = b;\n"
	    "}\n"
	    "}\n"
	    "}\n"
	    "}\n"
	    "\n",
	    width-2, width-exp-1, width-2, width-exp-1,
	    exp,
	    exp-1,
	    exp-1
	    );
    sprintf(input_reg_name, "ina<%d>, inb<%d>", width, width);
    sprintf(in_reg_argment, "ina, inb");
    sprintf(output_reg_name, "diff<%d>, outa<%d>, outb<%d>", exp, width, width);
    sprintf(out_reg_argment, "diff, outa, outb");
  }else if(step2 == 1){
    /**************************** step2 *******************************/
    BarrelShiftDec(exp, frac, frac+6, fp);
    BarrelShift(exp, frac, frac+6, fp);
    strcpy(module_name, GET_VAR_NAME(step2));
    sprintf(input_name, "Aa<%d>, Ab<%d>, exp_diff<%d>", width, width, exp);
    sprintf(in_argment, "Aa, Ab, exp_diff");
    sprintf(output_name, "Bs1, Bs2, Bexp<%d>, Bm1<%d>, Bm2<%d>", exp, frac+6, frac+6);
    sprintf(out_argment, "Bs1, Bs2, Bexp, Bm1, Bm2 ");

    fprintf(fp,
	    "circuit %s{\n"
	    "input %s;\n"
	    "output %s;\n"
	    "instrin do;\n"
	    "sel xmsb<3>, ymsb<3>;\n"
	    "BarrelShift bshift;\n",
	    module_name,
	    input_name,
	    output_name
	    );
    fprintf(fp,
	    "instruct do par{\n"
	    "alt{\n"
	    "(Aa<%d:0> == %d#0b0): xmsb = 0b000;\n"
	    "else: xmsb = 0b001;\n"
	    "}\n"
	    "alt{\n"
	    "(Ab<%d:0> == %d#0b0): ymsb = 0b000;\n"
	    "else: ymsb = 0b001;\n"
	    "}\n"
	    "Bm1 = xmsb || Aa<%d:0> || 0b000;\n"
	    "Bm2 = bshift.do((ymsb || Ab<%d:0> || 0b000), exp_diff).f;\n"
	    "Bs1 = Aa<%d>;\n"
	    "Bs2 = Ab<%d>;\n"
	    "Bexp = Aa<%d:%d>;\n"
	    "}\n"
	    "}\n\n",
	    width-2, width-1,
	    width-2, width-1,
	    frac-1,
	    frac-1,
	    width-1,
	    width-1,
	    width-2, frac
	    );
    
    sprintf(input_reg_name, "ina<%d>, inb<%d>, diff<%d>", width, width, exp);
    sprintf(in_reg_argment, "ina, inb, diff");
    sprintf(output_reg_name, "bs1, bs2, bexp<%d>, bm1<%d>, bm2<%d>", exp, frac+6, frac+6);
    sprintf(out_reg_argment, "bs1, bs2, bexp, bm1, bm2");
  }else if(step3 == 1){
    /**************************** step3 *******************************/
    MantissaAdderDec(frac+6, fp);  
    MantissaAdder(frac+6, fp);
    strcpy(module_name, GET_VAR_NAME(step3));
    sprintf(input_name, "Bs1, Bs2, Bexp<%d>, Bm1<%d>, Bm2<%d>", exp, frac+6, frac+6);
    sprintf(in_argment, "Bs1, Bs2, Bexp, Bm1, Bm2 ");
    sprintf(output_name, "Cm<%d>, Cs, Cexp<%d>", frac+6, exp);
    sprintf(out_argment, "Cm, Cs, Cexp");

    fprintf(fp,
	    "circuit %s{\n"
	    "input %s;\n"
	    "output %s;\n"
	    "instrin do;\n"
	    "sel m3<%d>;\n"
	    "MantissaAdder madd;\n",
	    module_name,
	    input_name,
	    output_name,
	    frac+6
	    );
    fprintf(fp,
	    "instruct do par{\n"
	    "m3 = madd.do(Bs1, Bs2, Bm1, Bm2).r1;\n"
	    "alt{\n"
	    "(m3<%d>): par{\n"
	    "Cm = ^m3 + 0b1;\n"
	    "Cs = m3<%d>;\n"
	    "Cexp=Bexp;\n"
	    "}\n"
	    "else: par{\n"
	    "Cm = m3;\n"
	    "Cs = m3<%d>;\n"
	    "Cexp=Bexp;\n"
	    "}\n"
	    "}\n"
	    "}\n"
	    "}\n\n",
	    frac+5,
	    frac+5,
	    frac+5
	    );
    
    sprintf(input_reg_name, "bs1, bs2, bexp<%d>, bm1<%d>, bm2<%d>", exp, frac+6, frac+6);
    sprintf(in_reg_argment, "bs1, bs2, bexp, bm1, bm2");
    sprintf(output_reg_name, "cm<%d>, cs, cexp<%d>", frac+6, exp);
    sprintf(out_reg_argment, "cm, cs, cexp");
  }else if(step4 == 1){
    /**************************** step4 *******************************/
    LeadingZeroShiftDec(exp, frac, frac+6, fp);
    LeadingZeroShift(exp, frac, frac+6, fp);
    strcpy(module_name, GET_VAR_NAME(step4));
    sprintf(input_name, "Cm<%d>, Cs, Cexp<%d>", frac+6, exp);
    sprintf(in_argment, "Cm, Cs, Cexp");
    sprintf(output_name, "Dm<%d>, Ds, Dexp<%d>", frac+6, exp);
    sprintf(out_argment, "Dm, Ds, Dexp");

    fprintf(fp,
	    "circuit %s{\n"
	    "input %s;\n"
	    "output %s;\n"
	    "instrin do;\n"
	    "LeadingZeroShift lzshift;\n",
	    module_name,
	    input_name,
	    output_name
	    );
    fprintf(fp,
	    "instruct do par{\n"
	    "alt{\n"
	    "(Cm == %d#0b0): par{\n"
	    "Ds = 0b0;\n"
	    "Dexp = %d#0b0;\n"
	    "Dm = %d#0b0;\n"
	    "}\n"
	    "else: par{\n"
	    "lzshift.do(Cm<%d:0> || 0b0);\n"
	    "Ds = Cs;\n"
	    "Dexp = (Cexp + ^(lzshift.amount) + 0b1) + 0b1;\n"
	    "Dm = lzshift.f;\n"
	    "}\n"
	    "}\n"
	    "}\n"
	    "}\n"
	    "\n",
	    frac+6,
	    exp,
	    frac+6,
	    frac+4
	    );
    
    sprintf(input_reg_name, "cm<%d>, cs, cexp<%d>", frac+6, exp);
    sprintf(in_reg_argment, "cm, cs, cexp");    
    sprintf(output_reg_name, "dm<%d>, ds, dexp<%d>", frac+6, exp);
    sprintf(out_reg_argment, "dm, ds, dexp");
  }else if(step5 == 1){
    /**************************** step5 *******************************/
    IncreaseFracDec(frac,  fp);
    IncreaseFrac(frac, fp);
    strcpy(module_name, GET_VAR_NAME(step5));
    sprintf(input_name, "Dm<%d>, Ds, Dexp<%d>", frac+6, exp);
    sprintf(in_argment, "Dm, Ds, Dexp");
    sprintf(output_name, "z<%d>", width);
    sprintf(out_argment, "z");

    fprintf(fp,
	    "circuit %s{\n"
	    "input %s;\n"
	    "output %s;\n"
	    "instrin do;\n"
	    "IncreaseFrac incfrac;\n"
	    "sel round;\n",
	    module_name,
	    input_name,
	    output_name
	    );
    fprintf(fp,
	    "instruct do par{\n"
	    "round = Dm<4>&(Dm<5>|Dm<3>|(/|Dm<2:0>));\n"
	    "incfrac.do(round, Dm<%d:5>);\n"
	    "z = Ds || (Dexp + incfrac.p) || incfrac.out;\n\n"
	    "}\n"
	    "}\n\n",
	    frac+4
	    );
    
    sprintf(input_reg_name, "dm<%d>, ds, dexp<%d>", frac+6, exp);
    sprintf(in_reg_argment, "dm, ds, dexp");
    sprintf(output_reg_name, "result<%d>", width);
    sprintf(out_reg_argment, "result");    
  }else{
    return ERROR;
  }

  /* Module decralation */
  fprintf(fp,
	  "declare %s{\n"
	  "input %s;\n"
	  "output %s;\n"
	  "instrin do;\n"
	  "instr_arg do(%s);\n"
	  "}\n"
	  "\n",
	  module_name,
	  input_name,
	  output_name,
	  in_argment
	  );
  
  /* Wrapper module */
  fprintf(fp,
	  "module wrapper{\n"
	  "input %s;\n"
	  "output %s;\n"
	  "instrin do;\n"
	  "%s %.4s;\n"
	  "reg_wr %s;\n"
	  "reg_wr %s;\n"
	  "instruct do par{\n",
	  input_name,
	  output_name,
	  module_name, module_name,
	  input_reg_name,
	  output_reg_name
	  );

  char *tmp[16];
  char *regtmp[16];
  int t, regt;
  int i;

  t = split(tmp, in_argment, ',');
  regt = split(regtmp, in_reg_argment, ',');
  for(i=0;i<t;i++){
    fprintf(fp,
	    "%s := %s;\n",
	    regtmp[i], tmp[i]
	    );
  }
  
  fprintf(fp, "%.4s.do(%s);\n", module_name, in_reg_argment);

  t = split(tmp, out_argment, ',');
  regt = split(regtmp, out_reg_argment, ',');
  for(i=0;i<t;i++){
    fprintf(fp,
	    "%s := %.4s.%s;\n",
	    regtmp[i], module_name, tmp[i]
	    );
  }

  for(i=0;i<t;i++){
    fprintf(fp,
	    "%s = %s;\n",
	    tmp[i],  regtmp[i]
	    );
  }
  
  fprintf(fp,
	  "}\n"
	  "}\n\n"
	  );
  
  free(*tmp);
  free(*regtmp);

  return 0;
}