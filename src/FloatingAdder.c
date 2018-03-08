#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int BarrelShiftDec(int exp, int frac, int bit_width, FILE *fp);
int BarrelShift(int exp, int frac, int bit_width, FILE *fp);
int LeadingZeroShiftDec(int exp, int frac, int bit_width, FILE *fp);
int LeadingZeroShift(int exp, int frac, int bit_width, FILE *fp);
int MantissaAdderDec(int bit_width, FILE *fp);  
int MantissaAdder(int bit_width, FILE *fp);
int IncreaseFracDec(int frac,  FILE *fp);
int IncreaseFrac(int frac, FILE *fp);
int add_float(int exp, int frac, int bit_width, FILE *fp);

int FloatingAdder(int exp, int frac, int bit_width, FILE *fp){
  int frac_bit = frac + 6;    // msb || frac || G, R, S bits
  int frac_msb = frac_bit - 1;

  /*
  BarrelShiftDec(exp, frac, bit_width, fp);
  BarrelShift(exp, frac, bit_width, fp);

  LeadingZeroShiftDec(exp, frac, bit_width, fp);
  LeadingZeroShift(exp, frac, bit_width, fp);
  
  MantissaAdderDec(bit_width, fp);  
  MantissaAdder(bit_width, fp);
  */

  BarrelShiftDec(exp, frac, frac_bit, fp);
  BarrelShift(exp, frac, frac_bit, fp);
  
  LeadingZeroShiftDec(exp, frac, frac_bit, fp);
  LeadingZeroShift(exp, frac, frac_bit, fp);
  
  MantissaAdderDec(frac_bit, fp);  
  MantissaAdder(frac_bit, fp);
  
  IncreaseFracDec(frac,  fp);
  IncreaseFrac(frac, fp);
  
  add_float(exp, frac, bit_width, fp);

  return 0;
}
