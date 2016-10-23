#ifndef _ARDUINO_NRF24_PIN_MANAGER_
#define _ARDUINO_NRF24_PIN_MANAGER_

class nRF24SelectPinArduino 
{
public :
	nRF24SelectPinArduino();
	~nRF24SelectPinArduino();

	void SetCEPin(unsigned char CEPin);
	void SetCSPin(unsigned char CSPin);

	void SetCELow();
	void SetCEHigh();
	void SetCSLow();
	void SetCSHigh();

protected : 
	unsigned char mCEPin;
	unsigned char mCSPin;
};

#endif // _ARDUINO_NRF24_PIN_MANAGER_