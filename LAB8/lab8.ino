#include <Wire.h>
#include <MPU6050.h>
MPU6050 mpu;
// Define LED pins from left to right
const int NUM_LEDS = 7;
const int LED_PINS[NUM_LEDS] = {7, 8, 9, 10, 11, 12, 13}; // Using working
pins 7-13
const int CENTER_LED = 3; // Index of center LED (4th LED, pin 10)
const float MAX_ACCEL = 10.14; // Maximum acceleration in g (adjust this
value!)
const float THIRD_ACCEL = MAX_ACCEL / 3.0; // One-third of maximum
acceleration
// Variables to track acceleration
float currentAccel = 0;
void setup()
{
Serial.begin(115200);
// Initialize LED pins
for (int i = 0; i < NUM_LEDS; i++) {
pinMode(LED_PINS[i], OUTPUT);
digitalWrite(LED_PINS[i], LOW);
}
Serial.println("Initialize MPU6050");
while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
{
Serial.println("Could not find a valid MPU6050 sensor, check
wiring!");
delay(500);
}
// Initial center LED illumination
digitalWrite(LED_PINS[CENTER_LED], HIGH);
}
void updateLEDs(float accel) {
// Turn off all LEDs first
for (int i = 0; i < NUM_LEDS; i++) {
digitalWrite(LED_PINS[i], LOW);
}
// Determine which LED to light based on acceleration
int ledIndex;
if (abs(accel) < 0.5) {
// If acceleration is very small, light center LED
ledIndex = CENTER_LED;
} else if (accel > 0) {
// Positive acceleration (rightward)
if (accel > MAX_ACCEL) accel = MAX_ACCEL;
ledIndex = CENTER_LED + ceil(accel / THIRD_ACCEL);
if (ledIndex >= NUM_LEDS) ledIndex = NUM_LEDS - 1;
} else {
// Negative acceleration (leftward)
if (accel < -MAX_ACCEL) accel = -MAX_ACCEL;
ledIndex = CENTER_LED - ceil(abs(accel) / THIRD_ACCEL);
if (ledIndex < 0) ledIndex = 0;
}
// Light the appropriate LED
digitalWrite(LED_PINS[ledIndex], HIGH);
}
void loop()
{
// Read normalized acceleration
Vector normAccel = mpu.readNormalizeAccel();
// We're only interested in X-axis acceleration
currentAccel = normAccel.XAxis;
// Update LED display
updateLEDs(currentAccel);
// Small delay to prevent flickering
delay(50);
}
