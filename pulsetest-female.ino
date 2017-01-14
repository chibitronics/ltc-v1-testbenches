/*
  pulse testing is done by mating two LTC stickers to each other, such that
  A0<->A5, A1<->A4 ... A5<->A0. There is a male half and a female half.
  The male half outputs test stimulus, and the female half analyzes
  the stimulus. 

  Setup: 
    Mate two LTC stickers to each other, such that A0<->A5, A1<->A4 ... A5<->A0.
    This is the female half of the IO tester. Power on the male side first.
    You will need a dataviewer to see the results.

  Expected result:
    The female side should print out via the dataviewer each channel observed,
    the mode, and the pulse width. High pulse should be 900,000 microseconds, 
    low pulse should be 400,000 microseconds.

*/

void setup() {
  int i;
  
  for( i = 0; i < 6; i++ ) {
    pinMode(i, INPUT);
  }
  Serial.begin(9600);
}

void loop() {
  int i;
  int width;

  for( i = 0; i < 6; i++ ) {
    width = pulseIn(i, HIGH);
    Serial.println(i);  // ideally we'd do some string catting but it doesn't work right now
    Serial.println(width);
  }
  for( i = 0; i < 6; i++ ) {
    width = pulseIn(i, LOW);
    Serial.println(i);
    Serial.println(width);
  }


  // this set should just return zeros as we timeout in 100,000 microseconds
  for( i = 0; i < 6; i++ ) {
    width = pulseIn(i, HIGH, 100000);
    Serial.println(i);   // ideally we cat in a flag here so we know what part of the cycle we're looking at
    Serial.println(width);
  }
  for( i = 0; i < 6; i++ ) {
    width = pulseIn(i, LOW, 100000);
    Serial.println(i);
    Serial.println(width);
  }
  
  
}
