#ifndef _NRF24_MANAGER_
#define _NRF24_MANAGER_

#include "nRF24SpiManagerArduino.h"
#include "nRF24SelectPinArduino.h"

#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define RF_CH       0x05
#define STATUS      0x07
#define RX_ADDR_P0  0x0A
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define FIFO_STATUS 0x17
#define DYN_PD		0x1C
#define FEATURES 	0x1D



#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define R_RX_PLD_WID   0x60
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define W_TX_PAYLOAD_NO_ACK  0xB0
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2

#define NOP_INSTR   0xFF

#define REGISTER_MASK 0x1F

#define ADDR_LEN 5

#define DYN_PAYLOAD	255

class nRF24Manager
{
public :
	nRF24Manager();
	~nRF24Manager();

	void SetSpiManager(nRF24SpiManagerArduino &rSpiManager);
	void SetSelectPin(nRF24SelectPinArduino &rSelectPin);
	void Initialize();
	void SetChannel(unsigned char channel);
	void SetTxAddr(unsigned char *addr);
	void ConfigurePipe(unsigned char pipe, unsigned char *addr, unsigned char payload, bool dynPayload = false);
	void SetFeature(bool bEnableNoAck, bool bEnableDynPayload = false);

	void Send(unsigned char *pData);
	void SendNoAck(unsigned char *pData);
	void SendDynPayload(unsigned char *pData, unsigned char size);
	void SendDynPayloadNoAck(unsigned char *pData, unsigned char size);
	void Read(unsigned char *pDataOut, unsigned char size);
	bool IsTxEmpty();
	bool IsTxFull();
	bool IsRxEmpty();
	bool IsRxFull();
	unsigned char GetFirstPayloadPipe();
	unsigned char GetFirstPayloadLength();
	//unsigned char GetReceivedPacketLength();
	
	void StandByMode();
	void PowerDown();
	void RxMode();
	void TxMode();

//protected :

	void SetCommand(unsigned char reg);
	void SetRegister(unsigned char reg, unsigned value);
	
	void WriteRegister(unsigned char reg, unsigned char *pDataOut, unsigned char size);
	void ReadRegister(unsigned char reg, unsigned char *pDataIn, unsigned char size);
	unsigned char GetStatus();
	unsigned char GetRegister(unsigned char reg); // a deplacer
	
	nRF24SpiManagerArduino *mpSpi;
	nRF24SelectPinArduino *mpSelectPin;
	unsigned char mConfig;
	unsigned char mPayloadLengh[7];
};

#endif // _NRF24_MANAGER_