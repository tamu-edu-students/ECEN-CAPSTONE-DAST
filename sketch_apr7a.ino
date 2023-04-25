#include <TimerOne.h>
#include <Wire.h>
#include <Arduino.h>

#define NPN_PIN 9
int phase = 1;
float vIn = A0;
float vBatt = A2;
float vOn = A3;
float bjtWave = 3;  // pulse width modulation square wave.
bool Q3 = false;    // sets up mosfet 3 to be off
float iBatt = 500;
float targetV = 14.7;  //phase 1 target
int count = 0;


void setup() {

  // Basic setup
  Serial.begin(9600);        //sets output to serial port
  pinMode(vIn, INPUT);       //sets vIn(A0) to an input port
  pinMode(vBatt, INPUT);     //sets vBatt to an Output port
  pinMode(vOn, INPUT);       //sets VOn to an input port.
  pinMode(bjtWave, OUTPUT);  //sets pwm_pin(pin 3) to an output port

  // Timer 1 pin
  Timer1.initialize(33.33);  // Period 33.33 us => Frequency ~30 kHz
  Timer1.pwm(NPN_PIN, 0);    // Sets up square wave to have a duty cycle of 0

  Serial.println(" Start ");

  if (iBatt < 50) {
    //Enter phase 2;
    phase = 2;
    targetV = 13.7;
    if (iBatt < 10) {
      //enter phase 3
      phase = 3;
      Q3 = false;  //turn current to battery off
    }
  }

  // Voltage input measurement
  //float vIn = analogRead(vIn);
  vIn = 1.75;  //test

  float voltageIn = vIn * 10.0;

  // Voltage battery measurement
  //float vBatt = analogRead(vBatt);
  vBatt = 1.47;  //test case
  float voltageBatt = vBatt * 10.0;

  // Charge control
  float duty = targetV / voltageIn;
  float offset = targetV / voltageBatt;
  if (offset > .9 && offset < 1.1) {
    Q3 = true;  //turn on current to battery
  } else if (offset <= .9) {
    duty += 0.01;
  } else if (offset >= 1.1) {
    duty -= 0.01;
  }

  // Set the duty cycle
  Timer1.pwm(NPN_PIN, duty * 1023);

  // Prints the result to the serial port
  Serial.print(" voltageIn: ");
  Serial.print(voltageIn);
  Serial.print(" voltageBatt: ");
  Serial.print(voltageBatt);
  Serial.print(" duty: ");
  Serial.print(duty * 100);
  Serial.println(" end of setup");
}

void loop() {

  Q3 = false;
  
  //testing changing battery current

  
  //resets testing
  if (iBatt < 0) {
    iBatt = 500;
    phase = 1;
    targetV = 14.7;
  }
  Serial.print("targetV: ");
  Serial.print(targetV);
  iBatt = iBatt + random(-100, 50);  //simulate test cases
  Serial.print(" iBatt: ");
  Serial.print(iBatt);

  if (iBatt < 50 && iBatt >= 10) {
    //Enter phase 2;
    phase = 2;
    targetV = 13.7;
  }
  if (iBatt < 10) {
      //enter phase 3
      phase = 3;
      Q3 = false;  //turn current to battery off
      targetV = 0;
    }
  Serial.print(" phase: ");
  Serial.print(phase);
  // Voltage input measurement
  //float vIn = analogRead(vIn);
  float rand = random(-5, 5);
  vIn = vIn + rand / 10;  //test for changing input voltage

  float voltageIn = vIn * 10.0;

  // Voltage battery measurement
  //float vBatt = analogRead(vBatt);
  float rand2 = random(-5, 5);
  vBatt = vBatt + rand2 / 10;  //test case
  float voltageBatt = vBatt * 10.0;

  // Charge control
  float duty = targetV / voltageIn;
  while (Q3 == false) {
    duty = targetV / voltageIn;
    float offset = targetV / voltageBatt;
    if (offset > .98 && offset < 1.02) {
      Q3 = true;  //turn on current to battery
      
    } else if (offset <= .98) {
      duty += 0.01;
      //for testing purposes
      voltageBatt -= 0.1;//should this be +
    } else if (offset >= 1.02) {
      duty -= 0.01;
      //for testing purposes
      voltageBatt += 0.1; //should this be -
    }
    // Set the duty cycle
    Timer1.pwm(NPN_PIN, duty * 1023);
  }



  // Print the result to the serial port

  Serial.print(" voltageIn: ");
  Serial.print(voltageIn);
  Serial.print(" voltageBatt: ");
  Serial.print(voltageBatt);
  Serial.print(" duty: ");
  Serial.print(duty * 100);
  Serial.println();
  vIn = 1.75;
  vBatt = 1.47;
  count++;
  while (count >= 50) {
  }
}
