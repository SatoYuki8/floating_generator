#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int add_float(int exp, int frac, int bit_width, FILE *fp){
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
  
  char *zero_exp, *zero_full;
  zero_exp = (char *)malloc(exp*2);
  zero_full = (char *)malloc(bit_width*2);
  ZeroStr(exp, zero_exp); ZeroStr(bit_width, zero_full);
  char *zero_frac;
  zero_frac = (char *)malloc(frac_bit*2);
  ZeroStr(frac_bit, zero_frac);
  
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

  //start
  fprintf(fp,
	  "sel wdiff<%d>;\n"
	  "sel exp_diff<%d>;\n"
	  "sel Aa<%d>, Ab<%d>;\n",
	  exp+1,
	  exp,
	  bit_width, bit_width
	  );

  //pre-shift for adder operation
  fprintf(fp,
	  "sel xmsb<3>, ymsb<3>;\n"
	  "sel Bm1<%d>, Bm2<%d>, Bs1, Bs2, Bexp<%d>;\n"
	  "",
	  frac_bit, frac_bit, exp
	  );

  //mantissa addition
  fprintf(fp,
	  "sel m3<%d>;\n"
	  "sel Cm<%d>;\n"
	  "sel Cs, Cexp<%d>;\n",
	  frac_bit,
	  frac_bit,
	  exp
	  );

  //leading zero shift
  fprintf(fp,
	  "sel round;\n"
	  "sel z<%d>;\n"
	  "",
	  bit_width
	  );

  /*************************************************/
  /*         モジュールの開始                      */
  /*************************************************/  
  
  fputs("\ninstruct do par{\n", fp);

  //start
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

  //pre-shift for adder operation
  char *zero_nonsign;    //binary zero string (frac-1bits)
  zero_nonsign = (char *)malloc(sign + 2);
  ZeroStr(sign, zero_nonsign);
  
  fprintf(fp,
	  "alt{\n"
	  "(Aa<%d:0> == %s): xmsb = 0b000;\n"
	  "else: xmsb = 0b001;\n"
	  "}\n"
	  "alt{\n"
	  "(Ab<%d:0> == %s): ymsb = 0b000;\n"
	  "else: ymsb = 0b001;\n"
	  "}\n"
	  "Bm1 = xmsb || Aa<%d:0> || 0b000;\n"
	  "Bm2 = bshift.do((ymsb || Ab<%d:0> || 0b000), exp_diff).f;\n"
	  "Bs1 = Aa<%d>;\n"
	  "Bs2 = Ab<%d>;\n"
	  "Bexp = Aa<%d:%d>;\n"
	  "\n",
	  ehead, zero_nonsign,
	  ehead, zero_nonsign,
	  fhead,
	  fhead,
	  sign, sign,
	  ehead, elast
	  );
  
  //mantissa addition  
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


  //leading zero shift
  fprintf(fp,
	  "alt{\n"
	  "(Cm == %s): z = %s;\n"
	  //"(Cexp == %s): z = Cs || Cexp || Cm<%d:3>;\n"
	  "else: par{\n"
	  "lzshift.do(Cm<%d:0> || 0b0);\n"
	  "round = lzshift.f<4>&(lzshift.f<5>|lzshift.f<3>|(/|lzshift.f<2:0>));\n"
	  "alt{\n"
	  "(round): par{\n"
	  "incfrac.do(0b1, lzshift.f<%d:5>);\n"
	  "z = Cs||(((Cexp+^(lzshift.amount)+0b1)+0b1)+incfrac.p)||incfrac.out;\n"
	  "}\n"
	  "else: z = Cs||((Cexp+^(lzshift.amount)+0b1)+0b1)||lzshift.f<%d:5>;\n"
	  "}\n"
	  "}\n"
	  "}\n"
	  "\n",
	  zero_frac, zero_full,
	  // zero_exp, fhead+3,
	  frac_msb-1,
	  frac_msb-1,
	  frac_msb-1
	  );

  //return result
  fprintf(fp,
	  "result = z;\n"
	  "}\n"
	  );

  fprintf(fp, "}\n\n");  //finish

  free(zero_nonsign);
  free(zero_exp);
  free(zero_frac);
  free(zero_full);
  
  return 0;
}
