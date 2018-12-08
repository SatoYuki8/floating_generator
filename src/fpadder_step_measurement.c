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

int fpadder_step_measurement(int exp, int frac, int width, FILE *fp, flags_t flag, char *top_module_name){
  int step1 = flag->step1_flag;
  int step2 = flag->step2_flag;
  int step3 = flag->step3_flag;
  int step4 = flag->step4_flag;
  int step5 = flag->step5_flag;

  char input_name[64];
  char input_reg_name[64];  
  char output_name[64];
  char output_reg_name[64];
  char in_argment[32];
  char in_reg_argment[32];
  char out_argment[32];
  char out_reg_argment[32];

  flag->pipeline_flag = 0;
  
  if(step1 == 1){
    /**************************** step1 *******************************/
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
	    top_module_name,
	    input_name,
	    output_name,
	    exp+1
	    );

    fprintf(fp, "instruct do par{\n");
    fpadd_exp_comparison(fp, exp, frac, width, flag);
    fprintf(fp,
	    "}\n"
	    "}\n"
	    );
        
    sprintf(input_reg_name, "ina<%d>, inb<%d>", width, width);
    sprintf(in_reg_argment, "ina, inb");
    sprintf(output_reg_name, "diff<%d>, outa<%d>, outb<%d>", exp, width, width);
    sprintf(out_reg_argment, "diff, outa, outb");
  }else if(step2 == 1){
    /**************************** step2 *******************************/
    BarrelShiftDec(exp, frac, frac+6, fp);
    BarrelShift(exp, frac, frac+6, fp);
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
	    top_module_name,
	    input_name,
	    output_name
	    );

    fprintf(fp, "instruct do par{\n");
    fpadd_preshift(fp, frac, width, flag);
    fprintf(fp,
	    "}\n"
	    "}\n"
	    );
        
    sprintf(input_reg_name, "ina<%d>, inb<%d>, diff<%d>", width, width, exp);
    sprintf(in_reg_argment, "ina, inb, diff");
    sprintf(output_reg_name, "bs1, bs2, bexp<%d>, bm1<%d>, bm2<%d>", exp, frac+6, frac+6);
    sprintf(out_reg_argment, "bs1, bs2, bexp, bm1, bm2");
  }else if(step3 == 1){
    /**************************** step3 *******************************/
    MantissaAdderDec(frac+6, fp);  
    MantissaAdder(frac+6, fp);
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
	    top_module_name,
	    input_name,
	    output_name,
	    frac+6
	    );

    fprintf(fp, "instruct do par{\n");
    fpadd_mantissa_add(fp, frac+6, flag);
    fprintf(fp,
	    "}\n"
	    "}\n"
	    );
        
    sprintf(input_reg_name, "bs1, bs2, bexp<%d>, bm1<%d>, bm2<%d>", exp, frac+6, frac+6);
    sprintf(in_reg_argment, "bs1, bs2, bexp, bm1, bm2");
    sprintf(output_reg_name, "cm<%d>, cs, cexp<%d>", frac+6, exp);
    sprintf(out_reg_argment, "cm, cs, cexp");
  }else if(step4 == 1){
    /**************************** step4 *******************************/
    LeadingZeroShiftDec(exp, frac, frac+5, fp);
    LeadingZeroShift(exp, frac, frac+5, fp);
    sprintf(input_name, "Cm<%d>, Cs, Cexp<%d>", frac+6, exp);
    sprintf(in_argment, "Cm, Cs, Cexp");
    sprintf(output_name, "Dm<%d>, Ds, Dexp<%d>", frac+4, exp);
    sprintf(out_argment, "Dm, Ds, Dexp");

    fprintf(fp,
	    "circuit %s{\n"
	    "input %s;\n"
	    "output %s;\n"
	    "instrin do;\n"
	    "LeadingZeroShift lzshift;\n",
	    top_module_name,
	    input_name,
	    output_name
	    );

    fprintf(fp, "instruct do par{\n");
    fpadd_leadingzeroshift(fp, frac+6, flag);
    fprintf(fp,
	    "}\n"
	    "}\n"
	    );
        
    sprintf(input_reg_name, "cm<%d>, cs, cexp<%d>", frac+6, exp);
    sprintf(in_reg_argment, "cm, cs, cexp");    
    sprintf(output_reg_name, "dm<%d>, ds, dexp<%d>", frac+4, exp);
    sprintf(out_reg_argment, "dm, ds, dexp");
  }else if(step5 == 1){
    /**************************** step5 *******************************/
    IncreaseFracDec(frac,  fp);
    IncreaseFrac(frac, fp);
    sprintf(input_name, "Dm<%d>, Ds, Dexp<%d>", frac+4, exp);
    sprintf(in_argment, "Dm, Ds, Dexp");
    sprintf(output_name, "z<%d>", width);
    sprintf(out_argment, "z");

    fprintf(fp,
	    "circuit %s{\n"
	    "input %s;\n"
	    "output %s;\n"
	    "instrin do;\n"
	    "IncreaseFrac incfrac;\n"
	    "sel rounded_frac<%d>;\n",
	    top_module_name,
	    input_name,
	    output_name,
	    frac
	    );

    fprintf(fp, "instruct do par{\n");
    fpadd_round(fp, frac, width, flag);
    fprintf(fp,
	    "}\n"
	    "}\n"
	    );
    
    sprintf(input_reg_name, "dm<%d>, ds, dexp<%d>", frac+4, exp);
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
	  top_module_name,
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
	  top_module_name, top_module_name,
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
  
  fprintf(fp, "%.4s.do(%s);\n", top_module_name, in_reg_argment);

  t = split(tmp, out_argment, ',');
  regt = split(regtmp, out_reg_argment, ',');
  for(i=0;i<t;i++){
    fprintf(fp,
	    "%s := %.4s.%s;\n",
	    regtmp[i], top_module_name, tmp[i]
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
