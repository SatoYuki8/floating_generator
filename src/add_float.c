#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate.h"

int fpadd_exp_comparison_declaration(FILE *fp, int exp, int width, char *sel, char *sel_or_reg){
  fprintf(fp,
	  "%s wdiff<%d>;\n"
	  "%s exp_diff<%d>;\n"
	  "%s Aa<%d>, Ab<%d>;\n",
	  sel, exp+1,
	  sel_or_reg, exp,
	  sel_or_reg, width, width
	  );
  return 1;
}

int fpadd_preshift_declaration(FILE *fp, int exp, int frac_bit, char *sel, char *sel_or_reg){
  fprintf(fp,
	  "%s xmsb<3>, ymsb<3>;\n"
	  "%s Bm1<%d>, Bm2<%d>, Bs1, Bs2, Bexp<%d>;\n"
	  "",
	  sel,
	  sel_or_reg, frac_bit, frac_bit, exp
	  );
  return 1;
}

int fpadd_mantissa_add_declaration(FILE *fp, int exp, int frac_bit, char *sel, char *sel_or_reg){
   fprintf(fp,
	  "%s m3<%d>;\n"
	  "%s Cm<%d>;\n"
	  "%s Cs, Cexp<%d>;\n",
	  sel,
	  frac_bit,
	  sel_or_reg, frac_bit,
	  sel_or_reg, exp
	  );
  return 1;
}

int fpadd_leadingzeroshift_declaration(FILE *fp, int exp, int width, char *sel_or_reg){
  fprintf(fp,
	  "%s Ds, Dexp<%d>, Dm<%d>;\n",
	  sel_or_reg, exp, width+4
	  );
  return 1;
}

int fpadd_round_and_finish_declaration(FILE *fp, int frac, int width, char *sel, char *sel_or_reg){
  fprintf(fp,
	  "%s rounded_frac<%d>;\n"
	  "%s z<%d>;\n"
	  "",
	  sel, frac,
	  sel_or_reg, width
	  );
  return 1;
}

int fpadd_exp_comparison(FILE *fp, int exp, int frac, int width, flags_t flag){
P_IF
  fprintf(fp,
	  "wdiff = (0b0||a<%d:%d>) + (^(0b0||b<%d:%d>) + 0b1);\n"
	  "alt{\n"
	  "(wdiff<%d>): par{\n"
	  "generate A.pshft(^wdiff<%d:0> + 0b1, b, a);\n"
	  "}\n"
	  "else: par{\n"
	  "generate A.pshft(wdiff<%d:0>, a, b);\n"
	  "}\n"
	  "}\n"
	  "}\n\n",
	  width - 2, frac, width - 2, frac,
	  exp,
	  exp-1,
	  exp-1
	  );
P_ELSE
  fprintf(fp,
	  "wdiff = (0b0||a<%d:%d>) + (^(0b0||b<%d:%d>) + 0b1);\n"
	  "alt{\n"
	  "(wdiff<%d>): par{\n"
	  "exp_diff = ^wdiff<%d:0> + 0b1;\n"
	  "Aa = b; Ab = a;\n"
	  "}\n"
	  "else: par{\n"
	  "exp_diff = wdiff<%d:0>;\n"
	  "Aa = a; Ab = b;\n"
	  "}\n"
	  "}\n"
	  "\n",
	  width - 2, frac, width - 2, frac,
	  exp,
	  exp-1,
	  exp-1
	  );
P_END
  
  return 1;
}

