#include "ADC.h"

#include "System.h"     // DSP281x Headerfile Include File
#include "DSP281x_Adc.h"

#define ADC_usDELAY  8000L
#define ADC_usDELAY2 20L

//#define CPU_RATE    6.667L   // for a 150MHz CPU clock speed (SYSCLKOUT)
//#define DELAY_US(A)  DSP28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)


#define ADC_MODCLK 0x3   // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)     = 25MHz
#define ADC_CKPS   0x0   // ADC module clock = HSPCLK/1      = 25MHz/(1)     = 25MHz
//#define ADC_SHCLK  0x1   // S/H width in ADC module periods                  = 2 ADC cycle
#define ADC_SHCLK  0x2   // S/H width in ADC module periods                  = 2 ADC cycle

//---------------------------------------------------------------------------
// InitAdc: 
//---------------------------------------------------------------------------
// This function initializes ADC to a known state.
//

void InitAdc(void) {
	extern void DSP28x_usDelay(Uint32 Count);
	
    // To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap and reference circuitry.
    // After a 5ms delay the rest of the ADC can be powered up. After ADC
    // powerup, another 20us delay is required before performing the first
    // ADC conversion. Please note that for the delay function below to
    // operate correctly the CPU_CLOCK_SPEED define statement in the
    // DSP28_Examples.h file must contain the correct CPU clock period in
    // nanoseconds. For example:

	AdcRegs.ADCTRL3.bit.ADCBGRFDN = 0x3;	// Power up bandgap/reference circuitry
//	DELAY_US(ADC_usDELAY);                  // Delay before powering up rest of ADC
	delay_us(50);
	AdcRegs.ADCTRL3.bit.ADCPWDN = 1;		// Power up rest of ADC
	delay_us(50);
//	DELAY_US(ADC_usDELAY2);                 // Delay after powering up ADC
}

void AdcSetup(void) {
	EALLOW;
	SysCtrlRegs.HISPCP.all = ADC_MODCLK;	// HSPCLK = SYSCLKOUT/ADC_MODCLK
	EDIS;

	// Sequential mode: Sample rate   = 1/[(2+ACQ_PS)*ADC clock in ns] = 1/(3*40ns) =8.3MHz
	AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;
	AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;     
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;        // 1  Cascaded mode
	AdcRegs.ADCMAXCONV.all = 0xC;

   AdcRegs.ADCCHSELSEQ1.all = 0x2103;
   AdcRegs.ADCCHSELSEQ2.all = 0x0210;
   AdcRegs.ADCCHSELSEQ3.all = 0x1021;
   AdcRegs.ADCCHSELSEQ4.all = 0x0002;


//   AdcRegs.ADCCHSELSEQ1.all = 0x0210;
//   AdcRegs.ADCCHSELSEQ2.all = 0x1021;
//   AdcRegs.ADCCHSELSEQ3.all = 0x2102;
//   AdcRegs.ADCCHSELSEQ4.all = 0x0000;

   AdcRegs.ADCTRL1.bit.CONT_RUN = 0;

}

int ATOD(int a) {
int convertv;

		AdcRegs.ADCTRL1.bit.ACQ_PS=0xF;
		AdcRegs.ADCTRL3.bit.ADCCLKPS=0x6;
		AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;
		AdcRegs.ADCCHSELSEQ1.bit.CONV00 = a;
		AdcRegs.ADCTRL2.all=0x2000;//Start Convert

		//Wait for end of Convert
		while(AdcRegs.ADCST.bit.INT_SEQ1==0){}
		asm(" RPT #11 || NOP");
		AdcRegs.ADCST.bit.INT_SEQ1_CLR=1;
		convertv = (unsigned int)AdcRegs.ADCRESULT0>>4;
		AdcRegs.ADCTRL2.bit.RST_SEQ1=1;
		
		return  convertv;  
	
}
