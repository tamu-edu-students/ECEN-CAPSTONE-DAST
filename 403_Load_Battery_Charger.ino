/*
Code Written by: Cooper Hamlin
Last Update: 4/28/2023
Purpose: Ecen 403 Load Subsystem

Summary is AI written and updated by me

The necessary libraries are imported, pins are defined, and basic setup is performed. 
The PWM frequency is set to around 30kHz, and the duty cycle is initially set to 0.

The code enters the setup() function, where it first checks if the battery current is less 
than 50A. If it is, the charger enters phase 2, where it targets a voltage of 13.7V. If the 
current is less than 10A, the charger enters phase 3, where it turns off the current to the 
battery.

The input voltage and battery voltage are measured using the analogRead() function, and the 
values are converted into readable voltage measurements. The duty cycle is calculated based on 
the target voltage and the measured input voltage and battery voltage. If the battery voltage 
is within 10% of the target voltage, the MOSFET is turned on to allow current to flow into the 
battery. Otherwise, the duty cycle is adjusted up or down by 0.01 to achieve the target voltage.

The loop() function starts by resetting some variables and simulating random values for the 
battery current and input voltage to test the charger under various conditions.

The phase is determined based on the battery current, and the target voltage is set accordingly.

The input voltage and battery voltage are measured again, and the duty cycle is calculated 
using the same process as in the setup() function. While the MOSFET is off, the duty cycle is 
adjusted to achieve the target voltage. Once the battery voltage is within 2% of the target 
voltage, the MOSFET is turned on and the charger starts supplying current to the battery.

The input solar panel/mppt voltage, buck converter output voltage, and duty cycle are printed to the serial monitor for 
debugging and monitoring purposes.
*/


#include <TimerOne.h>
#include <Wire.h>
#include <Arduino.h>

#define NPN_PIN 9
int phase = 1;
float vFromMPPT = A0; //voltage from solar panel and mppt output, voltage entering buck converter
float vBuckOutput = A1; //voltage output from buck converter that will be input into battery for charging
float bjtWave = 3;  // pulse width modulation square wave.
bool Q3 = false;    // sets up mosfet 3 to be off
bool batteryFullyCharged = false; //sets the battery to not fully charged and will override mosfet 3 being turned on
float iBatt = 500; //sets initial current accross battery for testing purposes
float targetVoltage = 14.7;  //phase 1 target voltage
int count = 0; //used to end looping of code. Used for testing purposes only


void setup() {
  // Basic setup
  Serial.begin(9600);        //sets output to serial port
  pinMode(vFromMPPT, INPUT);     //sets vFromMPPT(A0) to an input port
  pinMode(vBuckOutput, INPUT);   //sets vBuckOutput to an Output port
  pinMode(bjtWave, OUTPUT);  //sets pwm_pin(pin 3) to an output port

  // Timer 1 pin
  Timer1.initialize(33.33);  // Period 33.33 us => Frequency ~30 kHz
  Timer1.pwm(NPN_PIN, 0);    // Sets up square wave to have a duty cycle of 0

  Serial.println("Start ");

  if (iBatt < 50) {//Enter phase 2;
    phase = 2;
    targetVoltage = 13.7;
    if (iBatt < 10) {//enter phase 3
      phase = 3;
      Q3 = false;  //turn current to battery off
    }
  }

  // Voltage input measurement
  //float vFromMPPT = analogRead(vFromMPPT);
  vFromMPPT = 1.68;  //test

  float voltageFromMPPT = vFromMPPT * 10.0;

  // Voltage battery measurement
  //float vBuckOutput = analogRead(vBuckOutput);
  vBuckOutput = 1.47;  //test case
  float BuckOutputVoltage = vBuckOutput * 10.0;

  // Charge control
  float duty = targetVoltage / voltageFromMPPT; //calculate initial expected duty cycle of an ideal buckconverter
  float calculatedOffsetofTVaBCO = targetVoltage / BuckOutputVoltage; //describes how far off the output of the buck converter is from the target voltage
  if (calculatedOffsetofTVaBCO > .98 && calculatedOffsetofTVaBCO < 1.02) {
    Q3 = true;  //turn on current to battery
  } else if (calculatedOffsetofTVaBCO <= .99) {
    duty += 0.01;
  } else if (calculatedOffsetofTVaBCO >= 1.01) {
    duty -= 0.01;
  }

  // Set the duty cycle
  Timer1.pwm(NPN_PIN, duty * 1023);

  // Prints the result to the serial port
  Serial.print("voltageFromMPPT: ");
  Serial.print(voltageFromMPPT);
  Serial.print(" BuckOutputVoltage: ");
  Serial.print(BuckOutputVoltage);
  Serial.print(" duty: ");
  Serial.print(duty * 100);
  Serial.println(" end of setup");
}

