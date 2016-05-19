#include "SERIAL.h"
#include "system.h"

unsigned char BufferARX[SERIAL_BUF_SIZE];
unsigned char BufferATX[SERIAL_BUF_SIZE];

unsigned char BufferBRX[SERIAL_BUF_SIZE];
unsigned char BufferBTX[SERIAL_BUF_SIZE];

unsigned char HeadBufATX;
unsigned char TailBufATX;
unsigned char SizeBufATX;

unsigned char HeadBufARX;
unsigned char TailBufARX;
unsigned char SizeBufARX;
unsigned char NumBytesNeededA;

unsigned char HeadBufBTX;
unsigned char TailBufBTX;
unsigned char SizeBufBTX;

unsigned char HeadBufBRX;
unsigned char TailBufBRX;
unsigned char SizeBufBRX;
unsigned char NumBytesNeededB;

volatile unsigned char numPacketsA;
volatile unsigned char numPacketsB;

int GetDataA(unsigned char* buf, unsigned int length){
	Uint16 i;

	// Verify there is enough data in the buffer
	if (SizeBufARX<length)
		length = SizeBufARX;

	for (i=0; i < length; i++){
		//Copy the data
		buf[i] = BufferARX[HeadBufARX++];

		// Wrap the Tail to make circular buffer
		// Buffer is power of 2 so simple ANDing works
		HeadBufARX &=BUF_OVERFLOW;
		//Reduce buffer used
		SizeBufARX--;
	}


	return length;
}

int SendDataA(unsigned char* buf, unsigned int length){

	unsigned int i;

	//Wait Until There is Room on the Buffer
	i = SizeBufATX + length;
	while ( i >= SERIAL_BUF_SIZE){
		// Enable interrupt to clear the buffer
		SciaRegs.SCIFFTX.bit.TXFFIENA = 1;		
		//Delay 1 us to give the buffer time to clear
		delay_us(1);
		//Wait Until There is Room on the Buffer
		i = SizeBufATX + length;	
  	}

	// Place the data on the tx buffer	
	for (i= 0; i < length; i++){
		//Put the data at the end of the buffer
		BufferATX[TailBufATX++] = buf[i];
		
		// Wrap the Tail to make circular buffer
		// Buffer is power of 2 so simple ANDing works
		TailBufATX &=BUF_OVERFLOW;

	}

	// Enable FIFO interrupt
	SciaRegs.SCIFFTX.bit.TXFFIENA = 1;

	// Update the Buffer Size
	SizeBufATX += length;

	return length;

}
int GetDataB(unsigned char* buf, unsigned int length){
	Uint16 i;

	// Verify there is enough data in the buffer
	if (SizeBufBRX<length)
		length = SizeBufBRX;

	for (i=0; i < length; i++){
		//Copy the data
		buf[i] = BufferBRX[HeadBufBRX++];

		// Wrap the Tail to make circular buffer
		// Buffer is power of 2 so simple ANDing works
		HeadBufBRX &=BUF_OVERFLOW;
		//Reduce buffer used
		SizeBufBRX--;
	}


	return length;
}

int SendDataB(unsigned char* buf, unsigned int length){

	unsigned int i;

	//Wait Until There is Room on the Buffer
	i = SizeBufBTX + length;
	while ( i >= SERIAL_BUF_SIZE){
		// Enable interrupt to clear the buffer
		ScibRegs.SCIFFTX.bit.TXFFIENA = 1;		
		//Delay 1 us to give the buffer time to clear
		delay_us(1);
		
		//Wait Until There is Room on the Buffer
		i = SizeBufBTX + length;	
  	}

	// Place the data on the tx buffer	
	for (i= 0; i < length; i++){
		//Put the data at the end of the buffer
		BufferBTX[TailBufBTX++] = buf[i];
		
		// Wrap the Tail to make circular buffer
		// Buffer is power of 2 so simple ANDing works
		TailBufBTX &=BUF_OVERFLOW;

	}

	// Enable FIFO interrupt
	ScibRegs.SCIFFTX.bit.TXFFIENA = 1;

	// Update the Buffer Size
	SizeBufBTX += length;

	return length;

}

