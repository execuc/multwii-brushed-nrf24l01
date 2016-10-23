#include "nRF24Manager.h"


nRF24Manager::nRF24Manager()
	: mConfig(0x08)
{
	for(unsigned char it=0; it<8; it++)
	{
		mPayloadLengh[it] = 0;
	}
}


nRF24Manager::~nRF24Manager()
{
	/*EMPTY*/
}


void nRF24Manager::SetSpiManager(nRF24SpiManagerArduino &rSpiManager)
{
	mpSpi = &rSpiManager;
}

void nRF24Manager::SetSelectPin(nRF24SelectPinArduino &rSelectPin)
{
	mpSelectPin = &rSelectPin;
}

void nRF24Manager::Initialize()
{
mConfig = 0x08; // CRC enable
	mpSelectPin->SetCELow();
	mpSelectPin->SetCSHigh();
	
	
	
	PowerDown();
	
	SetCommand(FLUSH_RX);
	SetCommand(FLUSH_TX);

	//Disable all data pipe
	SetRegister(W_REGISTER | (REGISTER_MASK & EN_RXADDR), 0);

	
	
}

void nRF24Manager::Send(unsigned char *pData)
{
	WriteRegister(W_TX_PAYLOAD, pData, mPayloadLengh[0]);
}

void nRF24Manager::SendNoAck(unsigned char *pData)
{
	WriteRegister(W_TX_PAYLOAD_NO_ACK, pData, mPayloadLengh[0]);
}

void nRF24Manager::SendDynPayload(unsigned char *pData, unsigned char size)
{
	WriteRegister(W_TX_PAYLOAD, pData, size);
}

void nRF24Manager::SendDynPayloadNoAck(unsigned char *pData, unsigned char size)
{
	WriteRegister(W_TX_PAYLOAD_NO_ACK, pData, size);
}

void nRF24Manager::Read(unsigned char *pDataOut, unsigned char size)
{
	ReadRegister(R_RX_PAYLOAD, pDataOut, size); // pourrait récuperer le numéro du pipe pr déterminer la longueur depuis  mPayloadLengh[]
}

bool nRF24Manager::IsTxEmpty()
{
	unsigned char fifo_status = GetRegister(R_REGISTER | (REGISTER_MASK & FIFO_STATUS));
	return ( fifo_status & 0x10);
}

bool nRF24Manager::IsTxFull()
{
	unsigned char fifo_status = GetRegister(R_REGISTER | (REGISTER_MASK & FIFO_STATUS));
	return ( fifo_status & 0x20);
}

bool nRF24Manager::IsRxEmpty()
{
	unsigned char fifo_status = GetRegister(R_REGISTER | (REGISTER_MASK & FIFO_STATUS));
	return ( fifo_status & 0x01);
}

bool nRF24Manager::IsRxFull()
{
	unsigned char fifo_status = GetRegister(R_REGISTER | (REGISTER_MASK & FIFO_STATUS));
	return ( fifo_status & 0x02);
}

unsigned char nRF24Manager::GetFirstPayloadPipe()
{
	unsigned char pipe = GetStatus();
	return  (( pipe & 0x0E) >> 1);
}

unsigned char nRF24Manager::GetFirstPayloadLength()
{
	unsigned char size = GetRegister(R_RX_PLD_WID);
	if( size > 32)
	{
		SetCommand(FLUSH_RX);
		size = 0;
	}
	
	return size;
}

void nRF24Manager::StandByMode()
{
mpSelectPin->SetCELow();
	SetRegister(W_REGISTER | (REGISTER_MASK & CONFIG), mConfig | 0x02);
	
}

void nRF24Manager::PowerDown()
{
mpSelectPin->SetCELow();
	SetRegister(W_REGISTER | (REGISTER_MASK & CONFIG), mConfig | 0x00);
	mpSelectPin->SetCELow();
}

void nRF24Manager::RxMode()
{
	mpSelectPin->SetCELow();
	// PowerUp and set PRX
	SetRegister(W_REGISTER | (REGISTER_MASK & CONFIG), mConfig | 0x03);
	//Clear MAX_RT, TX&RX FIFO intterupt
	//SetRegister(W_REGISTER | (REGISTER_MASK & STATUS), 0x70);
	
	//SetCommand(FLUSH_TX);
	mpSelectPin->SetCEHigh();
	
	//SetCommand(FLUSH_RX);
}

