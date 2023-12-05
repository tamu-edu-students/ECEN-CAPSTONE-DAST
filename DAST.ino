const int motorXPin1 = 2; // X Axis Motor - IN1 on the L298N
const int motorXPin2 = 3; // X Axis Motor - IN2
const int motorXPin3 = 4; // X Axis Motor - IN3
const int motorXPin4 = 5; // X Axis Motor - IN4

const int motorYPin1 = 8; // Y Axis Motor - IN1 on the second L298N
const int motorYPin2 = 9; // Y Axis Motor - IN2
const int motorYPin3 = 10; // Y Axis Motor - IN3
const int motorYPin4 = 11; // Y Axis Motor - IN4

const int ldr1 = A0; // LDR 1
const int ldr2 = A1; // LDR 2
const int ldr3 = A2; // LDR 3
const int ldr4 = A3; // LDR 4

const int threshold = 300; // Threshold for change in resistance (expressed in ohms)

// Previous LDR resistances (initially zero, will be updated in loop)
float prev_resistance_1 = 0;
float prev_resistance_2 = 0;
float prev_resistance_3 = 0;
float prev_resistance_4 = 0;

void setup() {
  pinMode(motorXPin1, OUTPUT);
  pinMode(motorXPin2, OUTPUT);
  pinMode(motorXPin3, OUTPUT);
  pinMode(motorXPin4, OUTPUT);

  pinMode(motorYPin1, OUTPUT);
  pinMode(motorYPin2, OUTPUT);
  pinMode(motorYPin3, OUTPUT);
  pinMode(motorYPin4, OUTPUT);

  Serial.begin(9600); // Start Serial communication
}

void loop() {
  // Read current LDR values
  int ldr_value_1 = analogRead(ldr1);
  int ldr_value_2 = analogRead(ldr2);
  int ldr_value_3 = analogRead(ldr3);
  int ldr_value_4 = analogRead(ldr4);

  // Calculate the resistance and voltage drop of each LDR
  float resistance_1 = (float)(1023 - ldr_value_1) * 10000 / ldr_value_1;
  float voltage_drop_1 = (float)ldr_value_1 * 5 / 1023;

  float resistance_2 = (float)(1023 - ldr_value_2) * 10000 / ldr_value_2;
  float voltage_drop_2 = (float)ldr_value_2 * 5 / 1023;

  float resistance_3 = (float)(1023 - ldr_value_3) * 10000 / ldr_value_3;
  float voltage_drop_3 = (float)ldr_value_3 * 5 / 1023;

  float resistance_4 = (float)(1023 - ldr_value_4) * 10000 / ldr_value_4;
  float voltage_drop_4 = (float)ldr_value_4 * 5 / 1023;

  // Print the resistance and voltage drop values to the serial monitor
  Serial.print("LDR 1 - Resistance: "); Serial.print(resistance_1); Serial.print(" Ohms, Voltage Drop: "); Serial.print(voltage_drop_1); Serial.println(" V");
  Serial.print("LDR 2 - Resistance: "); Serial.print(resistance_2); Serial.print(" Ohms, Voltage Drop: "); Serial.print(voltage_drop_2); Serial.println(" V");
  Serial.print("LDR 3 - Resistance: "); Serial.print(resistance_3); Serial.print(" Ohms, Voltage Drop: "); Serial.print(voltage_drop_3); Serial.println(" V");
  Serial.print("LDR 4 - Resistance: "); Serial.print(resistance_4); Serial.print(" Ohms, Voltage Drop: "); Serial.print(voltage_drop_4); Serial.println(" V");

  // Calculate change in resistance
  float change_resistance_1 = abs(resistance_1 - prev_resistance_1);
  float change_resistance_2 = abs(resistance_2 - prev_resistance_2);
  float change_resistance_3 = abs(resistance_3 - prev_resistance_3);
  float change_resistance_4 = abs(resistance_4 - prev_resistance_4);

  // Update previous resistance values
  prev_resistance_1 = resistance_1;
  prev_resistance_2 = resistance_2;
  prev_resistance_3 = resistance_3;
  prev_resistance_4 = resistance_4;

  // Check if any LDR resistance has changed more than the threshold
  if (change_resistance_1 > threshold || change_resistance_2 > threshold || change_resistance_3 > threshold || change_resistance_4 > threshold) {
    // Resistance values have changed significantly; move motors
    // Define the logic for when and how to move the motors here
    // For now, let's rotate both motors forward as an example
    rotateMotor(motorXPin1, motorXPin2, motorXPin3, motorXPin4, true);
    rotateMotor(motorYPin1, motorYPin2, motorYPin3, motorYPin4, true);
    Serial.println("Motors moving forward");
  } else {
    // Resistance values have not changed significantly; do not move motors
    Serial.println("No significant change in resistance, motors are stopped.");
  }

  delay(1000); // Delay for a moment before next reading
}

void rotateMotor(int pin1, int pin2, int pin3, int pin4, bool direction) {
  // Rotate the motor in the specified direction
  // Define the stepping sequence based on your motor's specifications
  // This is just a placeholder and should be replaced with the actual step sequence
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, LOW);
  delay(700);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, HIGH);
  delay(700);
}
