#include "Qep.h"
#include "system.h"
#include "Flash281x_API_Library.h"

void initQEP(){
	//Turn on Pins
	EALLOW;
 	GpioMuxRegs.GPAMUX.all|=0x0300;
 	GpioMuxRegs.GPBMUX.all|=0x0300;
 	EDIS;

 	EvaRegs.T2PR = 0xFFFF;
  	EvaRegs.T2CMPR = 0x3C00;

  	EvbRegs.T4PR = 0xFFFF;
  	EvbRegs.T4CMPR = 0x3C00;

	EvaRegs.EXTCONA.bit.QEPIE = 1;
	EvbRegs.EXTCONB.bit.QEPIE = 1;


	//Enable Counter1
	EvaRegs.T2CNT = 0;
	EvaRegs.T2CON.all = 0x9870;
	 
	//Enable Counter2 
	EvbRegs.T4CNT = 0;
	EvbRegs.T4CON.all = 0x9870;

  
}
