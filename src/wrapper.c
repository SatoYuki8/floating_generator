#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int wrapp_adder(int bit_width, FILE *fp){
  fprintf(fp,
	  "declare FloatingAdder{\n"
	  "input a<%d>, b<%d>;\n"
	  "output result<%d>;\n"
	  "instrin do;\n"
	  "instr_arg do(a, b);\n"
	  "}\n\n",
	  bit_width, bit_width,
	  bit_width
	  );
  /*
  fprintf(fp,
	  "module test_bench{\n"
	  "input a<%d>, b<%d>;\n"
	  "output result<%d>;\n"
	  "instrin do;\n"
	  "FloatingAdder add;\n"
	  "reg_wr ina<%d>;\n"
	  "reg_wr inb<%d>;\n"
	  "reg_wr out<%d>;\n"
	  "stage_name TEST {task test(ina, inb);}\n"
	  "generate TEST.test(a, b);\n"
	  "stage TEST{\n"
	  "state_name st0, st1;\n"
	  "first_state st0;\n"
	  "state st0 par{\n"
	  "out := add.do(a,b).result;\n"
	  "goto st1;\n"
	  "}\n"
	  "state st1 par{\n"
	  "result = out;\n"
	  "goto st0;\n"
	  "finish;\n"
	  "}\n"
	  "}\n"
	  "}\n"
	  "\n",
	  bit_width,  bit_width,
	  bit_width,
	  bit_width,
	  bit_width,
	  bit_width
	  );
  */

  fprintf(fp,
	  "module wrapper{\n"
	  "input a<%d>, b<%d>;\n"
	  "output result<%d>;\n"
	  "instrin do;\n"
	  "FloatingAdder add;\n"
	  "reg_wr ina<%d>;\n"
	  "reg_wr inb<%d>;\n"
	  "reg_wr out<%d>;\n"
	  "instruct do par{\n"
	  "ina:=a;\n"
	  "inb:=b;\n"
	  "out:=add.do(ina, inb).result;\n"
	  "result = out;\n"
	  "}\n"
	  "}\n"
	  "\n",
	  bit_width,  bit_width,
	  bit_width,
	  bit_width,
	  bit_width,
	  bit_width
	  );
  
  return 0;
}

int wrapper(int bit_width, FILE *fp){

  wrapp_adder(bit_width, fp);
  
  return 0;
}
