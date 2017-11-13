#include <stdio.h>

int round_process(int frac){
  FILE *fp;
  int i=0;
  
  if ((fp = fopen("round_process.sfl", "w")) == NULL){
    fprintf(stderr, "File open error.\n");
    return 0;
  }
  
  fcomment(fp);
  
  fprintf(fp,
	  "circuit round_process{\n"
	  "input frac<%d>;\n",frac
	  );
  fprintf(fp,
	  "input sticky;\n"
	  "output result<%d>;\n", frac
	  );
  fprintf(fp,
	  "output cout;\n"
	  "instrin do;\n"
	  "\n"
	  );

  fprintf(fp, "sel_v tmp<%d>;\n\n", frac+1);

  fprintf(fp,
	  "instruct do par{\n"
	  "alt{\n"
	  "(frac<1> ==0b0): par{\n"
	  "result = frac;\n"
	  "cout = 0b0;\n"
	  "}\n"
	  "((^frac<2>&frac<1>&^frac<0>&^sticky) == 0b1): par{\n"
	  "result = frac;\n"
	  "cout = 0b0;\n"
	  "}\n"
	  "else: par{\n"
	  "tmp = (0b0||frac) + 0b"
	  );
  for (i=0;i<frac-2;i++){
    fputs("0", fp);    
  }
  fprintf(fp,
	  "100;\n"
	  "result = tmp<%d:0>;\n",
	  frac-1
	  );
  fprintf(fp,
	  "cout = tmp<%d>;\n",
	  frac);

  fprintf(fp,
	  "}\n"
	  "}\n"
	  "}\n"
	  "}\n"
	  );

  return 0;
}
