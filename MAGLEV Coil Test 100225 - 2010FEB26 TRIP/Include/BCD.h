#ifndef BCD_H
#define BCD_H

#define MINNEGFLOAT  -0.999999e-32F
#define MAXNEGFLOAT  -0.999999e+31F 
#define MINPOSFLOAT   0.999999e-32F
#define MAXPOSFLOAT   0.999999e+31F



int BCDtoFloat(long int bcdValue, float *ReturnVal);
int FloattoBCD(float floatValue, long int *ReturnVal);

#endif
