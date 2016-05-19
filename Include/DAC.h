#ifndef DAC_H
#define DAC_H

#include "system.h"
#include "MainLoop.h"

#define MCBSP_SRG_FREQ   150E6/4  	// SRG input is default LSPCLK for this example. (See note 1 at end)
#define MCBSP_SRG_PERIOD 1/MCBSP_SRG_FREQ
#define MCBSP_INIT_DELAY (2 * MCBSP_SRG_PERIOD)/150E6

void ConfigDAC(unsigned int mode, t_RemoteConfig* RemoteConfig);
void InitDAC(void);
void DAC_xmit(unsigned int level, unsigned int Channel);
void SetDACVolt(float voltage, unsigned int Channel);

#define DAC_CH1 	0x1000
#define DAC_CH2		0x2000
#define DAC_CH3		0x4000
#define DAC_CH4		0x8000
#define DAC_ALL		0xF000

#endif