int fpadd_preshift(FILE *fp, int frac, int width, flags_t flag){
P_IF
  fprintf(fp,
	  "stage A{\n"
	  "par{\n"
	  "alt{\n"
	  "(Aa<%d:0> == (%d#0b0)): xmsb = 0b000;\n"
	  "else: xmsb = 0b001;\n"
	  "}\n"
	  "alt{\n"
	  "(Ab<%d:0> == (%d#0b0)): ymsb = 0b000;\n"
	  "else: ymsb = 0b001;\n"
	  "}\n"
	  "relay B.manadd(xmsb || Aa<%d:0> || 0b000,\n"
	  "bshift.do((ymsb || Ab<%d:0> || 0b000), exp_diff).f,\n"
	  "Aa<%d>,\n"
	  "Ab<%d>,\n"
	  "Aa<%d:%d>\n"
	  ");\n"
	  "}\n"
	  "}\n"
	  "\n",
	  width - 2, width - 1,
	  width - 2, width - 1,
	  frac - 1,
	  frac - 1,
	  width - 1, width - 1,
	  width - 2, frac
	  );
P_ELSE
  fprintf(fp,
	  "alt{\n"
	  "(Aa<%d:0> == (%d#0b0)): xmsb = 0b000;\n"
	  "else: xmsb = 0b001;\n"
	  "}\n"
	  "alt{\n"
	  "(Ab<%d:0> == (%d#0b0)): ymsb = 0b000;\n"
	  "else: ymsb = 0b001;\n"
	  "}\n"
	  "Bm1 = xmsb || Aa<%d:0> || 0b000;\n"
	  "Bm2 = bshift.do((ymsb || Ab<%d:0> || 0b000), exp_diff).f;\n"
	  "Bs1 = Aa<%d>;\n"
	  "Bs2 = Ab<%d>;\n"
	  "Bexp = Aa<%d:%d>;\n"
	  "\n",
	  width - 2, width - 1,
	  width - 2, width - 1,
	  frac - 1,
	  frac - 1,
	  width - 1, width - 1,
	  width - 2, frac
	  );
P_END
  return 1;
}

int fpadd_mantissa_add(FILE *fp, int frac_bit, flags_t flag){
P_IF
  fprintf(fp,
	  "stage B{\n"
	  "par{\n"
	  "m3 = madd.do(Bs1, Bs2, Bm1, Bm2).r1;\n"
	  "alt{\n"
	  "(m3<%d>): par{\n"
	  "relay C.lzshft(^m3+0b1,\n"
	  "m3<%d>,\n"
	  "Bexp\n"
	  ");\n"
	  "}\n"
	  "else: par{\n"
	  "relay C.lzshft(m3,\n"
	  "m3<%d>,\n"
	  "Bexp\n"
	  ");\n"
	  "}\n"
	  "}\n"
	  "}\n"
	  "}\n"
	  "\n",
	  frac_bit-1,
	  frac_bit-1,
	  frac_bit-1
	  );
P_ELSE
  fprintf(fp,
	  "m3 = madd.do(Bs1, Bs2, Bm1, Bm2).r1;\n"
	  "alt{\n"
	  "(m3<%d>): par{\n"
	  "Cm = ^m3+0b1;\n"
	  "Cs = m3<%d>;\n"
	  "Cexp = Bexp;\n"
	  "}\n"
	  "else: par{\n"
	  "Cm = m3;\n"
	  "Cs = m3<%d>;\n"
	  "Cexp = Bexp;\n"
	  "}\n"
	  "}\n"
	  "\n",
	  frac_bit-1,
	  frac_bit-1,
	  frac_bit-1
	  );
P_END    
  return 1;
}

int fpadd_leadingzeroshift(FILE *fp, int frac_bit, flags_t flag){
P_IF
  fprintf(fp,
	  "stage C{\n"
	  "par{\n"
	  "lzshift.do(Cm<%d:0>);\n"
	  "relay D.rounding(Cs,\n"
	  "(Cexp + ^(lzshift.amount) + 0b1) + 0b1,\n"
	  "lzshift.f<%d:2> || /|lzshift.f<1:0>\n"
	  ");\n"
	  "}\n"
	  "}\n"
	  "\n",
	  frac_bit-2,
	  frac_bit-2
	  );
P_ELSE
  fprintf(fp,
	  "lzshift.do(Cm<%d:0>);\n"
	  "Ds = Cs;\n"
	  "Dexp = (Cexp + ^(lzshift.amount) + 0b1) + 0b1;\n"
	  "Dm = lzshift.f<%d:2> || /|lzshift.f<1:0>;\n"
	  "\n",
	  frac_bit-2,
	  frac_bit-2
	  );
P_END  
  return 1;
}

