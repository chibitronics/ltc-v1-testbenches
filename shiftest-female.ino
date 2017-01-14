/*
  shift testing is done by mating two LTC stickers to each other, such that
  A0<->A5, A1<->A4 ... A5<->A0. There is a male half and a female half.
  The male half outputs test stimulus, and the female half analyzes
  the stimulus. 

  Setup: 
    Mate two LTC stickers to each other, such that A0<->A5, A1<->A4 ... A5<->A0.
    Power on both halves. Press reset on the female side, then within 2 seconds 
    reset on the male side. 

  Expected result:
    The female will pull a flag low to indicate to the male when it's time to transmit (female pin 5)
    The male will transmit a byte of data, and then flash a light (male pin 2)

    If the byte is received correctly, the female side will flash a light.
    Otherwise, it will remain off.

    You should see two flashing lights, pins 2 and 3 on either side, flashing if
    all went well. If you just see one, it means the male transmitted but the female
    did not get a valid byte.

*/

void setup() {
  int i;

  Serial.begin(9600);
  
  for( i = 0; i < 6; i++ ) {
    pinMode(i, INPUT);
  }
  pinMode(5, OUTPUT);  // this is the flag pin
  digitalWrite(5, 1);

  delay(2000); // give time to reset the male side so we're at the same point

  digitalWrite(5, 0); // indicate readiness
  
  byte secret = shiftIn(0, 1, LSBFIRST);
  if( secret != 0xC5 ) {
    while(1) // hang, but print the byte we received...
      Serial.println(secret, HEX);
  }

}

void loop() {
  // indicate doneness
  digitalWrite(2, 0);
  delay(500);
  digitalWrite(2, 1);
  delay(500);
  
}
