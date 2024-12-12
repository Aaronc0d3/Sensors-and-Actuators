#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define TRIGGER_PIN 7
#define ECHO_PIN 6
#define RED_LED 4
#define GREEN_LED 5
#define BUZZER 3
#define START_BUTTON 2
#define HEAD_SERVO 9
#define ELIMINATION_SERVO 10

// I2C LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo objects
Servo headServo;
Servo eliminationServo;

// Game variables
unsigned long gameStartTime;
unsigned long lastStateChange;
bool isGameRunning = false;
bool isGreenLight = false;
bool hasFirstGreenOccurred = false;  // New flag for first green light
int previousDistance = 0;
int currentDistance = 0;

void setup() {
    // Initialize pins
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(START_BUTTON, INPUT_PULLUP);
    
    // Initialize servos
    headServo.attach(HEAD_SERVO);
    eliminationServo.attach(ELIMINATION_SERVO);
    
    // Initialize LCD
    Wire.begin();
    lcd.init();
    lcd.backlight();
    lcd.print("Press to Start!");
    
    // Set initial positions
    headServo.write(120);
    eliminationServo.write(0);
}

void loop() {
    if (!isGameRunning && digitalRead(START_BUTTON) == LOW) {
        startGame();
        delay(200);
    }
    
    if (isGameRunning) {
        // Update timer
        int timeLeft = 60 - ((millis() - gameStartTime) / 1000);
        
        // Display time and distance
        lcd.clear();
        lcd.print("Time: ");
        lcd.print(timeLeft);
        lcd.print("s");
        lcd.setCursor(0, 1);
        
        // Only show distance after first green light
        if (hasFirstGreenOccurred) {
            lcd.print("Dist: ");
            lcd.print(currentDistance);
            lcd.print("cm");
        } else {
            lcd.print("Waiting to start");
        }
        
        // Check if time's up
        if (timeLeft <= 0) {
            endGame("Time's up!");
            eliminationServo.write(180);
            delay(10);
            eliminationServo.write(0);
            return;
        }
        
        // Change light state every 2-5 seconds
        if (millis() - lastStateChange > random(2000, 5000)) {
            changeLightState();
        }
        
        // Only measure distance and check movement during red light and after first green
        if (!isGreenLight && hasFirstGreenOccurred) {
            currentDistance = getDistance();
            
            // Only check for movement if we have valid readings
            if (currentDistance > 0 && currentDistance < 400) {
                // Check for victory (within 5cm of sensor)
                if (currentDistance < 3) {
                    victory();
                    return;
                }
                
                // Detect significant movement during red light
                if (previousDistance > 0 && abs(currentDistance - previousDistance) > 4) {
                    // double check
                    int confirmDistance = getDistance();
                    if (abs(confirmDistance - previousDistance) > 4) {
                        endGame("Movement detected!");
                        return;
                    }
                }
                previousDistance = currentDistance;
            }
        }
        
        delay(50);
    }
}

int getDistance() {
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    
    long duration = pulseIn(ECHO_PIN, HIGH, 23529);
    int distance = duration * 0.034 / 2;
    
    if (distance > 400 || distance < 0) {
        return previousDistance;
    }
    return distance;
}

void startGame() {
    isGameRunning = true;
    isGreenLight = false;
    hasFirstGreenOccurred = false;  // Reset first green light flag
    gameStartTime = millis();
    lastStateChange = millis();
    currentDistance = 0;
    previousDistance = 0;
    
    headServo.write(120);
    eliminationServo.write(0);
    
    lcd.clear();
    lcd.print("Game Started!");
    delay(500);
}

void changeLightState() {
    isGreenLight = !isGreenLight;
    lastStateChange = millis();
    
    if (isGreenLight) {
        headServo.write(180);
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(RED_LED, LOW);
        tone(BUZZER, 1000, 200);
        currentDistance = 0;
        previousDistance = 0;
        hasFirstGreenOccurred = true;  // Set flag when first green light occurs
    } else {
        headServo.write(120);
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        tone(BUZZER, 2000, 200);
        delay(500);
        
        // Only establish baseline if first green has occurred
        if (hasFirstGreenOccurred) {
            currentDistance = getDistance();
            previousDistance = currentDistance;
        }
    }
}

void endGame(const char* reason) {
    isGameRunning = false;
    
    // Eliminate player
    eliminationServo.write(180);
    
    // Display message
    lcd.clear();
    lcd.print("Game Over!");
    lcd.setCursor(0, 1);
    lcd.print(reason);
    
    // Sound and lights
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER, 500, 1000);
    
    delay(2000);
    
    // Reset everything
    eliminationServo.write(0);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    noTone(BUZZER);
    
    lcd.clear();
    lcd.print("Press to restart");
}

void victory() {
    isGameRunning = false;
    
    lcd.clear();
    lcd.print("Victory!");
    
    // Victory celebration
    for (int i = 0; i < 3; i++) {
        digitalWrite(GREEN_LED, HIGH);
        tone(BUZZER, 2000);
        delay(200);
        digitalWrite(GREEN_LED, LOW);
        noTone(BUZZER);
        delay(200);
    }
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    lcd.clear();
    lcd.print("Press to restart");
}