#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
