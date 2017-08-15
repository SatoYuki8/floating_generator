#include <stdio.h>

int comp_bit(){
  comment();
  
  puts("/* 1bit comperision module */\n\n");
  
  puts("module comp_bit{");
  
  puts("input a, b;");
  puts("output l;");
  puts("output s;");
  puts("output eq;");
  puts("instrin do;");

  puts("instrin do par {");
  puts("l = a&(^b);");
  puts("eq = ^(a@b);");
  puts("a = (^a)&b;");

  puts("}");     // instrin do par {
  puts("}");     // module comp_bit{
  
  return 0;
}
