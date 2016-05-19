#include "system.h"
#include "Flash281x_API_Library.h"

#define StartCpuTimer0()  CpuTimer0Regs.TCR.bit.TSS = 0
#define StopCpuTimer0()   CpuTimer0Regs.TCR.bit.TSS = 1

unsigned int SysCtrl;


void initEVA(void) {

	// EVA Configure T1PWM, T2PWM, PWM1-PWM6 
	// Initalize the timers
	// Initalize EVA Timer1 
	//   EvaRegs.T1PR = 37500;       // Timer1 period
	EvaRegs.T1PR = 500;       // Timer1 period
	EvaRegs.T1CNT = 0x0000;      // Timer1 counter
	// TMODE = continuous up/down
	// Timer enable
	// Timer compare enable
	EvaRegs.T1CON.all = 0x1042;   


	// Setup T1PWM and T2PWM
	// Drive T1/T2 PWM by compare logic
	EvaRegs.GPTCONA.bit.TCMPOE = 1;
	// Polarity of GP Timer 1 Compare = Active low
	EvaRegs.GPTCONA.bit.T1PIN = 1;
	// Polarity of GP Timer 2 Compare = Active high
	//  EvaRegs.GPTCONA.bit.T2PIN = 2;

	// Enable compare for PWM1-PWM6
	//  EvaRegs.CMPR1 = 37500>>1;
	//  EvaRegs.CMPR2 = 37500>>2;
	//  EvaRegs.CMPR3 = 37500>>3;

	EvaRegs.CMPR1 = 5;
	EvaRegs.CMPR2 = 10;
	EvaRegs.CMPR3 = 15;

	// Compare action control.  Action that takes place
	// on a cmpare event
	// output pin 1 CMPR1 - active high
	// output pin 2 CMPR1 - active low
	// output pin 3 CMPR2 - active high
	// output pin 4 CMPR2 - active low
	// output pin 5 CMPR3 - active high
	// output pin 6 CMPR3 - active low
	EvaRegs.ACTRA.all = 0x0666;
	//  EvaRegs.DBTCONA.all = 0x0ffc;
	EvaRegs.DBTCONA.all = 0x0000;
	EvaRegs.COMCONA.all = 0xA600;

}

void initXINTF(void){

// All Zones---------------------------------
    // Timing for all zones based on XTIMCLK = SYSCLKOUT 
    XintfRegs.XINTCNF2.bit.XTIMCLK = 0;
    // Buffer up to 3 writes
    XintfRegs.XINTCNF2.bit.WRBUFF = 3;
    // XCLKOUT is enabled
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;
    // XCLKOUT = XTIMCLK / 2 
    XintfRegs.XINTCNF2.bit.CLKMODE = 1;
    
    
    // Zone 6------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING0.bit.XWRLEAD = 1;
    XintfRegs.XTIMING0.bit.XWRACTIVE = 3;
    XintfRegs.XTIMING0.bit.XWRTRAIL = 0;
    // Zone read timing
    XintfRegs.XTIMING0.bit.XRDLEAD = 1;
    XintfRegs.XTIMING0.bit.XRDACTIVE = 3;
    XintfRegs.XTIMING0.bit.XRDTRAIL = 0;
    
    // do not double all Zone read/write lead/active/trail timing 
    XintfRegs.XTIMING0.bit.X2TIMING = 0;
 
    // Zone will not sample READY 
    XintfRegs.XTIMING0.bit.USEREADY = 0;
    XintfRegs.XTIMING0.bit.READYMODE = 0;
    // Zone 6------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING2.bit.XWRLEAD = 2;
    XintfRegs.XTIMING2.bit.XWRACTIVE = 3;
    XintfRegs.XTIMING2.bit.XWRTRAIL = 0;
    // Zone read timing
    XintfRegs.XTIMING2.bit.XRDLEAD = 2;
    XintfRegs.XTIMING2.bit.XRDACTIVE = 3;
    XintfRegs.XTIMING2.bit.XRDTRAIL = 0;
    
    // do not double all Zone read/write lead/active/trail timing 
    XintfRegs.XTIMING2.bit.X2TIMING = 0;
 
    // Zone will not sample READY 
    XintfRegs.XTIMING2.bit.USEREADY = 0;
    XintfRegs.XTIMING2.bit.READYMODE = 0;  
 
    // Size must be 1,1 - other values are reserved
    XintfRegs.XTIMING6.bit.XSIZE = 3;

	XintfRegs.XTIMING6.bit.XWRLEAD = 2;
    XintfRegs.XTIMING6.bit.XWRACTIVE = 3;
    XintfRegs.XTIMING6.bit.XWRTRAIL = 0;
    // Zone read timing
    XintfRegs.XTIMING6.bit.XRDLEAD = 2;
    XintfRegs.XTIMING6.bit.XRDACTIVE = 3;
    XintfRegs.XTIMING6.bit.XRDTRAIL = 0;
    
    // do not double all Zone read/write lead/active/trail timing 
    XintfRegs.XTIMING6.bit.X2TIMING = 0;
 
    // Zone will not sample READY 
    XintfRegs.XTIMING6.bit.USEREADY = 0;
    XintfRegs.XTIMING6.bit.READYMODE = 0;  
 
    // Size must be 1,1 - other values are reserved
    XintfRegs.XTIMING6.bit.XSIZE = 3;

	//Force a pipeline flush to ensure that the write to 
   //the last register configured occurs before returning.  
   asm(" RPT #7 || NOP");
}

