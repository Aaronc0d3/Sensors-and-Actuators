int servoPin = 9; // select digital pin 9 for servomotor signal line
int pulseWidth; // initialize pulse width variable
int potPin = A0; // select analog pin 0 for potentiometer
// Function to generate servo control pulse
void servoPulse(int servoPin, int angle) {
// Convert angle (0-180) to pulse width (500-2480 microseconds)
pulseWidth = map(angle, 0, 180, 500, 2480);
digitalWrite(servoPin, HIGH);
delayMicroseconds(pulseWidth);
digitalWrite(servoPin, LOW);
delay(20); // Wait for 20ms (50Hz update rate)
}
void setup() {
pinMode(servoPin, OUTPUT);
Serial.begin(9600);
Serial.println("Servo Control Ready");
}
void loop() {
// Read potentiometer value (0-1023)
int potValue = analogRead(potPin);
// Map potentiometer value (0-1023) to servo angle (0-180)
int servoAngle = map(potValue, 0, 1023, 0, 180);
// Print potentiometer reading and servo angle
Serial.print("Pot Value: ");
Serial.print(potValue);
Serial.print(" | Servo Angle: ");
Serial.println(servoAngle);
// Update servo position
servoPulse(servoPin, servoAngle);
}
