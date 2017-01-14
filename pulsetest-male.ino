/*
  pulse testing is done by mating two LTC stickers to each other, such that
  A0<->A5, A1<->A4 ... A5<->A0. There is a male half and a female half.
  The male half outputs test stimulus, and the female half analyzes
  the stimulus. 

  Setup: 
    Mate two LTC stickers to each other, such that A0<->A5, A1<->A4 ... A5<->A0.
    This is the male half of the IO tester. Power on the male side first.

  Expected result:
    The male half should exhibit a simple all-pins pulsing. The waveform
    is asymmetric so we can test HIGH/LOW pulse counting

*/

void setup() {
  int i;
  
  for( i = 0; i < 6; i++ ) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  int i;

  for( i = 0; i < 6; i++ ) {
    digitalWrite(i, 1);
  }
  delay(900);
  for( i = 0; i < 6; i++ ) {
    digitalWrite(i, 0);
  }
  delay(400);
  
}
