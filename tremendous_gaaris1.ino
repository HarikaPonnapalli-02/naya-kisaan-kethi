#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Check if your LCD uses 0x27 or 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2); // Try changing 0x27 to 0x3F if needed

// Define sensor pins
const int soilMoisturePin = A1;
const int trigPin = 7;
const int echoPin = 6;
const int relayPin = 8;
const int motorPin = 13; // Motor control pin
const int buzzerPin = 9;
const int powerLED = 10;
const int pumpLED = 11;
const int alertLED = 12;

void setup() {
    Serial.begin(9600);
    lcd.init(); // Initialize LCD
    lcd.backlight(); // Turn on LCD backlight
    
    pinMode(soilMoisturePin, INPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(relayPin, OUTPUT);
    pinMode(motorPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(powerLED, OUTPUT);
    pinMode(pumpLED, OUTPUT);
    pinMode(alertLED, OUTPUT);
    
    digitalWrite(relayPin, LOW);
    digitalWrite(motorPin, LOW);
    digitalWrite(powerLED, HIGH); // Power LED ON
}

// Function to measure water level using Ultrasonic Sensor
long getWaterLevel() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    long distance = duration * 0.034 / 2;
    return distance;
}

void loop() {
    int soilMoisture = analogRead(soilMoisturePin);
    int moisturePercentage = map(soilMoisture, 1023, 0, 0, 100);
    long waterLevel = getWaterLevel();
    
    Serial.print("Soil Moisture: ");
    Serial.print(moisturePercentage);
    Serial.println("%");
    Serial.print("Water Level: ");
    Serial.print(waterLevel);
    Serial.println(" cm");
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Soil: ");
    lcd.print(moisturePercentage);
    lcd.print("% ");
    
    lcd.setCursor(0, 1);
    lcd.print("Water: ");
    lcd.print(waterLevel);
    lcd.print("cm ");
    
    // Water Pump & Motor Control
    if (moisturePercentage < 30) {
        digitalWrite(relayPin, HIGH);
        digitalWrite(motorPin, HIGH); // Turn ON motor
        digitalWrite(pumpLED, HIGH);
        lcd.setCursor(10, 0);
        lcd.print("PUMP ON");
    } else {
        digitalWrite(relayPin, LOW);
        digitalWrite(motorPin, LOW); // Turn OFF motor
        digitalWrite(pumpLED, LOW);
        lcd.setCursor(10, 0);
        lcd.print("PUMP OFF");
    }
    
    // Alert System
    if (waterLevel > 15) { // Example threshold
        digitalWrite(alertLED, HIGH);
        digitalWrite(buzzerPin, HIGH); // Turn ON buzzer
    } else {
        digitalWrite(alertLED, LOW);
        digitalWrite(buzzerPin, LOW); // Turn OFF buzzer
    }
    
    delay(2000);
}
