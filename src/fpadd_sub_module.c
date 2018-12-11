#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "add_gen.h"

int BarrelShiftDec(int exp, int frac, int bit_width, FILE *fp){

  fprintf(fp,
	  "declare BarrelShift{\n"
	  "input a<%d>;\n"
	  "input amount<%d>;\n"
	  "output f<%d>;\n"
	  "instrin do;\n"
	  "instr_arg do(a, amount);\n"
	  "}\n\n",
	  bit_width, exp, bit_width
	  );
  
  return 0;
}


int BarrelShift(int exp, int frac, int bit_width, FILE *fp){

  int shamt = (int)log2(bit_width-1) + 1;
  //int shamt = (int)log2(frac+5) + 1;

  if (shamt > exp) shamt = exp;
  
  fprintf(fp,
	  "module BarrelShift{\n"
	  "input a<%d>;\n"
	  "input amount<%d>;\n"
	  "output f<%d>;\n"
	  "instrin do;\n",
	  bit_width, exp, bit_width
	  );

  int i;
  for (i=0;i<shamt;i++){
    fprintf(fp, "sel t%d<%d>;\n", i, bit_width);
  }

  for (i=0;i<shamt;i++){
    fprintf(fp, "sel s%d<%d>;\n", i, (int)pow(2,i+1));
  }

  fprintf(fp, "instruct do par{\n");

  int ulp = bit_width - (frac + 3);
  char aort;
  for (i=0;i<shamt;i++){
    if (i==0){
      aort = 'a';
      fprintf(fp,
	      "alt{\n"
	      "(amount<%d>): par{\n"
	      "t%d = ",
	      i, i
	      );
      DecToBi(0, (int)pow(2,i), fp);  
      fprintf(fp,
	      " || %c<%d:1>;\n"
	      "s%d = %c<%d> || 0b0;\n"
	      "}\n",
	      aort, bit_width-1,
	      i, aort, ulp
	      );
      fprintf(fp,
	      "else: par{\n"
	      "t%d = %c;\n"
	      "s%d = 0b00;\n"
	      "}\n"
	      "}\n",
	      i, aort,
	      i
	      );
    }else{
      aort = 't';
      
      fprintf(fp,
	      "alt{\n"
	      "(amount<%d>): par{\n"
	      "t%d = ",
	      i, i
	      );
      DecToBi(0, (int)pow(2,i), fp);

      /********************************************************/
      if((ulp+((int)pow(2,i)-1)) >= bit_width){
	fprintf(fp,
		" || %c%d<%d:%d>;\n"
		"s%d = %d#(0b0) || %c%d<%d:%d> || s%d;\n"
		"}\n",
		aort, i-1, bit_width-1, (int)pow(2,i),
		i, ulp+((int)pow(2,i)-1) - (bit_width-1), aort, i-1, bit_width - 1, ulp, i-1
		);
      }else{
	fprintf(fp,
		" || %c%d<%d:%d>;\n"
		"s%d = %c%d<%d:%d> || s%d;\n"
		"}\n",
		aort, i-1, bit_width-1, (int)pow(2,i),
		i, aort, i-1, ulp+((int)pow(2,i) - 1), ulp, i-1
		);
      }
      /********************************************************/
      
      fprintf(fp,
	      "else: par{\n"
	      "t%d = %c%d;\n"
	      "s%d = s%d||",
	      i, aort, i-1,
	      i, i-1
	      );
      DecToBi(0, (int)pow(2,i), fp);
      fprintf(fp,
	      ";\n"
	      "}\n"
	      "}\n"
	      );
    }
  }

  char *zero_full;
  zero_full = (char *)malloc(bit_width*2);
  ZeroStr(bit_width, zero_full);
  if ((exp-1) < i) {
    fprintf(fp,
	    "f = t%d<%d:1> || /|(s%d<%d:0>);\n"
	    "}\n"
	    "}\n",
	    shamt-1, bit_width-1, shamt-1, (int)pow(2,shamt)-3
	    );
  }else{
    fprintf(fp,
	    "alt{\n"
	    "(/|amount<%d:%d>): f = %s;\n"
	    "else: f = t%d<%d:1> || /|(s%d<%d:0>);\n"
	    "}\n"
	    "}\n"
	    "}\n",
	    exp-1, i, zero_full,
	    shamt-1, bit_width-1, shamt-1, (int)pow(2,shamt)-3
	    );
  }
  
  free(zero_full);
  printf("\n");

  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "add_gen.h"

int IncreaseFracDec(int frac, FILE *fp){
  fprintf(fp,
	  "declare IncreaseFrac{\n"
	  "input cin;\n"
	  "input in<%d>;\n"
	  "output out<%d>;\n"
	  "output p;\n"
	  "instrin do;\n"
	  "instr_arg do(cin, in);\n"
	  "}\n\n",
	  frac, frac
	  );
  
  return 0;
}

int IncreaseFrac(int frac, FILE *fp){
  
  int bit_head = frac - 2;

  fprintf(fp,
	  "circuit IncreaseFrac{\n"
	  "input cin;\n"
	  "input in<%d>;\n"
	  "output out<%d>;\n"
	  "output p;\n"
	  "instrin do;\n\n"
	  "sel tmp<%d>;\n\n"
	  "instruct do par{\n"
	  "tmp = (0b0||in) + cin;\n"
	  "out = tmp<%d:0>;\n"
	  "p = tmp<%d>;\n"
	  "}\n}\n\n",
	  frac, frac, frac+1,
	  frac-1, frac
	  );
 

  return 0;
}

/*
int IncreaseFrac(int frac, FILE *fp){
  
  int bit_head = frac - 2;

  fprintf(fp,
	  "module IncreaseFrac{\n"
	  "input cin;\n"
	  "input in<%d>;\n"
	  "output out<%d>;\n"
	  "output p;\n"
	  "instrin do;\n\n"
	  "sel cry<%d>;\n\n"
	  "instruct do par{\n"
	  "p = /&(in) & cin;\n"
	  "cry = (/&(in<%d:0>) & cin)\n",
	  frac, frac, frac,
	  bit_head
	  );
  
  int i;
  for (i=bit_head-1; i >0; i--){
    fprintf(fp,
	    "|| (/&(in<%d:0>) & cin)\n",
	    i
	    );
  }

  fprintf(fp,
	  "|| (in<0> & cin)\n"
	  "|| cin;\n"
	  "out = in @ cry;\n"
	  "}\n"
	  "}\n\n"
	  );

  return 0;
}
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "add_gen.h"

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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "add_gen.h"

int MantissaAdderDec(int bit_width, FILE *fp){
  
  fprintf(fp,
	  "declare MantissaAdder{\n"
	  "input s1, s2;\n"
	  "input x1<%d>, x2<%d>;\n"
	  "output r1<%d>;\n"
	  "instrin do;\n"
	  "instr_arg do(s1, s2, x1, x2);\n"
	  "}\n\n",
	  bit_width,  bit_width, bit_width
	  );

  return 0;
}

int MantissaAdder(int bit_width, FILE *fp){

  fprintf(fp,
	  "circuit MantissaAdder{\n"
	  "input s1, s2;\n"
	  "input x1<%d>, x2<%d>;\n"
	  "output r1<%d>;\n"
	  "instrin do;\n\n"
	  "instruct do r1 = ((%d#s1 @ x1) + s1) + ((%d#s2 @ x2) + s2);\n"
	  "}\n\n",
	  bit_width,  bit_width, bit_width, bit_width, bit_width
	  );

  return 0;
}