void SysInit(void){
	volatile Uint16 i;
    
    EALLOW;		//System Register Access
       	
   	//Initialize the PLL
   	SysCtrlRegs.PLLCR.bit.DIV = 0xA;

	//Disable the Watchdog Timer
	SysCtrlRegs.WDCR= 0x0068;
   
    EDIS;

	//Wait for PLL to lock
	for(i= 0; i< ( (131072/2)/12 ); i++)
    {
   
    }

	EALLOW;		//System Register Access

	// HISPCP/LOSPCP prescale register settings
   	SysCtrlRegs.HISPCP.all = 0x0001;
   	SysCtrlRegs.LOSPCP.all = 0x0002;
   	
	// Peripheral clock enables set for the selected peripherals.   
	SysCtrlRegs.PCLKCR.bit.EVAENCLK=1;
	SysCtrlRegs.PCLKCR.bit.EVBENCLK=1;
	SysCtrlRegs.PCLKCR.bit.SCIAENCLK=1;
	SysCtrlRegs.PCLKCR.bit.SCIBENCLK=1;
	SysCtrlRegs.PCLKCR.bit.MCBSPENCLK=1;
   	SysCtrlRegs.PCLKCR.bit.SPIENCLK=1;
   	SysCtrlRegs.PCLKCR.bit.ECANENCLK=1;
   	SysCtrlRegs.PCLKCR.bit.ADCENCLK=1;
   	
   	EDIS;

	//Init Flash
	Flash_CPUScaleFactor = SCALE_FACTOR;
	Flash_CallbackPtr = 0;

	initXINTF();

}

void ConfigCpuTimer0(float Period, void (*IntFunc)(void))
{
	Uint32 	temp;
	
	// Initialize timer period:	
	temp = (long) (150 * Period);
	CpuTimer0Regs.PRD.all  = temp;
	// Initialize pre-scale counter to divide by 1 (SYSCLKOUT):	
	CpuTimer0Regs.TPR.all  = 0;
	CpuTimer0Regs.TPRH.all = 0;
	// Make sure timer is stopped:
	CpuTimer0Regs.TCR.bit.TSS = 1; // 1 = Stop timer, 0 = Start/Restart Timer 
	// Reload all counter register with period value:
	CpuTimer0Regs.TCR.bit.TRB = 1; // 1 = reload timer
	CpuTimer0Regs.TCR.bit.SOFT = 1;
	CpuTimer0Regs.TCR.bit.FREE = 1; // Timer Free Run
	CpuTimer0Regs.TCR.bit.TIE = 1; // 0 = Disable/ 1 = Enable Timer Interrupt
	
	EALLOW;  // This is needed to write to EALLOW protected registers
    PieVectTable.TINT0 = IntFunc;
   EDIS;    // This is needed to disable write to EALLOW protected registers

	
	//Enable the interrupt in the PIE Table
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	    
	//Start the System Timer
//	StartCpuTimer0();
	
}

