#include "nRF24SelectPinArduino.h"

#include<arduino.h>

nRF24SelectPinArduino::nRF24SelectPinArduino()
	: mCEPin(0),
	  mCSPin(0)
{
	/*EMPTY*/
}


nRF24SelectPinArduino::~nRF24SelectPinArduino()
{
	/*EMPTY*/
}

void nRF24SelectPinArduino::SetCELow()
{
	digitalWrite(mCEPin,LOW);
}

void nRF24SelectPinArduino::SetCEHigh()
{
	digitalWrite(mCEPin,HIGH);
}

void nRF24SelectPinArduino::SetCSLow()
{
	digitalWrite(mCSPin,LOW);
}

void nRF24SelectPinArduino::SetCSHigh()
{
	digitalWrite(mCSPin,HIGH);
}

void nRF24SelectPinArduino::SetCEPin(unsigned char CEPin)
{
	mCEPin = CEPin;
	pinMode(mCEPin, OUTPUT);
	SetCELow();
}

void nRF24SelectPinArduino::SetCSPin(unsigned char CSPin)
{
	mCSPin = CSPin;
	pinMode(mCSPin, OUTPUT);
	SetCSHigh();
}

