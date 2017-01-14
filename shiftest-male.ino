/*
  shift testing is done by mating two LTC stickers to each other, such that
  A0<->A5, A1<->A4 ... A5<->A0. There is a male half and a female half.
  The male half outputs test stimulus, and the female half analyzes
  the stimulus. 

  Setup: 
    Mate two LTC stickers to each other, such that A0<->A5, A1<->A4 ... A5<->A0.
    This is the male half of the IO tester. Power on the male side first.

  Expected result:
    The female will pull a flag low to indicate to the male when it's time to transmit (female pin 5)
    The male will transmit a byte of data, and then flash a light (male pin 2)

    If the byte is received correctly, the female side will flash a light.
    Otherwise, it will remain off.

*/

void setup() {
  int i;
  
  for( i = 0; i < 6; i++ ) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW); // make sure clock pin is low
  }
  pinMode(0, INPUT_PULLUP);  // this is the flag pin

  while( (digitalRead(0) == HIGH) )
    ;

  delay(100); // give some time for female init to finish
  shiftOut(5, 4, LSBFIRST, 0xc5);

}

void loop() {
  // indicate doneness
  digitalWrite(2, 0);
  delay(500);
  digitalWrite(2, 1);
  delay(500);
  
}
