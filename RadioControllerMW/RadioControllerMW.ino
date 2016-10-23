#include <SPI.h>
#include <nRF24Manager.h>

nRF24SpiManagerArduino spiManager;
nRF24SelectPinArduino SenderPin;
nRF24Manager nRF24ManagerSender;

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin

unsigned int array[25];

// variables will change:
int buttonState = 0, old_buttonState = 0;         // variable for reading the pushbutton status

void setup() {
    Serial.begin(115200);
    // initialize the pushbutton pin as an input:
    pinMode(buttonPin, INPUT);
    // Use SS pin (because it was disabled in spi.c)
    digitalWrite(SS, HIGH);
    pinMode(SS, OUTPUT);

    SenderPin.SetCSPin(7);
    SenderPin.SetCEPin(8);
    spiManager.Initialize();
    nRF24ManagerSender.SetSpiManager(spiManager);
    nRF24ManagerSender.SetSelectPin(SenderPin);
    nRF24ManagerSender.Initialize();
    nRF24ManagerSender.SetChannel(1);
    nRF24ManagerSender.SetFeature(true, false);
    nRF24ManagerSender.ConfigurePipe(0, (unsigned char*)"clie1", 10);
    nRF24ManagerSender.SetTxAddr((unsigned char*)"serv1");
    nRF24ManagerSender.StandByMode();

    nRF24ManagerSender.TxMode();

}

void WaitSynchro()
{
    unsigned char oldstate, newstate;
    while(1)
    {
        oldstate = newstate = digitalRead(buttonPin);
        unsigned long time = micros();

        // Wait raising edge ...
        while( oldstate != 0 || newstate != 1 )
        {
            if ( (micros() - time) > 3000 )
            {
                //Serial.println("return 1");
                // Too much time tha we are waiting the rising edge
                return ;
            }
            oldstate = newstate;
            newstate = digitalRead(buttonPin);
        }

        oldstate = newstate = digitalRead(buttonPin);
        time = micros();
        while( oldstate != 0 || newstate != 1 )
        {
            if ( (micros() - time ) > 3000 )
            {
                // Too much time before last synchro
                return ;
            }
            oldstate = newstate;
            newstate = digitalRead(buttonPin);
        }
    }
}

void GetChannel( unsigned short *array, unsigned char nbChannel )
{
    unsigned long timeArray[9];
//  Serial.println( "waitsynro start" );
    WaitSynchro();
//  Serial.println( "waitsynro end" );
    unsigned char oldstate, newstate;
    oldstate = newstate = digitalRead(buttonPin);

    for( unsigned char channelIt=0; channelIt < (nbChannel + 1); channelIt++)
    {
        //Serial.println( channelIt );
        oldstate = digitalRead(buttonPin);

        while( oldstate != 0 || newstate != 1 )
        {
            oldstate = newstate;
            newstate = digitalRead(buttonPin);
        }
        timeArray[channelIt] = micros();
    }

    for( unsigned char channelIt=0; channelIt < nbChannel; channelIt++)
        array[channelIt] = timeArray[channelIt+1] - timeArray[channelIt];
}

void loop()
{
    unsigned short array[10];

    GetChannel( array, 8 );

    nRF24ManagerSender.SendNoAck((byte *)&array[0]);
    //nRF24ManagerSender.SendNoAck((byte *)&dataToSend[0]);
    while(nRF24ManagerSender.IsTxEmpty() == false) ;
    /*
    for(unsigned char it=0; it<8; it++)
    {
      Serial.print( array[it] );
      Serial.print( "\t" );
    }*/
    /*
    for(unsigned char it=0; it<8; it++)
    {
      Serial.print( array[it] );
      Serial.print( "\t" );
    }*/

    Serial.println("");

}
