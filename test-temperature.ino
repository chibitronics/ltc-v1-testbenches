/*
  check temperature sensor

  Setup: needs a dataviewer to view the result

  Expected result: the current temperture in degrees C printed on the dataviewer,
   interleaved with the raw binary code readout from the temperature sensor

*/

void setup() {
  Serial.begin(9600);
  analogReadResolution(12);
}


void loop() {
  int tempcode = analogRead(A6); // A6 is the temperature sensor
  Serial.println(tempcode);
  float tempV = ((float) tempcode) * (3300.0 / 4095.0);

  float temp = 25.0 - ((tempV - 716.0) / 1.62); // 716mV and 1.62mV/C are from the KL02 datasheet

  Serial.println(temp, 2);
  delay(250);
}
