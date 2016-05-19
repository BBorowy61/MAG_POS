#ifndef SERIAL_H
#define SERIAL_H

//Serial Buffer Size must be power of 2
#define SERIAL_BUF_SIZE		64
#define BUF_OVERFLOW		63

typedef struct{
	unsigned char StartByte;
	unsigned char Address;
	unsigned char Command;
	unsigned char Index;
	unsigned char Data[4];
}t_packet;

#define START_BYTE 0x000A
#define BROADCAST 0xFF
#define PACKET_SIZE 8

#define WIRELESS_CTL 	*(unsigned int*)0x2005
#define WIRELESS_ON() 	WIRELESS_CTL = 0x0004
#define WIRELESS_OFF() 	WIRELESS_CTL = 0x000C

extern volatile unsigned char numPacketsA;
extern volatile unsigned char numPacketsB;

void PortAInit(long baud);
void PortBInit(long baud);

inline InitPacket(t_packet* packet){
	unsigned int i;
	unsigned int* ptr = (unsigned int*)packet;
	packet->StartByte = START_BYTE;
	for (i=1;i<PACKET_SIZE;i++)
		ptr[i] = 0;
}
interrupt void SciATxFifoIsr(void);
interrupt void SciARxFifoIsr(void);
interrupt void SciBTxFifoIsr(void);
interrupt void SciBRxFifoIsr(void);
interrupt void SciARxUSB(void);

void Hex2ASCII(unsigned long hex, unsigned int size, unsigned char* str);
void PutAWord(unsigned long uw);
unsigned int GetASCI(void);


#endif