void nRF24Manager::TxMode()
{
	mpSelectPin->SetCELow();
	// PowerUp and set PTX
	SetRegister(W_REGISTER | (REGISTER_MASK & CONFIG), mConfig | 0x02);
	//SetCommand(FLUSH_TX);
	mpSelectPin->SetCEHigh(); // Go stand by mode 2 directly
}

void nRF24Manager::SetChannel(unsigned char channel)
{
	SetRegister(W_REGISTER | (REGISTER_MASK & RF_CH), channel);
}

void nRF24Manager::SetTxAddr(unsigned char *addr)
{
	WriteRegister(W_REGISTER | (REGISTER_MASK & TX_ADDR),addr,ADDR_LEN);
}

void nRF24Manager::SetFeature(bool bEnableNoAck, bool bEnableDynPayload)
{
	unsigned char misc = GetRegister(R_REGISTER | (REGISTER_MASK & FEATURES));
	if( bEnableNoAck )
		misc = misc | 0x01;
	else
		misc = misc & 0xFE;
		
	if( bEnableDynPayload )
		misc = misc | 0x04;
	else
		misc = misc & 0xFB;
		
	SetRegister(W_REGISTER | (REGISTER_MASK & FEATURES), misc );
}

void nRF24Manager::ConfigurePipe(unsigned char pipe, unsigned char *addr, unsigned char payload, bool dynPayload)
{
	// Enable pipe
	unsigned char en_rxaddr = GetRegister(R_REGISTER | (REGISTER_MASK & EN_RXADDR));
	SetRegister(W_REGISTER | (REGISTER_MASK & EN_RXADDR), en_rxaddr | (1 << pipe));
	// Set payload
	SetRegister(W_REGISTER | (REGISTER_MASK & (RX_PW_P0+pipe)), payload);
	// Set pipe receive address
	WriteRegister(W_REGISTER | (REGISTER_MASK & (RX_ADDR_P0+pipe)), addr, ADDR_LEN);
	// Set auto acknowledge
	/*unsigned char en_aa = GetRegister(R_REGISTER | (REGISTER_MASK & EN_AA));
	SetRegister(W_REGISTER | (REGISTER_MASK & EN_AA), en_aa | ( (unsigned char)(autoAck) << pipe));*/
	// Set dynamic payload
	/*unsigned char en_dp = GetRegister(R_REGISTER | (REGISTER_MASK & DYN_PD));
	SetRegister(W_REGISTER | (REGISTER_MASK & DYN_PD), en_dp | ( (unsigned char)(dynPayload) << pipe));*/
	
	//if( dynPayload == false )
		mPayloadLengh[pipe] = payload;
	//else 
	//	mPayloadLengh[pipe] = DYN_PAYLOAD;
}


void nRF24Manager::SetRegister(unsigned char reg, unsigned value)
{
	mpSelectPin->SetCSLow();
	mpSpi->SyncSendByte(reg);
	mpSpi->SyncSendByte(value);
	mpSelectPin->SetCSHigh();
}

void nRF24Manager::SetCommand(unsigned char reg)
{
	mpSelectPin->SetCSLow();
	mpSpi->SyncSendByte(reg);
	mpSelectPin->SetCSHigh();
}

unsigned char nRF24Manager::GetRegister(unsigned char reg)
{
	unsigned char ret = 0xFF;

	mpSelectPin->SetCSLow();
	mpSpi->SyncSendByte(reg);
	ret = mpSpi->SyncSendByte(0);
	mpSelectPin->SetCSHigh();

	return ret;
}

unsigned char nRF24Manager::GetStatus()
{
	unsigned char status;
	mpSelectPin->SetCSLow();
	status = mpSpi->SyncSendByte(NOP_INSTR);
	mpSelectPin->SetCSHigh();
	return status;
}

void nRF24Manager::WriteRegister(unsigned char reg, unsigned char *pDataOut, unsigned char size)
{
	mpSelectPin->SetCSLow();
	mpSpi->SyncSendByte(reg);
	mpSpi->SyncSend(pDataOut, size);
	mpSelectPin->SetCSHigh();
}

void nRF24Manager::ReadRegister(unsigned char reg, unsigned char *pDataIn, unsigned char size)
{
	mpSelectPin->SetCSLow();
	mpSpi->SyncSendByte(reg);
	mpSpi->SyncReceive(pDataIn, size);
	mpSelectPin->SetCSHigh();
}
