#include <nRF24SelectPinArduino.h>
#include <nRF24SpiManagerArduino.h>

#include <SPI.h>
#include <nRF24Manager.h>

nRF24SpiManagerArduino spiManager;
nRF24Manager nRF24ManagerRecv;
nRF24SelectPinArduino RecvPin;


int   STD_LOOP_TIME  =         3000; //4
int lastLoopTime = STD_LOOP_TIME;
int lastLoopUsefulTime = STD_LOOP_TIME;
unsigned long loopStartTime = 0;

short data[6];

void setup() {

    Serial.begin(115200);
    Serial.println("start...");

    // car desactiver dans spi.cpp pour liberer la pin
    digitalWrite(SS, HIGH);
    pinMode(SS, OUTPUT);

    RecvPin.SetCSPin(7);
    RecvPin.SetCEPin(8);
    spiManager.Initialize();

    nRF24ManagerRecv.SetSpiManager(spiManager);
    nRF24ManagerRecv.SetSelectPin(RecvPin);
    nRF24ManagerRecv.Initialize();
    nRF24ManagerRecv.SetChannel(1);
    nRF24ManagerRecv.SetFeature(true, false);
    //nRF24ManagerRecv.ConfigurePipe(0, (unsigned char*)"clie1", 4);
    nRF24ManagerRecv.ConfigurePipe(0, (unsigned char*)"serv1", 10);
    nRF24ManagerRecv.SetTxAddr((unsigned char*)"clie1");
    nRF24ManagerRecv.StandByMode();
    nRF24ManagerRecv.RxMode();
}

void loop() {
    // Serial.println("loop");
    if( nRF24ManagerRecv.IsRxEmpty() == false )
    {
        // Serial.println( "data" );
        // Serial.println("nrf");
        int testStart = micros();
        while( nRF24ManagerRecv.IsRxEmpty() == false )
        {
            nRF24ManagerRecv.Read((byte *)&data[0], 10);
        }
        int testStop = micros();
        Serial.print("tps : ");
        Serial.println(testStop-testStart);
        //bSet = true;

        for(unsigned int it=0; it<5; it++)
        {
            Serial.print( data[it] );
            Serial.print( "\t" );

        }
        Serial.println("");
        
        /*if(power < 150 )
          power = 0;*/
        /*
        Serial.print(*(unsigned short*)(data));
        Serial.print(" ");
        Serial.println(power);*/

        //silent=0;
    }
    /*else
      silent++;*/

    // *********************** loop timing control **************************
    lastLoopUsefulTime = micros()-loopStartTime;
    //Serial.println(lastLoopUsefulTime);
    if(lastLoopUsefulTime<STD_LOOP_TIME)
    {
        //Serial.print("wait ");Serial.println(STD_LOOP_TIME-lastLoopUsefulTime);
        delayMicroseconds(STD_LOOP_TIME-lastLoopUsefulTime);
    }
    else
    {
        Serial.println("overtime");
    }
    lastLoopTime = micros() - loopStartTime;
    //Serial.println(lastLoopTime);
    loopStartTime = micros();
}

