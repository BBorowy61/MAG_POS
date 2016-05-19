#ifndef DISPLAY_H
#define DISPLAY_H

#include "System.h"


#define LCD_REG		(*(unsigned int*)0x2006) 
#define LCD_Enable()	(*(unsigned int*)0x2009) = SysCtrl |=0x04; 
#define LCD_Disable()	(*(unsigned int*)0x2009) = SysCtrl &=0xFB; 
extern char DispBuf[];

extern const char UnitNames[][4];

void LCD_WriteConf(unsigned char data);
void LCD_WriteData(unsigned char data);
void LCD_Update();
void DispPutS(char* buf, int pos, int fill);

#define LCD_FREEZE 1000 //1s


#define LCD_CMD1	0x28	// 4 bits, 2 lines, 5x7 matrix
#define LCD_CMD2	0x0C	// display on, cursor and blink off
#define LCD_CMD3	0x06	// increment cursor, no display shift

#define CLR_HOME	0x01	// clear and home command
#define RET_HOME	0x03	// return to home without clear

// line starts for each line on a 16(8x2) LCD
#define LCD_LINE1	0x80
#define LCD_LINE2	0xC0

#define FILL_ALL 1
#define NO_FILL 0

#endif
