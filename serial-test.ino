#include "ChibiOS.h"

/* 
   serial test

   setup: short TX to RX

   expected result: TX should be able to wiggle RX and make something happen, but
    due to a lack of OS call implementation on RX side, the test cannot be written

 */

static THD_WORKING_AREA(test_thd_wa, 64);
static THD_FUNCTION(do_test, arg) {
  (void)arg;

  while (1) {
    printf("foo\n\r");
    putchar('!');
    delay(500);
  }
}

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  digitalWrite(A0, 0);
  digitalWrite(A1, 0);

  delay(100);
  createThread(test_thd_wa, sizeof(test_thd_wa), 20, do_test, NULL);
}

void loop() {
  char c;
  if( cangetchar() ) {
    c = getchar();
    digitalWrite(A1, 1);
  }
}
