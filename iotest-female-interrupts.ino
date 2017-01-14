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
    This is the female half of the IO tester. Power on the male side first.

  Expected result:
    The female half will blink a pin that is +3 % 6 higher than the current
    pin that is being listened to as an interrupt. The pin only changes once
    the interrupt is detected on the listening pin; so, the input stimuli
    may have to complete a whole sweep before hitting the default starting
    interrupt pin, which is A0

*/

#include "ChibiOS.h"

int blink_chan = 3;
int blink_last = 3;

#define FADE_RATE 8

static THD_WORKING_AREA(blinky_area, 64);
static THD_FUNCTION(do_blinky, arg) {
  (void)arg;
  int depth = 0;
  int state = 0;

  while (1) {
    if( state == 1 ) {
      for( depth = 0; depth < 255; depth += FADE_RATE ) {
	analogWrite(blink_chan, depth);
	delay(1);
      }
      for( depth = 255; depth > 0; depth -= FADE_RATE ) {
	analogWrite(blink_chan, depth);
	delay(1);
      }
      state = 0;
    } else {
      digitalWrite(blink_chan, 0);
      //      threadSleep(50);  // threadSleep seems to be unstable
      delay(200);
      digitalWrite(blink_chan, 1);
      //      threadSleep(50);
      delay(200);
      digitalWrite(blink_chan, 0);
      //      threadSleep(50);
      delay(200);
      state = 1;
    }
  }
}

void setup() {
  int i;

  for( i = 0; i < 6; i++ ) {
    pinMode(i, INPUT);
  }

  createThread(blinky_area, sizeof(blinky_area), 20, do_blinky, NULL);
  pinMode(blink_chan, OUTPUT);
}

int intPin;
void loop() {

  for( intPin = 0; intPin < 6; intPin++ ) {
    attachInterrupt(digitalPinToInterrupt(intPin), blink, RISING);
    while(blink_chan == blink_last) {
      delay(1);
      yieldThread();
    }
    detachInterrupt(digitalPinToInterrupt(intPin);
    pinMode(blink_last, INPUT);
    pinMode(blink_chan, OUTPUT);
    blink_last = blink_chan;
    delay(10);
    yieldThread();
  }
}

void blink() {
  blink_chan = (intPin + 3) % 6;
}


