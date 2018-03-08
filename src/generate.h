//#ifndef GEN_H
//#define GEN_H
#pragma once

#include <stdio.h>
#include <stdlib.h>

int comment();
int fcomment(FILE *fp);
int DecToBi(int dec, int dig, FILE *fp);
char ZeroStr(int N, char in[]);

int help(){
  puts("******************* HELP **********************\n"
       "す　お\n"
       "い　な\n"
       "た　か\n"
       "。　が\n"
       );
  return 0;
}

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

  if (dig <= 0) return 0;

  buf = (int*)calloc(dig, sizeof(int));

  for(i=0; i<dig; i++){
    if(i != 0 && num == 0) break;
    if(num % 2 == 0) buf[(dig-1)-i] = 0;
    else buf[(dig-1)-i] = 1;

    num = num / 2;
  }

  fprintf(fp, "0b");
  for (i=0; i<dig; i++){
    fprintf(fp, "%d", buf[i]);
  }

  free(buf);
  return 0;
}

/* 0b00...0の形の文字列をつくる */
char ZeroStr(int N, char in[]){
  in[0] = '0';
  in[1] = 'b';

  int i;
  for(i=2; i < N+2; i++){
    in[i] = '0';
  }
  
  return 0;
}

/* 0b11...1の形の文字列をつくる */
char OneStr(int N, char in[]){
  in[0] = '0';
  in[1] = 'b';

  int i;
  for(i=2; i < N+2; i++){
    in[i] = '1';
  }
  
  return 0;
}


//#endif
