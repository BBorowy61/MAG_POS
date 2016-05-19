#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#include "Serial.h"
#include "Display.h"


#define FM_VERSION "VER: 1.00"
#define VERSION 1.00f
#define UNIQUETAG 0xA5AC

//300 ms Command Timeout
#define CMD_TIMEOUT 300 
// 3 Retries in Unit Communication
#define MAX_RETRIES 3

#define ADC_Reset() 	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
#define ADC_Convert()	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;



typedef struct{
	unsigned char FlashTag;
	unsigned char UnitAddress;
	unsigned char RemoteAddress;
	
	unsigned int CommMode;
	unsigned long SerialNumber;
	unsigned char Name[5];
	
	unsigned int DAC_Mode;
	float DAC_Voltage;
	int	DAC_Offset;
	float DAC_PosGain;
	float DAC_NegGain;
	
}t_UnitConfig;

typedef struct{
	unsigned int CurrentSensor;
	unsigned int ChannelNumber;
	unsigned int ShuntState;
	float DACPosGain;
	float DACNegGain;
	int DACOffset;
	float MaxOutput;
	unsigned int CalUnit;
	unsigned int OutputUnit;
	float UnitFactor;
	float Base;
	float Mvvf;
	float CalGain;
	float CalOffset;
	char Label[4];

}t_RemoteConfig;

enum{
	SERIAL_STATE,
	SYNC_STATE,
	SYNC_STOP_STREAMING,
	SYNC_CHECK_ADDRESS,
	SYNC_GET_LABEL,
	SYNC_GET_SHUNT_STATUS,
	SYNC_GET_BASE,
	SYNC_GET_CAL_UNIT,
	SYNC_GET_CH_NUM,
	SYNC_GET_MVVF,
	SYNC_GET_OUTPUT_UNIT,
	SYNC_GET_DAC_GAIN_POS,
	SYNC_GET_DAC_GAIN_NEG,
	SYNC_GET_DAC_OFFSET,
	SYNC_GET_OUTPUT_MAX,
	SYNC_GET_CAL_GAIN,
	SYNC_GET_CAL_INTERCEPT,
	SYNC_START_STREAMING
};


extern t_UnitConfig UnitConfig;

void RunLoop(void);
interrupt void SystemTimer(void);
long phase2freq(long phase, long oldphase);
void SlowFilter(void);
void Filter(void);
long cyclecounter(long phase, long ophase);
long observer(long command, long phi);
void positionMessage(long position);

extern Uint16 RamFuncs_loadstart;
extern Uint16 RamFuncs_loadend;
extern Uint16 RamFuncs_runstart;

extern Uint16 RamMath_loadstart;
extern Uint16 RamMath_loadend;
extern Uint16 RamMath_runstart;


extern unsigned int CommMode;

#endif
