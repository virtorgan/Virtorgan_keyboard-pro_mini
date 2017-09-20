#include "Arduino.h"
#include <MIDI.h>

#define premiereNote 36U
#define velociteDefault 64U
//scan every x mS
#define scanPeriod 1U

#define channel 1U

#define readKeys ((PIND & B11111100)>>2) + ((PINB & B00000011)<<6)
#define nop() delayMicroseconds(50) //__asm__("nop\n\t""nop\n\t""nop\n\t")

uint8_t previousKeys[8] = {0};
uint8_t inputImage[8] = {0};
uint8_t diffKey = 0 ;
unsigned long previousMillis = 0 ;

MIDI_CREATE_DEFAULT_INSTANCE();

void setup()
{
    MIDI.begin(MIDI_CHANNEL_OMNI);
//    Serial.begin(38400);
    DDRB &= B11000000; // only 6 lsb
    DDRC &= B11110000; // only 4 lsb
    DDRD &= B00000011; // only 6 msb
    // activate input pullups on real input pins
    PORTB |= B00000011; // only 2 lsb
    PORTD |= B11111100; // only 6 msb
}

// The loop function is called in an endless loop
void loop()
{
    //if (millis() >= previousMillis + scanPeriod){
    //    previousMillis = millis();
#if 1
    // scan keyboard
        for (uint8_t i = 0 ; i < 8 ; i++){
        	pinMode(i+10,OUTPUT);
      	    digitalWrite(i+10,LOW);
      	    delayMicroseconds(3);
        	inputImage[i] = readKeys;
        	pinMode(i+10,INPUT);
           }
#endif
#if 0
    // scan keyboard
        DDRB |= B00000100; //Pin10
        PORTB &= B11111011;
        nop();
        inputImage[0] = readKeys;
        DDRB &= B11000000;

        DDRB |= B00001000;//Pin11
        PORTB &= B11110111;
        nop();
        inputImage[1] = readKeys;
        DDRB = DDRB & B11000000;

        DDRB |= B00010000;//Pin12
        PORTB &= B11101111;
        nop();
        inputImage[2] = readKeys;
        DDRB = DDRB & B11000000;

        DDRB |= B00100000;//Pin13
        PORTB &= B11011111;
        nop();
        inputImage[3] = readKeys;
        DDRB &= B11000000;

        DDRC |= B00000001;//Pin14
        PORTC &= B11111110;
        nop();
        inputImage[4] = readKeys;
        DDRC &= B11110000;

        DDRC |= B00000010;//Pin15
        PORTC &= B11111101;
        nop();
        inputImage[5] = readKeys;
        DDRC &= B11110000;

        DDRC |= B00000100;//Pin16
        PORTC &= B11111011;
        nop();
        inputImage[6] = readKeys;
        DDRC &= B11110000;

        DDRC |= B00001000;//Pin17
        PORTC &= B11110111;
        nop();
        inputImage[7] = readKeys;
        DDRC &= B11110000;
#endif        
    // check key changes
    diffKey = 0 ;
    for (uint8_t i = 0 ; i < 8 ; i++) {
      diffKey = previousKeys[i] ^ inputImage[i]; // Xor = bits at 1 where key changed
      if (diffKey) { // if at least one key changed
          for (uint8_t j = 0 ; j < 8 ; j++){
              if ((diffKey>>j) & B00000001){ // if that particular key changed
                  //noteOn(((i * 8) + j + premiereNote), (~(inputImage[i]>>j) & B00000001) * 127);
            	  MIDI.sendNoteOn(((i * 8) + j + premiereNote) , (~(inputImage[i]>>j) & B00000001) * velociteDefault , channel); // noteOn if the key has changed and velocity 127/0 if On/Off
#if 0
                  Serial.print("NoteOn(");
                  Serial.print((i * 8) + j + premiereNote);
                  Serial.print(" , ");
                  Serial.print((~(inputImage[i] >> j) & B00000001) * 127);
                  Serial.println(" )");
#endif
              }
           previousKeys[i] = inputImage[i];
           }
      }

    }
    MIDI.read();
    //}

}
