#include <stdio.h>
#include <stdlib.h>
#include "generate.h"

int add_half(int exp, int frac, int bit_width){
  FILE *fp;
  
  if ((fp = fopen("add_half.sfl", "w")) == NULL){
    fprintf(stderr, "File open erroe.\n");
    return 1;
  }
  
  fcomment(fp);
  
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

  fprintf(fp,
	  "declare MantissaAdder{\n"
	  "input s1, s2;\n"
	  "input x1<%d>, x2<%d>;\n"
	  "output r1<%d>;\n"
	  "instrin do;\n"
	  "instr_arg do(s1, s2, x1, x2);\n"
	  "}\n\n",
	  bit_width, bit_width, bit_width
	  );
	  
  fprintf(fp,
	  "circuit MantissaAdder{\n"
	  "input s1, s2;\n"
	  "input x1<%d>, x2<%d>;\n"
	  "output r1<%d>;\n"
	  "instrin do;\n"
	  "\n"
	  "instruct do par{\n"
	  "r1 = (%d#s1 @ x1) + s1 + (%d#s2 @ x2) + s2;\n"
	  "}\n"
	  "}\n\n",
	  bit_width, bit_width, bit_width, bit_width, bit_width
	  );
  
	  

  return 0;
}