int fpadd_round(FILE *fp, int frac, int width, flags_t flag){
P_IF
  fprintf(fp,
	  "stage D{\n"
	  "par{\n"
	  "alt{\n"
	  "(Dm<%d>): par{\n"
	  "rounded_frac = incfrac.do(Dm<2>&(Dm<3>|Dm<1>|Dm<0>), Dm<%d:3>);\n"
	  "relay E.return_res( Ds || (Dexp + incfrac.p) || rounded_frac );\n"
	  "}\n"
	  "else: relay E.return_res(%d#0b0);\n"
	  "}\n"
	  "}\n"
	  "}\n"
	  "\n",
	  frac+3,
	  frac+2,
	  width
	  );
P_ELSE
  fprintf(fp,
	  "alt{\n"
	  "(Dm<%d>): par{\n"
	  "rounded_frac = incfrac.do(Dm<2>&(Dm<3>|Dm<1>|Dm<0>), Dm<%d:3>);\n"
	  "relay z = ( Ds || (Dexp + incfrac.p) || rounded_frac );\n"
	  "}\n"
	  "else: z = (%d#0b0);\n"
	  "}\n",
	  frac+3,
	  frac+2,
	  width
	  );
P_END
  return 1;
}

int add_float(int exp, int frac, int width, FILE *fp, flags_t flag, char *module_name){
  int moduleflag = 0;
  
  char type[8];
  if (moduleflag == 1) strcpy(type, "module");
  else strcpy(type, "circuit");
  

  int frac_bit = frac + 6;     // msb + frac + G,R,S bits (frac+6 bits)

  /*************************************************/
  /* 入出力と内部の端子宣言部                      */
  /*************************************************/  

  fprintf(fp,
	  "%s %s{\n"
	  "input a<%d>, b<%d>;\n"
	  "output result<%d>;\n"
	  "instrin do;\n"
	  "\n"
	  "BarrelShift bshift;\n"
	  "LeadingZeroShift lzshift;\n"
	  "MantissaAdder madd;\n"
	  "IncreaseFrac incfrac;\n"
	  "\n",
	  type, module_name,
	  width, width,
	  width
	  );

  char sel[4];
  strcpy(sel, "sel");
  char sel_or_reg[8];
P_IF
  strcpy(sel_or_reg, "reg_wr");
P_ELSE
  strcpy(sel_or_reg, "sel");
P_END
  
  //start
  fpadd_exp_comparison_declaration(fp, exp, width, sel, sel_or_reg);

  //pre-shift for adder operation
 fpadd_preshift_declaration(fp, exp, frac_bit, sel, sel_or_reg);
 
  //mantissa addition
 fpadd_mantissa_add_declaration(fp, exp, frac_bit, sel, sel_or_reg);

  //leading zero shift
 fpadd_leadingzeroshift_declaration(fp, exp, width, sel_or_reg);
 
  //rond and finish
 fpadd_round_and_finish_declaration(fp, frac, width, sel, sel_or_reg);
				    
  //stege declaration
P_IF
  fprintf(fp,
	  "\n"
	  "stage_name A{task pshft(exp_diff, Aa, Ab);}\n"
	  "stage_name B{task manadd(Bm1, Bm2, Bs1, Bs2, Bexp);}\n"
	  "stage_name C{task lzshft(Cm, Cs, Cexp);}\n"
	  "stage_name D{task rounding(Ds, Dexp, Dm);}\n"
	  "stage_name E{task return_res(z);}\n"
	  );
P_END
  
  /*************************************************/
  /*         モジュールの開始                      */
  /*************************************************/  
  
  fputs("\ninstruct do par{\n", fp);

  //start
 fpadd_exp_comparison(fp, exp, frac, width, flag);
 
  //pre-shift for adder operation
 fpadd_preshift(fp, frac, width, flag);
  
  //mantissa addition
 fpadd_mantissa_add(fp, frac_bit, flag);
 
  //leading zero shift
 fpadd_leadingzeroshift(fp, frac_bit, flag);
  
  //Round
 fpadd_round(fp, frac, width, flag);
 
  //return result
P_IF
  fprintf(fp, "stage E{\n");
P_END
  fprintf(fp,
	  "result = z;\n"
	  "}\n"
	  );

  fprintf(fp, "}\n\n");  //finish

  
  return 0;
}