//---------------------------------------------------------------------------
// InitPieCtrl: 
//---------------------------------------------------------------------------
// This function initializes the PIE control registers to a known state.
//
void InitPieCtrl(void)
{
    // Disable Interrupts at the CPU level:
    DINT;

    // Disable the PIE
    PieCtrlRegs.PIECRTL.bit.ENPIE = 0;

	// Clear all PIEIER registers:
	PieCtrlRegs.PIEIER1.all = 0;
	PieCtrlRegs.PIEIER2.all = 0;
	PieCtrlRegs.PIEIER3.all = 0;	
	PieCtrlRegs.PIEIER4.all = 0;
	PieCtrlRegs.PIEIER5.all = 0;
	PieCtrlRegs.PIEIER6.all = 0;
	PieCtrlRegs.PIEIER7.all = 0;
	PieCtrlRegs.PIEIER8.all = 0;
	PieCtrlRegs.PIEIER9.all = 0;
	PieCtrlRegs.PIEIER10.all = 0;
	PieCtrlRegs.PIEIER11.all = 0;
	PieCtrlRegs.PIEIER12.all = 0;

	// Clear all PIEIFR registers:
	PieCtrlRegs.PIEIFR1.all = 0;
	PieCtrlRegs.PIEIFR2.all = 0;
	PieCtrlRegs.PIEIFR3.all = 0;	
	PieCtrlRegs.PIEIFR4.all = 0;
	PieCtrlRegs.PIEIFR5.all = 0;
	PieCtrlRegs.PIEIFR6.all = 0;
	PieCtrlRegs.PIEIFR7.all = 0;
	PieCtrlRegs.PIEIFR8.all = 0;
	PieCtrlRegs.PIEIFR9.all = 0;
	PieCtrlRegs.PIEIFR10.all = 0;
	PieCtrlRegs.PIEIFR11.all = 0;
	PieCtrlRegs.PIEIFR12.all = 0;


}	

//---------------------------------------------------------------------------
// EnableInterrupts: 
//---------------------------------------------------------------------------
// This function enables the PIE module and CPU interrupts
//
void EnableInterrupts()
{

    // Enable the PIE
    PieCtrlRegs.PIECRTL.bit.ENPIE = 1;
    		
	// Enables PIE to drive a pulse into the CPU 
	PieCtrlRegs.PIEACK.all = 0xFFFF;  

	// Enable Interrupts at the CPU level 
    EINT;

}
/**********************************************************************
 * Function: gpio_init()
 *
 * Description: Initializes the shared GPIO pins on the F281x.
 *
 * MUX bits:	0: select GPIO function
 *		1: select periph function
 * DIR bits:	0: input
 *		1: output
 **********************************************************************/
