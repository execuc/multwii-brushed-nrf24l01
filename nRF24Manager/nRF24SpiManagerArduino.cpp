#include "nRF24SpiManagerArduino.h"


nRF24SpiManagerArduino::nRF24SpiManagerArduino()
{
	/*EMPTY*/
}


nRF24SpiManagerArduino::~nRF24SpiManagerArduino()
{
	/*EMPTY*/
}

void nRF24SpiManagerArduino::Initialize()
{
	SPI.begin();
	SPI.setDataMode(SPI_MODE0);
	//PI.setClockDivider(SPI_2XCLOCK_MASK);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
}

unsigned char nRF24SpiManagerArduino::SyncSendByte(unsigned char byte)
{
	return SPI.transfer(byte);
}

void nRF24SpiManagerArduino::SyncSend(unsigned char *pDataOut, unsigned char size)
{
	for(unsigned char it=0; it<size; it++)
	{
		SyncSendByte(*pDataOut++);
	}
}

void nRF24SpiManagerArduino::SyncSend(unsigned char *pDataOut, unsigned char size, unsigned char *pDataIn)
{
	for(unsigned char it=0; it<size; it++)
	{
		*pDataIn = SyncSendByte(*pDataOut);
		pDataIn++;
		pDataOut++;
	}
}

void nRF24SpiManagerArduino::SyncReceive(unsigned char *pDataIn, unsigned char size)
{
	for(unsigned char it=0; it<size; it++)
	{
		*pDataIn = SyncSendByte(0);
		pDataIn++;
	}
}
