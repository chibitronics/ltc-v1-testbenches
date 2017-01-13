/* floating point tests

   Objective: confirm that the kernel thunks exist and can do an operation
    - We trust that the underlying libraries pulled in are correct enough
    - The failure modes of concern are the thunks are missing or mis-matched 

   Setup: none

   Expected result:
    Lights 0-5 should turn on if all tests pass.
 */

#define ADVANCED 0   // turn to 1 to check atan, exp, ln

void setup() {
  Serial.begin(9600);
  
  int i = 0;

  // turn all LEDs off. As the tests run, the LEDs fill up and turn on
  for( i = 0; i < 6; i++ ) {
    pinMode(i, OUTPUT);
    digitalWrite(i, 0);
  }

  float f1, f2, f3;
  int i1, i2, i3;
  
  /// TEST CASTING
  int pass = 1;

  i2 = 0;
  f1 = 1.0;
  for( i = 0; i < 20; i++ ) {
    i1 = (int) f1;
    if( i1 == 1 )
      i2++;
    f1 += 0.1; // ADD
  }

  if( i2 != 10 )
    pass = 0;

  f2 = (float) i1;
  if( (f2 / 4.0) != 0.5 )
    pass = 0;

  i2 = 0;
  f1 = -2.0;
  for( i = 0; i < 20; i++ ) {
    i1 = (int) f1;
    if( i1 == -3 )
      i2++;
    f1 -= 0.1; // SUB
  }
  
  if( i2 != 10 ) // on x86, this is 9; on LTC, it is 10
    pass = 0;

  if( pass )
    digitalWrite(0, 1); // light up A0 to indicate cast testing passed
  else {
    while(1) {
      Serial.println(i2);
    }
  }
  
  //// COMPARE + ARITH TESTING
  pass = 1;
  f1 = -2.5;
  f2 = (float) i2 - 1.0; // this should be 9, the -1 is needed for LTC run
  f3 = 9.0;

  if( f1 > f2 )
    pass = 0;
  if( f1 == f2 )
    pass = 0;

  if( f2 == f3 )
    f1 = 81.0;
  
  if( f1 < f2 )
    pass = 0;

  f2 = f2 * f3; // should be 81.0 (MPY)
  if( !(f1 <= f2) )
    pass = 0;

  if( !(f1 >= f2) )
    pass = 0;

  if( f1 == f2 ) // (DIV)
    f1 = f1 / 3.0;

  if( f1 != 27 )
    pass = 0;

  if( pass )
    digitalWrite(1, 1); // light up A1 to indicate compare/arith testing passed

  /// TRANSCENDTAL TESTING
  // assume pass = 1 coming in
  f1 = tan((float) pass);
  f2 = sin((float) pass) / cos((float) pass);
  if( f1 != f2 )
    pass = 0;
#if ADVANCED  
  f3 = atan2(f1, 1.0);
  if( f3 != 1.0 )
    pass = 0;

  if( log(f3) != 0.0 )
    pass = 0;
  
  if( exp(f3) != 2.718281828459045 )
    pass = 0;
#endif

  f1 = (float) pass - 3.0; // should be -2
  f2 = f1 * (-2.0);
  f3 = sqrt(f2);
  if( f3 != 2.0 )
    pass = 0;

  if( pass )
    digitalWrite(2, 1); // light up A2 to indicate transcandental pass

  //// INTEGER STRING PRINTING
  pass = 1;
  char outstr[32];
  snprintf(outstr, 32, "test %d FOO!", -632);
  if( strncmp(outstr, "test -632 FOO!", 32) != 0 )
    pass = 0;
  if( !pass ) {
    while(1)
      Serial.println(outstr);
  }

  snprintf(outstr, 32, "result: %03d", i2);
  if( strncmp(outstr, "result: 010", 32) != 0 )
    pass = 0;
  if( !pass ) {
    while(1)
      Serial.println(outstr);
  }

  if( pass )
    digitalWrite(3, 1); // light up A3 to indicate string printing pass

  //// STRING MANIPULATION
  pass = 1;
  char str2[32];
  memset(str2, 0xff, 32);
  strcpy(outstr, "quick brown");
  strncpy(str2, "quick brown fox", 11); // copies quick brown with no null
  if( strcmp(outstr, str2) == 0 ) // there lacks a trailing null, and we memset to 0xFF prior to copy
    pass = 0;
  if( strncmp(outstr, str2, 5) != 0 )
    pass = 0;
  if(strlen(outstr) != 11 )
    pass = 0;
  if(strchr(outstr, 'b')[0] != 'b')
    pass = 0;

  if( atol("42") != 42)
    pass = 0;
  if( atol("-2") != -2)
    pass = 0;
  // if( strtoul("0x10", NULL, 16) != 16) // doesn't exist but in thunk table
  //  pass = 0;

  // if( strcmp(ltoa(30, outstr, 10), "30") != 0 ) // doesn't exist but in thunk table
  //  pass = 0;

  if( pass )
    digitalWrite(4, 1); // light up A4 to indicate string manipulation pass

  //// FLOATING POINT STRING PRINTING
  pass = 1;
  snprintf(outstr, 32, "%4.2f", f3 );
  if( strncmp(outstr, "2.0", 32) != 0 )
    pass = 0;

  if( !pass ) {
    while(1)
      Serial.println(outstr);
  }
  
  f1 = -1.23;
  snprintf(outstr, 32, "%4.2f", f1 );
  if( strncmp(outstr, "-1.23", 32) != 0 )
    pass = 0;
  if( !pass ) {
    while(1)
      Serial.println(outstr);
  }

  if( pass )
    digitalWrite(5, 1); // light up A5 to indicate fp string printing pass
  
}


void loop() {

  delay(1000);  // idle doing nothing, test is in the setup

}
