#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define P_IF if (pipe == 1){
#define P_ELSE }else{
#define P_END }

int add_float(int exp, int frac, int bit_width, FILE *fp, int pipe){
  int moduleflag = 0;
  
  char type[8];
  if (moduleflag == 1) strcpy(type, "module");
  else strcpy(type, "circuit");
  
  int sign = bit_width - 1;
  int ehead = bit_width - 2;  //exp head bit
  int elast = bit_width - exp - 1;    //exp last bit
  int fhead = elast - 1;       //frac head bit

  int frac_bit = frac + 6;     // msb + frac + G,R,S bits (frac+6 bits)
  int frac_msb = frac_bit - 1;

  /*************************************************/
  /* 入出力と内部の端子宣言部                      */
  /*************************************************/  
  fprintf(fp,
	  "%s FloatingAdder{\n"
	  "input a<%d>, b<%d>;\n"
	  "output result<%d>;\n"
	  "instrin do;\n"
	  "\n"
	  "BarrelShift bshift;\n"
	  "LeadingZeroShift lzshift;\n"
	  "MantissaAdder madd;\n"
	  "IncreaseFrac incfrac;\n"
	  "\n",
	  type,
	  bit_width, bit_width,
	  bit_width
	  );

  char sel_or_reg[8];
P_IF
  strcpy(sel_or_reg, "reg_wr");
P_ELSE
  strcpy(sel_or_reg, "sel");
P_END
  
  //start
  fprintf(fp,
	  "sel wdiff<%d>;\n"
	  "%s exp_diff<%d>;\n"
	  "%s Aa<%d>, Ab<%d>;\n",
	  exp+1,
	  sel_or_reg, exp,
	  sel_or_reg, bit_width, bit_width
	  );

  //pre-shift for adder operation
  fprintf(fp,
	  "sel xmsb<3>, ymsb<3>;\n"
	  "%s Bm1<%d>, Bm2<%d>, Bs1, Bs2, Bexp<%d>;\n"
	  "",
	  sel_or_reg, frac_bit, frac_bit, exp
	  );

  //mantissa addition
  fprintf(fp,
	  "sel m3<%d>;\n"
	  "%s Cm<%d>;\n"
	  "%s Cs, Cexp<%d>;\n",
	  frac_bit,
	  sel_or_reg, frac_bit,
	  sel_or_reg, exp
	  );

  //leading zero shift
  fprintf(fp,
	  "%s Ds, Dexp<%d>, Dm<%d>;\n",
	  sel_or_reg, exp, frac_bit
	  );

  //rond and finish
  fprintf(fp,
	  "sel round;\n"
	  "%s z<%d>;\n"
	  "",
	  sel_or_reg, bit_width
	  );

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
	  ehead, elast, ehead, elast,
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
	  ehead, elast, ehead, elast,
	  exp,
	  exp-1,
	  exp-1
	  );
P_END
 
  //pre-shift for adder operation
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
	  ehead, sign,
	  ehead, sign,
	  fhead,
	  fhead,
	  sign, sign,
	  ehead, elast
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
	  ehead, sign,
	  ehead, sign,
	  fhead,
	  fhead,
	  sign, sign,
	  ehead, elast
	  );
P_END
  
  //mantissa addition
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
	  frac_msb,
	  frac_msb,
	  frac_msb
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
	  frac_msb,
	  frac_msb,
	  frac_msb
	  );
P_END

  //leading zero shift
P_IF
  fprintf(fp,
	  "stage C{\n"
	  "par{\n"
	  "alt{\n"
	  "(Cm == (%d#0b0)): par{\n"
	  "relay D.rounding(0b0,\n"
	  "%d#(0b0),\n"
	  "%d#(0b0)\n"
	  ");\n"
	  "}\n"
	  "else: par{\n"
	  "lzshift.do(Cm<%d:0> || 0b0);\n"
	  "relay D.rounding(Cs,\n"
	  "(Cexp + ^(lzshift.amount) + 0b1) + 0b1,\n"
	  "lzshift.f\n"
	  ");\n"
	  "}\n"
	  "}\n"
	  "}\n"
	  "}\n"
	  "\n",
	  frac_bit,
	  exp,
	  frac_bit,
	  frac_bit-2
	  );
P_ELSE
  fprintf(fp,
	  "alt{\n"
	  "(Cm == (%d#0b0)): par{\n"
	  "Ds = 0b0;\n"
	  "Dexp = %d#(0b0);\n"
	  "Dm = %d#(0b0);\n"
	  "}\n"
	  "else: par{\n"
	  "lzshift.do(Cm<%d:0> || 0b0);\n"
	  "Ds = Cs;\n"
	  "Dexp = (Cexp + ^(lzshift.amount) + 0b1) + 0b1;\n"
	  "Dm = lzshift.f;\n"
	  "}\n"
	  "}\n"
	  "\n",
	  frac_bit,
	  exp,
	  frac_bit,
	  frac_bit-2
	  );
P_END
  
  //Round
P_IF
  fprintf(fp,
	  "stage D{\n"
	  "par{\n"
	  "round = Dm<4>&(Dm<5>|Dm<3>|(/|Dm<2:0>));\n"
	  "incfrac.do(round, Dm<%d:5>);\n"
	  "relay E.return_res( Ds || (Dexp + incfrac.p) || incfrac.out );\n"
	  "}\n"
	  "}\n"
	  "\n",
	  frac_msb-1
	  );
P_ELSE
  fprintf(fp,
	  "round = Dm<4>&(Dm<5>|Dm<3>|(/|Dm<2:0>));\n"
	  "incfrac.do(round, Dm<%d:5>);\n"
	  "z = Ds || (Dexp + incfrac.p) || incfrac.out;\n\n",
	  frac_msb-1
	  );
P_END
 
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
