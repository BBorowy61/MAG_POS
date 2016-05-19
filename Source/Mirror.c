//###########################################################################
//
// FILE:    Mirror.c
//
// TITLE:   Cell Mite Pro - Wireless Mirror
//   
//
// DESCRIPTION:
//
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 12 Jun 2008 | T.K. | Initial
//###########################################################################
#include "system.h"
#include "MainLoop.h"
#include "Flash281x_API_Library.h"

void main(void){

	const char welcome[] = "MAGLEV Position Determination System v1.00.03\r\n";

	// initialize the system control registers and XINTF
	SysInit();
  
	delay_us( 30000 );			// delay for 30 ms

	// Clear all interrupts and initialize PIE vector table:
	// Disable CPU interrupts 
	DINT;
	// Initialize PIE control registers to their default state.
  	// The default state is all PIE interrupts disabled and flags are cleared.
	// This function is found in the DSP281x_PieCtrl.c file.
	InitPieCtrl();
	// Disable CPU interrupts and clear all CPU interrupt flags:
	IER = 0x0000;
	IFR = 0x0000;
	// Initialize the PIE vector table with pointers to the shell Interrupt 
	// Service Routines (ISR).  
	// This will populate the entire table, even if the interrupt
	// is not used in this example.  This is useful for debug purposes.
	// The shell ISR routines are found in DSP281x_DefaultIsr.c.
	// This function is found in DSP281x_PieVect.c.
 	InitPieVectTable();

	GpioInit();
	InitDAC();
	InitAdc();
//	initQEP();
	delay_us( 30000 );			// delay for 30 ms

	GpioDataRegs.GPFCLEAR.bit.GPIOF11 = 1; //Enable CTS
	GpioDataRegs.GPACLEAR.bit.GPIOA7 = 1; //Enable DATA Mode
	
//	PortAInit(460800L);

	PortAInit(115200L);
//	PortBInit(115200L);

	  // Copy the Flash API functions to SARAM
	memcpy( &Flash28_API_RunStart,
	  &Flash28_API_LoadStart,
	  (&Flash28_API_LoadEnd - &Flash28_API_LoadStart)+1 );

	//Copy Ram Functions
	memcpy( &RamFuncs_runstart,
	  &RamFuncs_loadstart,
	  (&RamFuncs_loadend - &RamFuncs_loadstart)+1 );

	memcpy( &RamMath_runstart,
	  &RamMath_loadstart,
	  (&RamMath_loadend - &RamMath_loadstart)+1 );


	//Configure for 1ms count
	ConfigCpuTimer0(5, &SystemTimer);
	ConfigDAC(1, 0);
	AdcSetup();

	delay_us( 30000 );			// delay for 30 ms

	ADC_Reset();
	ADC_Convert();

//	initEVA();


#ifdef RS232DEBUG
	SendDataA(welcome, sizeof(welcome));
#endif

  	//Enable Interrupts
	IER |= M_INT1;
  	IER |= M_INT9;			// 0x0100;
	EINT;

	RunLoop();

	
}
