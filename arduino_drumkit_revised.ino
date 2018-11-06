#include <MIDI.h>
#define  MAX_PIEZO  2

uint8_t piezo[MAX_PIEZO] = { A0, A2,};
uint8_t drumType[MAX_PIEZO] = { 5, 4};

const int threshold = 500;  

void setup() 
{
 for (uint8_t i=0; i<MAX_PIEZO; i++)
   pinMode(piezo[i], INPUT);

 Serial.begin(57600);      
}

void MidiTX(unsigned char StatusByte, unsigned char Note, unsigned char Val)
{
 Serial.write(StatusByte);
 Serial.write(Note);
 Serial.write(Val);
}

void ProcessPiezo(uint8_t p)
{
 uint16_t sensorReading = analogRead(piezo[p]);
 uint16_t  val;

 if (sensorReading > threshold) 
 {
   uint32_t oldcas = millis() + 5;
   
   // this part of the code looks complex
   // explain what you are trying to do?
   while (analogRead(piezo[p]) > threshold)
   {
     if (millis() >= oldcas)
     {
       val = analogRead(piezo[p]) / 8;
     }
   }
   
   MidiTX(144, drumType[p], val);
   MidiTX(128, drumType[p], val);
 }
}

void loop()
{
 for (uint8_t i=0; i<MAX_PIEZO; i++)
   ProcessPiezo(i);
}

