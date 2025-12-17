#include <LiquidCrystal.h>

// LCD: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Pins
const int soilPin = A0;
const int pumpLED = 8;     // LED represents pump
const int rainButton = 9;

// Variables
int soilValue = 0;
int prevSoilValue = 0;

// Thresholds
const int DRY_THRESHOLD = 450;
const int DRYING_RATE = 20;

void setup() {
  pinMode(pumpLED, OUTPUT);
  pinMode(rainButton, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.print("Smart Irrigation");
  delay(2000);
  lcd.clear();

  prevSoilValue = analogRead(soilPin);
}

void loop() {
  soilValue = analogRead(soilPin);
  bool rainMode = (digitalRead(rainButton) == LOW);

  // Prediction logic: fast drying detection
  bool dryingFast = (prevSoilValue - soilValue) > DRYING_RATE;

  lcd.setCursor(0, 0);
  lcd.print("Soil:");
  lcd.print(soilValue);
  lcd.print("   ");

  if (rainMode) {
    digitalWrite(pumpLED, LOW);   // Force OFF
    lcd.setCursor(0, 1);
    lcd.print("Rain Mode ON   ");
  }
  else if (soilValue > DRY_THRESHOLD || dryingFast) {
    digitalWrite(pumpLED, HIGH);  // Pump ON
    lcd.setCursor(0, 1);
    lcd.print("Pump ON        ");
  }
  else {
    digitalWrite(pumpLED, LOW);   // Pump OFF
    lcd.setCursor(0, 1);
    lcd.print("Pump OFF       ");
  }

  prevSoilValue = soilValue;
  delay(1000);
}