void GpioInit()
{
  EALLOW;					// Enable EALLOW protected register access
  
  /*** Group A pins ***/
  GpioMuxRegs.GPAQUAL.all=0x0000;		// Input qualifier disabled

  GpioMuxRegs.GPAMUX.bit.C3TRIP_GPIOA15   = 0;	// GREEN1 (out)
  GpioMuxRegs.GPADIR.bit.GPIOA15	= 1;
  GpioMuxRegs.GPAMUX.bit.C2TRIP_GPIOA14   = 0;	// RED1 (out)
  GpioMuxRegs.GPADIR.bit.GPIOA14	= 1;
  GpioMuxRegs.GPAMUX.bit.C1TRIP_GPIOA13   = 0;	// TAG2_RX (out)
  GpioMuxRegs.GPADIR.bit.GPIOA13	= 1;
  GpioMuxRegs.GPAMUX.bit.TCLKINA_GPIOA12  = 0;	// TAG1_RX (out)
  GpioMuxRegs.GPADIR.bit.GPIOA12	= 1;
  GpioMuxRegs.GPAMUX.bit.TDIRA_GPIOA11    = 0;	// LDAC (out)
  GpioMuxRegs.GPADIR.bit.GPIOA11	= 1;
  GpioMuxRegs.GPAMUX.bit.CAP3QI1_GPIOA10  = 1;	// BCHIX (p)
  GpioMuxRegs.GPADIR.bit.GPIOA10	= 0;
  GpioMuxRegs.GPAMUX.bit.CAP2Q2_GPIOA9    = 1;	// BCHBX (p)
  GpioMuxRegs.GPADIR.bit.GPIOA9		= 0;
  GpioMuxRegs.GPAMUX.bit.CAP1Q1_GPIOA8    = 1;	// BCHAX (p)
  GpioMuxRegs.GPADIR.bit.GPIOA8		= 0;
  GpioMuxRegs.GPAMUX.bit.T2PWM_GPIOA7     = 0;	// Cmd/Mon (USB) (out)
  GpioMuxRegs.GPADIR.bit.GPIOA7		= 1;
  GpioMuxRegs.GPAMUX.bit.T1PWM_GPIOA6     = 0;	// SYNC_OUT (out)
  GpioMuxRegs.GPADIR.bit.GPIOA6		= 1;
  GpioMuxRegs.GPAMUX.bit.PWM6_GPIOA5      = 1;	// PWM6 (p)
  GpioMuxRegs.GPADIR.bit.GPIOA5		= 0;
  GpioMuxRegs.GPAMUX.bit.PWM5_GPIOA4      = 1;	// PWM5 (p)
  GpioMuxRegs.GPADIR.bit.GPIOA4		= 0;
  GpioMuxRegs.GPAMUX.bit.PWM4_GPIOA3      = 1;	// PWM4 (p)
  GpioMuxRegs.GPADIR.bit.GPIOA3		= 0;
  GpioMuxRegs.GPAMUX.bit.PWM3_GPIOA2      = 0;	// PWM3 (gpio - out)
  GpioMuxRegs.GPADIR.bit.GPIOA2		= 1;
//  GpioMuxRegs.GPAMUX.bit.PWM2_GPIOA1      = 0;	// PWM2 (gpio - out)
  GpioMuxRegs.GPAMUX.bit.PWM2_GPIOA1      = 1;	// PWM2 (pwm - out)
//  GpioMuxRegs.GPADIR.bit.GPIOA1		= 1;
  GpioMuxRegs.GPAMUX.bit.PWM1_GPIOA0      = 0;	// PWM1 (gpio - out)
//  GpioMuxRegs.GPAMUX.bit.PWM1_GPIOA0      = 1;	// PWM1 (pwm - out)
  GpioMuxRegs.GPADIR.bit.GPIOA0		= 1;

  
  /*** Group B pins ***/
  GpioMuxRegs.GPBQUAL.all = 0x0000;		// Input qualifier disabled
  
  GpioMuxRegs.GPBMUX.bit.C6TRIP_GPIOB15   = 0;	// DAC_CS4 (out)
  GpioMuxRegs.GPBDIR.bit.GPIOB15	= 1;
  GpioMuxRegs.GPBMUX.bit.C5TRIP_GPIOB14   = 0;	// DAC_CS3 (out)
  GpioMuxRegs.GPBDIR.bit.GPIOB14	= 1;
  GpioMuxRegs.GPBMUX.bit.C4TRIP_GPIOB13   = 0;	// DAC_CS2 (out)
  GpioMuxRegs.GPBDIR.bit.GPIOB13	= 1;
  GpioMuxRegs.GPBMUX.bit.TCLKINB_GPIOB12  = 0;	// DAC_CS1 (out)
  GpioMuxRegs.GPBDIR.bit.GPIOB12	= 1;
  GpioMuxRegs.GPBMUX.bit.TDIRB_GPIOB11    = 0;	// LCD_RW (out)
  GpioMuxRegs.GPBDIR.bit.GPIOB11	= 1;
  GpioMuxRegs.GPBMUX.bit.CAP6QI2_GPIOB10  = 1;	// BCHIY (p)
  GpioMuxRegs.GPBDIR.bit.GPIOB10	= 0;
  GpioMuxRegs.GPBMUX.bit.CAP5Q2_GPIOB9    = 1;	// BCHBY (p)
  GpioMuxRegs.GPBDIR.bit.GPIOB9		= 0;
  GpioMuxRegs.GPBMUX.bit.CAP4Q1_GPIOB8    = 1;	// BCHAY (p)
  GpioMuxRegs.GPBDIR.bit.GPIOB8		= 0;
  GpioMuxRegs.GPBMUX.bit.T4PWM_GPIOB7     = 0;	// A_SYNC (out)
  GpioMuxRegs.GPBDIR.bit.GPIOB7		= 1;
  GpioMuxRegs.GPBMUX.bit.T3PWM_GPIOB6     = 0;	// DIG_SYNC (out)
  GpioMuxRegs.GPBDIR.bit.GPIOB6		= 1;
  GpioMuxRegs.GPBMUX.bit.PWM12_GPIOB5     = 0;	// A0CS (out)
  GpioMuxRegs.GPBDIR.bit.GPIOB5		= 1;
  GpioMuxRegs.GPBMUX.bit.PWM11_GPIOB4     = 0;	// A1CS (out)
  GpioMuxRegs.GPBDIR.bit.GPIOB4		= 1;
  GpioMuxRegs.GPBMUX.bit.PWM10_GPIOB3     = 0;	// TAG_TX (out)
  GpioMuxRegs.GPBDIR.bit.GPIOB3		= 1;
  GpioMuxRegs.GPBMUX.bit.PWM9_GPIOB2      = 1;	// PWM9 (p)
  GpioMuxRegs.GPBDIR.bit.GPIOB2		= 0;
  GpioMuxRegs.GPBMUX.bit.PWM8_GPIOB1      = 1;	// PWM8 (p)
  GpioMuxRegs.GPBDIR.bit.GPIOB1		= 0;
  GpioMuxRegs.GPBMUX.bit.PWM7_GPIOB0      = 1;	// PWM7 (p)
  GpioMuxRegs.GPBDIR.bit.GPIOB0		= 0;
  
  /*** Group D pins ***/
  GpioMuxRegs.GPDQUAL.all=0x0000;		// Input qualifier disabled
  
  GpioMuxRegs.GPDMUX.bit.T4CTRIP_SOCB_GPIOD6 = 0;	// AREADY1 (floating - out)
  GpioMuxRegs.GPDDIR.bit.GPIOD6		= 1;
  GpioMuxRegs.GPDMUX.bit.T3CTRIP_PDPB_GPIOD5 = 0;	// AREADY0 (floating - out)
  GpioMuxRegs.GPDDIR.bit.GPIOD5		= 1;
  GpioMuxRegs.GPDMUX.bit.T2CTRIP_SOCA_GPIOD1 = 0;	// TAG4_RX (out)
  GpioMuxRegs.GPDDIR.bit.GPIOD1		= 1;
  GpioMuxRegs.GPDMUX.bit.T1CTRIP_PDPA_GPIOD0 = 0;	// TAG3_RX (out)
  GpioMuxRegs.GPDDIR.bit.GPIOD0		= 1;
  
  /*** Group E pins ***/
  GpioMuxRegs.GPEQUAL.all=0x0000;		// Input qualifier disabled
  
  GpioMuxRegs.GPEMUX.bit.XNMI_XINT13_GPIOE2  = 1;	// LOW_POWER (p)
  GpioMuxRegs.GPEDIR.bit.GPIOE2		= 0;
  GpioMuxRegs.GPEMUX.bit.XINT2_ADCSOC_GPIOE1 = 0;	// B_SYNC_IN (in)
  GpioMuxRegs.GPEDIR.bit.GPIOE1		= 0;
  GpioMuxRegs.GPEMUX.bit.XINT1_XBIO_GPIOE0   = 0;	// EXT_INT (in)
  GpioMuxRegs.GPEDIR.bit.GPIOE0		= 0;
  
  /*** Group F pins ***/
  GpioMuxRegs.GPFMUX.bit.XF_GPIOF14      = 0;	// GREEN2 (out)
  GpioMuxRegs.GPFDIR.bit.GPIOF14	= 1;
  GpioMuxRegs.GPFMUX.bit.MDRA_GPIOF13    = 0;	// RED2 (out)
  GpioMuxRegs.GPFDIR.bit.GPIOF13	= 1;
  GpioMuxRegs.GPFMUX.bit.MDXA_GPIOF12    = 1;	// DDATA (p)
  GpioMuxRegs.GPFDIR.bit.GPIOF12	= 0;
  GpioMuxRegs.GPFMUX.bit.MFSRA_GPIOF11   = 0;	// CTS (out)
  GpioMuxRegs.GPFDIR.bit.GPIOF11	= 1;
  GpioMuxRegs.GPFMUX.bit.MFSXA_GPIOF10   = 0;	// B_RTS (input)
  GpioMuxRegs.GPFDIR.bit.GPIOF10	= 0;
  GpioMuxRegs.GPFMUX.bit.MCLKRA_GPIOF9   = 1;	// TEMP (p)
  GpioMuxRegs.GPFDIR.bit.GPIOF9		= 0;
  GpioMuxRegs.GPFMUX.bit.MCLKXA_GPIOF8   = 1;	// DCLK (p)
  GpioMuxRegs.GPFDIR.bit.GPIOF8		= 0;
  GpioMuxRegs.GPFMUX.bit.CANRXA_GPIOF7   = 1;	// CAN_RX (eCAN) (p)
  GpioMuxRegs.GPFDIR.bit.GPIOF7		= 0;
  GpioMuxRegs.GPFMUX.bit.CANTXA_GPIOF6   = 1;	// CAN_TX (eCAN) (p)
  GpioMuxRegs.GPFDIR.bit.GPIOF6		= 0;
  GpioMuxRegs.GPFMUX.bit.SCIRXDA_GPIOF5  = 1;	// RXD0 (SCIA) (p)
  GpioMuxRegs.GPFDIR.bit.GPIOF5		= 0;
  GpioMuxRegs.GPFMUX.bit.SCITXDA_GPIOF4  = 1;	// TXD0 (SCIA) (p)
  GpioMuxRegs.GPFDIR.bit.GPIOF4		= 0;
  GpioMuxRegs.GPFMUX.bit.SPISTEA_GPIOF3  = 0;	// LCD_RS (out)
  GpioMuxRegs.GPFDIR.bit.GPIOF3		= 1;
  GpioMuxRegs.GPFMUX.bit.SPICLKA_GPIOF2  = 1;	// ACLK (SPI) (p)
  GpioMuxRegs.GPFDIR.bit.GPIOF2		= 0;
  GpioMuxRegs.GPFMUX.bit.SPISOMIA_GPIOF1 = 1;	// ADIN (SPI) (p)
  GpioMuxRegs.GPFDIR.bit.GPIOF1		= 0;
  GpioMuxRegs.GPFMUX.bit.SPISIMOA_GPIOF0 = 1;	// ADOUT (SPI) (p)
  GpioMuxRegs.GPFDIR.bit.GPIOF0		= 0;
  
  
  /*** Group G pins ***/
  GpioMuxRegs.GPGMUX.bit.SCIRXDB_GPIOG5  = 1;	// RXD1 (SCIB) (p)
  GpioMuxRegs.GPGDIR.bit.GPIOG5		= 0;
  GpioMuxRegs.GPGMUX.bit.SCITXDB_GPIOG4  = 1;	// TXD1 (SCIB) (p)
  GpioMuxRegs.GPGDIR.bit.GPIOG4		= 0;
  
  EDIS;						// Disable EALLOW protected register access

	// Turn off LEDs
	GpioDataRegs.GPASET.bit.GPIOA14 = 1;
	GpioDataRegs.GPASET.bit.GPIOA15 = 1;
	GpioDataRegs.GPFSET.bit.GPIOF14 = 1;
	GpioDataRegs.GPFSET.bit.GPIOF13 = 1;

	SysCtrl = 0;
}

