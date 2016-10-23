#ifndef _ARDUINO_NRF24_SPI_MANAGER_
#define _ARDUINO_NRF24_SPI_MANAGER_

#include <SPI.h>

class nRF24SpiManagerArduino 
{
public :
	nRF24SpiManagerArduino();
	~nRF24SpiManagerArduino();

	virtual void Initialize();
	virtual unsigned char SyncSendByte(unsigned char byte);
	void SyncSend(unsigned char *pDataOut, unsigned char size);
	virtual void SyncSend(unsigned char *pDataOut, unsigned char size, unsigned char *pDataIn);
	virtual void SyncReceive(unsigned char *pDataIn, unsigned char size);
};


#endif // _ARDUINO_NRF24_SPI_MANAGER_