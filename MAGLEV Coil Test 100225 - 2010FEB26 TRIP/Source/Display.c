#include "Display.h"

unsigned char lcd_buf[16];
char DispBuf[32];
unsigned char lcd_write = 1;
unsigned char lcd_pos;

const char UnitNames[][4] = 
{ "mVv",			// 0
    " lb",			// 1
    " kg",			// 2
    " in",			// 3
    " cm",			// 4
    " mm",			// 5
    " % ",			// 6
    "psi",			// 7
    " Nm",			// 8
    "   ",			// 9
	" --"			// 10
};
void LCD_WriteConf(unsigned char data){
	LCD_REG = data;
	LCD_Enable();
	GpioDataRegs.GPFCLEAR.bit.GPIOF3 = 1; //LCD_RS
	asm(" NOP");
	asm(" NOP");
	LCD_Disable();

}

void LCD_WriteData(unsigned char data){
	LCD_REG = data>>4;
	LCD_Enable();
	GpioDataRegs.GPFSET.bit.GPIOF3 = 1; //LCD_RS
	asm(" NOP");
	asm(" NOP");
	LCD_Disable();
	
	LCD_REG = data;
	LCD_Enable();
	asm(" NOP");
	asm(" NOP");
	LCD_Disable();

}
void DispPutS(char* buf, int pos, int fill){
	int i;

	lcd_write = 1;
	if (fill== FILL_ALL){
		for (i=0;i<16;i++){
			lcd_buf[i] = ' ';
		}
	}
	for (i=pos;i<16;i++){
		if (buf[i-pos]==0) break;
		lcd_buf[i] = buf[i-pos];
		
	}
	lcd_write = 0;

}
			

void InitDisplay(void){

	char i;
  
	lcd_write = 1;				// waiting time increased to 30ms for KS0066
	DELAY_US(30000 );			// min 30ms after VCC rises to 4.5V

	GpioDataRegs.GPFCLEAR.bit.GPIOF3 = 1; //LCD_RS	// write control bytes

	DELAY_US(15000);// power on delay
	LCD_WriteConf(0x03);
	DELAY_US(5000);

	LCD_WriteConf(0x03);
	DELAY_US(100);

	LCD_WriteConf(0x03);
	DELAY_US(5000);

  
	// change to 4 bit bus
 	LCD_WriteConf(0x02);
	DELAY_US(80);
  
	LCD_WriteConf(LCD_CMD1>>4);
	LCD_WriteConf(LCD_CMD1);
	DELAY_US(39);				// 39us min
	LCD_WriteConf(LCD_CMD2>>4 );
	LCD_WriteConf(LCD_CMD2);
	DELAY_US(1530);				// 39us min
	LCD_WriteConf(CLR_HOME>>4 );
	LCD_WriteConf(CLR_HOME);
	DELAY_US(1530);				// 1.53ms min
	LCD_WriteConf(LCD_CMD3>>4);
	LCD_WriteConf(LCD_CMD3);
  
	for( i = 0; i < 16; i++ ) {
    	lcd_buf[i] = ' ';
	}

	lcd_write = 0;
	lcd_pos=0;
}

void LCD_Update(){
	if( !lcd_write ) {
    
    	if( lcd_pos&0x10) {	// first char, first line
			LCD_WriteConf(LCD_LINE1>>4);
			LCD_WriteConf(LCD_LINE1);
			lcd_pos &=0x0F;
	    } else if( lcd_pos&0x20) {	// first char, second line
			LCD_WriteConf(LCD_LINE2>>4);
			LCD_WriteConf(LCD_LINE2);
			lcd_pos &=0x0F;
    	}else{
    	
			LCD_WriteData( lcd_buf[lcd_pos++] );		// normal
			if (lcd_pos>15) lcd_pos = 0x10;
			if (lcd_pos==8) lcd_pos |=0x20;
	    }  
		
	}

}