//----------------------------------------
#ifdef __cplusplus 
#pragma DATA_SECTION("PieCtrlRegsFile") 
#else
#pragma DATA_SECTION(PieCtrlRegs,"PieCtrlRegsFile");
#endif
volatile struct PIE_CTRL_REGS PieCtrlRegs;
//----------------------------------------
#ifdef __cplusplus 
#pragma DATA_SECTION("McbspaRegsFile") 
#else 
#pragma DATA_SECTION(McbspaRegs,"McbspaRegsFile");
#endif
volatile struct MCBSP_REGS McbspaRegs;

//----------------------------------------
#ifdef __cplusplus 
#pragma DATA_SECTION("SysCtrlRegsFile") 
#else
#pragma DATA_SECTION(SysCtrlRegs,"SysCtrlRegsFile");
#endif
volatile struct SYS_CTRL_REGS SysCtrlRegs;
//----------------------------------------
#ifdef __cplusplus 
#pragma DATA_SECTION("XintfRegsFile") 
#else
#pragma DATA_SECTION(XintfRegs,"XintfRegsFile");
#endif
volatile struct XINTF_REGS XintfRegs;

//----------------------------------------
#ifdef __cplusplus 
#pragma DATA_SECTION("GpioDataRegsFile") 
#else 
#pragma DATA_SECTION(GpioDataRegs,"GpioDataRegsFile");
#endif
volatile struct GPIO_DATA_REGS GpioDataRegs;

