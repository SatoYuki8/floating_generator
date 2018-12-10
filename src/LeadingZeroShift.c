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
	  "circuit LeadingZeroShift{\n"
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

  for (i=0;i<shamt;i++){
    fputs("alt{\n", fp);

    if (i == 0){
      fprintf(fp,
	      "(a<%d:%d> == (%d#0b0)): par{\n"
	      "a%d = 0b1; \n"
	      "t%d = a<%d:0>||(%d#0b0);\n"
	      "}\n"
	      "else: par{\n"
	      "a%d = 0b0; \n"
	      "t%d = a;\n"
	      "}\n",
	      bit_width-1, bit_width-(int)pow(2,shamt-i-1), (int)pow(2,shamt-i-1),
	      shamt-i-1,
	      i, bit_width-(int)pow(2,shamt-i-1)-1 ,(int)pow(2,shamt-i-1),	   
	      shamt-i-1,
	      i
	      );
    }else{
      fprintf(fp,
	      "(t%d<%d:%d> == (%d#0b0)): par{ \n"
	      "a%d = 0b1; \n"
	      "t%d = t%d<%d:0>||(%d#0b0); \n"
	      "}\n"
	      "else: par{ \n"
	      "a%d = 0b0; \n"
	      "t%d = t%d; \n"
	      "}\n",
	      i-1 ,bit_width-1, bit_width-(int)pow(2,shamt-i-1), (int)pow(2,shamt-i-1),
	      shamt-i-1,
	      i, i-1, bit_width-(int)pow(2,shamt-i-1)-1 ,(int)pow(2,shamt-i-1),	   
	      shamt-i-1,
	      i, i-1
	      );
    }
  
    fputs("}\n", fp);
  }
    //free(zero);

    int j;
    if (shamt > exp){     //指数部の幅で表せられる値がシフト量より小さい時
    fprintf(fp,
	    "alt{\n"
	    "("
	    );
    for(j=shamt-1; j>exp-1; j--){
      fprintf(fp, "a%d", j);
      if(j != exp) fputs("|", fp);
    }
    fprintf(fp,
	    "): amount = %d#0b1;\n"
	    "else: amount = ",
	    exp
	    );
    for (j=exp-1; j>=0; j--){
      if (j==exp-1) fprintf(fp, "a%d", j);
      else fprintf(fp, "||a%d", j);
    }
    fputs(";\n"
	  "}\n", fp);
  }else{                //指数部で表せられる値がシフト量以上の時
      fprintf(fp,
	      "amount = (%d#0b0)",
	      exp-shamt);
      //DecToBi(0, exp-shamt, fp);
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


  return 0;
}
