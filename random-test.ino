/*
  Test the random number generator and seed functions.
  Also tests EEPROM emulation.

  Setup: just one sticker, programmed. Requires at least one
    reboot to confirm proper operation.

  Expected result: If everything is in order LEDs A0 and A1 are lit.
   If A5 is flashing or lit, you need to reboot to complete the test
   Furthermore, random numbers are spewed to the TX

   It's also recommended to visually inspect the random stream coming out
   using a dataviewer.
*/

#include "EEPROM.h"

#define DEBUG 0

long randArray[256];

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
  unsigned long data;
  read_Word(0, &data);
  if(data == 0xFFFFFFFF) {
    // blank state, set LED A5
    digitalWrite(5, 1);
    // unseeded
    for( i = 0; i < 128; i++ ) {
      write_Word(i * 4, random(0x7FFFFFFF));
    }
    // seeded & ranged
    randomSeed(0xbabe);
    for( i = 128; i < 256; i++ ) {
      write_Word(i * 4, random(10, 100));
    }
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
  for( i = 0; i < 128; i++ ) {
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
  for( i = 128; i < 256; i++ ) {
    read_Word(i * 4, &data);
    if( data != (unsigned long) random(10, 100) )
      pass = 0;
    if( data == 0xFFFFFFFF )
      allFFcount++;
  }
  if( allFFcount > 1 )  // this probably indicates failure in EEPROM API
    pass = 0;
  
  if( pass )
    digitalWrite(1, 1);
}

void loop() {
  Serial.println(random(10,100));
  delay(250);
}

