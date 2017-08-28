#include <stdio.h>
#include <stdlib.h>

int comment(){
  puts("\n/* This is code of Floating-Calcuration generator. */");
  puts("/*                                                 */");
  puts("/* 2017~ Sato Yuki, Okayama Univercity             */");
  puts("/* If there is error, please send error report.    */\n\n\n");
  return 0;
}

int fcomment(FILE *fp){
  fputs("\n/* This is code of Floating-Calcuration generator. */\n", fp);
  fputs("/*                                                 */\n", fp);
  fputs("/* 2017~ Sato Yuki, Okayama Univercity             */\n", fp);
  fputs("/* If there is error, please send error report.    */\n\n\n\n", fp);
  return 0;
}


/* DecToBi function changes Decimal Num to Binary Num.  */
/* dec -> 2進数に変換したい10進数, dig -> 桁数            */
int DecToBi(int dec, int dig, FILE *fp){
  //char buf[dig+1];
  int *buf;
  int i;
  int num = dec;

  buf = (int*)calloc(dig, sizeof(int));

  for(i=0; i<dig; i++){
    if(i != 0 && num == 0) break;
    if(num % 2 == 0) buf[(dig-1)-i] = 0;
    else buf[(dig-1)-i] = 1;

    num = num / 2;
  }

  for (i=0; i<dig; i++){
    fprintf(fp, "%d", buf[i]);
  }

  free(buf);
  return 0;
}
