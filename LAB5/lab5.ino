int buzzer = 8; // select digital IO pin for the buzzer
int potPin = A0; // select analog pin for the potentiometer
void setup() {
pinMode(buzzer, OUTPUT); // set digital IO pin pattern, OUTPUT to be
output
Serial.begin(9600); // initialize serial communication at 9600 bps
}
void loop() {
int potValue = analogRead(potPin); // read the potentiometer value (0 to
1023)
int frequency = map(potValue, 0, 1023, 60, 10000); // map the value to a
range of 60 to 10,000 Hz
// Print the current frequency to the Serial Monitor
Serial.print("Frequency is ");
Serial.print(frequency);
Serial.println(" Hz");
// Calculate the period of one cycle in microseconds
unsigned long periodMicroseconds = 1000000 / frequency; // period in
microseconds
unsigned long halfPeriodMicroseconds = periodMicroseconds / 2; // half
of the period (high or low time)
// Generate the square wave signal for the buzzer
digitalWrite(buzzer, HIGH); // sound ON
delayMicroseconds(halfPeriodMicroseconds); // high for half the period
digitalWrite(buzzer, LOW); // sound OFF
delayMicroseconds(halfPeriodMicroseconds); // low for half the period
}