void loop() {

  
  //testing changing battery current given random solar panel output voltages and random voltage reads at the buck converter output
  //resets testing
  if (iBatt < 10) {
    iBatt = 500;
    phase = 1;
    targetVoltage = 14.7;
    batteryFullyCharged = false;
    Serial.println("Reset ");
  }

  Serial.print("targetVoltage: ");
  Serial.print(targetVoltage);
  if(iBatt >= 10){
    iBatt = iBatt + random(-100, 50);  //simulate test cases
    Serial.print(" iBatt: ");
    Serial.print(iBatt);
  }

  if (iBatt < 50 && iBatt >= 10) {    //Enter phase 2;
    phase = 2;
    targetVoltage = 13.7;
  }
  if (iBatt < 10) {     //enter phase 3
      phase = 3;
      Q3 = false;  //turn current to battery off
      batteryFullyCharged = true;
      //targetVoltage = 0; //comment this code to go through multiple iterations 
    }
  Serial.print(" phase: ");
  Serial.print(phase);
  // Voltage input measurement
  //float vFromMPPT = analogRead(vFromMPPT); //comment for testing
  float rand = random(-3, 5);
  vFromMPPT = 1.68 + rand / 10;  //test for changing input voltage

  float voltageFromMPPT = vFromMPPT * 10.0;

  // Voltage battery measurement
  //float vBuckOutput = analogRead(vBuckOutput); //comment for testing
  float rand2 = random(-5, 5); 
  vBuckOutput = 1.47 + rand2 / 10;  //test case
  float BuckOutputVoltage = vBuckOutput * 10.0;

  // Charge control
  float duty = targetVoltage / voltageFromMPPT;
  
  bool correctBuckOutputVoltage = false;
  while (correctBuckOutputVoltage == false) {
    float calculatedOffsetofTVaBCO = targetVoltage / BuckOutputVoltage;

    if (calculatedOffsetofTVaBCO > .98 && calculatedOffsetofTVaBCO < 1.02) {
      correctBuckOutputVoltage = true;  //turn on current to battery
      
    } else if (calculatedOffsetofTVaBCO <= .99) {
      //duty -= 0.01;
      //for testing purposes
      BuckOutputVoltage -= 0.1;
    } else if (calculatedOffsetofTVaBCO >= 1.01) {
      //duty += 0.01;
      //for testing purposes
      BuckOutputVoltage += 0.1;
    }
    // update the duty cycle
    if(duty > .99){
      break;
    }
    Timer1.pwm(NPN_PIN, duty * 1023);
  }



  // Print the result to the serial port
  if(batteryFullyCharged == false){
    if(duty > 1){
      Serial.print(" requiered duty cycle is too high: ");
      Serial.print(duty);
      Serial.println();
    }else{
      Serial.print(" voltageFromMPPT: ");
      Serial.print(voltageFromMPPT);
      Serial.print(" BuckOutputVoltage: ");
      Serial.print(BuckOutputVoltage);
      Serial.print(" duty: ");
      Serial.print(duty * 100);
      Serial.println();
    }
  }else {
      Serial.println();
      Serial.println("Battery is fully Charged");
  }
  count++;
  while (count >= 50) { //used to end output for testing purposes
  }
}
