#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
	fprintf(fp,
		" || %c%d<%d:%d>;\n"
		"s%d = %c%d<%d:%d> || s%d;\n"
		"}\n",
		aort, i-1, bit_width-1, (int)pow(2,i),
		i, aort, i-1, ulp+((int)pow(2,i) - 1), ulp, i-1
		);
      
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
  zero_full = (char *)malloc(bit_width+2);
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
  
  printf("\n");

  return 0;
}
