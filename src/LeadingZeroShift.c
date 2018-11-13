#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "generate.h"

int LeadingZeroShiftDec(int exp, int frac, int bit_width, FILE *fp){
  fprintf(fp,
	  "declare LeadingZeroShift{\n"
	  "input a<%d>;\n"
	  "output amount<%d>;\n"
	  "output f<%d>;\n"
	  "instrin do;\n"
	  "instr_arg do(a);\n"
	  "}\n\n",
	  bit_width, exp, bit_width
	  );
  return 0;
}

int LeadingZeroShift(int exp, int frac, int bit_width, FILE *fp){

  int shamt = (int)log2(bit_width-1) + 1;

  fprintf(fp,
	  "module LeadingZeroShift{\n"
	  "input a<%d>;\n"
	  "output amount<%d>;\n"
	  "output f<%d>;\n"
	  "instrin do;\n",
	  bit_width, exp, bit_width
	  );

  int i;
  for (i=0;i<shamt;i++){
    fprintf(fp, "sel t%d<%d>;\n", i, bit_width);
  }

  for (i=0;i<shamt;i++){
   fprintf(fp, "sel a%d;\n", i);
  }

  fprintf(fp, "instruct do par{\n");

  char *zero;
  for (i=0;i<shamt;i++){
    fputs("alt{\n", fp);
    
    zero = (char *)malloc((int)pow(2,shamt-i-1)*2);
    ZeroStr((int)pow(2,shamt-i-1), zero);

    if (i == 0){
      fprintf(fp,
	      "(a<%d:%d> == %s): par{ a%d = 0b1; t%d = a<%d:0>||%s; }\n"
	      "else: par{ a%d = 0b0; t%d = a; }\n",
	      bit_width-1, bit_width-(int)pow(2,shamt-i-1), zero, shamt-i-1, i, bit_width-(int)pow(2,shamt-i-1)-1 ,zero,	   
	      shamt-i-1, i
	      );
    }else{
      fprintf(fp,
	      "(t%d<%d:%d> == %s): par{ a%d = 0b1; t%d = t%d<%d:0>||%s; }\n"
	      "else: par{ a%d = 0b0; t%d = t%d; }\n",
	      i-1 ,bit_width-1, bit_width-(int)pow(2,shamt-i-1), zero, shamt-i-1, i, i-1, bit_width-(int)pow(2,shamt-i-1)-1 ,zero,	   
	      shamt-i-1, i, i-1
	      );
    }

    fputs("}\n", fp);
  }
  free(zero);

    int j;
    char *one_amt;
    one_amt = (char *)malloc(exp+2);
    OneStr(exp, one_amt);
    if (shamt > exp){     //指数部のビット幅がシフト量のビット幅より小さい時
    fprintf(fp,
	    "alt{\n"
	    "("
	    );
    for(j=shamt-1; j>exp-1; j--){
      fprintf(fp, "a%d", j);
      if(j != exp) fputs("|", fp);
    }
    fprintf(fp,
	    "): amount = %s;\n"
	    "else: amount = ",
	    one_amt
	    );
    for (j=exp-1; j>=0; j--){
      if (j==exp-1) fprintf(fp, "a%d", j);
      else fprintf(fp, "||a%d", j);
    }
    fputs(";\n"
	  "}\n", fp);
  }else{                //指数部のビット幅がシフト量のビット幅以上の時
    fputs("amount = ", fp); DecToBi(0, exp-shamt, fp);
    for (j=shamt-1; j>=0; j--){
      if ((exp-shamt <= 0) && (j == shamt-1)) fprintf(fp, "a%d", j);
      else fprintf(fp, "||a%d", j);
    }
    fputs(";\n", fp);
  }
  
  fprintf(fp,
	  "f = t%d;\n"
	  "}\n"
	  "}\n",
	  shamt-1
	  );

  free(one_amt);

  return 0;
}
