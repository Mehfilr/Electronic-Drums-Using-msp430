#include <MIDI.h>
#define DEBUG_A
#define DEBUG_B

#define  midichannel 1;                              // MIDI channel from 0 to 15 (+1 in "real world")

int pinRead;
byte PadNote[2] = { // MIDI notes from 0 to 127 (Mid C = 60)
  36, 38
};         
        
int MaxPlayTime[2] = { // Cycles before a 2nd hit is allowed
  90, 90
};               

//*******************************************************************************************************************
// Internal Use Variables
//*******************************************************************************************************************
boolean activePad[2] = {
  0, 0
};                   // Array of flags of pad currently playing
int PinPlayTime[2] = {
  0, 0
};                     // Counter since pad started to play
byte status1;
int pin = 0;
int hitavg = 0;


void setup()
{
  Serial.begin(57600);  // connect to the serial port
  pinMode(A9, INPUT);
  digitalWrite(A9, HIGH);
  pinMode(A8, INPUT);
  digitalWrite(A8, HIGH);
}

void loop()
{
    for(int pin=0; pin <= 1; pin++)                       
    {
      boolean on  = false;
      if(pin == 0)
         on = digitalRead(A9);
      else
         on = digitalRead(A8);
  
      if(!on)
      {
        if((activePad[pin] == false))
        {
            hitavg = 127;
  
#ifdef DEBUG_A
          Serial.println("PADNOTE = = ");
          Serial.println(PadNote[pin]);
          Serial.println(pin);
#endif
          MIDI_TX(144,PadNote[pin],hitavg); //note on
          PinPlayTime[pin] = 0;
          activePad[pin] = true;
        }
        else
        {
          PinPlayTime[pin] = PinPlayTime[pin] + 1;
        }
      }
      else if((activePad[pin] == true))
      {
        PinPlayTime[pin] = PinPlayTime[pin] + 1;
        if(PinPlayTime[pin] > MaxPlayTime[pin])
        {
          activePad[pin] = false;
          MIDI_TX(128,PadNote[pin],0);
        }
      }
    }
}

//*******************************************************************************************************************
// Transmit MIDI Message
//*******************************************************************************************************************
void MIDI_TX(byte MESSAGE, byte PITCH, byte VELOCITY)
{
    status1 = MESSAGE + midichannel;
    Serial.write(status1);
    Serial.write(PITCH);
    Serial.write(VELOCITY);
}


