#include <stdio.h>

int read_zero_and_shift(int exp, int frac){
  FILE *fp;
  int i=0;

  if ((fp = fopen("read_zero_and_shift.sfl", "w")) == NULL){
    fprintf(stderr, "File open error.\n");
    return 0;
  }
  
  fcomment(fp);

  fprintf(fp,
	  "circuit read_zero_and_shift{\n"
	  "input frac<%d>;\n"
	  "input exp<%d>;\n"
	  "output shifted_frac<%d>;\n"
	  "output sub_exp<%d>;\n"
	  "instrin do;\n\n"
	  , frac, exp, frac, exp
	  );
  
  int t=frac;
  int amount=1;
  
  while(t != 0){
    t=t/2;
    amount++;
  }

  fprintf(fp,
	  "sel_v tmp<%d>;\n"
	  "reg_wr z;\n\n"
	  , amount
	  );
  
  int top = frac - 1;  //最上位ビット

  fprintf(fp,
	  "instruct do par{\n"
	  "alt{\n"
	  "(frac<%d> == 0b1): tmp = 0b"
	  , top
	  );

  DecToBi(0, amount, fp); fputs(";\n", fp);

  fprintf(fp, "(frac == 0b");
  DecToBi(0, frac, fp);
  fprintf(fp, "): par{\n"
	 "z := 0b1;\n"
	 "tmp = 0b"
	 );
  DecToBi(0, amount, fp); fputs(";\n}\n", fp);

  
  for (i=top-1; i>0; i--){
    fprintf(fp,
	    "(frac<%d:%d> == 0b"
	    , top, i
	    );
    DecToBi(1, frac-i, fp);
    fputs("): tmp = 0b", fp);
    DecToBi(top-i, amount, fp);
    fputs(";\n", fp);
  }

  fprintf(fp, "(frac == 0b");
  DecToBi(1, frac, fp);
  fprintf(fp, "): tmp = 0b");
  DecToBi(top, amount, fp); fputs(";\n", fp);
  fputs("}\n\n", fp);
	  

  fputs("alt{\n"
	"(z == 0b1): par {\n"
	"shifted_frac = 0b"
	, fp);
  DecToBi(0, frac, fp); fputs(";\n",fp);
  fputs("sub_exp = 0b", fp);
  DecToBi(0, exp, fp); fputs(";\n",fp);
  fputs("}\n"
	"else: par{\n"
	"shifted_frac = frac << tmp;\n"
	"sub_exp = exp + (^tmp + 0b"
	, fp);
  DecToBi(1, exp, fp); fputs(");\n", fp);
  fputs("}\n"
	"}\n"
	"}\n"
	"}\n"
	, fp);

  fclose(fp);

  return 0;
}