void PortAInit(long baud){
	Uint16 brr;

	//Configure Transmission Buffer
	HeadBufATX = 0;
	TailBufATX = 0;
	SizeBufATX = 0;

	//Configure Receive Buffer
	HeadBufARX = 0;
	TailBufARX = 0;
	SizeBufARX = 0;
	NumBytesNeededA = 8;
	numPacketsA = 0;
  

#ifdef RS232DEBUG
	EALLOW;
	PieVectTable.RXAINT = &SciARxFifoIsr;	// set the interrupt vect
	PieVectTable.TXAINT = &SciATxFifoIsr;
	EDIS;
#endif
  
	SciaRegs.SCICCR.all = 0x0007;		// 1 stop bit,  No loopback 
					// No parity,8 char bits,
					// async mode, idle-line protocol
	SciaRegs.SCICTL1.all = 0x0043;	// enable ERR, TX, RX, internal SCICLK, 
					// Disable SLEEP, TXWAKE
	SciaRegs.SCICTL2.all = 0x0003;
  
  	// baudDiv(BRR) = (LSPCLK(37.5M)/(baud*8)) - 1
  	brr = (37500000 / (baud * 8)) - 1;	// calculate the baudDiv

  	SciaRegs.SCIHBAUD = 0xff & (brr>>8);	// set for baud rate
  	SciaRegs.SCILBAUD = 0xff & brr;
  
  	SciaRegs.SCIFFTX.all = 0xC000;	// enable tx FIFO, trigger 0 disable int here
  	SciaRegs.SCIFFRX.all = 0x0028;	// enable rx FIFO, trigger 1
	SciaRegs.SCIFFCT.all = 0x00;
    
	SciaRegs.SCICTL1.all = 0x0063;	// Relinquish SCI from Reset
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;

#ifdef RS232DEBUG
	PieCtrlRegs.PIECRTL.bit.ENPIE = 1;	// Enable the PIE block
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;	// PIE Group 9, INT1, RX
	PieCtrlRegs.PIEIER9.bit.INTx2 = 1;	// PIE Group 9, INT2, TX
#endif

}

void PortBInit(long baud){
	Uint16 brr;
  	
	//Configure Transmission Buffer
	HeadBufBTX = 0;
	TailBufBTX = 0;
	SizeBufBTX = 0;

	//Configure Receive Buffer
	HeadBufBRX = 0;
	TailBufBRX = 0;
	SizeBufBRX = 0;
	NumBytesNeededB = 8;
	numPacketsB = 0;

	EALLOW;
	PieVectTable.RXBINT = &SciBRxFifoIsr;	// set the interrupt vect
	PieVectTable.TXBINT = &SciBTxFifoIsr;
	EDIS;
  	
	//Turn on Wireless
	WIRELESS_ON();

  
	ScibRegs.SCICCR.all = 0x0007;		// 1 stop bit,  No loopback 
					// No parity,8 char bits,
					// async mode, idle-line protocol
	ScibRegs.SCICTL1.all = 0x0003;	// enable TX, RX, internal SCICLK, 
					// Disable RX ERR, SLEEP, TXWAKE
	ScibRegs.SCICTL2.all = 0x0003;
  
  	// baudDiv(BRR) = (LSPCLK(37.5M)/(baud*8)) - 1
  	brr = (37500000 / (baud * 8)) - 1;	// calculate the baudDiv

  	ScibRegs.SCIHBAUD = 0xff & (brr>>8);	// set for baud rate
  	ScibRegs.SCILBAUD = 0xff & brr;
  
  	ScibRegs.SCIFFTX.all = 0xC000;	// enable tx FIFO, trigger 0 disable int here
  	ScibRegs.SCIFFRX.all = 0x0028;	// enable rx FIFO, trigger 1
	ScibRegs.SCIFFCT.all = 0x00;
    
	ScibRegs.SCICTL1.all = 0x0023;	// Relinquish SCI from Reset
	ScibRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;
  
	PieCtrlRegs.PIECRTL.bit.ENPIE = 1;	// Enable the PIE block
   	PieCtrlRegs.PIEIER9.bit.INTx3 = 1;	// PIE Group 9, INT3, SCIB
  	PieCtrlRegs.PIEIER9.bit.INTx4 = 1;	// PIE Group 9, INT4, SCIB

}



interrupt void SciATxFifoIsr(void)
{

	//Check if there is data int the buffer to transmit
	if (SizeBufATX <= 0){
		//The buffer is empty to turn off Transmission
		SciaRegs.SCIFFTX.bit.TXFFIENA = 0;
	}else{
		// Fill up the Buffer
		while(( SizeBufATX > 0) && (SciaRegs.SCIFFTX.bit.TXFFST < 16 )){
			// Put in the next 
			SciaRegs.SCITXBUF = BufferATX[HeadBufATX++];

			// Wrap the Tail to make circular buffer
			// Buffer is power of 2 so simple ANDing works
			HeadBufATX &=BUF_OVERFLOW;
			SizeBufATX--;
		}
	}
   
	SciaRegs.SCIFFTX.bit.TXINTCLR=1;	// Clear SCI Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x100;      // Issue PIE ACK
}

