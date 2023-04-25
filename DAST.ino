int motorPin1 = 9;  // Motor 1 control pin
int motorPin2 = 10; // Motor 2 control pin
int LDR_Pin[] = {A0, A1, A2, A3};   // LDR sensor pins
int LDR_threshold[] = {500, 500, 500, 500}; // LDR threshold values for switching the direction of motor
int motorSpeeds[] = {0, 0, 0, 0}; // Motor speeds

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < 4; i++) {
    int LDR_val = analogRead(LDR_Pin[i]);
    motorSpeeds[i] = map(LDR_val, 0, 1023, 0, 255); // Map the LDR value to motor speed between 0 and 255
    
    // Switch the direction of motor if the LDR value is greater than the threshold value
    if (LDR_val > LDR_threshold[i]) {
      analogWrite(motorPin1, 255 - motorSpeeds[i]); // Set motor 1 speed to complement of the mapped LDR value
      analogWrite(motorPin2, motorSpeeds[i]); // Set motor 2 speed to the mapped LDR value
    } else {
      analogWrite(motorPin1, motorSpeeds[i]); // Set motor 1 speed to the mapped LDR value
      analogWrite(motorPin2, 255 - motorSpeeds[i]); // Set motor 2 speed to the complement of the mapped LDR value
    }
    
    Serial.print("LDR ");
    Serial.print(i);
    Serial.print(" value: ");
    Serial.println(LDR_val);
    delay(10);
  }
}
