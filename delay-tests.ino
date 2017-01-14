/*
  Delay testing.

  Setup: a single LTC sticker

  Expected result:
    LEDs A0-A3 turn on each for exactly 1 second in sequence.
 */

void setup() {
  int i ;
  for( i = 0; i < 6; i++ ) {
    pinMode(i, OUTPUT);
    digitalWrite(i, 0);
  }
}

void loop() {
  int i;
  int curtime;

  while(1) {
    digitalWrite(0, 1);
    delay(1000);
    digitalWrite(0, 0);
    
    digitalWrite(1, 1);
    curtime = millis();
    while( millis() - curtime < 1000 )
      ;
    digitalWrite(1, 0);
  
    digitalWrite(2, 1);
    delayMicroseconds(1000000);
    digitalWrite(2, 0);
    
    digitalWrite(3, 1);
    curtime = micros();
    while( micros() - curtime < 1000000 )
      ;
    digitalWrite(3, 0);
  }
  
}
