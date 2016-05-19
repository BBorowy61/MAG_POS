#include <math.h>
#include "BCD.h"


//////////////Translate BCD to and from Double/////////////////////////////
int BCDtoFloat(long bcdValue, float *ReturnVal)
{
	unsigned char byte = 0, mantissasign = 0; 
	unsigned char remainder = 0, dividend = 0;
	char exponent = 0, signedbyte = 0;
	float retVal = 0;
	
	//get first byte of the mantissa
	byte = (bcdValue >> 24) & 0x00FF;
	
	dividend = byte>>4;
	
	remainder = byte & 0x000F;
	
	retVal = (dividend/10.0) + (remainder/100.0);
	
	//get second byte of the mantissa
	byte = (bcdValue >> 16) & 0x00FF; ;
	
	dividend = byte>>4;
	
	remainder = byte& 0x000F;
	
	retVal = retVal + (dividend/1000.0) + (remainder/10000.0);
	
	//get third byte of the mantissa
	byte = (bcdValue >> 8)  & 0x00FF;;
	
	dividend = byte>>4;
	
	remainder = byte & 0x000F;
	
	retVal = retVal + (dividend/100000.0) + (remainder/1000000.0);
	
	//get last byte
	signedbyte = bcdValue & 0x00FF;;

	//figure out the sign from bit 7
	//	1111 0011
	// &1000 0000
	// =1000 0000
	// this will do it
	
	mantissasign = signedbyte & 0x80;
	
	if (mantissasign == 0x80)
	{
		retVal = -retVal;
	}
	
	//we can ignore bit 6 to see if the exponent is neg or pos - exp is 2s comp

	//shift the leftmost 2 bits out of the number
	signedbyte = signedbyte <<10;
	signedbyte = signedbyte >>10;

	//convert from 2s complement
	//exponent = (~signedbyte) + 1;
	exponent = signedbyte;
	
	*ReturnVal = retVal * (pow(10, exponent));
	
	return 0;
}


int FloattoBCD(float floatValue, long *ReturnVal)
{
	unsigned long int retVal = 0;
	char exponent = 0, sign = 0;
	unsigned char unsignedbyte = 0;
	unsigned char temp1 = 0, temp2 = 0;
	float tempFloat;
	
	
	if (floatValue == 0.0)
	{
		*ReturnVal = 0;
		return 0;
	}
	
	if (floatValue < 0.0)
	{
		if (floatValue > MINNEGFLOAT)   //-0.999999 E-32
		{
			//outside the bounds of BCD
			*ReturnVal = 0;	
			return 0;
		}
		else if (floatValue < MAXNEGFLOAT) //-0.999999 E+31 
		{
			//outside the bounds of BCD
			*ReturnVal = 0x999999DF;
			return 0;
		}
		sign = 1;
		floatValue = -floatValue;
	}
	else
	{
		if (floatValue < MINPOSFLOAT)
		{
			//outside the bounds of BCD		//0.999999 E-32
			*ReturnVal = 0;
			return 0;
		}
		else if (floatValue > MAXPOSFLOAT)
		{
			//outside the bounds of BCD	   //0.999999 E+31
			*ReturnVal = 0x9999995F;
			return 0;
		}
		sign = 0;
	}
	
	//we need to get the mantissa, so we put the number in exponent form
	//but we need the mantissa to be of the form 0.xxxxxx
	if (floatValue >= 1.0)
	{
		while(floatValue >= 1.0)
		{
			floatValue = floatValue/10;
			exponent++;
		}
	}
	else if ((floatValue > 0.1) && (floatValue < 1.0))
	{
		exponent = 0;
	}
	else if ((floatValue > 0.0) && (floatValue <= 0.1))
	{
		while(floatValue <= 0.1)
		{
			floatValue = floatValue*10;
			exponent--;
		}
	}
	
	//now we put it into BCD
	//get first byte of mantissa
	tempFloat = floatValue * 10;
	
	temp1 = tempFloat; 
	
	tempFloat = tempFloat * 10;
	
	temp2 = (long int) tempFloat % 10;
	
	retVal = retVal + (temp1 * 16) + temp2;
	
	retVal = retVal << 8;
	
	//get second byte of mantissa
	tempFloat = tempFloat * 10;
	
	temp1 = (long int) tempFloat % 10; 
	
	tempFloat = tempFloat * 10;
	
	temp2 = (long int) tempFloat % 10;
	
	retVal = retVal + (temp1 * 16) + temp2;
	
	retVal = retVal << 8;

	//get third byte of mantissa
	tempFloat = tempFloat * 10;
	
	temp1 = (long int) tempFloat % 10; 
	
	tempFloat = tempFloat * 10;
	
	temp2 = (long int) tempFloat % 10;
	
	retVal = retVal + (temp1 * 16) + temp2;
	
	retVal = retVal << 8;
	
	//temp1 = (~exponent)+ 1;
	unsignedbyte = exponent;
	
	//clear the leftmost 2 bits
	unsignedbyte = unsignedbyte << 2;
	unsignedbyte = unsignedbyte >> 2;
	
	//now we do the mantissa sign and the exponent sign
	if(sign == 1) //negative
	{
		unsignedbyte = unsignedbyte | 0x80;	  //make bit 7 a 1
	}
	else
	{
		unsignedbyte = unsignedbyte & 0x7F;	  //clear bit 7
	}
	
	if (exponent < 0)  //remember, it's backwards for exponent
	{
		unsignedbyte = unsignedbyte & 0xBF;   	//clear bit 6 
	}
	else
	{
		unsignedbyte = unsignedbyte | 0x40;	  //make bit 6 a 1
	}
	
	//add the sign and exponent byte
	*ReturnVal = retVal + unsignedbyte;

	return 0;
}