//----------------------------------------
#ifdef __cplusplus 
#pragma DATA_SECTION("GpioMuxRegsFile") 
#else 
#pragma DATA_SECTION(GpioMuxRegs,"GpioMuxRegsFile");
#endif
volatile struct GPIO_MUX_REGS GpioMuxRegs;

//----------------------------------------
#ifdef __cplusplus 
#pragma DATA_SECTION("PieVectTableFile") 
#else
#pragma DATA_SECTION(PieVectTable,"PieVectTableFile");
#endif
struct PIE_VECT_TABLE PieVectTable;

//----------------------------------------
#ifdef __cplusplus 
#pragma DATA_SECTION("SciaRegsFile") 
#else
#pragma DATA_SECTION(SciaRegs,"SciaRegsFile");
#endif
volatile struct SCI_REGS SciaRegs;

//----------------------------------------
#ifdef __cplusplus 
#pragma DATA_SECTION("ScibRegsFile") 
#else
#pragma DATA_SECTION(ScibRegs,"ScibRegsFile");
#endif
volatile struct SCI_REGS ScibRegs;

#ifdef __cplusplus 
#pragma DATA_SECTION("CpuTimer0RegsFile") 
#else 
#pragma DATA_SECTION(CpuTimer0Regs,"CpuTimer0RegsFile");
#endif
volatile struct CPUTIMER_REGS CpuTimer0Regs;

#ifdef __cplusplus 
#pragma DATA_SECTION("AdcRegsFile") 
#else 
#pragma DATA_SECTION(AdcRegs,"AdcRegsFile");
#endif 
volatile struct ADC_REGS AdcRegs;

#ifdef __cplusplus 
#pragma DATA_SECTION("EvaRegsFile") 
#else 
#pragma DATA_SECTION(EvaRegs,"EvaRegsFile");
#endif 
volatile struct EVA_REGS EvaRegs;

#ifdef __cplusplus 
#pragma DATA_SECTION("EvbRegsFile") 
#else 
#pragma DATA_SECTION(EvbRegs,"EvbRegsFile");
#endif 
volatile struct EVB_REGS EvbRegs;