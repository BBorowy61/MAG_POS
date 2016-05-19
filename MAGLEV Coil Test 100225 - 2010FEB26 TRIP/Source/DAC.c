#include "DAC.h"
#include "Command.h"


float DAC_PosGain;
float DAC_NegGain;
float DAC_PosOffset;
float DAC_NegOffset;

void ConfigDAC(unsigned int mode, t_RemoteConfig* RemoteConfig){
	if (mode){
		DAC_PosOffset = 0x8000L - UnitConfig.DAC_Offset;
		DAC_NegOffset = DAC_PosOffset;
		DAC_PosGain = 3276.8 * UnitConfig.DAC_PosGain;
		DAC_NegGain = 3276.8 * UnitConfig.DAC_NegGain;
	}else{
		DAC_PosGain = 32768.0 * RemoteConfig->DACPosGain * UnitConfig.DAC_PosGain / RemoteConfig->MaxOutput;
		DAC_NegGain = 32768.0 * RemoteConfig->DACNegGain * UnitConfig.DAC_NegGain / RemoteConfig->MaxOutput;
  
  		DAC_PosOffset = UnitConfig.DAC_Offset + RemoteConfig->DACOffset * UnitConfig.DAC_PosGain;
  		DAC_NegOffset = UnitConfig.DAC_Offset + RemoteConfig->DACOffset * UnitConfig.DAC_NegGain;
  
		DAC_PosOffset = 0x8000L - DAC_PosOffset;
		DAC_NegOffset = 0x8000L - DAC_NegOffset;
  
  		if( RemoteConfig->OutputUnit != RemoteConfig->CalUnit ) {
			DAC_PosGain = RemoteConfig->UnitFactor * DAC_PosGain;
			DAC_NegGain = RemoteConfig->UnitFactor * DAC_NegGain;

			if( RemoteConfig->OutputUnit == UNIT_MV ) {	// mv/v output
				//Remove intercept
				DAC_PosOffset -= RemoteConfig->CalOffset *DAC_PosGain;
				DAC_NegOffset -= RemoteConfig->CalOffset *DAC_NegGain;
		    } 
		}
	}
}



void InitDAC(void)
{
	long i;
	EALLOW;
	// Enable the GPIO pins for McBSP operation.
	GpioMuxRegs.GPFMUX.bit.MCLKXA_GPIOF8 = 1;
	GpioMuxRegs.GPFMUX.bit.MDXA_GPIOF12 = 1;

	GpioMuxRegs.GPAMUX.bit.TDIRA_GPIOA11 = 0;
	GpioMuxRegs.GPADIR.bit.GPIOA11 = 1;
	GpioMuxRegs.GPBMUX.all&=0x0FFF;
	GpioMuxRegs.GPBDIR.all|=0xF000;
	EDIS;

	//Disable all DAC Chip Selects
	GpioDataRegs.GPBSET.all=0xF000;
	GpioDataRegs.GPASET.bit.GPIOA11 = 1;


	//Enable FIFOs
    McbspaRegs.MFFTX.all=0x0000;
    McbspaRegs.MFFRX.all=0x001F;
    McbspaRegs.MFFCT.all=0x0;
    McbspaRegs.MFFINT.all=0x0;
    McbspaRegs.MFFST.all=0x0;
    McbspaRegs.MFFTX.bit.MFFENA=1;         // Enable FIFO
    McbspaRegs.MFFTX.bit.TXFIFO_RESET=1;   // Enable Transmit channel
    //McbspaRegs.MFFRX.bit.RXFIFO_RESET=1;   // Enable Receive channel



       // McBSP register settings in SPI master mode for Digital loop back
    McbspaRegs.SPCR2.all=0x0000;		// Reset FS generator, sample rate generator & transmitter
	McbspaRegs.SPCR1.all=0x0000;		// Reset Receiver, Right justify word, Digital loopback dis.
    McbspaRegs.PCR.all=0x0F08;          //(CLKXM=CLKRM=FSXM=FSRM= 1, FSXP = 1)
    McbspaRegs.SPCR1.bit.DLB = 0;
    McbspaRegs.SPCR1.bit.CLKSTP = 2;    // Together with CLKXP/CLKRP determines clocking scheme
	McbspaRegs.PCR.bit.CLKXP = 1;		// CPOL = 0, CPHA = 0 rising edge no delay
	McbspaRegs.PCR.bit.CLKRP = 0;
    McbspaRegs.RCR2.bit.RDATDLY=00;     // FSX setup time 1 in master mode. 0 for slave mode (Receive)
    McbspaRegs.XCR2.bit.XDATDLY=00;     // FSX setup time 1 in master mode. 0 for slave mode (Transmit)

	McbspaRegs.RCR1.bit.RWDLEN1=2;      // 16-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=2;      // 16-bit word

    McbspaRegs.SRGR2.all=0x2000; 	 	// CLKSM=1, FPER = 1 CLKG periods
    McbspaRegs.SRGR1.all= 0x000F;	    // Frame Width = 1 CLKG period, CLKGDV=16

    McbspaRegs.SPCR2.bit.GRST=1;        // Enable the sample rate generator
	for (i = 0; i < MCBSP_INIT_DELAY; i++) {}                       // Wait at least 2 SRG clock cycles
	McbspaRegs.SPCR2.bit.XRST=1;        // Release TX from Reset
	//McbspaRegs.SPCR1.bit.RRST=1;        // Release RX from Reset
    McbspaRegs.SPCR2.bit.FRST=1;        // Frame Sync Generator reset
}

void SetDACVolt(float voltage, unsigned int Channel){
	float f_dac;
	unsigned int i_dac;

	if (voltage>=0){
		f_dac = DAC_PosOffset;
		f_dac += voltage * DAC_PosGain;
	}else{
		f_dac = DAC_PosOffset;
		f_dac += voltage * DAC_NegGain;
	}
		
    if( f_dac < 0 ) {
      i_dac = 0;
    } else if( f_dac > 65535.0 ) {
      i_dac = 0xFFFF;
    } else {
      i_dac = (unsigned int)f_dac;
    }
    
    DAC_xmit(i_dac,Channel);

}

void DAC_xmit(unsigned int level, unsigned int Channel)
{
	long i;

	GpioDataRegs.GPBCLEAR.all= Channel;
    McbspaRegs.DXR1.all=level;

	for (i = 0; i < 20; i++) {}
	GpioDataRegs.GPBSET.all=Channel;

	for (i = 0; i < 2; i++) {}
	GpioDataRegs.GPACLEAR.bit.GPIOA11=1;
	for (i = 0; i < 2; i++) {}
	GpioDataRegs.GPASET.bit.GPIOA11=1;
}
