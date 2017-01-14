/* 
 tone test code. This is simplified to show tone() running just
 alone with no other interactions with other code.

 Setup: need a speaker that can be connected to each pin sequentially
 
 Expected result: 1kHz tone can play on each pin.
 */


void setup(void) {
  int i = 0;
  for( i = 0; i < 6; i++ ) {
    pinMode(i, OUTPUT);
    digitalWrite(i, 0);
  }
}

int j = 0;
void loop(void) {

  for( j = 0; j < 6; j++ ) {
    tone((j + 3) % 6, 1000);
    delay(500);
    noTone((j + 3) % 6);
    //digitalWrite((j+3)%6, 0); // tone should leave the pin at 0 when done
  }
  for( j = 5; j >= 0; j-- ) {
    tone((j + 3) % 6, 1000, 500);
    delay(500);
    //digitalWrite((j+3)%6, 0);
  }
}

