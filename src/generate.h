#include <stdio.h>

void comment(){
  puts("\n/* This is code of Floating-Calcuration generator. */");
  puts("/*                                                 */");
  puts("/* 2017~ Sato Yuki, Okayama Univercity             */");
  puts("/* If there is error, please send error report.    */\n\n\n");
}

void fcomment(FILE *fp){
  fputs("\n/* This is code of Floating-Calcuration generator. */\n", fp);
  fputs("/*                                                 */\n", fp);
  fputs("/* 2017~ Sato Yuki, Okayama Univercity             */\n", fp);
  fputs("/* If there is error, please send error report.    */\n\n\n\n", fp);
}
