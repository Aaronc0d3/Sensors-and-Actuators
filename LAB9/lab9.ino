#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin
#define LEDPin 13 // Onboard LED
#define buzzerPin 9 // buzzerpin
int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance
void setup() {
Serial.begin (9600);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
pinMode(buzzerPin, OUTPUT);
}
void loop() {
/* The following trigPin/echoPin cycle is used to determine the
distance of the nearest object by bouncing soundwaves off of it. */
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
//Calculate the distance (in cm) based on the speed of sound.
distance = duration/58.2;
if (distance > 12){
Serial.println(distance);
digitalWrite(buzzerPin, LOW);
}
else if (distance >= 2) {
int frequency = map(distance, 2, 12, 2500, 500);
tone(buzzerPin, frequency);
Serial.print(distance);
Serial.print(" cm - ");
Serial.print(frequency);
Serial.println(" Hz");
}
// If distance is less than 2cm, maximum frequency
else {
tone(buzzerPin, 2500);
Serial.print(distance);
Serial.println(" cm - 2500 Hz");
}
//Delay 50ms before next reading.
delay(100);
}
