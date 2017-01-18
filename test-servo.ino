/* 
 tone test code. 

 Setup: connect a servo to pin 0 and pin 5; a pot to pin 1 
 
 Expected result: pot on pin 1 controls the angle of servos on
   pin 0 and 5, which turn counter to each other.
  Pin 3 should glow dimly.
  Pin 2 should do nothing, as LPTIMER is being used for servo, even though tone is called

 */

#include <Servo.h>

Servo servo0; 
Servo servo1; 

void setup() {
  servo0.attach(0);
  servo1.attach(5);
  pinMode(1, INPUT);
  pinMode(4, INPUT);
  analogReadResolution(10);

  tone(2, 440);  // put a tone on pin 2, but it shouldn't work
  analogWrite(3, 16); // put a PWM on pin 3 to check cross-issues
}

void loop() {
  int v1, v2;

  v1 = analogRead(1); 
  v2 = map(v1, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  servo0.write(v2);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
  
  v2 = map(v1, 0, 1023, 180, 0);     // scale it to use it with the servo (value between 0 and 180)
  servo1.write(v2);                  // sets the servo position according to the scaled value
  delay(15);
}
