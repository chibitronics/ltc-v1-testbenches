/*
  Test the random number generator and seed functions.
  Also tests EEPROM emulation.

  Setup: just one sticker, programmed. Requires at least one
    reboot to confirm proper operation.

   This program needs to be loaded twice, once with CLEAR option set to 1
   and again with CLEAR option set to 0. This is because the EEPROM block
   is preserved through reboots, and its location can change depending
   upon the code size.

  Expected result: If everything is in order LEDs A0, A1, and A2 are lit.
   If A5 is flashing or lit, you need to reboot to complete the test
   Furthermore, random numbers are spewed to the TX

   It's also recommended to visually inspect the random stream coming out
   using a dataviewer.

   A0 indicates we pass basic RNG functionality, and across reboots we get the same random number
   A1 indicates with the same seed, we get the same random number
   A2 indicates that with different seeds, we get different sequences
     --> note that we're getting "random" seeds by sampling an analog input, so there is a slight (1% or so)
       chance you have to run the test a couple times before it passes
*/

#include "EEPROM.h"

#define DEBUG 0
#define CLEAR 1

//extern "C" void eeprom_flush(void *arg);

long randArray[128];

void read_Word(int addr, unsigned long *data) {
  *data = EEPROM.read(addr);
  *data |= (EEPROM.read(addr + 1) << 8);
  *data |= (EEPROM.read(addr + 2) << 16);
  *data |= (EEPROM.read(addr + 3) << 24);
}

void write_Word(int addr, unsigned long data) {
  EEPROM.write(addr, data & 0xFF);
  data >>= 8;
  EEPROM.write(addr+1, data & 0xFF);
  data >>= 8;
  EEPROM.write(addr+2, data & 0xFF);
  data >>= 8;
  EEPROM.write(addr+3, data & 0xFF);
}

void setup() {
  int i;

  Serial.begin(9600);

  for( i = 0; i < 6; i++ ) {
    pinMode(i, OUTPUT);
    digitalWrite(i, 0);
  }
  pinMode(4, INPUT);  // make it float to make some noise

#if CLEAR
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0xFF);
  }
  EEPROM.flush();
#endif
  
  unsigned long data;
  read_Word(0, &data);
  if(data == 0xFFFFFFFF) {
    // blank state, set LED A5
    digitalWrite(5, 1);
    // unseeded
    for( i = 0; i < 64; i++ ) {
      write_Word(i * 4, random(0x7FFFFFFF));
    }
    // seeded & ranged
    randomSeed(0xbabe);
    for( i = 64; i < 96; i++ ) {
      write_Word(i * 4, random(10, 100));
    }

    // random seeded, let's hope it's random enough
    randomSeed(analogRead(4));
    for( i = 96; i < 128; i++ ) {
      write_Word(i * 4, random(10000) );
    }
    //    eeprom_flush(NULL);
    EEPROM.flush();
#if DEBUG
    while(1) {
      for( i = 0; i < 4; i++ ) {
	read_Word( i * 4, &data );
	Serial.println(data, HEX);
	delay(50);
      }
    }
#endif
    // indicate that reboot is needed
    while(1) {
      digitalWrite(5, 0);
      delay(200);
      digitalWrite(5, 1);
      delay(200);
    }
  }
  // check that across reboots, the sequence is the same
  int pass = 1;
  int allFFcount = 0;
  for( i = 0; i < 64; i++ ) {
    read_Word(i * 4, &data);
    if( random(0x7FFFFFFF) != data ) {
      pass = 0;
    }
    if( data == 0xFFFFFFFF )
      allFFcount++;
  }
  if( allFFcount > 1 )  // this probably indicates failure in EEPROM API
    pass = 0;
  
  if( pass )
    digitalWrite(0, 1);

  // check that with a seed, we get the same sequence
  pass = 1;
  randomSeed(0xbabe);
  for( i = 64; i < 96; i++ ) {
    read_Word(i * 4, &data);
    if( data != (unsigned long) random(10, 100) )
      pass = 0;
    if( data == 0xFFFFFFFF )
      allFFcount++;
  }
  if( allFFcount > 1 )  // this probably indicates failure in EEPROM API
    pass = 0;


#if DEBUG
  if( !pass ) {
    while(1) {
      for( i = 0; i < 3; i++ ) {
	read_Word( i * 4, &data );
	Serial.println(data, HEX);
	delay(500);
      }
      Serial.println("FFcount:");
      Serial.println(allFFcount);
    }
  }
#endif  
  
  if( pass )
    digitalWrite(1, 1);

  // check that with a seed, we get the same sequence
  pass = 1;
  randomSeed(analogRead(4));
  for( i = 96; i < 128; i++ ) {
    read_Word(i * 4, &data);
    if( data == (unsigned long) random(10000) )
      pass = 0;
    if( data == 0xFFFFFFFF )
      allFFcount++;
  }
  if( allFFcount > 1 )  // this probably indicates failure in EEPROM API
    pass = 0;
  
  if( pass )
    digitalWrite(2, 1);
  
}

void loop() {
  Serial.println(random(1000,2000));
  delay(250);
}

