/*
  IO testing is done by mating two LTC stickers to each other, such that
  A0<->A5, A1<->A4 ... A5<->A0. There is a male half and a female half.
  The male half outputs test stimulus, and the female half analyzes
  the stimulus. Between the two, this testbench can validate the 
  plumbing of most IO functions. 

  Specifically, this test does *not* check for the full analog performance 
  of the ADC, it just confirms that the ADC API calls work. A separate
  "true analog" (vs PWM) voltage source is needed to characterize the
  analog performance of the ADC.

  Setup: 
    Mate two LTC stickers to each other, such that A0<->A5, A1<->A4 ... A5<->A0.
    This is the male half of the IO tester. Power on the male side first.

  Expected result:
    The male half should exhibit a simple scan pattern of LEDs from left-to-right.
    Only the "ON" LED is driven; the rest are configured as inputs when
    the LED is off, allowing the female half to drive its LEDs as feedback.

*/

void setup() {
  int i;
  
  for( i = 0; i < 6; i++ ) {
    pinMode(i, INPUT);
  }
}

void loop() {
  int i;

  // one pass as driven
  for( i = 0; i < 6; i++ ) {
    pinMode(i, OUTPUT);
    digitalWrite(i, 1);
    delay(3000);
    pinMode(i, INPUT);
  }

  // one pass as pullups
  for( i = 5; i >=0; i-- ) {
    pinMode(i, INPUT_PULLUP);
    delay(3000);
    pinMode(i, INPUT);
  }
  
}