interrupt void SciARxUSB(void)
{
	Uint16 ThisCh;	

	if (SciaRegs.SCIRXST.bit.RXERROR){
		SciaRegs.SCICTL1.bit.SWRESET = 0;
		SciaRegs.SCICTL1.bit.SWRESET = 1;

		SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
		SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
		PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
		return;
	}

	// Fill Up Buffer While There is Data
	while( SciaRegs.SCIFFRX.bit.RXFIFST ){
		//Get the data
		ThisCh = SciaRegs.SCIRXBUF.all; 
		
		//Check if there is room in the buffer
		if (SizeBufARX <= SERIAL_BUF_SIZE){
			// Read the data into the buffer
			BufferARX[TailBufARX++] = ThisCh;	 
			
			//Wrap pointer to prevent overflows
			TailBufARX &= BUF_OVERFLOW;

			//Update the size of the buffer
			SizeBufARX++;

		}else{
			// An Overflow Occured

		}
	}
	//Set interrupt to collect missing bytes of packet
	SciaRegs.SCIFFRX.bit.RXFFIL = 1;
	numPacketsA = SizeBufARX;

	SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
	SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
}
interrupt void SciARxFifoIsr(void)
{
	Uint16 ThisCh;	

	if (SciaRegs.SCIRXST.bit.RXERROR){
		SciaRegs.SCICTL1.bit.SWRESET = 0;
		SciaRegs.SCICTL1.bit.SWRESET = 1;

		SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
		SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
		PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
		return;
	}

	// Fill Up Buffer While There is Data
	while( SciaRegs.SCIFFRX.bit.RXFIFST ){
		//Get the data
		ThisCh = SciaRegs.SCIRXBUF.all; 
		
		// Check if in middle of Packet
		if (NumBytesNeededA < PACKET_SIZE){
			
			//Check if there is room in the buffer
			if (SizeBufARX <= SERIAL_BUF_SIZE){
				// Read the data into the buffer
				BufferARX[TailBufARX++] = ThisCh;	 
			
				//Wrap pointer to prevent overflows
				TailBufARX &= BUF_OVERFLOW;

				//Update the size of the buffer
				SizeBufARX++;

				//Update number of Bytes needed for new packed
				if (--NumBytesNeededA == 0){
					// A full packed has been received 
					numPacketsA++;
					NumBytesNeededA = PACKET_SIZE;
				}
			}else{
				// An Overflow Occured

			}
		// Looking for start of a new packet '0x0A'
		}else{
			if (ThisCh == START_BYTE){ 
				//A new packet is started
				NumBytesNeededA--;
			}
		}
	}
	//Set interrupt to collect missing bytes of packet
	SciaRegs.SCIFFRX.bit.RXFFIL = NumBytesNeededA;

	SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
	SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
}

interrupt void SciBTxFifoIsr(void)
{
	
	//Check if there is data int the buffer to transmit
	if (SizeBufBTX <= 0){
		//The buffer is empty to turn off Transmission
		ScibRegs.SCIFFTX.bit.TXFFIENA = 0;
	}else{
		// Fill up the Buffer
		while(( SizeBufBTX > 0) && (ScibRegs.SCIFFTX.bit.TXFFST < 16 )){
			// Put in the next byte
			ScibRegs.SCITXBUF = BufferBTX[HeadBufBTX++];

			// Wrap the Tail to make circular buffer
			// Buffer is power of 2 so simple ANDing works
			HeadBufBTX &=BUF_OVERFLOW;
			SizeBufBTX--;
		}
	}

	ScibRegs.SCIFFTX.bit.TXINTCLR=1;    // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x100;      // Issue PIE ACK
}

interrupt void SciBRxFifoIsr(void)
{
   	Uint16 ThisCh;	

	// Fill Up Buffer While There is Data
	while( ScibRegs.SCIFFRX.bit.RXFIFST ){
		//Get the data
		ThisCh = ScibRegs.SCIRXBUF.all; 
		
		// Check if in middle of Packet
		if (NumBytesNeededB <8){
			
			//Check if there is room in the buffer
			if (SizeBufBRX <= SERIAL_BUF_SIZE){
				// Read the data into the buffer
				BufferBRX[TailBufBRX++] = ThisCh;	 
			
				//Wrap pointer to prevent overflows
				TailBufBRX &= BUF_OVERFLOW;

				//Update the size of the buffer
				SizeBufBRX++;

				//Update number of Bytes needed for new packed
				if (--NumBytesNeededB == 0){
					// A full packed has been received 
					numPacketsB++;
					NumBytesNeededB = 8;
				}
			}else{
				// An Overflow Occured

			}
		// Looking for start of a new packet '0x0A'
		}else{
			if (ThisCh == 0x0A){
				//A new packet is started
				NumBytesNeededB--;
			}
		}
	}
	//Set interrupt to collect missing bytes of packet
	ScibRegs.SCIFFRX.bit.RXFFIL = NumBytesNeededB;

	ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;  // Clear Overflow flag
	ScibRegs.SCIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x100;  	// Issue PIE ack
}

void Hex2ASCII(unsigned long hex, unsigned int size, unsigned char* str){
	unsigned long temp;

	while(size>0){
		temp=(hex>>((size-1)*4))&0x0F;
		if( temp > 9 )
			*str = (char)('A' + temp - 10);
		else
			*str = '0' + temp;
		str++;
		size--;
	}
	*str = '\0';
}

void PutAWord(unsigned long uw){
	unsigned char mystr[9];
	Hex2ASCII(uw,8,mystr);

	SendDataA(mystr, sizeof(mystr));
}

unsigned int GetASCI(void){
	if(SciaRegs.SCIFFRX.bit.RXFIFST==1) // check for XRDY =1 for empty state
		return SciaRegs.SCIRXBUF.all;
	else
		return 0xFF00;
}
